// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <driver/gpio.h>
#include <driver/media_types.h>
#include <driver/lcd_types.h>
#include <driver/lcd_spi.h>
#include "bk_misc.h"
#include "lcd_devices.h"
#include "gpio_driver.h"


static const lcd_rgb_t lcd_rgb =
{
	.clk = LCD_20M,
	.data_out_clk_edge = NEGEDGE_OUTPUT,

	.hsync_pulse_width = 2,
	.vsync_pulse_width = 2,
	.hsync_back_porch = 40,  //3
	.hsync_front_porch = 5,  //2
	.vsync_back_porch = 8,   //1
	.vsync_front_porch = 8,  //1
};

static void lcd_rm68172_config(void)
{
	bk_gpio_set_output_low(LCD_SPI_RST);
	delay_ms(15);
	bk_gpio_set_output_high(LCD_SPI_RST);
	delay_ms(10);

	lcd_spi_write_hf_word_cmd(0xF000); lcd_spi_write_hf_word_data(0x55);
	lcd_spi_write_hf_word_cmd(0xF001); lcd_spi_write_hf_word_data(0xAA);
	lcd_spi_write_hf_word_cmd(0xF002); lcd_spi_write_hf_word_data(0x52);
	lcd_spi_write_hf_word_cmd(0xF003); lcd_spi_write_hf_word_data(0x08);
	lcd_spi_write_hf_word_cmd(0xF004); lcd_spi_write_hf_word_data(0x02);

	lcd_spi_write_hf_word_cmd(0xF600); lcd_spi_write_hf_word_data(0x60);
	lcd_spi_write_hf_word_cmd(0xF601); lcd_spi_write_hf_word_data(0x40);

	lcd_spi_write_hf_word_cmd(0xFE00); lcd_spi_write_hf_word_data(0x01);
	lcd_spi_write_hf_word_cmd(0xFE01); lcd_spi_write_hf_word_data(0x80);
	lcd_spi_write_hf_word_cmd(0xFE02); lcd_spi_write_hf_word_data(0x09);
	lcd_spi_write_hf_word_cmd(0xFE03); lcd_spi_write_hf_word_data(0x09);

	lcd_spi_write_hf_word_cmd(0xF000); lcd_spi_write_hf_word_data(0x55);
	lcd_spi_write_hf_word_cmd(0xF001); lcd_spi_write_hf_word_data(0xAA);
	lcd_spi_write_hf_word_cmd(0xF002); lcd_spi_write_hf_word_data(0x52);
	lcd_spi_write_hf_word_cmd(0xF003); lcd_spi_write_hf_word_data(0x08);
	lcd_spi_write_hf_word_cmd(0xF004); lcd_spi_write_hf_word_data(0x01);

	lcd_spi_write_hf_word_cmd(0xB000); lcd_spi_write_hf_word_data(0x07);


	lcd_spi_write_hf_word_cmd(0xB100); lcd_spi_write_hf_word_data(0x07);


	lcd_spi_write_hf_word_cmd(0xB500); lcd_spi_write_hf_word_data(0x08);


	lcd_spi_write_hf_word_cmd(0xB600); lcd_spi_write_hf_word_data(0x54);


	lcd_spi_write_hf_word_cmd(0xB700); lcd_spi_write_hf_word_data(0x44);

	lcd_spi_write_hf_word_cmd(0xB800); lcd_spi_write_hf_word_data(0x24);

	lcd_spi_write_hf_word_cmd(0xB900); lcd_spi_write_hf_word_data(0x34);

	lcd_spi_write_hf_word_cmd(0xBA00); lcd_spi_write_hf_word_data(0x14);

	lcd_spi_write_hf_word_cmd(0xBC00); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xBC01); lcd_spi_write_hf_word_data(0x78);
	lcd_spi_write_hf_word_cmd(0xBC02); lcd_spi_write_hf_word_data(0x13);

	lcd_spi_write_hf_word_cmd(0xBD00); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xBD01); lcd_spi_write_hf_word_data(0x78);
	lcd_spi_write_hf_word_cmd(0xBD02); lcd_spi_write_hf_word_data(0x13);

	lcd_spi_write_hf_word_cmd(0xBE00); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xBE01); lcd_spi_write_hf_word_data(0x26);

	//gamma
	lcd_spi_write_hf_word_cmd(0xD100); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD101); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD102); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD103); lcd_spi_write_hf_word_data(0x17 );
	lcd_spi_write_hf_word_cmd(0xD104); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD105); lcd_spi_write_hf_word_data(0x3E );
	lcd_spi_write_hf_word_cmd(0xD106); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD107); lcd_spi_write_hf_word_data(0x5E );
	lcd_spi_write_hf_word_cmd(0xD108); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD109); lcd_spi_write_hf_word_data(0x7B );
	lcd_spi_write_hf_word_cmd(0xD10A); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD10B); lcd_spi_write_hf_word_data(0xA9 );
	lcd_spi_write_hf_word_cmd(0xD10C); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD10D); lcd_spi_write_hf_word_data(0xCE );
	lcd_spi_write_hf_word_cmd(0xD10E); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD10F); lcd_spi_write_hf_word_data(0x0A );
	lcd_spi_write_hf_word_cmd(0xD110); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD111); lcd_spi_write_hf_word_data(0x37 );
	lcd_spi_write_hf_word_cmd(0xD112); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD113); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD114); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD115); lcd_spi_write_hf_word_data(0xB0 );
	lcd_spi_write_hf_word_cmd(0xD116); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD117); lcd_spi_write_hf_word_data(0xFF );
	lcd_spi_write_hf_word_cmd(0xD118); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD119); lcd_spi_write_hf_word_data(0x3D );
	lcd_spi_write_hf_word_cmd(0xD11A); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD11B); lcd_spi_write_hf_word_data(0x3F );
	lcd_spi_write_hf_word_cmd(0xD11C); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD11D); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD11E); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD11F); lcd_spi_write_hf_word_data(0xC4 );
	lcd_spi_write_hf_word_cmd(0xD120); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD121); lcd_spi_write_hf_word_data(0xF6 );
	lcd_spi_write_hf_word_cmd(0xD122); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD123); lcd_spi_write_hf_word_data(0x3A );
	lcd_spi_write_hf_word_cmd(0xD124); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD125); lcd_spi_write_hf_word_data(0x68 );
	lcd_spi_write_hf_word_cmd(0xD126); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD127); lcd_spi_write_hf_word_data(0xA0 );
	lcd_spi_write_hf_word_cmd(0xD128); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD129); lcd_spi_write_hf_word_data(0xBF );
	lcd_spi_write_hf_word_cmd(0xD12A); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD12B); lcd_spi_write_hf_word_data(0xE0 );
	lcd_spi_write_hf_word_cmd(0xD12C); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD12D); lcd_spi_write_hf_word_data(0xEC );
	lcd_spi_write_hf_word_cmd(0xD12E); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD12F); lcd_spi_write_hf_word_data(0xF5 );
	lcd_spi_write_hf_word_cmd(0xD130); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD131); lcd_spi_write_hf_word_data(0xFA );
	lcd_spi_write_hf_word_cmd(0xD132); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD133); lcd_spi_write_hf_word_data(0xFF );

	lcd_spi_write_hf_word_cmd(0xD200); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD201); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD202); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD203); lcd_spi_write_hf_word_data(0x17 );
	lcd_spi_write_hf_word_cmd(0xD204); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD205); lcd_spi_write_hf_word_data(0x3E );
	lcd_spi_write_hf_word_cmd(0xD206); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD207); lcd_spi_write_hf_word_data(0x5E );
	lcd_spi_write_hf_word_cmd(0xD208); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD209); lcd_spi_write_hf_word_data(0x7B );
	lcd_spi_write_hf_word_cmd(0xD20A); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD20B); lcd_spi_write_hf_word_data(0xA9 );
	lcd_spi_write_hf_word_cmd(0xD20C); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD20D); lcd_spi_write_hf_word_data(0xCE );
	lcd_spi_write_hf_word_cmd(0xD20E); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD20F); lcd_spi_write_hf_word_data(0x0A );
	lcd_spi_write_hf_word_cmd(0xD210); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD211); lcd_spi_write_hf_word_data(0x37 );
	lcd_spi_write_hf_word_cmd(0xD212); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD213); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD214); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD215); lcd_spi_write_hf_word_data(0xB0 );
	lcd_spi_write_hf_word_cmd(0xD216); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD217); lcd_spi_write_hf_word_data(0xFF );
	lcd_spi_write_hf_word_cmd(0xD218); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD219); lcd_spi_write_hf_word_data(0x3D );
	lcd_spi_write_hf_word_cmd(0xD21A); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD21B); lcd_spi_write_hf_word_data(0x3F );
	lcd_spi_write_hf_word_cmd(0xD21C); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD21D); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD21E); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD21F); lcd_spi_write_hf_word_data(0xC4 );
	lcd_spi_write_hf_word_cmd(0xD220); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD221); lcd_spi_write_hf_word_data(0xF6 );
	lcd_spi_write_hf_word_cmd(0xD222); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD223); lcd_spi_write_hf_word_data(0x3A );
	lcd_spi_write_hf_word_cmd(0xD224); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD225); lcd_spi_write_hf_word_data(0x68 );
	lcd_spi_write_hf_word_cmd(0xD226); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD227); lcd_spi_write_hf_word_data(0xA0 );
	lcd_spi_write_hf_word_cmd(0xD228); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD229); lcd_spi_write_hf_word_data(0xBF );
	lcd_spi_write_hf_word_cmd(0xD22A); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD22B); lcd_spi_write_hf_word_data(0xE0 );
	lcd_spi_write_hf_word_cmd(0xD22C); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD22D); lcd_spi_write_hf_word_data(0xEC );
	lcd_spi_write_hf_word_cmd(0xD22E); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD22F); lcd_spi_write_hf_word_data(0xF5 );
	lcd_spi_write_hf_word_cmd(0xD230); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD231); lcd_spi_write_hf_word_data(0xFA );
	lcd_spi_write_hf_word_cmd(0xD232); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD233); lcd_spi_write_hf_word_data(0xFF );

	lcd_spi_write_hf_word_cmd(0xD300); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD301); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD302); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD303); lcd_spi_write_hf_word_data(0x17 );
	lcd_spi_write_hf_word_cmd(0xD304); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD305); lcd_spi_write_hf_word_data(0x3E );
	lcd_spi_write_hf_word_cmd(0xD306); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD307); lcd_spi_write_hf_word_data(0x5E );
	lcd_spi_write_hf_word_cmd(0xD308); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD309); lcd_spi_write_hf_word_data(0x7B );
	lcd_spi_write_hf_word_cmd(0xD30A); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD30B); lcd_spi_write_hf_word_data(0xA9 );
	lcd_spi_write_hf_word_cmd(0xD30C); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD30D); lcd_spi_write_hf_word_data(0xCE );
	lcd_spi_write_hf_word_cmd(0xD30E); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD30F); lcd_spi_write_hf_word_data(0x0A );
	lcd_spi_write_hf_word_cmd(0xD310); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD311); lcd_spi_write_hf_word_data(0x37 );
	lcd_spi_write_hf_word_cmd(0xD312); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD313); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD314); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD315); lcd_spi_write_hf_word_data(0xB0 );
	lcd_spi_write_hf_word_cmd(0xD316); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD317); lcd_spi_write_hf_word_data(0xFF );
	lcd_spi_write_hf_word_cmd(0xD318); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD319); lcd_spi_write_hf_word_data(0x3D );
	lcd_spi_write_hf_word_cmd(0xD31A); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD31B); lcd_spi_write_hf_word_data(0x3F );
	lcd_spi_write_hf_word_cmd(0xD31C); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD31D); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD31E); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD31F); lcd_spi_write_hf_word_data(0xC4 );
	lcd_spi_write_hf_word_cmd(0xD320); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD321); lcd_spi_write_hf_word_data(0xF6 );
	lcd_spi_write_hf_word_cmd(0xD322); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD323); lcd_spi_write_hf_word_data(0x3A );
	lcd_spi_write_hf_word_cmd(0xD324); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD325); lcd_spi_write_hf_word_data(0x68 );
	lcd_spi_write_hf_word_cmd(0xD326); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD327); lcd_spi_write_hf_word_data(0xA0 );
	lcd_spi_write_hf_word_cmd(0xD328); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD329); lcd_spi_write_hf_word_data(0xBF );
	lcd_spi_write_hf_word_cmd(0xD32A); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD32B); lcd_spi_write_hf_word_data(0xE0 );
	lcd_spi_write_hf_word_cmd(0xD32C); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD32D); lcd_spi_write_hf_word_data(0xEC );
	lcd_spi_write_hf_word_cmd(0xD32E); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD32F); lcd_spi_write_hf_word_data(0xF5 );
	lcd_spi_write_hf_word_cmd(0xD330); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD331); lcd_spi_write_hf_word_data(0xFA );
	lcd_spi_write_hf_word_cmd(0xD332); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD333); lcd_spi_write_hf_word_data(0xFF );

	lcd_spi_write_hf_word_cmd(0xD400); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD401); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD402); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD403); lcd_spi_write_hf_word_data(0x17 );
	lcd_spi_write_hf_word_cmd(0xD404); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD405); lcd_spi_write_hf_word_data(0x3E );
	lcd_spi_write_hf_word_cmd(0xD406); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD407); lcd_spi_write_hf_word_data(0x5E );
	lcd_spi_write_hf_word_cmd(0xD408); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD409); lcd_spi_write_hf_word_data(0x7B );
	lcd_spi_write_hf_word_cmd(0xD40A); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD40B); lcd_spi_write_hf_word_data(0xA9 );
	lcd_spi_write_hf_word_cmd(0xD40C); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD40D); lcd_spi_write_hf_word_data(0xCE );
	lcd_spi_write_hf_word_cmd(0xD40E); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD40F); lcd_spi_write_hf_word_data(0x0A );
	lcd_spi_write_hf_word_cmd(0xD410); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD411); lcd_spi_write_hf_word_data(0x37 );
	lcd_spi_write_hf_word_cmd(0xD412); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD413); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD414); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD415); lcd_spi_write_hf_word_data(0xB0 );
	lcd_spi_write_hf_word_cmd(0xD416); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD417); lcd_spi_write_hf_word_data(0xFF );
	lcd_spi_write_hf_word_cmd(0xD418); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD419); lcd_spi_write_hf_word_data(0x3D );
	lcd_spi_write_hf_word_cmd(0xD41A); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD41B); lcd_spi_write_hf_word_data(0x3F );
	lcd_spi_write_hf_word_cmd(0xD41C); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD41D); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD41E); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD41F); lcd_spi_write_hf_word_data(0xC4 );
	lcd_spi_write_hf_word_cmd(0xD420); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD421); lcd_spi_write_hf_word_data(0xF6 );
	lcd_spi_write_hf_word_cmd(0xD422); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD423); lcd_spi_write_hf_word_data(0x3A );
	lcd_spi_write_hf_word_cmd(0xD424); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD425); lcd_spi_write_hf_word_data(0x68 );
	lcd_spi_write_hf_word_cmd(0xD426); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD427); lcd_spi_write_hf_word_data(0xA0 );
	lcd_spi_write_hf_word_cmd(0xD428); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD429); lcd_spi_write_hf_word_data(0xBF );
	lcd_spi_write_hf_word_cmd(0xD42A); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD42B); lcd_spi_write_hf_word_data(0xE0 );
	lcd_spi_write_hf_word_cmd(0xD42C); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD42D); lcd_spi_write_hf_word_data(0xEC );
	lcd_spi_write_hf_word_cmd(0xD42E); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD42F); lcd_spi_write_hf_word_data(0xF5 );
	lcd_spi_write_hf_word_cmd(0xD430); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD431); lcd_spi_write_hf_word_data(0xFA );
	lcd_spi_write_hf_word_cmd(0xD432); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD433); lcd_spi_write_hf_word_data(0xFF );

	lcd_spi_write_hf_word_cmd(0xD500); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD501); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD502); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD503); lcd_spi_write_hf_word_data(0x17 );
	lcd_spi_write_hf_word_cmd(0xD504); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD505); lcd_spi_write_hf_word_data(0x3E );
	lcd_spi_write_hf_word_cmd(0xD506); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD507); lcd_spi_write_hf_word_data(0x5E );
	lcd_spi_write_hf_word_cmd(0xD508); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD509); lcd_spi_write_hf_word_data(0x7B );
	lcd_spi_write_hf_word_cmd(0xD50A); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD50B); lcd_spi_write_hf_word_data(0xA9 );
	lcd_spi_write_hf_word_cmd(0xD50C); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD50D); lcd_spi_write_hf_word_data(0xCE );
	lcd_spi_write_hf_word_cmd(0xD50E); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD50F); lcd_spi_write_hf_word_data(0x0A );
	lcd_spi_write_hf_word_cmd(0xD510); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD511); lcd_spi_write_hf_word_data(0x37 );
	lcd_spi_write_hf_word_cmd(0xD512); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD513); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD514); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD515); lcd_spi_write_hf_word_data(0xB0 );
	lcd_spi_write_hf_word_cmd(0xD516); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD517); lcd_spi_write_hf_word_data(0xFF );
	lcd_spi_write_hf_word_cmd(0xD518); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD519); lcd_spi_write_hf_word_data(0x3D );
	lcd_spi_write_hf_word_cmd(0xD51A); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD51B); lcd_spi_write_hf_word_data(0x3F );
	lcd_spi_write_hf_word_cmd(0xD51C); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD51D); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD51E); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD51F); lcd_spi_write_hf_word_data(0xC4 );
	lcd_spi_write_hf_word_cmd(0xD520); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD521); lcd_spi_write_hf_word_data(0xF6 );
	lcd_spi_write_hf_word_cmd(0xD522); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD523); lcd_spi_write_hf_word_data(0x3A );
	lcd_spi_write_hf_word_cmd(0xD524); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD525); lcd_spi_write_hf_word_data(0x68 );
	lcd_spi_write_hf_word_cmd(0xD526); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD527); lcd_spi_write_hf_word_data(0xA0 );
	lcd_spi_write_hf_word_cmd(0xD528); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD529); lcd_spi_write_hf_word_data(0xBF );
	lcd_spi_write_hf_word_cmd(0xD52A); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD52B); lcd_spi_write_hf_word_data(0xE0 );
	lcd_spi_write_hf_word_cmd(0xD52C); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD52D); lcd_spi_write_hf_word_data(0xEC );
	lcd_spi_write_hf_word_cmd(0xD52E); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD52F); lcd_spi_write_hf_word_data(0xF5 );
	lcd_spi_write_hf_word_cmd(0xD530); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD531); lcd_spi_write_hf_word_data(0xFA );
	lcd_spi_write_hf_word_cmd(0xD532); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD533); lcd_spi_write_hf_word_data(0xFF );

	lcd_spi_write_hf_word_cmd(0xD600); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD601); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD602); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD603); lcd_spi_write_hf_word_data(0x17 );
	lcd_spi_write_hf_word_cmd(0xD604); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD605); lcd_spi_write_hf_word_data(0x3E );
	lcd_spi_write_hf_word_cmd(0xD606); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD607); lcd_spi_write_hf_word_data(0x5E );
	lcd_spi_write_hf_word_cmd(0xD608); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD609); lcd_spi_write_hf_word_data(0x7B );
	lcd_spi_write_hf_word_cmd(0xD60A); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD60B); lcd_spi_write_hf_word_data(0xA9 );
	lcd_spi_write_hf_word_cmd(0xD60C); lcd_spi_write_hf_word_data(0x00 );
	lcd_spi_write_hf_word_cmd(0xD60D); lcd_spi_write_hf_word_data(0xCE );
	lcd_spi_write_hf_word_cmd(0xD60E); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD60F); lcd_spi_write_hf_word_data(0x0A );
	lcd_spi_write_hf_word_cmd(0xD610); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD611); lcd_spi_write_hf_word_data(0x37 );
	lcd_spi_write_hf_word_cmd(0xD612); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD613); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD614); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD615); lcd_spi_write_hf_word_data(0xB0 );
	lcd_spi_write_hf_word_cmd(0xD616); lcd_spi_write_hf_word_data(0x01 );
	lcd_spi_write_hf_word_cmd(0xD617); lcd_spi_write_hf_word_data(0xFF );
	lcd_spi_write_hf_word_cmd(0xD618); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD619); lcd_spi_write_hf_word_data(0x3D );
	lcd_spi_write_hf_word_cmd(0xD61A); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD61B); lcd_spi_write_hf_word_data(0x3F );
	lcd_spi_write_hf_word_cmd(0xD61C); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD61D); lcd_spi_write_hf_word_data(0x7C );
	lcd_spi_write_hf_word_cmd(0xD61E); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD61F); lcd_spi_write_hf_word_data(0xC4 );
	lcd_spi_write_hf_word_cmd(0xD620); lcd_spi_write_hf_word_data(0x02 );
	lcd_spi_write_hf_word_cmd(0xD621); lcd_spi_write_hf_word_data(0xF6 );
	lcd_spi_write_hf_word_cmd(0xD622); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD623); lcd_spi_write_hf_word_data(0x3A );
	lcd_spi_write_hf_word_cmd(0xD624); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD625); lcd_spi_write_hf_word_data(0x68 );
	lcd_spi_write_hf_word_cmd(0xD626); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD627); lcd_spi_write_hf_word_data(0xA0 );
	lcd_spi_write_hf_word_cmd(0xD628); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD629); lcd_spi_write_hf_word_data(0xBF );
	lcd_spi_write_hf_word_cmd(0xD62A); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD62B); lcd_spi_write_hf_word_data(0xE0 );
	lcd_spi_write_hf_word_cmd(0xD62C); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD62D); lcd_spi_write_hf_word_data(0xEC );
	lcd_spi_write_hf_word_cmd(0xD62E); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD62F); lcd_spi_write_hf_word_data(0xF5 );
	lcd_spi_write_hf_word_cmd(0xD630); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD631); lcd_spi_write_hf_word_data(0xFA );
	lcd_spi_write_hf_word_cmd(0xD632); lcd_spi_write_hf_word_data(0x03 );
	lcd_spi_write_hf_word_cmd(0xD633); lcd_spi_write_hf_word_data(0xFF );

	//P3
	lcd_spi_write_hf_word_cmd(0xF000); lcd_spi_write_hf_word_data(0x55);
	lcd_spi_write_hf_word_cmd(0xF001); lcd_spi_write_hf_word_data(0xAA);
	lcd_spi_write_hf_word_cmd(0xF002); lcd_spi_write_hf_word_data(0x52);
	lcd_spi_write_hf_word_cmd(0xF003); lcd_spi_write_hf_word_data(0x08);
	lcd_spi_write_hf_word_cmd(0xF004); lcd_spi_write_hf_word_data(0x03);

	lcd_spi_write_hf_word_cmd(0xB000); lcd_spi_write_hf_word_data(0x05);
	lcd_spi_write_hf_word_cmd(0xB001); lcd_spi_write_hf_word_data(0x17);
	lcd_spi_write_hf_word_cmd(0xB002); lcd_spi_write_hf_word_data(0xF9);
	lcd_spi_write_hf_word_cmd(0xB003); lcd_spi_write_hf_word_data(0x53);
	lcd_spi_write_hf_word_cmd(0xB004); lcd_spi_write_hf_word_data(0x53);
	lcd_spi_write_hf_word_cmd(0xB005); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB006); lcd_spi_write_hf_word_data(0x30);


	lcd_spi_write_hf_word_cmd(0xB100); lcd_spi_write_hf_word_data(0x05);
	lcd_spi_write_hf_word_cmd(0xB101); lcd_spi_write_hf_word_data(0x17);
	lcd_spi_write_hf_word_cmd(0xB102); lcd_spi_write_hf_word_data(0xFB);
	lcd_spi_write_hf_word_cmd(0xB103); lcd_spi_write_hf_word_data(0x55);
	lcd_spi_write_hf_word_cmd(0xB104); lcd_spi_write_hf_word_data(0x53);
	lcd_spi_write_hf_word_cmd(0xB105); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB106); lcd_spi_write_hf_word_data(0x30);

	lcd_spi_write_hf_word_cmd(0xB200); lcd_spi_write_hf_word_data(0xFb);
	lcd_spi_write_hf_word_cmd(0xB201); lcd_spi_write_hf_word_data(0xFc);
	lcd_spi_write_hf_word_cmd(0xB202); lcd_spi_write_hf_word_data(0xFd);
	lcd_spi_write_hf_word_cmd(0xB203); lcd_spi_write_hf_word_data(0xFe);
	lcd_spi_write_hf_word_cmd(0xB204); lcd_spi_write_hf_word_data(0xF0);
	lcd_spi_write_hf_word_cmd(0xB205); lcd_spi_write_hf_word_data(0x53);
	lcd_spi_write_hf_word_cmd(0xB206); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB207); lcd_spi_write_hf_word_data(0xC5);
	lcd_spi_write_hf_word_cmd(0xB208); lcd_spi_write_hf_word_data(0x08);

	lcd_spi_write_hf_word_cmd(0xB300); lcd_spi_write_hf_word_data(0x5B);
	lcd_spi_write_hf_word_cmd(0xB301); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB302); lcd_spi_write_hf_word_data(0xFb);
	lcd_spi_write_hf_word_cmd(0xB303); lcd_spi_write_hf_word_data(0x5a);
	lcd_spi_write_hf_word_cmd(0xB304); lcd_spi_write_hf_word_data(0x5a);
	lcd_spi_write_hf_word_cmd(0xB305); lcd_spi_write_hf_word_data(0x0C);

	lcd_spi_write_hf_word_cmd(0xB400); lcd_spi_write_hf_word_data(0xFf);
	lcd_spi_write_hf_word_cmd(0xB401); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB402); lcd_spi_write_hf_word_data(0x01);
	lcd_spi_write_hf_word_cmd(0xB403); lcd_spi_write_hf_word_data(0x02);
	lcd_spi_write_hf_word_cmd(0xB404); lcd_spi_write_hf_word_data(0xc0);
	lcd_spi_write_hf_word_cmd(0xB405); lcd_spi_write_hf_word_data(0x40);
	lcd_spi_write_hf_word_cmd(0xB406); lcd_spi_write_hf_word_data(0x05);
	lcd_spi_write_hf_word_cmd(0xB407); lcd_spi_write_hf_word_data(0x08);
	lcd_spi_write_hf_word_cmd(0xB408); lcd_spi_write_hf_word_data(0x53);

	lcd_spi_write_hf_word_cmd(0xB500); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB501); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB502); lcd_spi_write_hf_word_data(0xFf);
	lcd_spi_write_hf_word_cmd(0xB503); lcd_spi_write_hf_word_data(0x83);
	lcd_spi_write_hf_word_cmd(0xB504); lcd_spi_write_hf_word_data(0x5f);
	lcd_spi_write_hf_word_cmd(0xB505); lcd_spi_write_hf_word_data(0x5e);
	lcd_spi_write_hf_word_cmd(0xB506); lcd_spi_write_hf_word_data(0x50);
	lcd_spi_write_hf_word_cmd(0xB507); lcd_spi_write_hf_word_data(0x50);
	lcd_spi_write_hf_word_cmd(0xB508); lcd_spi_write_hf_word_data(0x33);
	lcd_spi_write_hf_word_cmd(0xB509); lcd_spi_write_hf_word_data(0x33);
	lcd_spi_write_hf_word_cmd(0xB50A); lcd_spi_write_hf_word_data(0x55);

	lcd_spi_write_hf_word_cmd(0xB600); lcd_spi_write_hf_word_data(0xbc);
	lcd_spi_write_hf_word_cmd(0xB601); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB602); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB603); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB604); lcd_spi_write_hf_word_data(0x2a);
	lcd_spi_write_hf_word_cmd(0xB605); lcd_spi_write_hf_word_data(0x80);
	lcd_spi_write_hf_word_cmd(0xB606); lcd_spi_write_hf_word_data(0x00);

	lcd_spi_write_hf_word_cmd(0xB700); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB701); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB702); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB703); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB704); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB705); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB706); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB707); lcd_spi_write_hf_word_data(0x00);

	lcd_spi_write_hf_word_cmd(0xB800); lcd_spi_write_hf_word_data(0x11);
	lcd_spi_write_hf_word_cmd(0xB801); lcd_spi_write_hf_word_data(0x60);
	lcd_spi_write_hf_word_cmd(0xB802); lcd_spi_write_hf_word_data(0x00);

	lcd_spi_write_hf_word_cmd(0xB900); lcd_spi_write_hf_word_data(0x90);

	//
	lcd_spi_write_hf_word_cmd(0xBA00); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBA01); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBA02); lcd_spi_write_hf_word_data(0x08 );
	lcd_spi_write_hf_word_cmd(0xBA03); lcd_spi_write_hf_word_data(0xAC );
	lcd_spi_write_hf_word_cmd(0xBA04); lcd_spi_write_hf_word_data(0xE2 );
	lcd_spi_write_hf_word_cmd(0xBA05); lcd_spi_write_hf_word_data(0x64 );
	lcd_spi_write_hf_word_cmd(0xBA06); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBA07); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBA08); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBA09); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBA0A); lcd_spi_write_hf_word_data(0x47 );
	lcd_spi_write_hf_word_cmd(0xBA0B); lcd_spi_write_hf_word_data(0x3F );
	lcd_spi_write_hf_word_cmd(0xBA0C); lcd_spi_write_hf_word_data(0xDB );
	lcd_spi_write_hf_word_cmd(0xBA0D); lcd_spi_write_hf_word_data(0x91 );
	lcd_spi_write_hf_word_cmd(0xBA0E); lcd_spi_write_hf_word_data(0x54 );
	lcd_spi_write_hf_word_cmd(0xBA0F); lcd_spi_write_hf_word_data(0x44 );

	lcd_spi_write_hf_word_cmd(0xBB00); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBB01); lcd_spi_write_hf_word_data(0x43 );
	lcd_spi_write_hf_word_cmd(0xBB02); lcd_spi_write_hf_word_data(0x79 );
	lcd_spi_write_hf_word_cmd(0xBB03); lcd_spi_write_hf_word_data(0xFD );
	lcd_spi_write_hf_word_cmd(0xBB04); lcd_spi_write_hf_word_data(0xB5 );
	lcd_spi_write_hf_word_cmd(0xBB05); lcd_spi_write_hf_word_data(0x14 );
	lcd_spi_write_hf_word_cmd(0xBB06); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBB07); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBB08); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBB09); lcd_spi_write_hf_word_data(0x44 );
	lcd_spi_write_hf_word_cmd(0xBB0A); lcd_spi_write_hf_word_data(0x40 );
	lcd_spi_write_hf_word_cmd(0xBB0B); lcd_spi_write_hf_word_data(0x4A );
	lcd_spi_write_hf_word_cmd(0xBB0C); lcd_spi_write_hf_word_data(0xCE );
	lcd_spi_write_hf_word_cmd(0xBB0D); lcd_spi_write_hf_word_data(0x86 );
	lcd_spi_write_hf_word_cmd(0xBB0E); lcd_spi_write_hf_word_data(0x24 );
	lcd_spi_write_hf_word_cmd(0xBB0F); lcd_spi_write_hf_word_data(0x44 );

	lcd_spi_write_hf_word_cmd(0xBC00); lcd_spi_write_hf_word_data(0xE0);
	lcd_spi_write_hf_word_cmd(0xBC01); lcd_spi_write_hf_word_data(0x1F);
	lcd_spi_write_hf_word_cmd(0xBC02); lcd_spi_write_hf_word_data(0xF8);
	lcd_spi_write_hf_word_cmd(0xBC03); lcd_spi_write_hf_word_data(0x07);

	lcd_spi_write_hf_word_cmd(0xBD00); lcd_spi_write_hf_word_data(0xE0);
	lcd_spi_write_hf_word_cmd(0xBD01); lcd_spi_write_hf_word_data(0x1F);
	lcd_spi_write_hf_word_cmd(0xBD02); lcd_spi_write_hf_word_data(0xF8);
	lcd_spi_write_hf_word_cmd(0xBD03); lcd_spi_write_hf_word_data(0x07);
	//

	//P0
	lcd_spi_write_hf_word_cmd(0xF000); lcd_spi_write_hf_word_data(0x55);
	lcd_spi_write_hf_word_cmd(0xF001); lcd_spi_write_hf_word_data(0xAA);
	lcd_spi_write_hf_word_cmd(0xF002); lcd_spi_write_hf_word_data(0x52);
	lcd_spi_write_hf_word_cmd(0xF003); lcd_spi_write_hf_word_data(0x08);
	lcd_spi_write_hf_word_cmd(0xF004); lcd_spi_write_hf_word_data(0x00);

	lcd_spi_write_hf_word_cmd(0xB000); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xB001); lcd_spi_write_hf_word_data(0x10);
	//lcd_spi_write_hf_word_cmd(0xB002); lcd_spi_write_hf_word_data(0x0a);

	//lcd_spi_write_hf_word_cmd(0xB100); lcd_spi_write_hf_word_data(0xfc);

	lcd_spi_write_hf_word_cmd(0xB400); lcd_spi_write_hf_word_data(0x10);

	//lcd_spi_write_hf_word_cmd(0xBa00); lcd_spi_write_hf_word_data(0x01);

	lcd_spi_write_hf_word_cmd(0xb800); lcd_spi_write_hf_word_data(0x00);
	lcd_spi_write_hf_word_cmd(0xb801); lcd_spi_write_hf_word_data(0x13);
	lcd_spi_write_hf_word_cmd(0xb802); lcd_spi_write_hf_word_data(0x13);
	lcd_spi_write_hf_word_cmd(0xb803); lcd_spi_write_hf_word_data(0x13);

	lcd_spi_write_hf_word_cmd(0xB500); lcd_spi_write_hf_word_data(0x6B);//

	lcd_spi_write_hf_word_cmd(0xBC00); lcd_spi_write_hf_word_data(0x02);

	//lcd_spi_write_hf_word_cmd(0x3500); lcd_spi_write_hf_word_data(0x00);

	lcd_spi_write_hf_word_cmd(0x1100);

	delay_ms(120);

	lcd_spi_write_hf_word_cmd(0x2900);

	delay_ms(100);
	lcd_spi_write_hf_word_cmd(0x2c00);
    

}

static void lcd_rm68172_backlight_io_init(void)
{
	gpio_dev_unmap(LCD_BACKLIGHT_CTRL_GPIO);
	bk_gpio_set_capacity(LCD_BACKLIGHT_CTRL_GPIO, 0);
	BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_BACKLIGHT_CTRL_GPIO));
	BK_LOG_ON_ERR(bk_gpio_pull_down(LCD_BACKLIGHT_CTRL_GPIO));
}

static void lcd_rm68172_init(void)
{
	os_printf("lcd_rm68172_init---------\r\n");
	lcd_rm68172_backlight_io_init();
	lcd_spi_init_gpio();
	lcd_rm68172_config();
}

static void rm68172_lcd_backlight_open(void)
{
	BK_LOG_ON_ERR(bk_gpio_pull_up(LCD_BACKLIGHT_CTRL_GPIO));
	// pull up gpio, enable lcd backlight control
	bk_gpio_set_output_high(LCD_BACKLIGHT_CTRL_GPIO);
}

static void rm68172_lcd_backlight_close(void)
{
	bk_gpio_set_output_low(LCD_BACKLIGHT_CTRL_GPIO);
}

const lcd_device_t lcd_device_rm68172 =
{
	.id = LCD_DEVIDE_RM68172,
	.name = "rm68172",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_480X854,
	.rgb = &lcd_rgb,
	.backlight_open = rm68172_lcd_backlight_open,
	.backlight_set = NULL,
	.init = lcd_rm68172_init,
	.backlight_close = rm68172_lcd_backlight_close,
	.lcd_off = NULL,
};

