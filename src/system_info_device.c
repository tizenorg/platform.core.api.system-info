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

#include <X11/extensions/XI2.h>
#include <X11/extensions/XI2proto.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XInput2.h>

#include <dlog.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

#define PROP_MULTITOUCH	"EvdevMultitouch MultiTouch"

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

extern char *strcasestr(const char *s, const char *find);

int xinput_extension_init(Display *disp)
{
	int opcode;
	int event, error;
	int major = XI_2_Major, minor = XI_2_Minor;

	if (!XQueryExtension(disp, "XInputExtension", &opcode, &event, &error)) {
		LOGE("XInput Extension isn't supported.");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (XIQueryVersion(disp, &major, &minor) == BadRequest) {
		LOGE("Failed to query XI version.");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!(major >= XI_2_Major && minor >= XI_2_Minor)) {
		LOGE("XI2 is not supported ! (major:%d, minor:%d)", major, minor);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}
	return opcode;
}

int get_device_property_value(Display *disp, int deviceid, Atom prop)
{
	Atom act_type;
	unsigned long nitems, bytes_after;
	unsigned char *data;
	int act_format, ret = -1;

	if (XIGetProperty(disp, deviceid, prop, 0, 1000, False,
							XA_INTEGER, &act_type, &act_format,
							&nitems, &bytes_after, &data) != Success) {
		LOGE("Failed to get XI2 device property !(deviceid=%d)", deviceid);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!nitems)
		goto out;

	ret = (int)*data;

out:
	if (data)
		XFree(data);

	return ret;
}

int get_multitouch_max_count(Display *disp)
{
	int i;
	int max_count;
	int ndevices;
	XIDeviceInfo *dev, *info = NULL;
	Atom atomMultiTouch;
	int xi_opcode = xinput_extension_init(disp);

	if (0 >= xi_opcode) {
		LOGE("Failed to initialize X Input Extension !");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	atomMultiTouch = XInternAtom(disp, PROP_MULTITOUCH, True);

	if (!atomMultiTouch) {
		LOGE("Failed to make an atom for multitouch property !");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	info = XIQueryDevice(disp, XIAllDevices, &ndevices);

	if (!info) {
		LOGE("Failed to query XI device.");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	for (i = 0; i < ndevices ; i++) {
		dev = &info[i];

		switch (dev->use) {
		case XISlavePointer:
			if (strcasestr(dev->name, "virtual") && !strcasestr(dev->name, "maru"))
				continue;
			if (strcasestr(dev->name, "extended"))
				continue;
			if (!strcasestr(dev->name, "touch"))
				continue;
			max_count = get_device_property_value(disp, dev->deviceid, atomMultiTouch);
			goto out;
		}
	}

	XIFreeDeviceInfo(info);
	return -1;

out:
	XIFreeDeviceInfo(info);
	return max_count;
}

int system_info_get_multi_point_touch_count(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *count;
	Display *disp;

	count = (int *)value;

	disp = XOpenDisplay(NULL);

	if (!disp) {
		LOGE("Failed to open display!");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	*count = get_multitouch_max_count(disp);

	return SYSTEM_INFO_ERROR_NONE;
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
