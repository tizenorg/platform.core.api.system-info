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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <dlog.h>

#include <system_info.h>
#include <system_info_private.h>
#include <sys/utsname.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

#define SYSTEM_INFO_MAX -1

typedef struct {
	system_info_key_e key;
	system_info_data_type_e data_type;
	system_info_get_value_cb get_value_cb;
} system_info_s;

typedef system_info_s * system_info_h;

system_info_s system_info_table[] = {

{
	 /**< The model of the device */
	SYSTEM_INFO_KEY_MODEL,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_model
},

{
	 /**< The version of the Tizen supported by the platform */
	SYSTEM_INFO_KEY_TIZEN_VERSION,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_tizen_version
},

{
	/**< The height of the screen in pixels */
	SYSTEM_INFO_KEY_SCREEN_HEIGHT,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_screen_height
},

{
	/**< The width of the screen in pixels */
	SYSTEM_INFO_KEY_SCREEN_WIDTH,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_screen_width
},

{
	/**< The Name of the Platform */
	SYSTEM_INFO_KEY_PLATFORM_NAME,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_platform_name
},

{
	/**< The Name of the Tizen version */
	SYSTEM_INFO_KEY_TIZEN_VERSION_NAME,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_tizen_version_name
},

{
	/**< The CORE CPU architecture of model */
	SYSTEM_INFO_KEY_CORE_CPU_ARCH,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_core_cpu_arch
},

{
	/**< The CORE CPU frequency of model */
	SYSTEM_INFO_KEY_CORE_CPU_FREQ,
	SYSTEM_INFO_DATA_TYPE_DOUBLE,
	system_info_get_core_cpu_freq
},

{
	/**< The height of the physical screen size in millimeters */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_HEIGHT,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_physical_screen_height
},

{
	/**< The width of the physical screen size in millimeters */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_WIDTH,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_physical_screen_width
},

{
	/**< The build string of the platform binary */
	SYSTEM_INFO_KEY_BUILD_STRING,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_build_string
},

{
	/**< The build date of the platform binary */
	SYSTEM_INFO_KEY_BUILD_DATE,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_build_date
},

{
	/**< The build time of the platform binary */
	SYSTEM_INFO_KEY_BUILD_TIME,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_build_time
},

{
	/**< The manufacturer of the device */
	SYSTEM_INFO_KEY_MANUFACTURER,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_manufacturer
},

{
	/**< Indicates whether the device supports tethering */
	SYSTEM_INFO_KEY_TETHERING_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_tethering_supported
},

{
	SYSTEM_INFO_MAX, -1, NULL
}

};

static system_info_mode_type_e system_info_system_info_model_type;

system_info_mode_type_e system_info_get_system_info_model_type(void)
{
	return system_info_system_info_model_type;
}

void __attribute__((constructor)) system_info_init(void)
{
	int ret, len, i;
	char *str = NULL;

	ret = system_info_get_platform_string("tizen.org/system/model_name", &str);

	if (ret != SYSTEM_INFO_ERROR_NONE) {
		LOGE("initialize error");
		return;
	}

	if (!str) {
		LOGE("Failed to get model name");
		return;
	}

	len = strlen(str);
	for (i = 0 ; i < len ; i++) {
		str[i] = toupper(str[i]);
	}

	if (!strcmp(str, "EMULATOR"))
		system_info_system_info_model_type = SYSTEM_INFO_MODEL_TYPE_EMULATOR;
	else
		system_info_system_info_model_type = SYSTEM_INFO_MODEL_TYPE_TARGET;

	free(str);
}

static int system_info_get(system_info_key_e key, system_info_h *system_info)
{
	int index = 0;

	while (system_info_table[index].key != SYSTEM_INFO_MAX) {
		if (system_info_table[index].key == key) {
			*system_info = &system_info_table[index];
			return 0;
		}

		index++;
	}

	return -1;
}

int system_info_get_value(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	system_info_h system_info;
	system_info_get_value_cb system_info_getter;

	if (value == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x) : invalid output param", SYSTEM_INFO_ERROR_INVALID_PARAMETER);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	if (system_info_get(key, &system_info)) {
		LOGE("INVALID_PARAMETER(0x%08x) : invalid key", SYSTEM_INFO_ERROR_INVALID_PARAMETER);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	if (system_info->data_type != data_type) {
		LOGE("INVALID_PARAMETER(0x%08x) : invalid data type", SYSTEM_INFO_ERROR_INVALID_PARAMETER);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	system_info_getter = system_info->get_value_cb;

	if (system_info_getter == NULL) {
		LOGE("IO_ERROR(0x%08x) : failed to call getter for the system information", SYSTEM_INFO_ERROR_IO_ERROR);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return system_info_getter(key, system_info->data_type, value);
}

API int system_info_get_platform_bool(const char *key, bool *value)
{
	int ret;
	bool *supported;
	char *string = NULL;

	supported = (bool *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, BOOL_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s", key);
		return ret;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

API int system_info_get_platform_int(const char *key, int *value)
{
	int ret;
	int *ret_val;
	char *string = NULL;

	ret_val = (int *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, INT_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s", key);
		return ret;
	}

	*ret_val = atoi(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

API int system_info_get_platform_double(const char *key, double *value)
{
	int ret;
	double *ret_val;
	char *string = NULL;

	ret_val = (double *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, DBL_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s", key);
		return ret;
	}

	*ret_val = atof(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

API int system_info_get_platform_string(const char *key, char **value)
{
	int ret;
	char *string = NULL;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_file(key, (void**)&string);
	if (ret == 0) {
		*value = string;
		return SYSTEM_INFO_ERROR_NONE;
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, STR_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s", key);
		return ret;
	}

	*value = string;

	return SYSTEM_INFO_ERROR_NONE;
}

API int system_info_get_custom_bool(const char *key, bool *value)
{
	int ret;
	bool *supported;
	char *string = NULL;

	supported = (bool *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, BOOL_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s info from %s!!!", key, CONFIG_FILE_PATH);
		return ret;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

API int system_info_get_custom_int(const char *key, int *value)
{
	int ret;
	int *ret_val;
	char *string = NULL;

	ret_val = (int *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, INT_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s info from %s!!!", key, CONFIG_FILE_PATH);
		return ret;
	}

	*ret_val = atoi(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

API int system_info_get_custom_double(const char *key, double *value)
{
	int ret;
	double *ret_val;
	char *string = NULL;

	ret_val = (double *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, DBL_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s info from %s!!!", key, CONFIG_FILE_PATH);
		return ret;
	}

	*ret_val = atof(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

API int system_info_get_custom_string(const char *key, char **value)
{
	int ret;
	char *string = NULL;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, STR_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s info from %s!!!", key, CONFIG_FILE_PATH);
		return ret;
	}

	*value = string;

	return SYSTEM_INFO_ERROR_NONE;
}
