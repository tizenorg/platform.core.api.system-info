/*
 * system-info
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
 *
 */

#include "system_info_private.h"
#include "system_info_type.h"
#include "system_info_intf.h"
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

#define BUF_MAX 256

#define EXTERNAL_SYSTEM_INFO "/usr/lib/libsystem-info-external-plugin.so"

int external_get_value(const char *tag, const char *key,
		const char *type, char **val)
{
	void *handle = NULL;
	int ret;
	char buf[BUF_MAX];
	const system_info_external_plugin_interface *plugin;
	const system_info_external_plugin_interface *(*system_info_get_external_plugin_interface)(void);


	if (access(EXTERNAL_SYSTEM_INFO, F_OK) != 0) {
		_E("No external system info library");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	handle = dlopen(EXTERNAL_SYSTEM_INFO, RTLD_NOW);
	if (!handle) {
		_E("dlopen(%s) failed(%s)", EXTERNAL_SYSTEM_INFO, dlerror());
		ret = SYSTEM_INFO_ERROR_IO_ERROR;
		goto out;
	}

	system_info_get_external_plugin_interface = dlsym(handle, "system_info_get_external_plugin_interface");
	if (!system_info_get_external_plugin_interface) {
		_E("dlsym failed(%s)", dlerror());
		ret = SYSTEM_INFO_ERROR_IO_ERROR;
		goto out;
	}

	plugin = system_info_get_external_plugin_interface();
	if (!plugin) {
		_E("Failed to get plugin");
		ret = SYSTEM_INFO_ERROR_IO_ERROR;
		goto out;
	}

	if (plugin->get_value_external == NULL) {
		_E("Invalid plugin function");
		ret = SYSTEM_INFO_ERROR_IO_ERROR;
		goto out;
	}

	ret = plugin->get_value_external(tag, key, type, buf, sizeof(buf));
	if (ret < 0) {
		_E("Failed to get value from plugin (ret:%d)", ret);
		ret = SYSTEM_INFO_ERROR_IO_ERROR;
		goto out;
	}

	*val = strdup(buf);
	if (*val == NULL) {
		_E("strdup() failed");
		ret = SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	ret = SYSTEM_INFO_ERROR_NONE;

out:
	if (handle)
		dlclose(handle);

	return ret;
}
