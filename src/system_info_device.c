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

/*
** Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
** Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
** Digital Equipment Corporation, Maynard, Mass.
** Copyright (c) 1998 Microsoft.
** To anyone who acknowledges that this file is provided "AS IS"
** without any express or implied warranty: permission to use, copy,
** modify, and distribute this file for any purpose is hereby
** granted without fee, provided that the above copyright notices and
** this notice appears in all source code copies, and that none of
** the names of Open Software Foundation, Inc., Hewlett-Packard
** Company, Microsoft, or Digital Equipment Corporation be used in
** advertising or publicity pertaining to distribution of the software
** without specific, written prior permission. Neither Open Software
** Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital
** Equipment Corporation makes any representations about the
** suitability of this software for any purpose.
**
** This license applies to all the permissions of the below mentioned functions.
** Functions: format_uuid_v3or5, uuid_create_sha1_from_name
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <vconf.h>
#include <vconf-internal-csc-keys.h>
#include <dlog.h>

#include <system_info.h>
#include <system_info_private.h>

#include <haptic.h>

#include <X11/extensions/XI2.h>
#include <X11/extensions/XI2proto.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XInput2.h>

#include <openssl/sha.h>
#include <arpa/inet.h>

#include <net_nfc.h>
#include <net_nfc_typedef.h>

#include <location.h>
#include <location-types.h>

#include <bluetooth-api.h>

#include <mm_radio.h>
#include <mm_types.h>

#include <sound_manager.h>
#include <sensor.h>

#define PROP_MULTITOUCH	"EvdevMultitouch MultiTouch"
#define DEVICE_UUID_STRING_SIZE 37

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

#define CAM_INI_FILE_PATH "/usr/etc/mmfw_camcorder.ini"
#define CAM_VIDEO_PRI_FILE_PATH "/usr/etc/mmfw_camcorder_dev_video_pri.ini"
#define CAM_VIDEO_SEC_FILE_PATH "/usr/etc/mmfw_camcorder_dev_video_sec.ini"

#define NFC_INFO_FILE_PATH "/etc/config/nfc/sysinfo-nfc-ug.xml"
#define TETHERING_INFO_FILE_PATH "/etc/config/connectivity/sysinfo-tethering.xml"
#define TTS_INFO_FILE_PATH "/etc/config/sysinfo-tts.xml"
#define STT_INFO_FILE_PATH "/etc/config/sysinfo-stt.xml"

static char *FRONT_CAM_PATH;
static char *BACK_CAM_PATH;

typedef struct {
	uint32_t  time_low;
	uint16_t  time_mid;
	uint16_t  time_hi_and_version;
	uint8_t  clock_seq_hi_and_reserved;
	uint8_t  clock_seq_low;
	uint8_t node[6];
} _UUID_OBJECT;

/*
    Tizen Default vender key = f7884739-3c9f-5f7c-af-e1-fc9f5da56003
     This value should be modified by each venders.
*/
_UUID_OBJECT VenderKey = {0xf7884739, 0x3c9f, 0x5f7c, 0xaf, 0xe1, {0xfc, 0x9f, 0x5d, 0xa5, 0x60, 0x3} };
/*
    format_uuid_v3or5 -- make a UUID from a (pseudo)random 128-bit
    number
*/
void format_uuid_v3or5(_UUID_OBJECT *uuid, unsigned char hash[16], int v)
{
	/* convert UUID to local byte order */
	memcpy(uuid, hash, sizeof(*uuid));
	uuid->time_low = ntohl(uuid->time_low);
	uuid->time_mid = ntohs(uuid->time_mid);
	uuid->time_hi_and_version = ntohs(uuid->time_hi_and_version);

	/* put in the variant and version bits */
	uuid->time_hi_and_version &= 0x0FFF;
	uuid->time_hi_and_version |= (v << 12);
	uuid->clock_seq_hi_and_reserved &= 0x3F;
	uuid->clock_seq_hi_and_reserved |= 0x80;
}

void uuid_create_sha1_from_name(_UUID_OBJECT *uuid, _UUID_OBJECT nsid, void *name, int namelen)
{
	SHA_CTX c;
	unsigned char hash[20];
	_UUID_OBJECT net_nsid;

	/* put name space ID in network byte order so it hashes the same
	   no matter what endian machine we're on */
	net_nsid = nsid;
	net_nsid.time_low = htonl(net_nsid.time_low);
	net_nsid.time_mid = htons(net_nsid.time_mid);
	net_nsid.time_hi_and_version = htons(net_nsid.time_hi_and_version);

	SHA1_Init(&c);
	SHA1_Update(&c, &net_nsid, sizeof(net_nsid));
	SHA1_Update(&c, name, namelen);
	SHA1_Final(hash, &c);

	/* the hash is in network byte order at this point */
	format_uuid_v3or5(uuid, hash, 5);
}

int uuid_object_to_string(_UUID_OBJECT uuid, char **device_uuid_string)
{
	*device_uuid_string = (char *) calloc(1, DEVICE_UUID_STRING_SIZE + 1);

	if (*device_uuid_string == NULL)
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;

	snprintf(*device_uuid_string, DEVICE_UUID_STRING_SIZE, "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		(unsigned long)uuid.time_low,
		(unsigned int)uuid.time_mid,
		(unsigned int)uuid.time_hi_and_version,
		(unsigned int)uuid.clock_seq_hi_and_reserved,
		(unsigned int)uuid.clock_seq_low,
		(unsigned int)uuid.node[0],
		(unsigned int)uuid.node[1],
		(unsigned int)uuid.node[2],
		(unsigned int)uuid.node[3],
		(unsigned int)uuid.node[4],
		(unsigned int)uuid.node[5]);

	return SYSTEM_INFO_ERROR_NONE;
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

/**
 * @brief Setting face direction path
 *
 */

void set_camera_direction_path(void)
{
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	int direction = -1;
	FILE *info = NULL;

	info = fopen(CAM_VIDEO_PRI_FILE_PATH, "r");

	if (NULL != info) {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp(";", str, strlen(";")))
				continue;
			else if (!strncmp("FacingDirection", str, strlen("FacingDirection"))) {
				sscanf(str, "%s = %d", tmpStr, &direction);
				if (direction == 1) {
					FRONT_CAM_PATH = strdup(CAM_VIDEO_PRI_FILE_PATH);
					BACK_CAM_PATH = strdup(CAM_VIDEO_SEC_FILE_PATH);
					fclose(info);
					return;
				}
			} else
				continue;
		}

		fclose(info);
	}

	/* default setting */
	FRONT_CAM_PATH = strdup(CAM_VIDEO_SEC_FILE_PATH);
	BACK_CAM_PATH = strdup(CAM_VIDEO_PRI_FILE_PATH);

	return;
}

int system_info_get_bluetooth_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool *)value;

	if (bluetooth_is_supported())
			*supported = true;
		else
			*supported = false;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_camera_count(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *count;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	FILE *info = NULL;

	count = (int *)value;
	/* default camera count is 2*/
	*count = 2;

	info = fopen(CAM_INI_FILE_PATH, "r");
	if (NULL == info) {
		LOGE("cannot file open %s file!!!", CAM_INI_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp(";", str, strlen(";")))
				continue;
			else if (!strncmp("DeviceCount", str, strlen("DeviceCount"))) {
				sscanf(str, "%s = %d", tmpStr, count);
				break;
			} else
				continue;
		}
	}
	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_fmradio_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	MMHandleType hradio;

	supported = (bool *)value;

	memset(&hradio, 0x0, sizeof(MMHandleType));

	if (mm_radio_create(&hradio) == MM_ERROR_NONE) {
		if(mm_radio_realize(hradio) == MM_ERROR_NONE) {
			*supported = true;
			if (mm_radio_unrealize(hradio) == MM_ERROR_NONE) {
				if (mm_radio_destroy(hradio) == MM_ERROR_NONE)
					return SYSTEM_INFO_ERROR_NONE;
				else {
					LOGE("cannot destroy mm_radio handle!!!");
					return SYSTEM_INFO_ERROR_IO_ERROR;
				}
			} else {
				LOGE("cannot unrealize mm_radio handle!!!");
				return SYSTEM_INFO_ERROR_IO_ERROR;
			}
		} else {
			*supported = false;
			if (mm_radio_destroy(hradio) == MM_ERROR_NONE)
				return SYSTEM_INFO_ERROR_NONE;
			else {
				LOGE("cannot destroy mm_radio handle!!!");
				return SYSTEM_INFO_ERROR_IO_ERROR;
			}
		}
	} else {
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}
}

int system_info_get_gps_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool *)value;

	if (location_is_supported_method(LOCATION_METHOD_GPS))
		*supported = true;
	else
		*supported = false;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_cps_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool *)value;

	if (location_is_supported_method(LOCATION_METHOD_CPS))
		*supported = true;
	else
		*supported = false;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_wps_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool *)value;

	if (location_is_supported_method(LOCATION_METHOD_WPS))
		*supported = true;
	else
		*supported = false;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_keyboard_type(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *string = NULL;
	char *model = "default";

	if (system_info_get_value_from_xml(XML_FILE_PATH, model, "Keyboad_type", &string)) {
		LOGE("cannot get Keyboad_type info from %s!!!", XML_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "NULL")) {
		*value = NULL;
		free(string);
		return SYSTEM_INFO_ERROR_NONE;
	}

	*value = string;

	return SYSTEM_INFO_ERROR_NONE;
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
	bool *supported;
	int nfc_supported = 0;

	supported = (bool *)value;

	if (NET_NFC_OK == net_nfc_is_supported(&nfc_supported))
		*supported = true;
	else
		*supported = false;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_tvout_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (system_info_get_value_from_xml(XML_FILE_PATH, model, "tv_out_support", &string)) {
		LOGE("cannot get tv_out_support info from %s!!!", NFC_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_wifi_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (system_info_get_system_info_model_type() == SYSTEM_INFO_MODEL_TYPE_EMULATOR) {
	*supported = false;
	return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(XML_FILE_PATH, model, "wifi_support", &string)) {
		LOGE("cannot get wifi_support info from %s!!!", NFC_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_wifi_direct_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (system_info_get_system_info_model_type() == SYSTEM_INFO_MODEL_TYPE_EMULATOR) {
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(XML_FILE_PATH, model, "wifi_direct_support", &string)) {
		LOGE("cannot get wifi_direct_support info from %s!!!", NFC_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_haptic_supproted(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	int count;
	int retVal;

	supported = (bool *)value;

	retVal = haptic_get_count(&count);
	if (retVal < 0)
		return SYSTEM_INFO_ERROR_IO_ERROR;

	if (count <= 0)
		*supported = false;
	else
		*supported = true;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_csc_sales_code(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *CSC_SALES_CODE;

	CSC_SALES_CODE = (char *)value;

	if (system_info_vconf_get_value_string(VCONFKEY_CSC_SALESCODE, &CSC_SALES_CODE))
		return SYSTEM_INFO_ERROR_IO_ERROR;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_device_uuid(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *imei = NULL;
	char *UUID;
	_UUID_OBJECT device_uuid;
	int retVal;

	retVal = system_info_get_value_string(SYSTEM_INFO_KEY_MOBILE_DEVICE_ID, &imei);
	if (SYSTEM_INFO_ERROR_NONE != retVal) {
		LOGE("cannot get an IMEI value!!!");
		return retVal;
	}

	if (imei) {
		uuid_create_sha1_from_name(&device_uuid, VenderKey, imei, strlen(imei));

		if (uuid_object_to_string(device_uuid, &UUID))
			return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;

		*value = UUID;

		free(imei);
		return SYSTEM_INFO_ERROR_NONE;
	} else {
		LOGE("imei is NULL!!!");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}
}

int system_info_get_usb_host_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	int usbhost_state = 0;

	supported = (bool *)value;

	if (vconf_get_int(VCONFKEY_SYSMAN_USB_HOST_STATUS, &usbhost_state))
		*supported = false;
	else
		*supported = true;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_usb_accessory_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	int usbhost_state = 0;

	supported = (bool *)value;

	if (vconf_get_int(VCONFKEY_USB_ACCESSORY_STATUS, &usbhost_state))
		*supported = false;
	else
		*supported = true;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_front_camera_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool *)value;

	/* setting camera direction */
	set_camera_direction_path();

	/* There is no FacingDirection field in ini file */
	if (!access(FRONT_CAM_PATH, R_OK))
		*supported = true;
	else
		*supported = false;

	/* free used memory for camera direction */
	free(FRONT_CAM_PATH);
	free(BACK_CAM_PATH);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_front_camera_af_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	int tmpInt[6];
	FILE *info = NULL;

	supported = (bool *)value;
	/* default value is false */
	*supported = false;

	/* setting camera direction */
	set_camera_direction_path();

	/* Return false, if there is no "/usr/etc/mmfw_camcorder_dev_video_sec.ini" file */
	if (access(FRONT_CAM_PATH, R_OK))
		return SYSTEM_INFO_ERROR_NONE;

	info = fopen(FRONT_CAM_PATH, "r");
	if (NULL == info) {
		LOGE("cannot file open %s file!!!", FRONT_CAM_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp(";", str, strlen(";")))
				continue;
			else if (!strncmp("FocusMode", str, strlen("FocusMode"))) {
				sscanf(str, "%s = %d,%d,%d,%d,%d,%d", tmpStr, &tmpInt[0], &tmpInt[1], &tmpInt[2], &tmpInt[3], &tmpInt[4], &tmpInt[5]);
				if (tmpInt[2] != -255 || tmpInt[4] != -255)
					*supported = true;
				break;
			} else
				continue;
		}
	}

	/* free used memory for camera direction */
	free(FRONT_CAM_PATH);
	free(BACK_CAM_PATH);

	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_front_camera_flash_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	int tmpInt[8];
	int i = 0;
	FILE *info = NULL;

	supported = (bool *)value;
	/* default value is false */
	*supported = false;

	/* setting camera direction */
	set_camera_direction_path();

	/* Return false, if there is no "/usr/etc/mmfw_camcorder_dev_video_sec.ini" file */
	if (access(FRONT_CAM_PATH, R_OK))
		return SYSTEM_INFO_ERROR_NONE;

	info = fopen(FRONT_CAM_PATH, "r");
	if (NULL == info) {
		LOGE("cannot file open %s file!!!", FRONT_CAM_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp(";", str, strlen(";")))
				continue;
			else if (!strncmp("StrobeMode", str, strlen("StrobeMode"))) {
				sscanf(str, "%s = %d,%d,%d,%d,%d,%d,%d,%d", tmpStr, &tmpInt[0], &tmpInt[1], &tmpInt[2], &tmpInt[3], &tmpInt[4], &tmpInt[5], &tmpInt[6], &tmpInt[7]);
				for (i = 1; i < 8; i++) {
					if (tmpInt[i] != -255)
						*supported = true;
				}
				break;
			} else
				continue;
		}
	}

	/* free used memory for camera direction */
	free(FRONT_CAM_PATH);
	free(BACK_CAM_PATH);

	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_back_camera_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool *)value;

	/* setting camera direction */
	set_camera_direction_path();

	if (!access(BACK_CAM_PATH, R_OK))
		*supported = true;
	else
		*supported = false;

	/* free used memory for camera direction */
	free(FRONT_CAM_PATH);
	free(BACK_CAM_PATH);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_back_camera_af_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	int tmpInt[6];
	FILE *info = NULL;

	supported = (bool *)value;
	/* default value is false */
	*supported = false;

	/* setting camera direction */
	set_camera_direction_path();

	/* Return false, if there is no "/usr/etc/mmfw_camcorder_dev_video_pri.ini" file */
	if (access(BACK_CAM_PATH, R_OK))
		return SYSTEM_INFO_ERROR_NONE;

	info = fopen(BACK_CAM_PATH, "r");
	if (NULL == info) {
		LOGE("cannot file open %s file!!!", BACK_CAM_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp(";", str, strlen(";")))
				continue;
			else if (!strncmp("FocusMode", str, strlen("FocusMode"))) {
				sscanf(str, "%s = %d,%d,%d,%d,%d,%d", tmpStr, &tmpInt[0], &tmpInt[1], &tmpInt[2], &tmpInt[3], &tmpInt[4], &tmpInt[5]);
				if (tmpInt[2] != -255 || tmpInt[4] != -255)
					*supported = true;
				break;
			} else
				continue;
		}
	}

	/* free used memory for camera direction */
	free(FRONT_CAM_PATH);
	free(BACK_CAM_PATH);

	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_back_camera_flash_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	int tmpInt[8];
	int i = 0;
	FILE *info = NULL;

	supported = (bool *)value;
	/* default value is false */
	*supported = false;

	/* setting camera direction */
	set_camera_direction_path();

	/* Return false, if there is no "/usr/etc/mmfw_camcorder_dev_video_pri.ini" file */
	if (access(BACK_CAM_PATH, R_OK))
		return SYSTEM_INFO_ERROR_NONE;

	info = fopen(BACK_CAM_PATH, "r");
	if (NULL == info) {
		LOGE("cannot file open %s file!!!", BACK_CAM_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp(";", str, strlen(";")))
				continue;
			else if (!strncmp("StrobeMode", str, strlen("StrobeMode"))) {
				sscanf(str, "%s = %d,%d,%d,%d,%d,%d,%d,%d", tmpStr, &tmpInt[0], &tmpInt[1], &tmpInt[2], &tmpInt[3], &tmpInt[4], &tmpInt[5], &tmpInt[6], &tmpInt[7]);
				for (i = 1; i < 8; i++) {
					if (tmpInt[i] != -255)
						*supported = true;
				}
				break;
			} else
				continue;
		}
	}

	/* free used memory for camera direction */
	free(FRONT_CAM_PATH);
	free(BACK_CAM_PATH);

	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}


int system_info_get_sip_voip_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (system_info_get_value_from_xml(XML_FILE_PATH, model, "sip_voip_support", &string)) {
		LOGE("cannot get sip_voip_support info from %s!!!", NFC_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_microphone_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	sound_device_in_e in;
	sound_device_out_e out;

	supported = (bool *)value;

	if (SOUND_MANAGER_ERROR_NONE == sound_manager_get_active_device(&in, &out)) {
		if (in == SOUND_DEVICE_IN_MIC)
			*supported = true;
		else
			*supported = false;
	} else
		*supported = false;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_speech_recognition_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (access(STT_INFO_FILE_PATH, R_OK)) {
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(STT_INFO_FILE_PATH, model, "stt-support", &string)) {
		LOGE("cannot get stt-support info from %s!!!", STT_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_speech_synthesis_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (access(TTS_INFO_FILE_PATH, R_OK)) {
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(TTS_INFO_FILE_PATH, model, "tts-support", &string)) {
		LOGE("cannot get tts-support info from %s!!!", TTS_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_barometer_sensor_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool *)value;

	if (sf_is_sensor_event_available(BAROMETER_SENSOR, 0))
		*supported = false;
	else
		*supported = true;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_manufacturer(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *string = NULL;
	char *model = "default";

	if (system_info_get_value_from_xml(XML_FILE_PATH, model, "MANUFACTURER", &string)) {
		LOGE("cannot get MANUFACTURER info from %s!!!", XML_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

				*value = string;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_cp_interface(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *string = NULL;
	char *model = "default";

	if (system_info_get_value_from_xml(XML_FILE_PATH, model, "CP_Interface", &string)) {
		LOGE("cannot get CP_Interface info from %s!!!", XML_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

				*value = string;

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
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (access(NFC_INFO_FILE_PATH, R_OK)) {
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(NFC_INFO_FILE_PATH, model, "reserved-push-support", &string)) {
		LOGE("cannot get reserved-push-support info from %s!!!", NFC_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}
