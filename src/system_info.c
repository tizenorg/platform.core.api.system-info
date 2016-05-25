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

#include <system_info.h>
#include <system_info_private.h>
#include <sys/utsname.h>

#define SYSTEM_INFO_MAX -1

//LCOV_EXCL_START Not used function
API int system_info_get_value_int(system_info_key_e key, int *value)
{
	return SYSTEM_INFO_ERROR_NOT_SUPPORTED;
}

API int system_info_get_value_bool(system_info_key_e key, bool *value)
{
	return SYSTEM_INFO_ERROR_NOT_SUPPORTED;
}

API int system_info_get_value_double(system_info_key_e key, double *value)
{
	return SYSTEM_INFO_ERROR_NOT_SUPPORTED;
}

API int system_info_get_value_string(system_info_key_e key, char **value)
{
	return SYSTEM_INFO_ERROR_NOT_SUPPORTED;
}
//LCOV_EXCL_STOP

API int system_info_get_platform_bool(const char *key, bool *value)
{
	int ret;
	bool *supported;
	char *string = NULL;

	supported = (bool *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
//LCOV_EXCL_START System Error
		_E("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, BOOL_TYPE, &string);
	if (ret) {
		_E("cannot get %s", key);
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
//LCOV_EXCL_START System Error
		_E("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, INT_TYPE, &string);
	if (ret) {
		_E("cannot get %s", key);
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
//LCOV_EXCL_START System Error
		_E("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, DBL_TYPE, &string);
	if (ret) {
		_E("cannot get %s", key);
		return ret;
	}

//LCOV_EXCL_START Not supported feature
	*ret_val = atof(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
//LCOV_EXCL_STOP
}

API int system_info_get_platform_string(const char *key, char **value)
{
	int ret;
	char *string = NULL;

	if (access(CONFIG_FILE_PATH, R_OK)) {
//LCOV_EXCL_START System Error
		_E("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	ret = system_info_get_file(key, (void**)&string);
	if (ret == 0) {
		*value = string;
		return SYSTEM_INFO_ERROR_NONE;
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, STR_TYPE, &string);
	if (ret) {
		_E("cannot get %s", key);
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
//LCOV_EXCL_START System Error
		_E("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, BOOL_TYPE, &string);
	if (ret) {
		_I("cannot get %s info from %s!!!", key, CONFIG_FILE_PATH);
		ret = external_get_value(CUSTOM_TAG, key, BOOL_TYPE, &string);
		if (ret) {
			_E("Cannot find key (%s) in the plugin (%d)", key, ret);
			return ret;
		}
	}

//LCOV_EXCL_START Not supported feature
	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
//LCOV_EXCL_STOP
}

API int system_info_get_custom_int(const char *key, int *value)
{
	int ret;
	int *ret_val;
	char *string = NULL;

	ret_val = (int *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
//LCOV_EXCL_START System Error
		_E("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, INT_TYPE, &string);
	if (ret) {
		_I("cannot get %s info from %s!!!", key, CONFIG_FILE_PATH);
		ret = external_get_value(CUSTOM_TAG, key, INT_TYPE, &string);
		if (ret) {
			_E("Cannot find key (%s) in the plugin (%d)", key, ret);
			return ret;
		}
	}

//LCOV_EXCL_START Not supported feature
	*ret_val = atoi(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
//LCOV_EXCL_STOP
}

API int system_info_get_custom_double(const char *key, double *value)
{
	int ret;
	double *ret_val;
	char *string = NULL;

	ret_val = (double *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
//LCOV_EXCL_START System Error
		_E("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, DBL_TYPE, &string);
	if (ret) {
		_I("cannot get %s info from %s!!!", key, CONFIG_FILE_PATH);
		ret = external_get_value(CUSTOM_TAG, key, DBL_TYPE, &string);
		if (ret) {
			_E("Cannot find key (%s) in the plugin (%d)", key, ret);
			return ret;
		}
	}

//LCOV_EXCL_START Not supported feature
	*ret_val = atof(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
//LCOV_EXCL_STOP
}

API int system_info_get_custom_string(const char *key, char **value)
{
	int ret;
	char *string = NULL;

	if (access(CONFIG_FILE_PATH, R_OK)) {
//LCOV_EXCL_START System Error
		_E("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, STR_TYPE, &string);
	if (ret) {
		_I("cannot get %s info from %s!!!", key, CONFIG_FILE_PATH);
		ret = external_get_value(CUSTOM_TAG, key, STR_TYPE, &string);
		if (ret) {
			_E("Cannot find key (%s) in the plugin (%d)", key, ret);
			return ret;
		}
	}

//LCOV_EXCL_START Not supported feature
	*value = string;

	return SYSTEM_INFO_ERROR_NONE;
//LCOV_EXCL_STOP Not supported feature
}

//LCOV_EXCL_START Not used function
static int get_type_from_str(char *type)
{
	size_t len;

	if (!type)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	len = strlen(type) + 1;
	if (!strncmp(BOOL_TYPE, type, len))
		return SYSTEM_INFO_BOOL;
	if (!strncmp(INT_TYPE, type, len))
		return SYSTEM_INFO_INT;
	if (!strncmp(DBL_TYPE, type, len))
		return SYSTEM_INFO_DOUBLE;
	if (!strncmp(STR_TYPE, type, len))
		return SYSTEM_INFO_STRING;

	return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
}

static int system_info_get_type(const char *key,
		const char *tag, system_info_type_e *type)
{
	char type_str[16];
	int ret;

	if (!key || !tag || !type)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		_E("cannot find file %s!!!", CONFIG_FILE_PATH);
		if (errno == EPERM || errno == EACCES)
			return SYSTEM_INFO_ERROR_PERMISSION_DENIED;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_type_from_config_xml(tag,
			key, type_str, sizeof(type_str));
	if (ret != SYSTEM_INFO_ERROR_NONE) {
		if (!strncmp(tag, PLATFORM_TAG, sizeof(PLATFORM_TAG))) {
			ret = system_info_get_type_file(key);
			if (ret >= 0)
				goto out;
		}
		_E("Failed to get type of key (%s)", key);
		return ret;
	}

	ret = get_type_from_str(type_str);
	if (ret < 0) {
		_E("Invalid type (key:%s, type:%s)", key, type_str);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

out:
	*type = ret;

	return SYSTEM_INFO_ERROR_NONE;
}


API int system_info_get_platform_type(const char *key, system_info_type_e *type)
{
	return system_info_get_type(key, PLATFORM_TAG, type);
}

API int system_info_get_custom_type(const char *key, system_info_type_e *type)
{
	return system_info_get_type(key, CUSTOM_TAG, type);
}
//LCOV_EXCL_STOP
