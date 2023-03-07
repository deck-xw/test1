#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>
#include "cli.h"
#include "media_cli.h"
#include "aud_api.h"
#include "lcd_act.h"
#include "gpio_driver.h"
//#include "media_cli_comm.h"
#include "media_app.h"
#include <driver/gpio.h>

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

void user_app_main(void){

}

extern void app_demo_softap_start(char *oob_ssid, char *connect_key);
extern void key_task_int(void);
int main(void)
{
#if (!CONFIG_SLAVE_CORE)
	rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
    //bk_set_printf_sync(true);
	//shell_set_log_level(BK_LOG_WARN);
#endif
	legacy_init();
    
/*	key_task_int();*/
#if 0
    gpio_dev_unmap(GPIO_36);
	bk_gpio_set_capacity(GPIO_36, 0);
	bk_gpio_enable_output(GPIO_36);
	bk_gpio_set_output_high(GPIO_36);
	rtos_delay_milliseconds(100);
    
	media_ppi_t ppi = PPI_864X480;//PPI_CAP_640X480;//PPI_864X480;
     app_camera_type_t camera_type =APP_CAMERA_UVC_MJPEG;// APP_CAMERA_DVP_JPEG;//APP_CAMERA_UVC_MJPEG;
    // //media_app_register_uvc_connect_state_cb(uvc_connect_state_callback);
	 media_app_camera_open(camera_type, ppi);

	lcd_open_t lcd_open;
	lcd_open.device_ppi = PPI_480X854;//PPI_480X272;
	lcd_open.device_name = "nt35510";
	media_app_lcd_rotate(true);
	media_app_lcd_open(&lcd_open);
	rtos_delay_milliseconds(100);

#endif	 

    

	123
    

 /*   
	extern void lcd_driver_clear_screen(uint16_t x_size,uint16_t y_size,uint16_t color); //y是行数，x是每行的像素
    lcd_driver_clear_screen(480,480,0x001f);
	rtos_delay_milliseconds(2000);
	lcd_driver_clear_screen(480,480,0x07e0);
    rtos_delay_milliseconds(2000);
	lcd_driver_clear_screen(480,480,0xf800);
  */  
	
	return 0;
}