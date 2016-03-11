/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <system_info.h>
#include <system_info_private.h>

#define SERIAL_TOK_DELIMITER ","
#define BUF_MAX 256

#define ARRAY_SIZE(name) (sizeof(name)/sizeof(name[0]))

static int get_tizenid(char **value)
{
	char id[BUF_MAX];
	FILE *fp;

	fp = fopen(TIZEN_ID_PATH, "r");
	if (!fp) {
		_E("Failed to open file (%s)", TIZEN_ID_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (fgets(id, sizeof(id), fp) == NULL) {
		_E("Failed to get string (errno:%d)", errno);
		fclose(fp);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	fclose(fp);

	if (strlen(id) == 0) {
		_E("String length of id is 0");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	*value = strdup(id);

	return 0;
}

static int get_build_date(char **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "build:date", value);
}

static int get_build_str(char **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "version:build", value);
}

static int get_build_time(char **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "build:time", value);
}

static int get_build_id(char **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "build:id", value);
}

static int get_build_type(char **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "build:type", value);
}

static int get_build_variant(char **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "build:variant", value);
}

static int get_build_version_release(char **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "version:release", value);
}

struct system_info_file_key {
	const char *key;
	int (*get_value)(char **val);
	system_info_type_e type;
} info_file_key [] = {
	{ "tizen.org/system/tizenid",       get_tizenid,       SYSTEM_INFO_STRING },
	{ "tizen.org/system/build.date",    get_build_date,    SYSTEM_INFO_STRING },
	{ "tizen.org/system/build.string",  get_build_str,     SYSTEM_INFO_STRING },
	{ "tizen.org/system/build.time",    get_build_time,    SYSTEM_INFO_STRING },
	{ "tizen.org/system/build.id",      get_build_id,      SYSTEM_INFO_STRING },
	{ "tizen.org/system/build.type",    get_build_type,    SYSTEM_INFO_STRING },
	{ "tizen.org/system/build.variant", get_build_variant, SYSTEM_INFO_STRING },
	{ "tizen.org/system/build.version.release", get_build_version_release, SYSTEM_INFO_STRING }
};

int system_info_get_file(const char *key, void **value)
{
	char *p_key;
	int i, len;

	if (!key || !value)
		return -EINVAL;

	p_key = strstr(key, "http://");
	if (p_key && p_key == key)
		p_key = (char *)key + strlen("http://");
	else
		p_key = (char *)key;

	len = strlen(p_key) + 1;
	for (i = 0 ; i < ARRAY_SIZE(info_file_key); i++)
		if (!strncmp(p_key, info_file_key[i].key, len))
			return info_file_key[i].get_value((char **)value);

	return -ENOENT;
}

int system_info_get_type_file(const char *key)
{
	char *p_key;
	int i, len;

	if (!key)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	p_key = strstr(key, "http://");
	if (p_key && p_key == key)
		p_key = (char *)key + strlen("http://");
	else
		p_key = (char *)key;

	len = strlen(p_key) + 1;
	for (i = 0 ; i < ARRAY_SIZE(info_file_key); i++) {
		if (!strncmp(p_key, info_file_key[i].key, len))
			return info_file_key[i].type;
	}

	return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
}
