#include <stdio.h>
#include <string.h>

#include "components/netif.h"
#include "components/event.h"
#include "components/log.h"
#include "modules/wifi.h"
#include "os/os.h"

#include "FreeRTOS.h"
#include "task.h"

#include "driver/timer.h"

#include <driver/uart.h>
#include <string.h>
#include "boot.h"
#if CONFIG_BLE
#include "modules/ble.h"
#include "ble_api_5_x.h"
#include "legacy_include/bluetooth_legacy_include.h"
#endif


#include "bk_private/legacy_init.h"


int __init(void)
{

#ifdef CONFIG_VND_CAL
	extern void vnd_cal_overlay(void);
	vnd_cal_overlay();
#endif

#if (CONFIG_SOC_BK7256XX && !CONFIG_SLAVE_CORE)

	#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_START_WIFI_INIT_TIME);
	#endif
	// app_wifi_init();

	#if (CONFIG_WIFI_ENABLE)
	wifi_init_config_t wifi_config = WIFI_DEFAULT_INIT_CONFIG();
	BK_LOG_ON_ERR(bk_event_init());
	BK_LOG_ON_ERR(bk_netif_init());
	BK_LOG_ON_ERR(bk_wifi_init(&wifi_config));
	#endif

	#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_FINISH_WIFI_INIT_TIME);
	#endif

	extern void rtos_user_app_launch_over(void);
	rtos_user_app_launch_over();

#ifdef CONFIG_MEDIA
	extern bk_err_t media_major_init(void);
	media_major_init();
#endif


#if (CONFIG_BLUETOOTH)
	// app_ble_init();
	#if CONFIG_BLE
    bk_ble_init_ext(NULL);
	#endif
#endif

#elif (CONFIG_SLAVE_CORE)

#else
	app_sdio_init();
	app_key_init();
	app_usb_charge_init();
	// app_wifi_init();
	#if (CONFIG_WIFI_ENABLE)
	wifi_init_config_t wifi_config = WIFI_DEFAULT_INIT_CONFIG();
	BK_LOG_ON_ERR(bk_event_init());
	BK_LOG_ON_ERR(bk_netif_init());
	BK_LOG_ON_ERR(bk_wifi_init(&wifi_config));
	#endif
#if (CONFIG_BLUETOOTH)
	app_bt_init();
	// app_ble_init();
	#if CONFIG_BLE
    bk_ble_init_ext(NULL);
	#endif
#endif
	app_mp3_player_init();
	app_uart_debug_init_todo();
#endif

	// app_cli_init();

#if defined(CONFIG_MEDIA) && defined(CONFIG_SLAVE_CORE)
	media_minor_init();
#endif

#if (CONFIG_FREERTOS)
#if CONFIG_SEMI_HOSTED
	semi_host_init();
#endif
#endif

#if CONFIG_UDISK_MP3
	um_init();
#endif

#if (CONFIG_OTA_UPDATE_DEFAULT_PARTITION&& CONFIG_HTTP_AB_PARTITION)
    #ifdef CONFIG_OTA_UPDATE_B_PARTITION
    os_printf("exec part a\r\n");
    bk_ota_confirm_update_partition(CONFIRM_EXEC_A);
    #else
    os_printf("exec part b\r\n");
    bk_ota_confirm_update_partition(CONFIRM_EXEC_B);
    #endif
#endif
	return 0;
}



void __main_task(void*arg){
	extern void app_main(void);
    
    app_main();

	vTaskDelete(NULL);
}

int main(void){

	legacy_init();

	// __init();
	xTaskCreate(__main_task, "__main_task", 1024, NULL, 4, NULL);

	return 0;
}