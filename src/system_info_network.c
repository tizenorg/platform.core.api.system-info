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

#include <dlog.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

int system_info_get_network_type(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool service_type = 0;
	char *NET_TYPE = NULL;
	char NETWORK_TYPE[MAXBUFSIZE];

	NETWORK_TYPE[0] = '\0';

	strcat(NETWORK_TYPE, "Emergency ");

	if (system_info_get_platform_bool("tizen.org/feature/network.telephony.service.cdma", &service_type) == SYSTEM_INFO_ERROR_NONE
		&& service_type == true)
		strcat(NETWORK_TYPE, "| cdma ");
	if (system_info_get_platform_bool("tizen.org/feature/network.telephony.service.edge", &service_type) == SYSTEM_INFO_ERROR_NONE
		&& service_type == true)
		strcat(NETWORK_TYPE, "| edge ");
	if (system_info_get_platform_bool("tizen.org/feature/network.telephony.service.gprs", &service_type) == SYSTEM_INFO_ERROR_NONE
		&& service_type == true)
		strcat(NETWORK_TYPE, "| gprs ");
	if (system_info_get_platform_bool("tizen.org/feature/network.telephony.service.gsm", &service_type) == SYSTEM_INFO_ERROR_NONE
		&& service_type == true)
		strcat(NETWORK_TYPE, "| gsm ");
	if (system_info_get_platform_bool("tizen.org/feature/network.telephony.service.hsdpa", &service_type) == SYSTEM_INFO_ERROR_NONE
		&& service_type == true)
		strcat(NETWORK_TYPE, "| hsdpa ");
	if (system_info_get_platform_bool("tizen.org/feature/network.telephony.service.hspa", &service_type) == SYSTEM_INFO_ERROR_NONE
		&& service_type == true)
		strcat(NETWORK_TYPE, "| hspa ");
	if (system_info_get_platform_bool("tizen.org/feature/network.telephony.service.hsupa", &service_type) == SYSTEM_INFO_ERROR_NONE
		&& service_type == true)
		strcat(NETWORK_TYPE, "| hsupa ");
	if (system_info_get_platform_bool("tizen.org/feature/network.telephony.service.umts", &service_type) == SYSTEM_INFO_ERROR_NONE
		&& service_type == true)
		strcat(NETWORK_TYPE, "| umts ");
	if (system_info_get_platform_bool("tizen.org/feature/network.telephony.service.lte", &service_type) == SYSTEM_INFO_ERROR_NONE
		&& service_type == true)
		strcat(NETWORK_TYPE, "| lte ");

	NET_TYPE = strdup(NETWORK_TYPE);

	if (NET_TYPE == NULL) {
		LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = NET_TYPE;

	return SYSTEM_INFO_ERROR_NONE;
}
