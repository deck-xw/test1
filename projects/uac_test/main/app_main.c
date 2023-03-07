#include "os/os.h"

#include <string.h>

#include "bk_gpio.h"
#include <driver/gpio.h>
#include <driver/hal/hal_gpio_types.h>
#include "gpio_driver.h"

#include <driver/adc.h>
#include "adc_statis.h"

#include "FreeRTOS.h"
#include "task.h"

// #include "components/dvp_camera.h"

#include <common/bk_include.h>
#include "video_demo_config.h"
#include <components/video_transfer.h>
// #include <components/dvp_camera_types.h>

#include "media_app.h"
#include "camera_act.h"
#include "lcd_act.h"

#include "components/aud_intf.h"
#include "components/aud_intf_types.h"

#include "video_demo_pub.h"

#include "components/netif.h"
#include "components/event.h"
#include "modules/wifi.h"

#include "bk_private/legacy_init.h"

static int __send_packet(uint8_t *data, uint32_t len){

	os_printf(".");

	bk_aud_intf_write_spk_data(data, len);
	return len;
}


void __mem_show(void) {
    // static char InfoBuffer[512] = {0}; 
    // vTaskList((char *) &InfoBuffer);
    // os_printf("\r\n任务名      任务状态    优先级   剩余栈 任务序号\r\n%s", InfoBuffer);

    // size_t freeHeap = xPortGetFreeHeapSize();
    // os_printf("Free heap size: %u bytes\n", freeHeap);

    uint32_t total_size,free_size,mini_size;
	
	total_size = rtos_get_total_heap_size();
	free_size  = rtos_get_free_heap_size();
	mini_size  = rtos_get_minimum_free_heap_size();
	os_printf("\n    %-5s   %-5s   %-5s   %-5s   %-5s\n    heap \t%d\t%d\t%d\t%d", "name", "total", "free", "minimum", "peak", total_size, free_size ,mini_size, total_size - mini_size);

#if CONFIG_PSRAM_AS_SYS_MEMORY
	total_size = rtos_get_psram_total_heap_size();
	free_size  = rtos_get_psram_free_heap_size();
	mini_size  = rtos_get_psram_minimum_free_heap_size();
	os_printf("\n    psram \t%d\t%d\t%d\t%d", total_size, free_size, mini_size, total_size - mini_size);
#endif

    os_printf("\r\n");
}




void app_main(void){

    os_printf("app_main\n");

	bk_set_printf_sync(true);
 	shell_set_log_level(BK_LOG_WARN);

    static aud_intf_drv_setup_t aud_drv_setup;
	aud_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
	aud_drv_setup.task_config.priority = 3;
	aud_drv_setup.aud_intf_rx_spk_data = NULL;
	aud_drv_setup.aud_intf_tx_mic_data = __send_packet;
	bk_aud_intf_drv_init(&aud_drv_setup);

	bk_aud_intf_set_mode(AUD_INTF_WORK_MODE_VOICE);

	static aud_intf_voc_setup_t aud_voc_setup;
	aud_voc_setup.aec_enable 	= true;
	aud_voc_setup.spk_type 		= AUD_INTF_SPK_TYPE_UAC;
	aud_voc_setup.mic_type 		= AUD_INTF_MIC_TYPE_UAC;
	aud_voc_setup.samp_rate 	= AUD_INTF_VOC_SAMP_RATE_16K;
	aud_voc_setup.data_type 	= AUD_INTF_VOC_DATA_TYPE_PCM;
	aud_voc_setup.mic_gain 		= 0x2d;
	aud_voc_setup.spk_gain 		= 0x2d;
	aud_voc_setup.spk_mode 		= AUD_DAC_WORK_MODE_SIGNAL_END;
	aud_voc_setup.aec_cfg.ec_depth 	= 20;
	aud_voc_setup.aec_cfg.TxRxThr 	= 30;
	aud_voc_setup.aec_cfg.TxRxFlr 	= 6;
	aud_voc_setup.aec_cfg.ns_level 	= 2;
	aud_voc_setup.aec_cfg.ns_para 	= 1;
	bk_aud_intf_voc_init(aud_voc_setup);
	bk_aud_intf_voc_start();


    __mem_show();

    while (1){
        __mem_show();

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}