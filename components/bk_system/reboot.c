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
#include <components/log.h>
#include <common/bk_err.h>
#include <components/system.h>
#include <driver/wdt.h>
#include "bk_misc.h"
#include "reset_reason.h"
#include "drv_model_pub.h"
#include "param_config.h"
#include "aon_pmu_driver.h"

#define TAG "sys"

extern volatile unsigned int g_enter_exception;

#if (CONFIG_SOC_BK7256XX)
static uint32_t bk_get_return_address_value()
{
	UINT32 return_address_value;

	__asm volatile( "mv %0, ra":"=r"( return_address_value ) );

	return return_address_value;
}
#endif

void bk_reboot_ex(uint32_t reset_reason)
{
	UINT32 wdt_val = 5;

	BK_LOGI(TAG, "bk_reboot\r\n");

#if (CONFIG_SOC_BK7256XX)
	BK_DUMP_OUT("sys: Return address value:%x\r\n", bk_get_return_address_value());
	set_reboot_tag(REBOOT_TAG_REQ);
	g_enter_exception = 1;
#endif
#if (CONFIG_SYSTEM_CTRL)
	uint32_t param =0;
	param = aon_pmu_drv_reg_get(PMU_REG0x41);
	param &= ~0x3; //select clk_DIVD as lpo_src
	aon_pmu_drv_reg_set(PMU_REG0x41,param);
	aon_pmu_drv_wdt_rst_dev_enable();
#endif

	if (reset_reason < RESET_SOURCE_UNKNOWN) {
		bk_misc_set_reset_reason(reset_reason);
	}

	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();

	bk_wdt_stop();
	BK_LOGI(TAG, "wdt reboot\r\n");
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7256XX)
	delay_ms(100); //add delay for bk_writer BEKEN_DO_REBOOT cmd
#endif
#if CONFIG_AON_RTC_KEEP_TIME_SUPPORT
{
	/* 
	 * NOTES:special requirements
	 * Some customers system reboot, needs to reserve the network time,
	 * maybe after reboot, the network can't work at once.
	 * so before reboot, save the network time to flash.
	 */
	extern bk_err_t aon_rtc_enter_reboot(void);
	aon_rtc_enter_reboot();
}
#endif
	bk_wdt_start(wdt_val);
	while (1);
	GLOBAL_INT_RESTORE();
}

void bk_reboot(void)
{
	bk_reboot_ex(RESET_SOURCE_REBOOT);
}
