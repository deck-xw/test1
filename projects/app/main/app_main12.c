#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>

#include "FreeRTOS.h"
#include "task.h"

#include "bk_gpio.h"
#include <driver/gpio.h>
#include <driver/hal/hal_gpio_types.h>
#include "gpio_driver.h"

#include <driver/adc.h>
#include "adc_statis.h"

#include <common/bk_include.h>
#include "video_demo_config.h"
#include <components/video_transfer.h>

#include "components/aud_intf.h"
#include "components/aud_intf_types.h"

#include "video_demo_pub.h"

#include "media_app.h"
#include "camera_act.h"

#include "lcd_act.h"
#include "lcd_task.h"

#include "bk_pm_internal_api.h"
#include <modules/ble.h>
#include "modules/wifi.h"

#include <string.h>

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

void test_pm_gpio_callback(gpio_id_t gpio_id)
{
    os_printf("====gpio callback====\r\n");

    //IO ���Ѻ��˳���ѹģʽ
    bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0);
    os_printf("===exit bk_pm_module_vote_sleep_ctrl===\r\n");
}

void user_app_main(void)
{
	wifi_sta_config_t sta_config = WIFI_DEFAULT_STA_CONFIG();

	strncpy(sta_config.ssid, "XW", WIFI_SSID_STR_LEN);
	strncpy(sta_config.password, "XW888888", WIFI_PASSWORD_LEN);

	BK_LOG_ON_ERR(bk_wifi_sta_set_config(&sta_config));
	BK_LOG_ON_ERR(bk_wifi_sta_start());

    vTaskDelay(20000/portTICK_PERIOD_MS);
	
	extern ble_err_t bk_ble_deinit(void);
	bk_ble_deinit();
    bk_wifi_send_listen_interval_req(10);//set interval 10:1s;
	os_printf("123------\r\n");
	rtos_delay_milliseconds(200);
	os_printf("%s enter!\r\n", __func__);

	gpio_dev_unmap(GPIO_18);
    BK_LOG_ON_ERR(bk_gpio_enable_input(GPIO_18));

	bk_gpio_register_isr(GPIO_18, test_pm_gpio_callback);
	bk_gpio_register_wakeup_source(GPIO_18, GPIO_INT_TYPE_FALLING_EDGE);
	bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);

	// �����ѹģʽ
	os_printf("===enter bk_pm_module_vote_sleep_ctrl===\r\n");
	bk_pm_sleep_mode_set(PM_MODE_LOW_VOLTAGE);

//	pm_debug_ctrl(0x01);
	
	bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x1, 0); // finally, set app vote , let system enter low voltage sleep mode

}

int main(void)
{
#if (!CONFIG_SLAVE_CORE)
	rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
	bk_set_printf_sync(true);
	shell_set_log_level(BK_LOG_WARN);
#endif
	legacy_init();

	return 0;
}