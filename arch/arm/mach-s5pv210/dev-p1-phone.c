/* linux/arch/arm/mach-s5pv210/dev-p1-phone.c
 * Copyright (C) 2010 Samsung Electronics. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/irq.h>

#include <mach/map.h>
#include <mach/gpio.h>
#if defined(CONFIG_TARGET_LOCALE_KOR)
#include <mach/gpio-p1.kor.h>
#else
#include <mach/gpio-p1.h>
#endif

#if !defined(CONFIG_KOR_MODEL_M180K) && !defined(CONFIG_KOR_MODEL_M180W)

#include "../../../drivers/misc/samsung_modemctl/onedram/onedram.h"
#include "../../../drivers/misc/samsung_modemctl/modemctl/modemctl.h"

/* onedram */
static void onedram_cfg_gpio(void)
{
	s3c_gpio_cfgpin(GPIO_nINT_ONEDRAM_AP, S3C_GPIO_SFN(GPIO_nINT_ONEDRAM_AP_AF));
	s3c_gpio_setpull(GPIO_nINT_ONEDRAM_AP, S3C_GPIO_PULL_UP);
	set_irq_type(GPIO_nINT_ONEDRAM_AP, IRQ_TYPE_LEVEL_LOW);
}

static struct onedram_platform_data onedram_data = {
		.cfg_gpio = onedram_cfg_gpio,
		};

static struct resource onedram_res[] = {
	[0] = {
		.start = (S5PV210_PA_SDRAM + 0x05000000),
		.end = (S5PV210_PA_SDRAM + 0x05000000 + SZ_16M - 1),
		.flags = IORESOURCE_MEM,
		},
	[1] = {
		.start = IRQ_EINT11,
		.end = IRQ_EINT11,
		.flags = IORESOURCE_IRQ,
		},
	};

static struct platform_device onedram = {
		.name = "onedram",
		.id = -1,
		.num_resources = ARRAY_SIZE(onedram_res),
		.resource = onedram_res,
		.dev = {
			.platform_data = &onedram_data,
			},
		};

/* Modem control */

static void modemctl_cfg_gpio(void);

static struct modemctl_platform_data mdmctl_data = {
#if defined(CONFIG_KOR_MODEL_M180S) || defined(CONFIG_KOR_MODEL_M180L)
	.name = "msm", // QC : "msm", Infinion : "xmm"
	.gpio_phone_on = GPIO_PHONE_ON,
	.gpio_phone_active = GPIO_PHONE_ACTIVE,
	.gpio_pda_active = GPIO_PDA_ACTIVE,
	.gpio_cp_reset = GPIO_CP_RST,
	.gpio_sim_ndetect = GPIO_SIM_nDETECT,
	.gpio_usim_boot = GPIO_USIM_BOOT,
	.gpio_flm_sel = GPIO_FLM_SEL,
	.cfg_gpio = modemctl_cfg_gpio,
#else
	.name = "xmm",
	.gpio_phone_on = NULL,
	.gpio_phone_active = GPIO_PHONE_ACTIVE,
	.gpio_pda_active = GPIO_PDA_ACTIVE,
	.gpio_cp_reset = GPIO_CP_RST,
	.gpio_reset_req_n = GPIO_RESET_REQ_N,
	.gpio_sim_ndetect = GPIO_SIM_nDETECT,
	.cfg_gpio = modemctl_cfg_gpio,
#endif
	};

static struct resource mdmctl_res[] = {
	[0] = {
		.start = IRQ_EINT15,
		.end = IRQ_EINT15,
		.flags = IORESOURCE_IRQ,
		},
	[1] = {
		.start = IRQ_EINT(27),
		.end = IRQ_EINT(27),
		.flags = IORESOURCE_IRQ,
		},
	};

static struct platform_device modemctl = {
		.name = "modemctl",
		.id = -1,
		.num_resources = ARRAY_SIZE(mdmctl_res),
		.resource = mdmctl_res,
		.dev = {
			.platform_data = &mdmctl_data,
			},
		};

static void modemctl_cfg_gpio(void)
{
	int err = 0;
	
	unsigned gpio_phone_on = mdmctl_data.gpio_phone_on;
	unsigned gpio_phone_active = mdmctl_data.gpio_phone_active;
	unsigned gpio_cp_rst = mdmctl_data.gpio_cp_reset;
	unsigned gpio_pda_active = mdmctl_data.gpio_pda_active;
	unsigned gpio_sim_ndetect = mdmctl_data.gpio_sim_ndetect;
#if defined(CONFIG_KOR_MODEL_M180S) || defined(CONFIG_KOR_MODEL_M180L)
	unsigned gpio_usim_boot = mdmctl_data.gpio_usim_boot;
	unsigned gpio_flm_sel = mdmctl_data.gpio_flm_sel;
#endif

#if defined(CONFIG_KOR_MODEL_M180S) || defined(CONFIG_KOR_MODEL_M180L)
	err = gpio_request(gpio_phone_on, "PHONE_ON");
	if (err) {
		printk("fail to request gpio %s\n","PHONE_ON");
	} else {
		gpio_direction_output(gpio_phone_on, GPIO_LEVEL_HIGH);
		s3c_gpio_setpull(gpio_phone_on, S3C_GPIO_PULL_NONE);
	}
#endif
	err = gpio_request(gpio_cp_rst, "CP_RST");
	if (err) {
		printk("fail to request gpio %s\n","CP_RST");
	} else {
		gpio_direction_output(gpio_cp_rst, GPIO_LEVEL_LOW);
		s3c_gpio_setpull(gpio_cp_rst, S3C_GPIO_PULL_NONE);
	}
	err = gpio_request(gpio_pda_active, "PDA_ACTIVE");
	if (err) {
		printk("fail to request gpio %s\n","PDA_ACTIVE");
	} else {
		gpio_direction_output(gpio_pda_active, GPIO_LEVEL_LOW);
		s3c_gpio_setpull(gpio_pda_active, S3C_GPIO_PULL_NONE);
	}
#if defined(CONFIG_KOR_MODEL_M180S) || defined(CONFIG_KOR_MODEL_M180L)
	err = gpio_request(gpio_usim_boot, "USIM_BOOT");
	if (err) {
		printk("fail to request gpio %s\n","USIM_BOOT");
	} else {
		gpio_direction_output(gpio_usim_boot, GPIO_LEVEL_LOW);
		s3c_gpio_setpull(gpio_usim_boot, S3C_GPIO_PULL_NONE);
	}
	err = gpio_request(gpio_flm_sel, "FLM_SEL");
	if (err) {
		printk("fail to request gpio %s\n","FLM_SEL");
	} else {
		gpio_direction_output(gpio_flm_sel, GPIO_LEVEL_LOW);
		s3c_gpio_setpull(gpio_flm_sel, S3C_GPIO_PULL_NONE);
	}
#endif

	if (mdmctl_data.gpio_reset_req_n) {
		err = gpio_request(mdmctl_data.gpio_reset_req_n, "RST_REQN");
		if (err) {
			printk("fail to request gpio %s\n","RST_REQN");
		} else
			gpio_direction_output(mdmctl_data.gpio_reset_req_n, GPIO_LEVEL_LOW);
	}

	s3c_gpio_cfgpin(gpio_phone_active, S3C_GPIO_SFN(0xF));
#if defined(CONFIG_KOR_MODEL_M180S) || defined(CONFIG_KOR_MODEL_M180L)
	s3c_gpio_setpull(gpio_phone_active, S3C_GPIO_PULL_DOWN);
#else
	s3c_gpio_setpull(gpio_phone_active, S3C_GPIO_PULL_NONE);
#endif
	set_irq_type(gpio_phone_active, IRQ_TYPE_EDGE_BOTH);

	s3c_gpio_cfgpin(gpio_sim_ndetect, S3C_GPIO_SFN(0xF));
	s3c_gpio_setpull(gpio_sim_ndetect, S3C_GPIO_PULL_NONE);
	set_irq_type(gpio_sim_ndetect, IRQ_TYPE_EDGE_BOTH);
}
#endif /* #if defined(CONFIG_KOR_MODEL_M180K) || defined(CONFIG_KOR_MODEL_M180W)*/

static int __init p1_init_phone_interface(void)
{
#if defined(CONFIG_KOR_MODEL_M180K) || defined(CONFIG_KOR_MODEL_M180W)
	return 0; // not telephony model
#else
	platform_device_register(&modemctl);
	platform_device_register(&onedram);
	return 0;
#endif
}
device_initcall(p1_init_phone_interface);
