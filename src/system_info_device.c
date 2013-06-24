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
#define CAMERA_INFO_FILE_PATH	"/usr/etc/mmfw_camcorder.ini"

int system_info_get_camera_count(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *count;
	char *devicecount;

	count = (int *)value;
	*count = 0;

	if (system_info_ini_get_string(CAMERA_INFO_FILE_PATH, "videoinput:devicecount", &devicecount)
			!= SYSTEM_INFO_ERROR_NONE) {
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	*count = atoi(devicecount);

	free(devicecount);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_bluetooth_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/network.bluetooth", (bool *)value);
}

int system_info_get_fmradio_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/fmradio", (bool *)value);
}

int system_info_get_gps_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/location.gps", (bool *)value);
}

int system_info_get_wps_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/location.wps", (bool *)value);
}

int system_info_get_keyboard_type(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_string("tizen.org/feature/input.keyboard.layout", (char**)value);
}

int system_info_get_multi_point_touch_count(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_int("tizen.org/feature/multi_point_touch.point_count", (int *)value);
}

int system_info_get_nfc_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/network.nfc", (bool *)value);
}

int system_info_get_wifi_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/network.wifi", (bool *)value);
}

int system_info_get_wifi_direct_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/network.wifi.direct", (bool *)value);
}

int system_info_get_usb_host_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/usb.host", (bool *)value);
}

int system_info_get_usb_accessory_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/usb.accessory", (bool *)value);
}

int system_info_get_front_camera_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/camera.front", (bool *)value);
}

int system_info_get_front_camera_flash_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/camera.front.flash", (bool *)value);
}

int system_info_get_back_camera_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/camera.back", (bool *)value);
}

int system_info_get_back_camera_flash_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/camera.back.flash", (bool *)value);
}

int system_info_get_sip_voip_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/sip.voip", (bool *)value);
}

int system_info_get_microphone_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/microphone", (bool *)value);
}

int system_info_get_speech_recognition_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/speech.recognition", (bool *)value);
}

int system_info_get_speech_synthesis_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/speech.synthesis", (bool *)value);
}

int system_info_get_barometer_sensor_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/sensor.barometer", (bool *)value);
}

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

int system_info_get_nfc_reserved_push_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/network.nfc.reserved_push", (bool *)value);
}
