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

#include <dlog.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

#define SERIAL_TOK_DELIMITER ","
#define BUF_MAX 256

static int get_tizenid(char **value)
{
	char id[BUF_MAX];
	FILE *fp;

	fp = fopen(TIZEN_ID_PATH, "r");
	if (!fp) {
		LOGE("Failed to open file (%s)", TIZEN_ID_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (fgets(id, sizeof(id), fp) == NULL) {
		LOGE("Failed to get string (errno:%d)", errno);
		fclose(fp);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	fclose(fp);

	if (strlen(id) == 0) {
		LOGE("String length of id is 0");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	*value = strdup(id);

	return 0;
}

int system_info_get_no_file(const char *key, void **value)
{
	char *p_key;

	if (!key || !value)
		return -EINVAL;

	p_key = strstr(key, "http://");
	if (p_key && p_key == key)
		p_key = (char *)key + strlen("http://");
	else
		p_key = (char *)key;

	if (!strncmp(p_key, "tizen.org/system/tizenid", strlen(p_key)))
		return get_tizenid((char **)value);

	if (!strncmp(p_key, "tizen.org/system/build.date", strlen(p_key)))
		return system_info_ini_get_string(INFO_FILE_PATH, "build:date", (char **)value);

	if (!strncmp(p_key, "tizen.org/system/build.string", strlen(p_key)))
		return system_info_ini_get_string(INFO_FILE_PATH, "version:build", (char **)value);

	if (!strncmp(p_key, "tizen.org/system/build.time", strlen(p_key)))
		return system_info_ini_get_string(INFO_FILE_PATH, "build:time", (char **)value);

	return -ENOENT;
}
