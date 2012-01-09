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

#define SYSTEM_INFO_MAX -1

typedef struct {
	system_info_key_e key;
	system_info_data_type_e data_type;
	system_info_get_value_cb get_value_cb;
} system_info_s;

typedef system_info_s *system_info_h;

system_info_s system_info_table[] = {

{
	 /**< The model of the device */
	SYSTEM_INFO_KEY_MODEL,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_model
},

{
	 /**< The version of the Tizen supported by the platform */
	SYSTEM_INFO_KEY_TIZEN_VERSION,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_tizen_version
},

{
	/**< Indicates whether the device supports Bluetooth */
	SYSTEM_INFO_KEY_BLUETOOTH_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_bluetooth_supported
},

{
	/**< The number of cameras in the device */
	SYSTEM_INFO_KEY_CAMERA_COUNT,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_camera_count
},

{
	/**< Indicates whether the device supports FM radio */
	SYSTEM_INFO_KEY_FMRADIO_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_fmradio_supported
},

{
	/**< Indicates whether the device supports GPS */
	SYSTEM_INFO_KEY_GPS_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_gps_supported
},

{
	/**< The type of the keyboard */
	SYSTEM_INFO_KEY_KEYBOARD_TYPE,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_keyboard_type
},

{
	/**< The maximum number of concurrent touch points supported in the device */
	SYSTEM_INFO_KEY_MULTI_POINT_TOUCH_COUNT,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_multi_point_touch_count
},

{
	/**< The supported network type */
	SYSTEM_INFO_KEY_NETWORK_TYPE,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_network_type
},

{
	/**< Indicates whether the device supports NFC */
	SYSTEM_INFO_KEY_NFC_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_nfc_supported
},

{
	/**< The supported version of the OpenGL ES */
	SYSTEM_INFO_KEY_OPENGLES_VERSION,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_opengles_version
},

{
	/**< The number of bits per pixel */
	SYSTEM_INFO_KEY_SCREEN_BITS_PER_PIXEL,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_screen_bits_per_pixel
},

{
	/**< The height of the screen in pixels */
	SYSTEM_INFO_KEY_SCREEN_HEIGHT,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_screen_height
},

{
	/**< The width of the screen in pixels */
	SYSTEM_INFO_KEY_SCREEN_WIDTH,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_screen_width
},

{
	/**< Indicates whether the device supports TV-out */
	SYSTEM_INFO_KEY_TVOUT_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_tvout_supported
},

{
	 /**< Indicates whether the device supports Wi-Fi */
	SYSTEM_INFO_KEY_WIFI_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_wifi_supported
},

{
	SYSTEM_INFO_MAX, -1, NULL
}

};

static int system_info_get(system_info_key_e key, system_info_h *system_info)
{
	int index = 0;

	while (system_info_table[index].key != SYSTEM_INFO_MAX)
	{
		if (system_info_table[index].key == key)
		{
			*system_info = &system_info_table[index];
			return 0;
		}

		index++;
	}

 	return -1;
}

int system_info_get_value(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	system_info_h system_info;
	system_info_get_value_cb system_info_getter;

	if (value == NULL)
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid output param", __FUNCTION__, SYSTEM_INFO_ERROR_INVALID_PARAMETER);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	if (system_info_get(key, &system_info))
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid key", __FUNCTION__, SYSTEM_INFO_ERROR_INVALID_PARAMETER);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	if (system_info->data_type != data_type)
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid data type", __FUNCTION__, SYSTEM_INFO_ERROR_INVALID_PARAMETER);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	system_info_getter = system_info->get_value_cb;

	if (system_info_getter == NULL)
	{
		LOGE("[%s] IO_ERROR(0x%08x) : failed to call getter for the system information", __FUNCTION__, SYSTEM_INFO_ERROR_IO_ERROR);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return system_info_getter(key, system_info->data_type, value);
}

int system_info_get_value_int(system_info_key_e key, int *value)
{
	return system_info_get_value(key, SYSTEM_INFO_DATA_TYPE_INT, (void**)value);
}

int system_info_get_value_bool(system_info_key_e key, bool *value)
{
	return system_info_get_value(key, SYSTEM_INFO_DATA_TYPE_BOOL, (void**)value);
}

int system_info_get_value_double(system_info_key_e key, double *value)
{
	return system_info_get_value(key, SYSTEM_INFO_DATA_TYPE_DOUBLE, (void**)value);
}

int system_info_get_value_string(system_info_key_e key, char **value)
{
	return system_info_get_value(key, SYSTEM_INFO_DATA_TYPE_STRING, (void**)value);
}

