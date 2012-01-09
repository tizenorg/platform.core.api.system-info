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

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZEN_N_SYSTEM_INFO"

static const bool BLUETOOTH_SUPPORT = true;
static const int CAMERA_COUNT = 2;
static const bool FM_RADIO_SUPPORT = true;
static const bool GPS_SUPPORT = true;
static const char *KEYBOARD_TYPE = NULL;
static const int MULTI_POINT_TOUCH_COUNT = 3;
static const bool NFC_SUPPORT = true;
static const bool TVOUT_SUPPORT = true;
static const bool WIFI_SUPPORT = true;

int system_info_get_bluetooth_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool*)value;

	*supported = BLUETOOTH_SUPPORT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_camera_count(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *count;

	count = (int*)value;

	*count = CAMERA_COUNT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_fmradio_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool*)value;

	*supported = FM_RADIO_SUPPORT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_gps_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool*)value;

	*supported = GPS_SUPPORT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_keyboard_type(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *keyboard_type;

	if (KEYBOARD_TYPE != NULL)
	{
		keyboard_type = strdup(KEYBOARD_TYPE);

		if (keyboard_type == NULL)
		{
			LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __FUNCTION__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
			return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
		}

		*value = keyboard_type;
	}
	else
	{
		*value = NULL;
	}

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_multi_point_touch_count(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *count;

	count = (int*)value;

	*count = MULTI_POINT_TOUCH_COUNT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_nfc_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool*)value;

	*supported = NFC_SUPPORT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_tvout_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool*)value;

	*supported = TVOUT_SUPPORT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_wifi_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool*)value;

	*supported = WIFI_SUPPORT;

	return SYSTEM_INFO_ERROR_NONE;
}

