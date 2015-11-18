/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License"),
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/


#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dlog.h>
#include <errno.h>

#include <system_info_intf.h>

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "SYSTEM_INFO_PLUGIN"

#define ARRAY_SIZE(name) (sizeof(name)/sizeof(name[0]))

static int get_sample(char *buf, unsigned int len)
{
	snprintf(buf, len, "Sample value");
	return 0;
}

static const struct external_system_info_key {
	const char *key;
	const char *tag;
	const char *type;
	int (*get_value)(char *buf, unsigned int len);
} system_info_key[] = {
	{ "http://developer.tizen.org/tizen/sample"	, TAG_CUSTOM, TYPE_STR, get_sample	},
};

static int get_value_external(const char *tag,
		const char *key, const char *type,
		char *buf, unsigned int len)
{
	int i;
	int key_len, tag_len, type_len;

	if (!tag || !key || !type || !buf)
		return -EINVAL;

	key_len = strlen(key);
	tag_len = strlen(tag);
	type_len = strlen(type);
	if (key_len == 0 || tag_len == 0 || type_len == 0)
		return -EINVAL;

	for (i = 0 ; i < ARRAY_SIZE(system_info_key) ; i++) {
		if (strncmp(system_info_key[i].type, type, type_len))
			continue;
		if (strncmp(system_info_key[i].tag, tag, tag_len))
			continue;
		if (strncmp(system_info_key[i].key, key, key_len))
			continue;

		if (system_info_key[i].get_value)
			return system_info_key[i].get_value(buf, len);
	}

	LOGE("Failed to find system info key in the plugin (%s)", key);
	return -EINVAL;
}

const system_info_external_plugin_interface external_plugin = {
	.get_value_external = get_value_external,
};

API const system_info_external_plugin_interface *system_info_get_external_plugin_interface(void)
{
	return &external_plugin;
}
