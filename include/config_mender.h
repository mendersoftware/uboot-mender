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

#ifdef CONFIG_ENV_IS_NOWHERE
# error A CONFIG_ENV_IS_IN_<storage-type> define is required for Mender to work. For standard Mender setups this should be CONFIG_ENV_IS_IN_MMC for HD/SSD/MMC/SD storage setup, and CONFIG_ENV_IS_IN_UBI for Flash/UBI storage.
#endif

#ifndef CONFIG_BOOTCOUNT_LIMIT
# error CONFIG_BOOTCOUNT_LIMIT is required for Mender to work. Make sure that: 1) All the instructions at docs.mender.io/devices/integrating-with-u-boot have been followed. 2) All required layers are included in bblayers.conf, including any board specific layers such as meta-mender-<board>
#endif

/* Currently Mender needs bootcount to reside in environment. */
#ifndef CONFIG_BOOTCOUNT_ENV
# error CONFIG_BOOTCOUNT_ENV is required for Mender to work. Make sure that: 1) All the instructions at docs.mender.io/devices/integrating-with-u-boot have been followed. 2) All required layers are included in bblayers.conf, including any board specific layers such as meta-mender-<board>
#endif

#ifdef MENDER_UBI
# ifndef CONFIG_MTDIDS_DEFAULT
#  define CONFIG_MTDIDS_DEFAULT MENDER_MTDIDS
# endif
# ifndef CONFIG_MTDPARTS_DEFAULT
#  define CONFIG_MTDPARTS_DEFAULT "mtdparts=" ## MENDER_MTDPARTS
# endif
# ifndef CONFIG_ENV_UBI_PART
#  define CONFIG_ENV_UBI_PART          MENDER_MTD_UBI_DEVICE_NAME
# endif
# ifndef CONFIG_ENV_UBI_VOLUME
#  define CONFIG_ENV_UBI_VOLUME        "u-boot-env-1"
# endif
# ifndef CONFIG_ENV_UBI_VOLUME_REDUND
#  define CONFIG_ENV_UBI_VOLUME_REDUND "u-boot-env-2"
# endif
#else
# if defined(CONFIG_ENV_OFFSET)
#  if CONFIG_ENV_OFFSET != MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_1
#   error CONFIG_ENV_OFFSET is not the same as MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_1. Either set it to the same value (check for example in the defconfig file), or make sure it is not defined at all. Make sure that: 1) All the instructions at docs.mender.io/devices/integrating-with-u-boot have been followed. 2) All required layers are included in bblayers.conf, including any board specific layers such as meta-mender-<board>
#  endif
# else
#  define CONFIG_ENV_OFFSET MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_1
# endif
# if defined(CONFIG_ENV_OFFSET_REDUND)
#  if CONFIG_ENV_OFFSET_REDUND != MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_2
#   error CONFIG_ENV_OFFSET_REDUND is not the same as MENDER_ENV_OFFSET_REDUND. Either set it to the same value (check for example in the defconfig file), or make sure it is not defined at all. Make sure that: 1) All the instructions at docs.mender.io/devices/integrating-with-u-boot have been followed. 2) All required layers are included in bblayers.conf, including any board specific layers such as meta-mender-<board>
#  endif
# else
#  define CONFIG_ENV_OFFSET_REDUND MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_2
# endif
# ifdef CONFIG_SYS_MMC_ENV_DEV
#  if CONFIG_SYS_MMC_ENV_DEV != MENDER_UBOOT_STORAGE_DEVICE
#   error CONFIG_SYS_MMC_ENV_DEV is not the same as MENDER_UBOOT_STORAGE_DEVICE. Either set it to the same value (check for example in the defconfig file), or make sure it is not defined at all. Make sure that: 1) All the instructions at docs.mender.io/devices/integrating-with-u-boot have been followed. 2) All required layers are included in bblayers.conf, including any board specific layers such as meta-mender-<board>
#  endif
# else
#  define CONFIG_SYS_MMC_ENV_DEV   MENDER_UBOOT_STORAGE_DEVICE
# endif
# ifdef CONFIG_SYS_MMC_ENV_PART
#  if CONFIG_SYS_MMC_ENV_PART != 0
#   error CONFIG_SYS_MMC_ENV_PART need to be set to 0. Make sure that: 1) All the instructions at docs.mender.io/devices/integrating-with-u-boot have been followed. 2) All required layers are included in bblayers.conf, including any board specific layers such as meta-mender-<board>
#  endif
# else
   /* Use MMC partition zero to select whole user area of memory card. */
#  define CONFIG_SYS_MMC_ENV_PART  0
# endif
#endif

#ifdef CONFIG_ENV_SIZE
# if MENDER_BOOTENV_SIZE != CONFIG_ENV_SIZE
#  error 'CONFIG_ENV_SIZE' define must be equal to bitbake variable 'BOOTENV_SIZE' set in U-Boot build recipe.
# endif
#else
# define CONFIG_ENV_SIZE MENDER_BOOTENV_SIZE
#endif

#endif /* !MENDER_AUTO_PROBING */

#endif /* HEADER_CONFIG_MENDER_H */
