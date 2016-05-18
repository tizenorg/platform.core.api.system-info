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

#define KEY_PREFIX "http://"

#define BUF_MAX 256
#define KEY_MAX 128

#define ARRAY_SIZE(name) (sizeof(name)/sizeof(name[0]))

static int get_tizenid(char *val, size_t len)
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

	snprintf(val, len, "%s", id);
	return SYSTEM_INFO_ERROR_NONE;
}

struct system_info_file_key {
	const char *key;
	int (*get_value)(char *val, size_t len);
	system_info_type_e type;
} info_file_key[] = {
	{ "http://tizen.org/system/tizenid", get_tizenid, SYSTEM_INFO_STRING },
};

int system_info_get_file(const char *key, char *value, size_t len)
{
	char p_key[KEY_MAX];
	int i;
	size_t p_len;

	if (!key || !value)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	if (strstr(key, KEY_PREFIX) == key)
		snprintf(p_key, sizeof(p_key), "%s", key);
	else
		snprintf(p_key, sizeof(p_key), "%s%s", KEY_PREFIX, key);

	p_len = strlen(p_key) + 1;
	for (i = 0 ; i < ARRAY_SIZE(info_file_key); i++)
		if (!strncmp(p_key, info_file_key[i].key, p_len))
			return info_file_key[i].get_value(value, len);

	return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
}

int system_info_get_type_file(const char *key, system_info_type_e *type)
{
	char p_key[KEY_MAX];
	int i, len;

	if (!key)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	if (strstr(key, KEY_PREFIX) == key)
		snprintf(p_key, sizeof(p_key), "%s", key);
	else
		snprintf(p_key, sizeof(p_key), "%s%s", KEY_PREFIX, key);

	len = strlen(p_key) + 1;
	for (i = 0 ; i < ARRAY_SIZE(info_file_key); i++) {
		if (strncmp(p_key, info_file_key[i].key, len))
			continue;
		*type = info_file_key[i].type;
		return SYSTEM_INFO_ERROR_NONE;
	}

	return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
}
