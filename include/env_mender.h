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

#ifndef HEADER_ENV_MENDER_H
#define HEADER_ENV_MENDER_H

/* Avoid Mender specific code if we are just probing for configuration. */
#ifdef MENDER_AUTO_PROBING
#define MENDER_ENV_SETTINGS
#else

#include <config_mender_defines.h>

#ifdef MENDER_NO_DEFAULT_ALTBOOTCMD
# define MENDER_DEFAULT_ALTBOOTCMD
#else
# define MENDER_DEFAULT_ALTBOOTCMD "altbootcmd=run mender_altbootcmd; " \
    "run bootcmd\0"
#endif

#ifdef MENDER_UBI
# define MENDER_UBI_SETTINGS \
    "mender_mtd_ubi_dev_name=" MENDER_MTD_UBI_DEVICE_NAME "\0"
#else
# define MENDER_UBI_SETTINGS
#endif

#define MENDER_ENV_SETTINGS                                             \
    MENDER_DEFAULT_ALTBOOTCMD                                           \
    MENDER_UBI_SETTINGS                                                 \
                                                                        \
    "bootlimit=1\0"                                                     \
    "bootcount=0\0"                                                     \
                                                                        \
    "upgrade_available=0\0"                                             \
                                                                        \
    "mender_boot_part=" __stringify(MENDER_ROOTFS_PART_A_NUMBER) "\0"   \
                                                                        \
    "mender_boot_part_hex=" __stringify(MENDER_ROOTFS_PART_A_NUMBER_HEX) "\0"   \
                                                                        \
    "mender_uboot_boot=" MENDER_UBOOT_STORAGE_INTERFACE " " __stringify(MENDER_UBOOT_STORAGE_DEVICE) ":" __stringify(MENDER_BOOT_PART_NUMBER_HEX) "\0" \
                                                                        \
    "mender_uboot_if=" MENDER_UBOOT_STORAGE_INTERFACE "\0"              \
                                                                        \
    "mender_uboot_dev=" __stringify(MENDER_UBOOT_STORAGE_DEVICE) "\0"   \
                                                                        \
    "mender_boot_kernel_type=" MENDER_BOOT_KERNEL_TYPE "\0"             \
                                                                        \
    "mender_kernel_name=" MENDER_KERNEL_NAME "\0"                       \
                                                                        \
    "mender_dtb_name=" MENDER_DTB_NAME "\0"                             \
                                                                        \
    "mender_pre_setup_commands=" MENDER_UBOOT_PRE_SETUP_COMMANDS "\0"   \
                                                                        \
    "mender_post_setup_commands=" MENDER_UBOOT_POST_SETUP_COMMANDS "\0" \
                                                                        \
    "mender_check_saveenv_canary=1\0"                                   \
                                                                        \
    "mender_setup="                                                     \
    "if test \"${mender_saveenv_canary}\" != \"1\"; then "              \
    "setenv mender_saveenv_canary 1; "                                  \
    "saveenv; "                                                         \
    "fi; "                                                              \
    "if test \"${mender_pre_setup_commands}\" != \"\"; "                \
    "then "                                                             \
    "run mender_pre_setup_commands; "                                   \
    "fi; "                                                              \
    "setenv mender_kernel_root " MENDER_STORAGE_DEVICE_BASE "${mender_boot_part}; "    \
    "if test ${mender_boot_part} = " __stringify(MENDER_ROOTFS_PART_A_NUMBER) "; "     \
    "then "                                                                            \
    "setenv mender_boot_part_name " MENDER_ROOTFS_PART_A_NAME "; "                     \
    "else "                                                                             \
    "setenv mender_boot_part_name " MENDER_ROOTFS_PART_B_NAME "; "                     \
    "fi; "                                                                             \
    "setenv mender_kernel_root_name ${mender_boot_part_name}; "         \
    "setenv mender_uboot_root " MENDER_UBOOT_STORAGE_INTERFACE " " __stringify(MENDER_UBOOT_STORAGE_DEVICE) ":${mender_boot_part_hex}; " \
    "setenv mender_uboot_root_name ${mender_boot_part_name}; "          \
    "setenv expand_bootargs \"setenv bootargs \\\\\"${bootargs}\\\\\"\"; "              \
    "run expand_bootargs; "                                             \
    "setenv expand_bootargs; "                                          \
    "if test \"${mender_post_setup_commands}\" != \"\"; "               \
    "then "                                                             \
    "run mender_post_setup_commands; "                                  \
    "fi\0"                                                              \
                                                                        \
    "mender_altbootcmd="                                                \
    "if test ${mender_boot_part} = " __stringify(MENDER_ROOTFS_PART_A_NUMBER) "; "     \
    "then "                                                                            \
    "setenv mender_boot_part " __stringify(MENDER_ROOTFS_PART_B_NUMBER) "; "           \
    "setenv mender_boot_part_hex " __stringify(MENDER_ROOTFS_PART_B_NUMBER_HEX) "; "           \
    "else "                                                                            \
    "setenv mender_boot_part " __stringify(MENDER_ROOTFS_PART_A_NUMBER) "; "           \
    "setenv mender_boot_part_hex " __stringify(MENDER_ROOTFS_PART_A_NUMBER_HEX) "; "           \
    "fi; "                                                              \
    "setenv upgrade_available 0; "                                      \
    "saveenv; "                                                         \
    "run mender_setup\0"                                                \
                                                                        \
    "mender_try_to_recover="                                            \
    "if test ${upgrade_available} = 1; "                                \
    "then reset; "                                                      \
    "fi\0"

#ifdef MENDER_UBI
# define MENDER_BOOTARGS                                                \
    "setenv bootargs root=${mender_kernel_root} "                       \
    "${mtdparts} "                                                      \
    "ubi.mtd=${mender_mtd_ubi_dev_name} "                               \
    "rootfstype=ubifs "                                                 \
    "${bootargs}; "
# define MENDER_LOAD_KERNEL_AND_FDT                                     \
    "ubi part ${mender_mtd_ubi_dev_name}; "                             \
    "ubifsmount ${mender_uboot_root_name}; "                            \
    "if test \"${fdt_addr_r}\" != \"\"; then "                          \
    "ubifsload ${fdt_addr_r} /boot/${mender_dtb_name}; "                \
    "fi; "                                                              \
    "ubifsload ${kernel_addr_r} /boot/${mender_kernel_name}; "
#else
# define MENDER_BOOTARGS                                                \
    "setenv bootargs root=${mender_kernel_root} ${bootargs}; "
# define MENDER_LOAD_KERNEL_AND_FDT                                     \
    "if test \"${fdt_addr_r}\" != \"\"; then "                          \
    "load ${mender_uboot_root} ${fdt_addr_r} /boot/${mender_dtb_name}; " \
    "fi; "                                                              \
    "load ${mender_uboot_root} ${kernel_addr_r} /boot/${mender_kernel_name}; "
#endif

#define CONFIG_MENDER_BOOTCOMMAND                                       \
    "run mender_setup; "                                                \
    MENDER_BOOTARGS                                                     \
    MENDER_LOAD_KERNEL_AND_FDT                                          \
    "${mender_boot_kernel_type} ${kernel_addr_r} - ${fdt_addr_r}; "     \
    "run mender_try_to_recover"

#endif /* !MENDER_AUTO_PROBING */

#endif /* HEADER_ENV_MENDER_H */
