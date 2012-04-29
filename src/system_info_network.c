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

#include <vconf.h>
#include <dlog.h>

#include <TapiCommon.h>
#include <ITapiMisc.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZEN_N_SYSTEM_INFO"

int system_info_get_network_type(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char * network_type;

	network_type = strdup("GSM/UMTS");

	if (network_type == NULL)
	{
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __FUNCTION__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = network_type;
	
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_mobile_device_id(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	TelMiscSNInformation imei = {0,};

	if (tel_init() != TAPI_API_SUCCESS)
	{
		LOGE("[%s] IO_ERROR(0x%08x)", __FUNCTION__, SYSTEM_INFO_ERROR_IO_ERROR);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (tel_get_misc_me_sn(TAPI_MISC_ME_IMEI, &imei) != TAPI_API_SUCCESS)
	{
		tel_deinit();
		LOGE("[%s] IO_ERROR(0x%08x)", __FUNCTION__, SYSTEM_INFO_ERROR_IO_ERROR);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	*value = strdup((char*)imei.szNumber);

	tel_deinit();

	return SYSTEM_INFO_ERROR_NONE;
}

