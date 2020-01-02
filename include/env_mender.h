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
#include <configs/ti_armv7_common.h>

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

#define MENDER_UNAME_BOOT \
	"mender_loadimage=load ${mender_uboot_root} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"mender_loadrd=load ${mender_uboot_root} ${rdaddr} ${bootdir}/${rdfile}; setenv rdsize ${filesize}\0" \
	"mender_loadfdt=echo loading ${fdtdir}/${fdtfile} ...; load ${mender_uboot_root} ${fdtaddr} ${fdtdir}/${fdtfile}\0" \
	"mender_loadoverlay=echo uboot_overlays: loading ${uboot_overlay} ...; " \
		"load ${mender_uboot_root} ${rdaddr} ${uboot_overlay}; " \
		"fdt addr ${fdtaddr}; fdt resize ${fdt_buffer}; " \
		"fdt apply ${rdaddr}; fdt resize ${fdt_buffer};\0" \
	"mender_virtualloadoverlay=if test -e ${mender_uboot_root} ${uboot_overlay}; then " \
				"run mender_loadoverlay;" \
			"else " \
				"echo uboot_overlays: unable to find [${mender_uboot_root} ${uboot_overlay}]...;" \
			"fi;\0" \
	"mender_capeloadoverlay=if test -e ${mender_uboot_root} ${uboot_overlay}; then " \
				"run mender_loadoverlay;" \
				"setenv cape_uboot bone_capemgr.uboot_capemgr_enabled=1; " \
			"else " \
				"echo uboot_overlays: unable to find [${mender_uboot_root} ${uboot_overlay}]...;" \
			"fi;\0" \
	"mender_uname_boot="\
		"setenv bootdir /boot; " \
		"setenv bootfile vmlinuz-${uname_r}; " \
                "echo checking ${mender_uboot_root} ${bootdir}/${bootfile} ...; "\
		"if test -e ${mender_uboot_root} ${bootdir}/${bootfile}; then " \
			"echo loading ${bootdir}/${bootfile} ...; "\
			"run mender_loadimage;" \
			"setenv fdtdir /boot/dtbs/${uname_r}; " \
			"echo debug: [enable_uboot_overlays=${enable_uboot_overlays}] ... ;" \
			"if test -n ${enable_uboot_overlays}; then " \
				"echo debug: [enable_uboot_cape_universal=${enable_uboot_cape_universal}] ... ;" \
				"if test -n ${enable_uboot_cape_universal}; then " \
					"echo debug: [uboot_base_dtb_univ=${uboot_base_dtb_univ}] ... ;" \
					"if test -n ${uboot_base_dtb_univ}; then " \
						"echo uboot_overlays: [uboot_base_dtb=${uboot_base_dtb_univ}] ... ;" \
						"if test -e ${mender_uboot_root} ${fdtdir}/${uboot_base_dtb_univ}; then " \
							"setenv fdtfile ${uboot_base_dtb_univ};" \
							"echo uboot_overlays: Switching too: dtb=${fdtfile} ...;" \
							"if test -n ${uboot_try_cape_universal}; then " \
								"env delete -f uboot_try_cape_universal; " \
							"fi;" \
							"setenv cape_uboot bone_capemgr.uboot_capemgr_enabled=1; " \
						"else " \
							"echo debug: unable to find [${uboot_base_dtb_univ}] using [${uboot_base_dtb}] instead ... ;" \
							"echo debug: [uboot_base_dtb_univ=${uboot_base_dtb}] ... ;" \
							"if test -n ${uboot_base_dtb}; then " \
								"echo uboot_overlays: [uboot_base_dtb=${uboot_base_dtb}] ... ;" \
								"if test -e ${mender_uboot_root} ${fdtdir}/${uboot_base_dtb}; then " \
									"setenv fdtfile ${uboot_base_dtb};" \
									"echo uboot_overlays: Switching too: dtb=${fdtfile} ...;" \
								"fi;" \
							"fi;" \
						"fi;" \
					"fi;" \
				"else " \
					"echo debug: [uboot_base_dtb_univ=${uboot_base_dtb}] ... ;" \
					"if test -n ${uboot_base_dtb}; then " \
						"echo uboot_overlays: [uboot_base_dtb=${uboot_base_dtb}] ... ;" \
						"if test -e ${mender_uboot_root} ${fdtdir}/${uboot_base_dtb}; then " \
							"setenv fdtfile ${uboot_base_dtb};" \
							"echo uboot_overlays: Switching too: dtb=${fdtfile} ...;" \
						"fi;" \
					"fi;" \
				"fi;" \
			"fi;" \
			"if test -e ${mender_uboot_root} ${fdtdir}/${fdtfile}; then " \
			        "echo debug: [loadfdt ${mender_uboot_root} ${fdtdir}/${fdtfile}];" \
				"run mender_loadfdt;" \
			"else " \
			        "echo debug: [cannot find ${mender_uboot_root} ${fdtdir}/${fdtfile}];" \
				"setenv fdtdir /usr/lib/linux-image-${uname_r}; " \
				"if test -e ${mender_uboot_root} ${fdtdir}/${fdtfile}; then " \
					"run mender_loadfdt;" \
				"else " \
					"setenv fdtdir /lib/firmware/${uname_r}/device-tree; " \
					"if test -e ${mender_uboot_root} ${fdtdir}/${fdtfile}; then " \
						"run mender_loadfdt;" \
					"else " \
						"setenv fdtdir /boot/dtb-${uname_r}; " \
						"if test -e ${mender_uboot_root} ${fdtdir}/${fdtfile}; then " \
							"run mender_loadfdt;" \
						"else " \
							"setenv fdtdir /boot/dtbs; " \
							"if test -e ${mender_uboot_root} ${fdtdir}/${fdtfile}; then " \
								"run mender_loadfdt;" \
							"else " \
								"setenv fdtdir /boot/dtb; " \
								"if test -e ${mender_uboot_root} ${fdtdir}/${fdtfile}; then " \
									"run mender_loadfdt;" \
								"else " \
									"setenv fdtdir /boot; " \
									"if test -e ${mender_uboot_root} ${fdtdir}/${fdtfile}; then " \
										"run mender_loadfdt;" \
									"else " \
										"if test -e ${mender_uboot_root} ${fdtfile}; then " \
											"run mender_loadfdt;" \
										"else " \
											"echo; echo unable to find [dtb=${fdtfile}] did you name it correctly? ...; " \
											"run failumsboot;" \
										"fi;" \
									"fi;" \
								"fi;" \
							"fi;" \
						"fi;" \
					"fi;" \
				"fi;" \
			"fi; " \
			"if test -n ${enable_uboot_overlays}; then " \
				"setenv fdt_buffer 0x60000;" \
				"if test -n ${uboot_fdt_buffer}; then " \
					"setenv fdt_buffer ${uboot_fdt_buffer};" \
				"fi;" \
				"echo uboot_overlays: [fdt_buffer=${fdt_buffer}] ... ;" \
				"if test -n ${uboot_silicon}; then " \
					"setenv uboot_overlay ${uboot_silicon}; " \
					"run mender_virtualloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_model}; then " \
					"setenv uboot_overlay ${uboot_model}; " \
					"run mender_virtualloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr0}; then " \
					"if test -n ${disable_uboot_overlay_addr0}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_overlay_addr0}] disabled by /boot/uEnv.txt [disable_uboot_overlay_addr0=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_overlay_addr0}; " \
						"run mender_capeloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr1}; then " \
					"if test -n ${disable_uboot_overlay_addr1}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_overlay_addr1}] disabled by /boot/uEnv.txt [disable_uboot_overlay_addr1=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_overlay_addr1}; " \
						"run mender_capeloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr2}; then " \
					"if test -n ${disable_uboot_overlay_addr2}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_overlay_addr2}] disabled by /boot/uEnv.txt [disable_uboot_overlay_addr2=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_overlay_addr2}; " \
						"run mender_capeloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr3}; then " \
					"if test -n ${disable_uboot_overlay_addr3}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_overlay_addr3}] disabled by /boot/uEnv.txt [disable_uboot_overlay_addr3=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_overlay_addr3}; " \
						"run mender_capeloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr4}; then " \
					"setenv uboot_overlay ${uboot_overlay_addr4}; " \
					"run mender_capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr5}; then " \
					"setenv uboot_overlay ${uboot_overlay_addr5}; " \
					"run mender_capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr6}; then " \
					"setenv uboot_overlay ${uboot_overlay_addr6}; " \
					"run mender_capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr7}; then " \
					"setenv uboot_overlay ${uboot_overlay_addr7}; " \
					"run mender_capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_emmc}; then " \
					"if test -n ${disable_uboot_overlay_emmc}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_emmc}] disabled by /boot/uEnv.txt [disable_uboot_overlay_emmc=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_emmc}; " \
						"run mender_virtualloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_video}; then " \
					"if test -n ${disable_uboot_overlay_video}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_video}] disabled by /boot/uEnv.txt [disable_uboot_overlay_video=1]...;" \
					"else " \
						"if test -n ${disable_uboot_overlay_audio}; then " \
							"echo uboot_overlays: uboot loading of [${uboot_video}] disabled by /boot/uEnv.txt [disable_uboot_overlay_audio=1]...;" \
							"setenv uboot_overlay ${uboot_video_naudio}; " \
							"run mender_virtualloadoverlay;" \
						"else " \
							"setenv uboot_overlay ${uboot_video}; " \
							"run mender_virtualloadoverlay;" \
						"fi;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_wireless}; then " \
					"if test -n ${disable_uboot_overlay_wireless}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_wireless}] disabled by /boot/uEnv.txt [disable_uboot_overlay_wireless=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_wireless}; " \
						"run mender_virtualloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_adc}; then " \
					"if test -n ${disable_uboot_overlay_adc}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_adc}] disabled by /boot/uEnv.txt [disable_uboot_overlay_adc=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_adc}; " \
						"run mender_virtualloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_pru}; then " \
					"setenv uboot_overlay ${uboot_overlay_pru}; " \
					"run mender_virtualloadoverlay;" \
				"fi;" \
				"if test -n ${dtb_overlay}; then " \
					"setenv uboot_overlay ${dtb_overlay}; " \
					"echo uboot_overlays: [dtb_overlay=${uboot_overlay}] ... ;" \
					"run mender_capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_detected_capes}; then " \
					"echo uboot_overlays: [uboot_detected_capes=${uboot_detected_capes_addr0}${uboot_detected_capes_addr1}${uboot_detected_capes_addr2}${uboot_detected_capes_addr3}] ... ;" \
					"setenv uboot_detected_capes uboot_detected_capes=${uboot_detected_capes_addr0}${uboot_detected_capes_addr1}${uboot_detected_capes_addr2}${uboot_detected_capes_addr3}; " \
				"fi;" \
				"if test -n ${uboot_try_cape_universal}; then " \
					"if test -n ${enable_uboot_cape_universal}; then " \
						"if test -n ${cape_uboot}; then " \
							"echo uboot_overlays: cape universal disabled, external cape enabled or detected...;" \
						"else " \
							"if test -n ${uboot_cape_universal_bbb}; then " \
								"if test -n ${disable_uboot_overlay_emmc}; then " \
									"if test -n ${disable_uboot_overlay_video}; then " \
										"setenv uboot_overlay /lib/firmware/univ-bbb-xxx-00A0.dtbo; " \
									"else " \
										"if test -n ${disable_uboot_overlay_audio}; then " \
											"setenv uboot_overlay /lib/firmware/univ-bbb-xVx-00A0.dtbo; " \
										"else " \
											"setenv uboot_overlay /lib/firmware/univ-bbb-xVA-00A0.dtbo; " \
										"fi;" \
									"fi;" \
								"else " \
									"if test -n ${disable_uboot_overlay_video}; then " \
										"setenv uboot_overlay /lib/firmware/univ-bbb-Exx-00A0.dtbo; " \
									"else " \
										"if test -n ${disable_uboot_overlay_audio}; then " \
											"setenv uboot_overlay /lib/firmware/univ-bbb-EVx-00A0.dtbo; " \
										"else " \
											"setenv uboot_overlay /lib/firmware/univ-bbb-EVA-00A0.dtbo; " \
										"fi;" \
									"fi;" \
								"fi;" \
								"run mender_capeloadoverlay;" \
							"fi;" \
							"if test -n ${uboot_cape_universal_bbg}; then " \
								"if test -n ${disable_uboot_overlay_emmc}; then " \
									"setenv uboot_overlay /lib/firmware/univ-bbb-xxx-00A0.dtbo; " \
								"else " \
									"setenv uboot_overlay /lib/firmware/univ-bbb-Exx-00A0.dtbo; " \
								"fi;" \
								"run mender_capeloadoverlay;" \
							"fi;" \
							"if test -n ${uboot_cape_universal_bbgw}; then " \
								"if test -n ${disable_uboot_overlay_emmc}; then " \
									"if test -n ${disable_uboot_overlay_wireless}; then " \
										"setenv uboot_overlay /lib/firmware/univ-bbgw-xx-00A0.dtbo; " \
									"else " \
										"setenv uboot_overlay /lib/firmware/univ-bbgw-xW-00A0.dtbo; " \
									"fi;" \
								"else " \
									"if test -n ${disable_uboot_overlay_wireless}; then " \
										"setenv uboot_overlay /lib/firmware/univ-bbgw-Ex-00A0.dtbo; " \
									"else " \
										"setenv uboot_overlay /lib/firmware/univ-bbgw-EW-00A0.dtbo; " \
									"fi;" \
								"fi;" \
								"run mender_capeloadoverlay;" \
							"fi;" \
						"fi;" \
					"else " \
						"echo uboot_overlays: add [enable_uboot_cape_universal=1] to /boot/uEnv.txt to enable...;" \
					"fi;" \
				"fi;" \
			"else " \
				"echo uboot_overlays: add [enable_uboot_overlays=1] to /boot/uEnv.txt to enable...;" \
			"fi;" \
			"setenv rdfile initrd.img-${uname_r}; " \
                        "echo loading ${bootdir}/${rdfile} ...; "\
                        "run mender_loadrd;" \
                        "bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}; " \
		"fi;\0" \

#define MENDER_ENV_SETTINGS                                             \
    MENDER_UBI_SETTINGS                                                 \
    MENDER_UNAME_BOOT                                                   \
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
    "run mender_uname_boot; "
#endif

#define CONFIG_MENDER_BOOTCOMMAND                                       \
    "run mender_setup; "                                                \
    MENDER_BOOTARGS                                                     \
    MENDER_LOAD_KERNEL_AND_FDT                                          \
    "${mender_boot_kernel_type} ${kernel_addr_r} - ${fdt_addr_r}; "     \
    "run mender_try_to_recover"

#endif /* !MENDER_AUTO_PROBING */

#endif /* HEADER_ENV_MENDER_H */
