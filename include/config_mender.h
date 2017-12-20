/*
  Copyright 2017 Northern.tech AS

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef HEADER_CONFIG_MENDER_H
#define HEADER_CONFIG_MENDER_H

/* Avoid Mender specific code if we are just probing for configuration. */
#ifndef MENDER_AUTO_PROBING

#include <config_mender_defines.h>

#if !defined(CONFIG_ENV_IS_IN_MMC) && !defined(CONFIG_ENV_IS_IN_NAND) && !defined(CONFIG_ENV_IS_IN_FLASH)
# error CONFIG_ENV_IS_IN_MMC, CONFIG_ENV_IS_IN_NAND or CONFIG_ENV_IS_IN_FLASH is required for Mender to work
#endif

#ifndef CONFIG_BOOTCOUNT_LIMIT
# error CONFIG_BOOTCOUNT_LIMIT is required for Mender to work
#endif

/* Currently Mender needs bootcount to reside in environment. */
#ifndef CONFIG_BOOTCOUNT_ENV
# error CONFIG_BOOTCOUNT_ENV is required for Mender to work
#endif

#ifdef CONFIG_SYS_MMC_ENV_DEV
# error CONFIG_SYS_MMC_ENV_DEV should not be defined explicitly (will be auto-configured).
#endif
#ifdef CONFIG_SYS_MMC_ENV_PART
# error CONFIG_SYS_MMC_ENV_PART should not be defined explicitly (will be auto-configured).
#endif
#ifdef CONFIG_ENV_OFFSET
# error CONFIG_ENV_OFFSET should not be defined explicitly (will be auto-configured).
#endif
#ifdef CONFIG_ENV_OFFSET_REDUND
# error CONFIG_ENV_OFFSET_REDUND should not be defined explicitly (will be auto-configured).
#endif

#if MENDER_BOOTENV_SIZE != CONFIG_ENV_SIZE
# error 'CONFIG_ENV_SIZE' define must be equal to bitbake variable 'BOOTENV_SIZE' set in U-Boot build recipe.
#endif

#ifdef MENDER_BOOTENV_RANGE

#ifndef CONFIG_ENV_RANGE
#error bitbake variable 'BOOTENV_RANGE' is set U-Boot recipe, but CONFIG_ENV_RANGE is not set in configuration
#endif  /* CONFIG_ENV_RANGE */

#if MENDER_BOOTENV_RANGE != CONFIG_ENV_RANGE
#error 'CONFIG_ENV_RANGE' define must be equal to bitbake variable 'BOOTENV_RANGE' set in U-Boot build recipe.
#endif

#else

#ifdef CONFIG_ENV_RANGE
#error CONFIG_ENV_RANGE is set but BOOTENV_RANGE is not configured in U-Boot build recipe.
#endif

#endif  /* MENDER_BOOTENV_RANGE */

#define CONFIG_SYS_MMC_ENV_DEV   MENDER_UBOOT_STORAGE_DEVICE
/* Use MMC partition zero to select whole user area of memory card. */
#define CONFIG_SYS_MMC_ENV_PART  0
#define CONFIG_ENV_OFFSET        MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_1
#define CONFIG_ENV_OFFSET_REDUND MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_2

#endif /* !MENDER_AUTO_PROBING */

#endif /* HEADER_CONFIG_MENDER_H */
