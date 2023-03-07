#include <string.h>
#include "key_task.h"
#include <components/system.h>
#include <os/os.h>

#include "bk_gpio.h"
#include <driver/gpio.h>
#include <driver/hal/hal_gpio_types.h>
#include "gpio_driver.h"
#include "bk_saradc.h"
#include <driver/adc.h>
#include "adc_statis.h"

#include "bk_private/legacy_init.h"
#include <components/shell_task.h>

#include "FreeRTOS.h"
#include "task.h"

static beken_timer_t key_timer;

beken_thread_t key_thread_handle = NULL;
beken_queue_t key_msg_que = NULL;

typedef enum
{
	TIMER_POLL,
	THREAD_EXIT,
} DEV_STATE;

typedef struct key_message
{
	DEV_STATE key_msg;
} KEY_MSG_T;

#define ADC_KEY 1
#define MULTI_KEY 1

#define TAG "usr_key"
#define debug_printf(...) BK_LOGW(TAG, ##__VA_ARGS__)

//=================================================================================
#define DEBOUNCE_TICKS 2	   // 2X20MS
#define LONG_TICKS 75		   // 75X20MS
#define DOUB_INTERVAL_TICKS 10 // 10X20MS

#define NULL_KEY 0

#define KEY_CODE_TYPE uint16_t

typedef void (*key_callback)(void *);

typedef enum
{
	NONE_PRESS,
	SHORT_CLICK_DOWN,
	SHORT_CLICK_UP,
	DOUBLE_CLICK,
	LONG_RRESS_START,
	LONG_PRESS_HOLD,
	LONG_PRESS_UP,
	END_PRESS_EVT,
} PRESS_EVT;

typedef struct _key_
{
	uint8_t repeat : 4;
	uint8_t event : 4;
	uint8_t state : 3;
	uint8_t debounce_cnt : 3;

#ifdef MULTI_KEY
	uint8_t null : 2;
	KEY_CODE_TYPE active_level;
	KEY_CODE_TYPE key_level;
#else
	uint8_t active_level : 1;
	uint8_t key_level : 1;
#endif

	KEY_CODE_TYPE key_code;
	uint16_t ticks;
	uint16_t repeat_ticks;
	uint8_t gpio;
	uint8_t (*hal_key_Level)(uint8_t);
	key_callback cb[END_PRESS_EVT];
	struct _key_ *next;
} KEY_S;

#define EVENT_CB(ev)    \
	if (handle->cb[ev]) \
	handle->cb[ev]((KEY_S *)handle)

typedef struct
{
	uint8_t index;
	int32_t value;
	int32_t margin; // ADC正负范围
} abc_btn_cfg_t;

#ifdef MULTI_KEY
KEY_S gpio_button_test[1];
#else
KEY_S gpio_button_test[4];
#endif


//=================================================================================
// adc设置
//=================================================================================
static abc_btn_cfg_t g_abc_btn[] = {
	{.index = 1, .value = 10, .margin = 100},  // next
	{.index = 2, .value = 335, .margin = 100}, // prev
	{.index = 3, .value = 995, .margin = 100}, // menu
	{.index = 4, .value = 674, .margin = 100}, // play
};

#define ADC_BTN_NUM (sizeof(g_abc_btn) / sizeof(g_abc_btn[0]))

#define ADC_KEY_GPIO_ID GPIO_28
#define ADC_KEY_SADC_CHAN_ID 4
//=================================================================================
static void hal_adc_init(void)
{
	adc_config_t config = {0};

	BK_LOG_ON_ERR(gpio_dev_unmap(ADC_KEY_GPIO_ID));
	// os_memset(&config, 0, sizeof(adc_config_t));
	int adc_chan = ADC_KEY_SADC_CHAN_ID;
	config.chan = adc_chan;

	// config.adc_mode = 2;
	config.adc_mode = ADC_CONTINUOUS_MODE;
	config.src_clk = ADC_SCLK_XTAL_26M;
	config.clk = 203125;
	config.saturate_mode = 3;
	config.steady_ctrl = 7;
	config.adc_filter = 0;
	config.sample_rate = 0;
	if (config.adc_mode == ADC_CONTINUOUS_MODE)
	{
		config.sample_rate = 32;
	}
	BK_LOG_ON_ERR(bk_adc_driver_init());

	BK_LOG_ON_ERR(bk_adc_acquire());
	BK_LOG_ON_ERR(bk_adc_init(adc_chan));
	BK_LOG_ON_ERR(bk_adc_set_config(&config));
	BK_LOGI(TAG, "adc init test:adc_channel:%x, adc_mode:%x,src_clk:%x,adc_clk:%x, saturate_mode:%x, sample_rate:%x\r\n",
			adc_chan, config.adc_mode, config.src_clk, config.clk, config.saturate_mode, config.sample_rate);

	BK_LOG_ON_ERR(bk_adc_set_channel(adc_chan));
	BK_LOG_ON_ERR(bk_adc_enable_bypass_clalibration());
	BK_LOG_ON_ERR(bk_adc_start());
}

//=================================================================================
static int8_t adc_read_key(void)
{
	uint16_t value = 0;
	float cali_value = 0;
	int32_t key_value = 0;

	if (bk_adc_set_channel(ADC_KEY_SADC_CHAN_ID))
	{
		bk_adc_release();
		return 0;
	}

	bk_adc_read(&value, 100);
	cali_value = saradc_calculate(value);
	key_value = (int32_t)(cali_value * 1000 * 2);
	// debug_printf("start read adc value %d\r\n",key_value);
	for (uint8_t idx = 0; idx < ADC_BTN_NUM; idx++)
	{
		uint8_t level = (((g_abc_btn[idx].value - g_abc_btn[idx].margin) <= key_value) && (key_value <= (g_abc_btn[idx].value + g_abc_btn[idx].margin))) ? 0 : 1;
		if (level == 0)
		{
			return g_abc_btn[idx].index;
		}
	}
	return 0;
}

//=================================================================================
//=================================================================================
static KEY_S *head_handle = NULL;

//=================================================================================
void key_reg(KEY_S *handle, uint8_t (*pin_level)(uint8_t), uint8_t active_level, uint8_t gpio, KEY_CODE_TYPE key_code)
{
	memset(handle, 0, sizeof(KEY_S));
	handle->event = (uint8_t)NONE_PRESS;
	handle->hal_key_Level = pin_level;
	handle->gpio = gpio;
	handle->key_level = !active_level;	 // 按键没有按下时的电平
	handle->active_level = active_level; // 按键按下的电平
	handle->key_code = key_code;
}

//=================================================================================
void key_attach(KEY_S *handle, PRESS_EVT event, key_callback cb)
{
	handle->cb[event] = cb;
}

//=================================================================================
PRESS_EVT key_get_event(KEY_S *handle)
{
	return (PRESS_EVT)(handle->event);
}

//=================================================================================
void button_handler(KEY_S *handle)
{
#if ADC_KEY
	uint8_t read_gpio_level = adc_read_key();
#elif

#else
	uint8_t read_gpio_level = handle->hal_key_Level(handle->gpio);
#endif

	if ((handle->state) > 0)
	{
		handle->ticks++;
	}
	else
	{
		if (handle->repeat_ticks)
		{
			handle->repeat_ticks--;
			if (handle->repeat_ticks == 0)
			{
				handle->repeat = 0;
			}
		}
	}

	handle->key_level = read_gpio_level;
	/*-----------------State machine-------------------*/
	switch (handle->state)
	{
	case 0:
#if MULTI_KEY | ADC_KEY
		if (handle->key_level != NULL_KEY)
#else
		if (handle->key_level == handle->active_level)
#endif
		{ // start press down
			handle->ticks = 0;
			handle->state = 1;
#if MULTI_KEY | ADC_KEY
			handle->active_level = handle->key_level;
#endif
		}
		else
		{
			handle->event = (uint8_t)NONE_PRESS;
		}
		break;

	case 1:
		if (handle->key_level == handle->active_level)
		{
			if (handle->ticks >= DEBOUNCE_TICKS)
			{
#if MULTI_KEY | ADC_KEY
				handle->key_code = handle->key_level;
#endif

				handle->event = (uint8_t)SHORT_CLICK_DOWN;
				EVENT_CB(SHORT_CLICK_DOWN);
				handle->state = 2;

				handle->repeat++;
				if (handle->repeat == 2)
				{
					EVENT_CB(DOUBLE_CLICK);
				}
				handle->repeat_ticks = DOUB_INTERVAL_TICKS;
			}
		}
		else
		{
			handle->event = (uint8_t)NONE_PRESS;
			handle->state = 0;
		}
		break;

	case 2:
		if (handle->key_level == handle->active_level)
		{
			if (handle->ticks >= LONG_TICKS)
			{
				handle->event = (uint8_t)LONG_RRESS_START;
				EVENT_CB(LONG_RRESS_START);
				handle->repeat = 0;
				handle->repeat_ticks = 0;
				handle->state = 3;
			}
		}
		else
		{
			if (handle->event == SHORT_CLICK_DOWN)
			{
				handle->event = (uint8_t)SHORT_CLICK_UP;
				EVENT_CB(SHORT_CLICK_UP);
			}
			else
			{
				handle->event = (uint8_t)LONG_PRESS_UP;
				EVENT_CB(LONG_PRESS_UP);
			}
			handle->state = 0; // reset
		}
		break;

	case 3:
		if (handle->key_level == handle->active_level)
		{
			if (handle->ticks >= LONG_TICKS)
			{
				handle->event = (uint8_t)LONG_PRESS_HOLD;
				EVENT_CB(LONG_PRESS_HOLD);
				handle->repeat = 0;
				handle->repeat_ticks = 0;
				handle->state = 3;
			}
		}
		else
		{
			handle->event = (uint8_t)LONG_PRESS_UP;
			EVENT_CB(LONG_PRESS_UP);
			handle->state = 0; // reset
		}
		break;

	default:
		break;
	}
}

//=================================================================================
int key_add_list(KEY_S *handle)
{
	KEY_S *target = head_handle;
	while (target)
	{
		if (target == handle)
		{
			return -1; // already exist.
		}
		target = target->next;
	}
	handle->next = head_handle;
	head_handle = handle;
	return 0;
}

//=================================================================================
void key_del_list(KEY_S *handle)
{
	KEY_S **curr;

	for (curr = &head_handle; *curr;)
	{
		KEY_S *entry = *curr;
		if (entry == handle)
		{
			*curr = entry->next;
		}
		else
			curr = &entry->next;
	}
}

//=================================================================================
// 这个程序需要定时扫它
//=================================================================================
void key_ticks(void *param)
{
	KEY_S *target;
	for (target = head_handle; target; target = target->next)
	{
		button_handler(target);
	}
}



//=================================================================================
void app_key_send_msg(DEV_STATE new_msg)
{
	bk_err_t ret;
	KEY_MSG_T msg;

	if (key_msg_que)
	{
		msg.key_msg = new_msg;

		ret = rtos_push_to_queue(&key_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			os_printf("app_key_send_msg failed\r\n");
	}
}
//=================================================================================
static void key_scan_timer(void *data)
{
	app_key_send_msg(TIMER_POLL);
}

//=================================================================================
static void app_key_main(void)
{
	hal_adc_init();

	bk_err_t err = rtos_init_timer(&key_timer,
								   1 * 20,
								   key_scan_timer,
								   (void *)0);
	BK_ASSERT(kNoErr == err);

	err = rtos_start_timer(&key_timer);
	BK_ASSERT(kNoErr == err);

	while (1)
	{
		KEY_MSG_T msg;
		err = rtos_pop_from_queue(&key_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err)
		{
			switch (msg.key_msg)
			{
			case TIMER_POLL:
				key_ticks(NULL);
				break;
			case THREAD_EXIT:
			    goto app_key_exit;
			    break;

			default:
				break;
			}
		}
	}

app_key_exit:
	debug_printf("app_key_main exit\r\n");

    rtos_stop_timer(&key_timer);

	rtos_deinit_queue(&key_msg_que);
	key_msg_que = NULL;

	key_thread_handle = NULL;
	rtos_delete_thread(NULL);
}



//=================================================================================
//按键短按
static void key_click_down(void *param)
{
	KEY_S *handle = (KEY_S *)param;

	switch (handle->key_code)
	{
	case 1: //next key
	    debug_printf("next key short click!\r\n");
		break;
	case 2:  //prev key
		break;
	case 3:  //menu key
		break;
	case 4:  //play key
		break;
	default:
		break;
	}
}

//=================================================================================
//按键长按
static void key_long_pressdown(void *param)
{
	KEY_S *handle = (KEY_S *)param;

	switch (handle->key_code)
	{
	case 1:
	    debug_printf("next key long press down!\r\n");
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
	    app_key_send_msg(THREAD_EXIT);   //test
		break;
	default:
		break;
	}
}

//=================================================================================
//按键短按松开
static void key_click_up(void *param)
{
    KEY_S *handle = (KEY_S *)param;

	switch (handle->key_code)
	{
	case 1:
	    debug_printf("next key click up!\r\n");
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

//=================================================================================
//按键双击
static void key_double_click(void *param)
{
	KEY_S *handle = (KEY_S *)param;

	switch (handle->key_code)
	{
	case 1:
	    debug_printf("next key double click!\r\n");
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}


//=================================================================================
void key_task_int(void)
{
	bk_err_t err = -1;

#if MULTI_KEY | ADC_KEY
    //注册各种按键处理事件
	key_reg(&gpio_button_test[0], NULL, 0, 0, 1);
	key_attach(&gpio_button_test[0], SHORT_CLICK_DOWN, key_click_down);
	key_attach(&gpio_button_test[0], LONG_RRESS_START, key_long_pressdown);
	key_attach(&gpio_button_test[0], SHORT_CLICK_UP, key_click_up);
	key_attach(&gpio_button_test[0], DOUBLE_CLICK, key_double_click);
	key_add_list(&gpio_button_test[0]);
#endif

	if ((!key_thread_handle) && (!key_msg_que))
	{
		err = rtos_init_queue(&key_msg_que,
							  "key_queue",
							  sizeof(KEY_MSG_T),
							  2);
		if (kNoErr != err)
		{
			debug_printf("app key ceate queue failed\r\n");
			return;
		}

		err = rtos_create_thread(&key_thread_handle, BEKEN_APPLICATION_PRIORITY, "usr_key", (beken_thread_function_t)app_key_main, 1024 * 1, NULL);
		BK_ASSERT(kNoErr == err);
	}
}

















#if 0
//=================================================================================
//                                 DEMO
//=================================================================================
#ifdef MULTI_KEY
KEY_S gpio_button_test[1];
#else
KEY_S gpio_button_test[4];
#endif

//=================================================================================
#define K1 0x12
#define K2 0x13
#define K3 0x26
#define K4 0x27

#ifdef MULTI_KEY
static KEY_CODE_TYPE key_get_gpio_level(uint8_t gpio)
{
    KEY_CODE_TYPE key = 0;

	if(!gpio_get_input(K1))
	{
        key |= 0x01;
	}

	if(!gpio_get_input(K2))
	{
        key |= 0x02;
	}

	if(!gpio_get_input(K3))
	{
        key |= 0x04;
	}

	if(!gpio_get_input(K4))
	{
        key |= 0x08;
	}
	return key;
}
#else
static KEY_CODE_TYPE key_get_gpio_level(uint8_t gpio)
{
	return gpio_get_input(gpio);
}
#endif

//=================================================================================
//短按按下处理
static void key_short_press_k1(void *param)
{
	os_printf("key_short_press_k1\r\n");
}

//短按松开处理
static void key_short_up_k1(void *param)
{
	os_printf("key_short_up_k1\r\n");
}

//双击
static void key_double_press_k1(void *param)
{
	os_printf("key_double_press_k1\r\n");
}


//长按开始按下处理
static void key_long_startpress_k1(void *param)
{
	os_printf("key_long_press_k1\r\n");
}

//=================================================================================
//短按按下处理
static void key_short_up_k2(void *param)
{
	os_printf("key_short_up_k2\r\n");
}

//长按开始按下处理
static void key_long_startpress_k2(void *param)
{
	os_printf("key_long_press_k2\r\n");
}

//=================================================================================
//多个按键调用同一个键值处理程序，根据键值来区分
//短按按下处理
static void key_short_up_k34(void *param)
{
    //uint8_t key_code = (BUTTON_S*)param->key_code;
    KEY_S* handle = (KEY_S*)param;

	os_printf("key_short_up_k34 %d\r\n",handle->key_code);

	switch(handle->key_code)
	{
        case 3:
		   break;
		case 4:
		   break;
		default:
		   break;
	}
}

//长按开始按下处理
static void key_long_startpress_k34(void *param)
{
    KEY_S* handle = (KEY_S*)param;
    
	os_printf("key_long_press_k34 %d\r\n",handle->key_code);
}


//=================================================================================
void keyInit(void)
{
    os_printf("%s\r\n", __func__);

#if 0
#ifdef MULTI_KEY
//所有按键都共用同样的按键处理程序
    gpio_config(K1, INPUT, PULL_HIGH);
    gpio_config(K2, INPUT, PULL_HIGH);
	gpio_config(K3, INPUT, PULL_HIGH);
	gpio_config(K4, INPUT, PULL_HIGH);
    key_reg(&gpio_button_test[0], key_get_gpio_level, 0, 0, 1);			
	key_attach(&gpio_button_test[0], SHORT_CLICK_DOWN, key_short_up_k34);		
	key_attach(&gpio_button_test[0], LONG_RRESS_START, key_long_startpress_k34);	
	key_add_list(&gpio_button_test[0]);

#else
//每个按键分配单独的按键处理程序，适于用RAM空间大的芯片，每个按键
//需要占48个字节RAM
    uint8_t gpio;

	gpio = K1;
	gpio_config(gpio, INPUT, PULL_HIGH);	
	key_reg(&gpio_button_test[0], key_get_gpio_level, 0, gpio, 1);			
	key_attach(&gpio_button_test[0], SHORT_CLICK_DOWN, key_short_press_k1);	
	key_attach(&gpio_button_test[0], SHORT_CLICK_UP,   key_short_up_k1);		
	key_attach(&gpio_button_test[0], DOUBLE_CLICK,	   key_double_press_k1);
	key_attach(&gpio_button_test[0], LONG_RRESS_START, key_long_startpress_k1);
	key_add_list(&gpio_button_test[0]);

	gpio = K2;
	gpio_config(gpio, INPUT, PULL_HIGH);	
	key_reg(&gpio_button_test[1], key_get_gpio_level, 0, gpio, 2);		
	key_attach(&gpio_button_test[1], SHORT_CLICK_UP,   key_short_up_k2);		
	key_attach(&gpio_button_test[1], LONG_RRESS_START, key_long_startpress_k2);	
	key_add_list(&gpio_button_test[1]);

	gpio = K3;
	gpio_config(gpio, INPUT, PULL_HIGH);	
	key_reg(&gpio_button_test[2], key_get_gpio_level, 0, gpio, 3);		
	key_attach(&gpio_button_test[2], SHORT_CLICK_UP, key_short_up_k34);		
	key_attach(&gpio_button_test[2], LONG_RRESS_START, key_long_startpress_k34);	
	key_add_list(&gpio_button_test[2]);

	gpio = K4;
	gpio_config(gpio, INPUT, PULL_HIGH);	
	key_reg(&gpio_button_test[3], key_get_gpio_level, 0, gpio, 4);		
	key_attach(&gpio_button_test[3], SHORT_CLICK_UP, key_short_up_k34);		
	key_attach(&gpio_button_test[3], LONG_RRESS_START, key_long_startpress_k34);	
	key_add_list(&gpio_button_test[3]);
#endif
#endif
}
#endif
