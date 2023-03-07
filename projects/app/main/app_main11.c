#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>


#include <components/log.h>
#include "cli.h"
#include "media_cli.h"
#include "aud_api.h"

//#include "media_cli_comm.h"
#include "media_app.h"

#include <driver/dvp_camera.h>
#include <driver/jpeg_enc.h>
#include "lcd_act.h"
#include "storage_act.h"
#include <components/aud_intf.h>
#include <driver/uvc_camera.h>
#include <driver/gpio.h>
#include "wlan_ui_pub.h"
#include "gpio_driver.h"

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

void user_app_main(void){

}

static int wifi_connect = 0;
static void app_demo_softap_rw_event_func(void *new_evt)
{
    wifi_linkstate_reason_t info = *((wifi_linkstate_reason_t *)new_evt);

    if (info.state == WIFI_LINKSTATE_AP_CONNECTED)
    {
        wifi_connect = 1;
        os_printf("WIFI_LINKSTATE_AP_CONNECTED\r\n");
    }
    else if (info.state == WIFI_LINKSTATE_AP_DISCONNECTED)
    {
        wifi_connect = 0;
        os_printf("WIFI_LINKSTATE_AP_DISCONNECTED\r\n");
    }
}

int main(void)
{
#if (!CONFIG_SLAVE_CORE)
	rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
    bk_set_printf_sync(true);
	shell_set_log_level(BK_LOG_WARN);
#endif
	legacy_init();
	
#if 0
    lcd_open_t lcd_open;
	lcd_open.device_ppi = PPI_480X272;
	lcd_open.device_name = NULL;
	media_app_lcd_open(&lcd_open);

	media_ppi_t ppi = PPI_640X480;
	app_camera_type_t camera_type = APP_CAMERA_UVC_MJPEG;
    media_app_camera_open(camera_type, ppi);
#endif		
#if 1
    gpio_dev_unmap(GPIO_36);
	bk_gpio_set_capacity(GPIO_36, 0);
	bk_gpio_enable_output(GPIO_36);
	bk_gpio_set_output_high(GPIO_36);
	rtos_delay_milliseconds(100);

    // lcd_open_t lcd_open;
	// lcd_open.device_ppi = PPI_480X272;
	// lcd_open.device_name = NULL;
	// media_app_lcd_open(&lcd_open);

    bk_wlan_status_register_cb(app_demo_softap_rw_event_func);
    
    //extern void cmd_video_transfer_test(void);
    //cmd_video_transfer_test();
    
    extern void demo_softap_app_init(char *ap_ssid, char *ap_key, char *ap_channel);
    demo_softap_app_init("XW-TEST","12345678",NULL);

    while(1)
    {
        if(wifi_connect)
           break;
        rtos_delay_milliseconds(1000);
    }

    extern bk_err_t demo_doorbell_udp_init(void);
    demo_doorbell_udp_init();
#endif
	return 0;
}
