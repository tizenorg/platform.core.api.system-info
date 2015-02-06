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
	char *manufacturer = NULL;

	manufacturer = strdup("samsung");
	if (manufacturer == NULL) {
		LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = manufacturer;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_tethering_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (access(TETHERING_INFO_FILE_PATH, R_OK)) {
			*supported = false;
			return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(TETHERING_INFO_FILE_PATH, model, "tethering-support", &string)) {
			LOGE("cannot get tethering-support info from %s!!!", TETHERING_INFO_FILE_PATH);
			return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
			*supported = true;
	else
			*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}
