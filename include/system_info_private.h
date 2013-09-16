/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__
#define __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

#define INFO_FILE_PATH "/etc/info.ini"
#define OS_RELEASE_FILE_PATH "/etc/os-release"
#define CPU_INFO_FILE_PATH "/proc/cpuinfo"
#define CPU_INFO_MAX_FREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
#define CONFIG_FILE_PATH "/etc/config/model-config.xml"
#define MAXBUFSIZE 512

#define PLATFORM_TAG	"platform"
#define CUSTOM_TAG	"custom"
#define INTERNAL_TAG	"internal"

#define BOOL_TYPE	"bool"
#define INT_TYPE	"int"
#define DBL_TYPE	"double"
#define STR_TYPE	"string"

typedef enum {
	SYSTEM_INFO_DATA_TYPE_STRING,
	SYSTEM_INFO_DATA_TYPE_INT,
	SYSTEM_INFO_DATA_TYPE_DOUBLE,
	SYSTEM_INFO_DATA_TYPE_BOOL
} system_info_data_type_e;

/**
 * @brief Enumeration of Mode type
 */
typedef enum {
	SYSTEM_INFO_MODEL_TYPE_EMULATOR,
	SYSTEM_INFO_MODEL_TYPE_TARGET
} system_info_mode_type_e;

typedef int (*system_info_get_value_cb) (system_info_key_e key, system_info_data_type_e data_type, void **value);

system_info_mode_type_e system_info_get_system_info_model_type(void);

int system_info_ini_get_string(char *ini_file, char *key, char **output);
int system_info_get_value_from_xml(char *xml_file_path, char *model, char *id_field, char **value);
int system_info_get_value_from_config_xml(char *feature_tag, const char *name_field, char *type_field, char **value);

int system_info_vconf_get_value_int(const char *vconf_key, int *value);
int system_info_vconf_get_value_bool(const char *vconf_key, bool *value);
int system_info_vconf_get_value_double(const char *vconf_key, double *value);
int system_info_vconf_get_value_string(const char *vconf_key, char **value);

int system_info_get_model(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_tizen_version(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_platform_name(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_tizen_version_name(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_core_cpu_arch(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_core_cpu_freq(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_screen_height(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_screen_width(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_physical_screen_height(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_physical_screen_width(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_manufacturer(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_build_string(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_build_date(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_build_time(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_tethering_supported(system_info_key_e key, system_info_data_type_e data_type, void **value);
#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__ */
