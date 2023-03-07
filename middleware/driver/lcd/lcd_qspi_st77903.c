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


#include <common/bk_include.h>
#include <driver/lcd_qspi_types.h>
#include "bk_misc.h"


#define LCD_QSPI_ST77903_REGISTER_WRITE_COMMAND		0xDE
#define LCD_QSPI_ST77903_REGISTER_READ_COMMAND		0xDD
#define LCD_QSPI_ST77903_HSYNC_COMMAND				0x60
#define LCD_QSPI_ST77903_VSYNC_COMMAND				0x61
#define LCD_QSPI_ST77903_VSW	(1)
#define LCD_QSPI_ST77903_HFP	(4)
#define LCD_QSPI_ST77903_HBP	(4)


#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
extern beken_semaphore_t lcd_qspi_semaphore;
#endif

static const lcd_qspi_init_cmd_t st77903_init_cmds[] = {
	{0xf0, {0xc3}, 1},
    {0xf0, {0x96}, 1},
    {0xf0, {0xa5}, 1},
    {0xe9, {0x20}, 1},
    {0xe7, {0x80, 0x77, 0x1f, 0xcc}, 4},
    {0xc1, {0x77, 0x07, 0xc2, 0x07}, 4},
    {0xc2, {0x77, 0x07, 0xc2, 0x07}, 4},
    {0xc3, {0x22, 0x02, 0x22, 0x04}, 4},
    {0xc4, {0x22, 0x02, 0x22, 0x04}, 4},
    {0xc5, {0x71}, 1},
    {0xe0, {0x87, 0x09, 0x0c, 0x06, 0x05, 0x03, 0x29, 0x32, 0x49, 0x0f, 0x1b, 0x17, 0x2a, 0x2f}, 14},
    {0xe1, {0x87, 0x09, 0x0c, 0x06, 0x05, 0x03, 0x29, 0x32, 0x49, 0x0f, 0x1b, 0x17, 0x2a, 0x2f}, 14},
    {0xe5, {0xb2, 0xf5, 0xbd, 0x24, 0x22, 0x25, 0x10, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}, 14},
    {0xe6, {0xb2, 0xf5, 0xbd, 0x24, 0x22, 0x25, 0x10, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}, 14},
    {0xec, {0x40, 0x03}, 2},
    {0x36, {0x04}, 1},
    {0x3a, {0x07}, 1},
    {0xb2, {0x00}, 1},
    {0xb3, {0x01}, 1},
    {0xb4, {0x00}, 1},
    {0xb5, {0x00, 0x08, 0x00, 0x08}, 4},
    {0xb6, {0xc7, 0x31}, 2},
    {0xa5, {0x00, 0x00, 0x00, 0x00, 0x20, 0x15, 0x2a, 0x8a, 0x02}, 9},
    {0xa6, {0x00, 0x00, 0x00, 0x00, 0x20, 0x15, 0x2a, 0x8a, 0x02}, 9},
    {0xba, {0x0a, 0x5a, 0x23, 0x10, 0x25, 0x02, 0x00}, 7},
    {0xbb, {0x00, 0x30, 0x00, 0x29, 0x88, 0x87, 0x18, 0x00}, 8},
    {0xbc, {0x00, 0x30, 0x00, 0x29, 0x88, 0x87, 0x18, 0x00}, 8},
    {0xbd, {0xa1, 0xb2, 0x2b, 0x1a, 0x56, 0x43, 0x34, 0x65, 0xff, 0xff, 0x0f}, 11},
    {0x35, {0x00}, 1},
    {0x21, {0x00}, 0},
    {0x11, {0x00}, 0},
	{0xff, {0x78}, 1},
    {0x29, {0x00}, 0},
    {0xff, {0x78}, 1},
//	{0xfe, {0x00}, 0},
//	{0xee, {0x00}, 0},
//	{0xb0, {0xa5}, 1},
//	{0xcc, {0x40, 0x00, 0x3f, 0x00, 0x14, 0x14, 0x20, 0x20, 0x03}, 9},
};

static uint8_t st77903_cmd[4] = {0xDE, 0x00, 0x60, 0x00};

//st77903 screen without display ram, and need to send data continuously.
const lcd_qspi_device_t lcd_qspi_device_st77903 =
{
	.id = LCD_QSPI_DEVICE_ST77903,
	.name = "st77903",
	.ppi = PPI_400X400,
	.refresh_method = LCD_QSPI_REFRESH_BY_LINE,
	.reg_write_cmd = LCD_QSPI_ST77903_REGISTER_WRITE_COMMAND,
	.reg_read_config = {0},
	.pixel_write_config.cmd = st77903_cmd,
	.pixel_write_config.cmd_len = sizeof(st77903_cmd),
	.init_cmd = st77903_init_cmds,
	.device_init_cmd_len = sizeof(st77903_init_cmds) / sizeof(lcd_qspi_init_cmd_t),
	.backlight_init = NULL,
	.backlight_set = NULL,
	.backlight_deinit = NULL,
	.refresh_config.hsync_cmd = LCD_QSPI_ST77903_HSYNC_COMMAND,
	.refresh_config.vsync_cmd = LCD_QSPI_ST77903_VSYNC_COMMAND,
	.refresh_config.vsw = LCD_QSPI_ST77903_VSW,
	.refresh_config.hfp = LCD_QSPI_ST77903_HFP,
	.refresh_config.hbp = LCD_QSPI_ST77903_HBP,
	.frame_len = (PPI_400X400 >> 16) * (PPI_400X400 & 0xFFFF) * 3,
};

