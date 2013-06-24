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

#include <vconf.h>
#include <dlog.h>

#include <system_info.h>
#include <system_info_private.h>
#include <sys/utsname.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

#define SYSTEM_INFO_MAX -1

typedef struct {
	system_info_key_e key;
	system_info_data_type_e data_type;
	system_info_get_value_cb get_value_cb;
} system_info_s;

typedef system_info_s * system_info_h;

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
	/**< The maximum number of concurrent touch points supported
	in the device */
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
	 /**< Indicates whether the device supports Wi-Fi */
	SYSTEM_INFO_KEY_WIFI_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_wifi_supported
},

{
	/**< The Name of the Platform */
	SYSTEM_INFO_KEY_PLATFORM_NAME,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_platform_name
},

{
	/**< The Name of the Tizen version */
	SYSTEM_INFO_KEY_TIZEN_VERSION_NAME,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_tizen_version_name
},

{
	/**< Indicates whether the device supports wifi direct */
	SYSTEM_INFO_KEY_WIFI_DIRECT_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_wifi_direct_supported
},

{
	/**< The height of the screen DPI */
	SYSTEM_INFO_KEY_SCREEN_DPI,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_screen_DPI
},

{
	/**< Indicates whether the device supports GPS */
	SYSTEM_INFO_KEY_WPS_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_wps_supported
},

{
	/**< Indicates whether the device supports USB host */
	SYSTEM_INFO_KEY_USB_HOST_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_usb_host_supported
},

{
	/**< Indicates whether the device supports USB accessory */
	SYSTEM_INFO_KEY_USB_ACCESSORY_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_usb_accessory_supported
},

{
	/**< The CORE CPU architecture of model */
	SYSTEM_INFO_KEY_CORE_CPU_ARCH,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_core_cpu_arch
},

{
	/**< The CORE FPU architecture of model */
	SYSTEM_INFO_KEY_CORE_FPU_ARCH,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_core_fpu_arch
},

{
	/**< Indicates whether the device supports front camera */
	SYSTEM_INFO_KEY_FRONT_CAMERA_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_front_camera_supported
},

{
	/**< Indicates whether the device supports front camera flash */
	SYSTEM_INFO_KEY_FRONT_CAMERA_FLASH_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_front_camera_flash_supported
},

{
	/**< Indicates whether the device supports back camera */
	SYSTEM_INFO_KEY_BACK_CAMERA_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_back_camera_supported
},

{
	/**< Indicates whether the device supports back camera flash */
	SYSTEM_INFO_KEY_BACK_CAMERA_FLASH_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_back_camera_flash_supported
},

{
	/**< Indicates whether the device supports HDMI */
	SYSTEM_INFO_KEY_HDMI_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_hdmi_supported
},

{
	/**< Indicates whether the device supports RCA */
	SYSTEM_INFO_KEY_RCA_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_rca_supported
},

{
	/**< Indicates whether the device supports SIP VOIP */
	SYSTEM_INFO_KEY_SIP_VOIP_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_sip_voip_supported
},

{
	/**< Indicates whether the device supports Microphone */
	SYSTEM_INFO_KEY_MICROPHONE_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_microphone_supported
},

{
	/**< Indicates whether the device supports stt */
	SYSTEM_INFO_KEY_SPEECH_RECOGNITION_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_speech_recognition_supported
},

{
	/**< Indicates whether the device supports barometer sensor */
	SYSTEM_INFO_KEY_BAROMETER_SENSOR_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_barometer_sensor_supported
},

{
	/**< The supported texture format of the OpenGL ES */
	SYSTEM_INFO_KEY_OPENGLES_TEXTURE_FORMAT,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_opengles_texture_format
},

{
	/**< The CORE CPU frequency of model */
	SYSTEM_INFO_KEY_CORE_CPU_FREQ,
	SYSTEM_INFO_DATA_TYPE_DOUBLE,
	system_info_get_core_cpu_freq
},

{
	/**< The height of the physical screen size in millimeters */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_HEIGHT,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_physical_screen_height
},

{
	/**< The width of the physical screen size in millimeters */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_WIDTH,
	SYSTEM_INFO_DATA_TYPE_INT,
	system_info_get_physical_screen_width
},

{
	/**< The build string of the platform binary */
	SYSTEM_INFO_KEY_BUILD_STRING,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_build_string
},

{
	/**< The build date of the platform binary */
	SYSTEM_INFO_KEY_BUILD_DATE,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_build_date
},

{
	/**< The build time of the platform binary */
	SYSTEM_INFO_KEY_BUILD_TIME,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_build_time
},

{
	/**< The manufacturer of the device */
	SYSTEM_INFO_KEY_MANUFACTURER,
	SYSTEM_INFO_DATA_TYPE_STRING,
	system_info_get_manufacturer
},

{
	/**< Indicates whether the device supports MMS */
	SYSTEM_INFO_KEY_MMS_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_mms_supported
},

{
	/**< Indicates whether the device supports SMS */
	SYSTEM_INFO_KEY_SMS_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_sms_supported
},

{
	/**< Indicates whether the device supports CBS */
	SYSTEM_INFO_KEY_CBS_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_cbs_supported
},

{
	/**< Indicates whether the device supports nfc-reserved push */
	SYSTEM_INFO_KEY_NFC_RESERVED_PUSH_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_nfc_reserved_push_supported
},

{
	/**< Indicates whether the device supports tethering */
	SYSTEM_INFO_KEY_TETHERING_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_tethering_supported
},

{
	/**< Indicates whether the device supports tts */
	SYSTEM_INFO_KEY_SPEECH_SYNTHESIS_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_speech_synthesis_supported
},

{
	/**< Indicates whether the device supports graphics hardware acceleration */
	SYSTEM_INFO_KEY_GRAPHICS_HWACCEL_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_graphics_hwaccel_supported
},

{
	/**< Indicates whether the device supports auto rotation feature */
	SYSTEM_INFO_KEY_FEATURE_AUTO_ROTATION_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_feature_auto_rotation_supported
},

{
	/**< Indicates whether the device supports pinch zoom feature */
	SYSTEM_INFO_KEY_FEATURE_PINCH_ZOOM_SUPPORTED,
	SYSTEM_INFO_DATA_TYPE_BOOL,
	system_info_get_feature_pinch_zoom_supported
},

{
	SYSTEM_INFO_MAX, -1, NULL
}

};

static system_info_mode_type_e system_info_system_info_model_type;
static int system_info_initialized;

int system_info_get_system_info_initialized(void)
{
	return system_info_initialized;
}

void system_info_set_system_info_initialized(int value)
{
	system_info_initialized = value;
}

system_info_mode_type_e system_info_get_system_info_model_type(void)
{
	return system_info_system_info_model_type;
}

int system_info_init(void)
{
	struct utsname device_name;
	extern char *strcasestr(const char *s, const char *find);

	int error = uname(&device_name);

	if (error != 0) {
		LOGE("uname returns error!!!");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		if (strcasestr(device_name.machine, "emulated"))
				system_info_system_info_model_type = SYSTEM_INFO_MODEL_TYPE_EMULATOR;
		else
		system_info_system_info_model_type = SYSTEM_INFO_MODEL_TYPE_TARGET;

		system_info_set_system_info_initialized(1);
	}
	return SYSTEM_INFO_ERROR_NONE;
}

static int system_info_get(system_info_key_e key, system_info_h *system_info)
{
	int index = 0;
	int ret_val;

	if (0 == system_info_get_system_info_initialized()) {
		ret_val = system_info_init();
		if (ret_val) {
			LOGE("system information initialize fail!!!");
			return ret_val;
		}
	}

	while (system_info_table[index].key != SYSTEM_INFO_MAX) {
		if (system_info_table[index].key == key) {
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

	if (value == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x) : invalid output param", SYSTEM_INFO_ERROR_INVALID_PARAMETER);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	if (system_info_get(key, &system_info)) {
		LOGE("INVALID_PARAMETER(0x%08x) : invalid key", SYSTEM_INFO_ERROR_INVALID_PARAMETER);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	if (system_info->data_type != data_type) {
		LOGE("INVALID_PARAMETER(0x%08x) : invalid data type", SYSTEM_INFO_ERROR_INVALID_PARAMETER);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	system_info_getter = system_info->get_value_cb;

	if (system_info_getter == NULL) {
		LOGE("IO_ERROR(0x%08x) : failed to call getter for the system information", SYSTEM_INFO_ERROR_IO_ERROR);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return system_info_getter(key, system_info->data_type, value);
}

int system_info_get_value_int(system_info_key_e key, int *value)
{
	return system_info_get_value(key, SYSTEM_INFO_DATA_TYPE_INT, (void **)value);
}

int system_info_get_value_bool(system_info_key_e key, bool *value)
{
	return system_info_get_value(key, SYSTEM_INFO_DATA_TYPE_BOOL, (void **)value);
}

int system_info_get_value_double(system_info_key_e key, double *value)
{
	return system_info_get_value(key, SYSTEM_INFO_DATA_TYPE_DOUBLE, (void **)value);
}

int system_info_get_value_string(system_info_key_e key, char **value)
{
	return system_info_get_value(key, SYSTEM_INFO_DATA_TYPE_STRING, (void **)value);
}

int system_info_get_platform_bool(const char *key, bool *value)
{
	int ret;
	bool *supported;
	char *string = NULL;

	supported = (bool *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, BOOL_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s", key);
		return ret;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_platform_int(const char *key, int *value)
{
	int ret;
	int *ret_val;
	char *string = NULL;

	ret_val = (int *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, INT_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s", key);
		return ret;
	}

	*ret_val = atoi(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_platform_double(const char *key, double *value)
{
	int ret;
	double *ret_val;
	char *string = NULL;

	ret_val = (double *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, DBL_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s", key);
		return ret;
	}

	*ret_val = atof(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_platform_string(const char *key, char **value)
{
	int ret;
	char *string = NULL;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(PLATFORM_TAG, key, STR_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s", key);
		return ret;
	}

	*value = string;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_custom_bool(const char *key, bool *value)
{
	int ret;
	bool *supported;
	char *string = NULL;

	supported = (bool *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, BOOL_TYPE, &string);
	if (ret) {
		LOGI("cannot get %s", key);
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_custom_int(const char *key, int *value)
{
	int ret;
	int *ret_val;
	char *string = NULL;

	ret_val = (int *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, INT_TYPE, &string);
	if (ret) {
		LOGI("cannot get %s", key);
		*ret_val = 0;
		return SYSTEM_INFO_ERROR_NONE;
	}

	*ret_val = atoi(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_external_bool(const char *key, bool *value)
{
	char vconfkey[MAXBUFSIZE] = {0,};

	snprintf(vconfkey, strlen(EXTERNAL_VCONF_PREFIX)+strlen(key)+1, "%s%s", EXTERNAL_VCONF_PREFIX, key);

	if (system_info_vconf_get_value_bool(vconfkey, value)) {
		LOGE("key : %s, failed get bool value", key);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_external_int(const char *key, int *value)
{
	char vconfkey[MAXBUFSIZE] = {0,};

	snprintf(vconfkey, strlen(EXTERNAL_VCONF_PREFIX)+strlen(key)+1, "%s%s", EXTERNAL_VCONF_PREFIX, key);

	if (system_info_vconf_get_value_int(vconfkey, value)) {
		LOGE("key : %s, failed get int value", key);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_external_double(const char *key, double *value)
{
	char vconfkey[MAXBUFSIZE] = {0,};

	snprintf(vconfkey, strlen(EXTERNAL_VCONF_PREFIX)+strlen(key)+1, "%s%s", EXTERNAL_VCONF_PREFIX, key);

	if (system_info_vconf_get_value_double(vconfkey, value)) {
		LOGE("key : %s, failed get double value", key);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_external_string(const char *key, char **value)
{
	char vconfkey[MAXBUFSIZE] = {0,};

	snprintf(vconfkey, strlen(EXTERNAL_VCONF_PREFIX)+strlen(key)+1, "%s%s", EXTERNAL_VCONF_PREFIX, key);

	if (system_info_vconf_get_value_string(vconfkey, value)) {
		LOGE("key : %s, failed get string value", key);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return SYSTEM_INFO_ERROR_NONE;
}
int system_info_get_custom_double(const char *key, double *value)
{
	int ret;
	double *ret_val;
	char *string = NULL;

	ret_val = (double *)value;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, DBL_TYPE, &string);
	if (ret) {
		LOGI("cannot get %s", key);
		*ret_val = 0;
		return SYSTEM_INFO_ERROR_NONE;
	}

	*ret_val = atof(string);

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_custom_string(const char *key, char **value)
{
	int ret;
	char *string = NULL;

	if (access(CONFIG_FILE_PATH, R_OK)) {
		LOGE("cannot find file %s!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	ret = system_info_get_value_from_config_xml(CUSTOM_TAG, key, STR_TYPE, &string);
	if (ret) {
		LOGE("cannot get %s info from %s!!!", key, CONFIG_FILE_PATH);
		return ret;
	}

	*value = string;

	return SYSTEM_INFO_ERROR_NONE;
}
