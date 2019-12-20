/*
 * config_defaults.h - sane defaults for everyone
 *
 * Copyright (c) 2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _CONFIG_DEFAULTS_H_
#define _CONFIG_DEFAULTS_H_

/* Support bootm-ing different OSes */
#define CONFIG_BOOTM_LINUX 1
#define CONFIG_BOOTM_NETBSD 1
#define CONFIG_BOOTM_PLAN9 1
#define CONFIG_BOOTM_RTEMS 1
#define CONFIG_BOOTM_VXWORKS 1

#define CONFIG_GZIP 1
#define CONFIG_ZLIB 1

#endif
#define CONFIG_ENV_SIZE 0x20000
#define CONFIG_ENV_OFFSET 0x800000
#define CONFIG_ENV_OFFSET_REDUND 0x1000000
#ifdef CONFIG_ROOTFS_EMMC
#  define CONFIG_SYS_MMC_ENV_DEV 1
#else
#  define CONFIG_SYS_MMC_ENV_DEV 0
#endif
#define CONFIG_SYS_MMC_ENV_PART 0
