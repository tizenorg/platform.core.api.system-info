/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <gdbm.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <system_info.h>
#include <system_info_private.h>
#include <sys/utsname.h>

#define ARRAY_SIZE(name) (sizeof(name)/sizeof(name[0]))
#define KEY_PREFIX "http://"

#define SYSTEM_INFO_MAX -1
#define KEY_MAX 256
#define STR_MAX 256

#define GDBM_CACHE_SIZE 10 /* GDBM default == 100 */

enum tag_type {
	TAG_TYPE_PLATFORM,
	TAG_TYPE_CUSTOM,
};

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

static int db_get_value(enum tag_type tag, const char *key,
		const char *type, char *value, size_t len)
{
	char key_internal[KEY_MAX];
	GDBM_FILE db = NULL;
	datum d_key;
	datum d_data;
	int ret;
	char *tag_s;
	int cache_size = GDBM_CACHE_SIZE;

	if (!key || !type || !value)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	switch (tag) {
	case TAG_TYPE_PLATFORM:
		tag_s = TAG_TYPE_PLATFORM_STR;
		break;
	case TAG_TYPE_CUSTOM:
		tag_s = TAG_TYPE_CUSTOM_STR;
		break;
	default:
		return -EINVAL;
	}

	db = gdbm_open(SYSTEM_INFO_DB_PATH, 0, GDBM_READER, S_IRUSR | S_IRGRP | S_IROTH, NULL);
	if (!db) {
		_E("Failed to open db (%d, %s)", gdbm_errno, gdbm_strerror(gdbm_errno));
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = gdbm_setopt(db, GDBM_CACHESIZE, &cache_size, sizeof(cache_size));
	if (ret < 0)
		_E("Failed to set cache size to (%d) (ret:%d)", cache_size, gdbm_errno);

	if (strstr(key, KEY_PREFIX) == key)
		snprintf(key_internal, sizeof(key_internal),
				"%s:%s:%s", key, type, tag_s);
	else
		snprintf(key_internal, sizeof(key_internal),
				"%s%s:%s:%s", KEY_PREFIX, key, type, tag_s);

	d_key.dptr = key_internal;
	d_key.dsize = strlen(key_internal) + 1;

	d_data = gdbm_fetch(db, d_key);
	if (!d_data.dptr) {
		_E("Failed to find key (%s, %s)", key, type);
		ret = SYSTEM_INFO_ERROR_INVALID_PARAMETER;
		goto out;
	}

	snprintf(value, len, "%s", d_data.dptr);
	ret = SYSTEM_INFO_ERROR_NONE;

out:
	if (db)
		gdbm_close(db);
	return ret;
}

struct sysinfo_type {
	system_info_type_e type_e;
	const char *type_str;
} info_type[] = {
	{ SYSTEM_INFO_BOOL,   BOOL_TYPE },
	{ SYSTEM_INFO_INT,    INT_TYPE  },
	{ SYSTEM_INFO_DOUBLE, DBL_TYPE  },
	{ SYSTEM_INFO_STRING, STR_TYPE  },
};

static int system_info_get_type(enum tag_type tag, const char *key,
		system_info_type_e *type)
{
	char key_internal[KEY_MAX];
	GDBM_FILE db = NULL;
	datum d_key;
	datum d_data;
	int ret, i;
	char *tag_s;
	int cache_size = GDBM_CACHE_SIZE;

	if (!key || !type)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	switch (tag) {
	case TAG_TYPE_PLATFORM:
		tag_s = TAG_TYPE_PLATFORM_STR;
		break;
	case TAG_TYPE_CUSTOM:
		tag_s = TAG_TYPE_CUSTOM_STR;
		break;
	default:
		return -EINVAL;
	}

	db = gdbm_open(SYSTEM_INFO_DB_PATH, 0, GDBM_READER, S_IRUSR | S_IRGRP | S_IROTH, NULL);
	if (!db) {
		_E("Failed to open db (%d, %s)", gdbm_errno, gdbm_strerror(gdbm_errno));
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = gdbm_setopt(db, GDBM_CACHESIZE, &cache_size, sizeof(cache_size));
	if (ret < 0)
		_E("Failed to set cache size to (%d) (ret:%d)", cache_size, gdbm_errno);

	for (i = 0 ; i < ARRAY_SIZE(info_type); i++) {
		if (strstr(key, KEY_PREFIX) == key)
			snprintf(key_internal, sizeof(key_internal),
					"%s:%s:%s", key, info_type[i].type_str, tag_s);
		else
			snprintf(key_internal, sizeof(key_internal),
					"%s%s:%s:%s", KEY_PREFIX, key, info_type[i].type_str, tag_s);

		d_key.dptr = key_internal;
		d_key.dsize = strlen(key_internal) + 1;

		d_data = gdbm_fetch(db, d_key);
		if (d_data.dptr) {
			*type = info_type[i].type_e;
			ret = SYSTEM_INFO_ERROR_NONE;
			goto out;
		}
	}

	if (tag == TAG_TYPE_PLATFORM)
		ret = system_info_get_type_file(key, type);
	else
		ret = SYSTEM_INFO_ERROR_INVALID_PARAMETER;

out:
	if (db)
		gdbm_close(db);
	return ret;
}

static int system_info_get_bool(enum tag_type tag, const char *key, bool *value)
{
	int ret;
	char val[8];
	char *valp;
	size_t len;

	if (!key || !value)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	ret = db_get_value(tag, key, BOOL_TYPE, val, sizeof(val));
	if (ret == SYSTEM_INFO_ERROR_NONE)
		goto out;

	if (tag == TAG_TYPE_CUSTOM) {
		ret = external_get_value(TAG_TYPE_CUSTOM_STR, key, BOOL_TYPE, &valp);
		if (ret == SYSTEM_INFO_ERROR_NONE) {
			snprintf(val, sizeof(val), "%s", valp);
			free(valp);
			goto out;
		}
	}

	_E("Invalid key (%s), type:bool", key);

	return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

out:
	len = strlen(val) + 1;
	if (!strncmp(val, "true", len) || !strncmp(val, "TRUE", len))
		*value = true;
	else
		*value = false;

	return SYSTEM_INFO_ERROR_NONE;
}

static int system_info_get_int(enum tag_type tag, const char *key, int *value)
{
	int ret;
	char val[128];
	char *valp;

	if (!key || !value)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	ret = db_get_value(tag, key, INT_TYPE, val, sizeof(val));
	if (ret == SYSTEM_INFO_ERROR_NONE)
		goto out;

	if (tag == TAG_TYPE_CUSTOM) {
		ret = external_get_value(TAG_TYPE_CUSTOM_STR, key, INT_TYPE, &valp);
		if (ret == SYSTEM_INFO_ERROR_NONE) {
			snprintf(val, sizeof(val), "%s", valp);
			free(valp);
			goto out;
		}
	}

	_E("Invalid key (%s), type:integer", key);

	return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

out:
	*value = atoi(val);

	return SYSTEM_INFO_ERROR_NONE;
}

static int system_info_get_double(enum tag_type tag, const char *key, double *value)
{
	int ret;
	char val[128];
	char *valp;

	if (!key || !value)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	ret = db_get_value(tag, key, DBL_TYPE, val, sizeof(val));
	if (ret == SYSTEM_INFO_ERROR_NONE)
		goto out;

	if (tag == TAG_TYPE_CUSTOM) {
		ret = external_get_value(TAG_TYPE_CUSTOM_STR, key, DBL_TYPE, &valp);
		if (ret == SYSTEM_INFO_ERROR_NONE) {
			snprintf(val, sizeof(val), "%s", valp);
			free(valp);
			goto out;
		}
	}

	_E("Invalid key (%s), type:double", key);

	return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

out:
	*value = atof(val);

	return SYSTEM_INFO_ERROR_NONE;
}

static int system_info_get_string(enum tag_type tag, const char *key, char **value)
{
	int ret;
	char val[STR_MAX];
	char *string;
	char *valp;

	if (!key || !value)
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

	ret = db_get_value(tag, key, STR_TYPE, val, sizeof(val));
	if (ret == SYSTEM_INFO_ERROR_NONE)
		goto out;

	if (tag == TAG_TYPE_PLATFORM) {
		ret = system_info_get_file(key, val, sizeof(val));
		if (ret == SYSTEM_INFO_ERROR_NONE)
			goto out;
	}

	if (tag == TAG_TYPE_CUSTOM) {
		ret = external_get_value(TAG_TYPE_CUSTOM_STR, key, DBL_TYPE, &valp);
		if (ret == SYSTEM_INFO_ERROR_NONE) {
			snprintf(val, sizeof(val), "%s", valp);
			free(valp);
			goto out;
		}
	}

	_E("Invalid key (%s) type:string", key);

	return SYSTEM_INFO_ERROR_INVALID_PARAMETER;

out:
	string = strdup(val);
	if (!string) {
		_E("malloc failed");
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = string;

	return SYSTEM_INFO_ERROR_NONE;
}

API int system_info_get_platform_bool(const char *key, bool *value)
{
	return system_info_get_bool(TAG_TYPE_PLATFORM, key, value);
}

API int system_info_get_platform_int(const char *key, int *value)
{
	return system_info_get_int(TAG_TYPE_PLATFORM, key, value);
}

API int system_info_get_platform_double(const char *key, double *value)
{
	return system_info_get_double(TAG_TYPE_PLATFORM, key, value);
}

API int system_info_get_platform_string(const char *key, char **value)
{
	return system_info_get_string(TAG_TYPE_PLATFORM, key, value);
}

API int system_info_get_custom_bool(const char *key, bool *value)
{
	return system_info_get_bool(TAG_TYPE_CUSTOM, key, value);
}

API int system_info_get_custom_int(const char *key, int *value)
{
	return system_info_get_int(TAG_TYPE_CUSTOM, key, value);
}

API int system_info_get_custom_double(const char *key, double *value)
{
	return system_info_get_double(TAG_TYPE_CUSTOM, key, value);
}

API int system_info_get_custom_string(const char *key, char **value)
{
	return system_info_get_string(TAG_TYPE_CUSTOM, key, value);
}

API int system_info_get_platform_type(const char *key, system_info_type_e *type)
{
	return system_info_get_type(TAG_TYPE_PLATFORM, key, type);
}

API int system_info_get_custom_type(const char *key, system_info_type_e *type)
{
	int ret;
	char *val;
	size_t len;

	ret = system_info_get_type(TAG_TYPE_CUSTOM, key, type);
	if (ret == SYSTEM_INFO_ERROR_NONE)
		return SYSTEM_INFO_ERROR_NONE;

	ret = external_get_type(TAG_TYPE_CUSTOM_STR, key, &val);
	if (ret != SYSTEM_INFO_ERROR_NONE) {
		_E("Failed to get type of key (%s)", key);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	len = strlen(val) + 1;
	if (!strncmp(BOOL_TYPE, val, len))
		*type = SYSTEM_INFO_BOOL;
	else if (!strncmp(INT_TYPE, val, len))
		*type = SYSTEM_INFO_INT;
	else if (!strncmp(DBL_TYPE, val, len))
		*type = SYSTEM_INFO_DOUBLE;
	else if (!strncmp(STR_TYPE, val, len))
		*type = SYSTEM_INFO_STRING;
	else {
		_E("Invalid type (%s)", val);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	return SYSTEM_INFO_ERROR_NONE;
}
