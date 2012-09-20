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

#include <tapi_common.h>
#include <ITapiModem.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZEN_N_SYSTEM_INFO"

int system_info_get_network_type(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int service_type = 0;
	char *NETWORK_TYPE = NULL;

	if (system_info_vconf_get_value_int(VCONFKEY_TELEPHONY_SVCTYPE, &service_type))
		return SYSTEM_INFO_ERROR_IO_ERROR;

	switch (service_type) {
	case VCONFKEY_TELEPHONY_SVCTYPE_NONE:
		NETWORK_TYPE = strdup("NoService");
		break;
	case VCONFKEY_TELEPHONY_SVCTYPE_NOSVC:
		NETWORK_TYPE = strdup("NoService");
		break;
	case VCONFKEY_TELEPHONY_SVCTYPE_EMERGENCY:
		NETWORK_TYPE = strdup("Emergency");
		break;
	case VCONFKEY_TELEPHONY_SVCTYPE_2G:
		NETWORK_TYPE = strdup("GSM");
		break;
	case VCONFKEY_TELEPHONY_SVCTYPE_2_5G:
		NETWORK_TYPE = strdup("GPRS");
		break;
	case VCONFKEY_TELEPHONY_SVCTYPE_2_5G_EDGE:
		NETWORK_TYPE = strdup("EDGE");
		break;
	case VCONFKEY_TELEPHONY_SVCTYPE_3G:
		NETWORK_TYPE = strdup("UMTS");
		break;
	case VCONFKEY_TELEPHONY_SVCTYPE_HSDPA:
		NETWORK_TYPE = strdup("HSDPA");
		break;
	}

	if (NETWORK_TYPE == NULL) {
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = NETWORK_TYPE;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_mobile_device_id(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	TapiHandle *handle = NULL;
	char *imei = NULL;
	char *MOBILE_DEVICE_ID = NULL;

	handle = tel_init(0);

	if (NULL == handle) {
		LOGE("[%s] tel_init ERROR", __func__);
		*value = NULL;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	imei = tel_get_misc_me_imei_sync(handle);

	if (imei == NULL) {
		LOGE("[%s] IMEI value is NULL", __func__);
		tel_deinit(handle);
		*value = NULL;
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	MOBILE_DEVICE_ID = strdup((char *)imei);

	if (MOBILE_DEVICE_ID == NULL) {
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		free(imei);
		tel_deinit(handle);
		*value = NULL;
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	free(imei);
	tel_deinit(handle);
	*value = MOBILE_DEVICE_ID;
	return SYSTEM_INFO_ERROR_NONE;
}
