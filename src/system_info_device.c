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

#include <dlog.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

#define TETHERING_INFO_FILE_PATH "/etc/config/connectivity/sysinfo-tethering.xml"

int system_info_get_manufacturer(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int ret;
	char *manufacturer;

	ret = system_info_get_platform_string(
			"tizen.org/system/manufacturer",
			&manufacturer);
	if (ret == SYSTEM_INFO_ERROR_NONE)
		*value = manufacturer;

	return ret;
}

int system_info_get_tethering_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	bool tethering;
	int ret;

	supported = (bool *)value;

	ret = system_info_get_platform_bool(
			"tizen.org/feature/network.tethering",
			&tethering);
	if (ret == SYSTEM_INFO_ERROR_NONE)
		*supported = tethering;

	return ret;
}
