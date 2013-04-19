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


#ifndef __TIZEN_SYSTEM_SYSTEM_INFO_H__
#define __TIZEN_SYSTEM_SYSTEM_INFO_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup CAPI_SYSTEM_SYSTEM_INFO_MODULE
 * @{
 */

/**
 * @brief Enumeration of error code for system information
 */
typedef enum {
	SYSTEM_INFO_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	SYSTEM_INFO_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	SYSTEM_INFO_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	SYSTEM_INFO_ERROR_IO_ERROR =  TIZEN_ERROR_IO_ERROR,	/**< An input/output error occurred when read value from system */
} system_info_error_e;

/**
 * @brief Enumeration of key for system information
 */
typedef enum {
	SYSTEM_INFO_KEY_MODEL, /**< The model of the device */
	SYSTEM_INFO_KEY_TIZEN_VERSION, /**< The version of the Tizen API */
	SYSTEM_INFO_KEY_CAMERA_COUNT, /**< The number of cameras in the device */
	SYSTEM_INFO_KEY_KEYBOARD_TYPE, /**< The type of the keyboard */
	SYSTEM_INFO_KEY_MULTI_POINT_TOUCH_COUNT, /**< The maximum number of concurrent touch points supported in the device */
	SYSTEM_INFO_KEY_NETWORK_TYPE, /**< The supported network type */
	SYSTEM_INFO_KEY_OPENGLES_VERSION, /**< The supported version of the OpenGL ES */
	SYSTEM_INFO_KEY_SCREEN_BITS_PER_PIXEL, /**< The number of bits per pixel */
	SYSTEM_INFO_KEY_SCREEN_HEIGHT, /**< The height of the screen in pixels */
	SYSTEM_INFO_KEY_SCREEN_WIDTH, /**< The width of the screen in pixels */
	SYSTEM_INFO_KEY_MOBILE_DEVICE_ID, /**< The unique ID to identify GSM, UMTS and CDMA mobile devices */
	SYSTEM_INFO_KEY_CSC_SALES_CODE, /**< The sales code of CSC */
	SYSTEM_INFO_KEY_PLATFORM_NAME, /**< The name of platform */
	SYSTEM_INFO_KEY_TIZEN_VERSION_NAME, /**< The name of tizen version  */
	SYSTEM_INFO_KEY_SCREEN_DPI, /**< The width of the screen DPI */
	SYSTEM_INFO_KEY_DEVICE_UUID, /**< The Device unique ID to identify */
	SYSTEM_INFO_KEY_CORE_CPU_ARCH, /**< The CORE CPU architecture of model */
	SYSTEM_INFO_KEY_CORE_FPU_ARCH, /**< The CORE FPU architecture of model */
	SYSTEM_INFO_KEY_OPENGLES_TEXTURE_FORMAT, /**< The supported texture format of the OpenGL ES */
	SYSTEM_INFO_KEY_CORE_CPU_FREQ, /**< The CORE CPU frequency of model */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_HEIGHT, /**< The physical screen height in millimeters */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_WIDTH, /**< The physical screen width in millimeters */
	SYSTEM_INFO_KEY_BUILD_STRING, /**< The build string of platform binary */
	SYSTEM_INFO_KEY_BUILD_DATE, /**< The build date of platform binary */
	SYSTEM_INFO_KEY_BUILD_TIME, /**< The build time of platform binary */
	SYSTEM_INFO_KEY_BLUETOOTH_SUPPORTED, /**< Indicates whether the device supports Bluetooth */
	SYSTEM_INFO_KEY_FMRADIO_SUPPORTED, /**< Indicates whether the device supports FM radio */
	SYSTEM_INFO_KEY_GPS_SUPPORTED, /**< Indicates whether the device supports GPS */
	SYSTEM_INFO_KEY_NFC_SUPPORTED, /**< Indicates whether the device supports NFC */
	SYSTEM_INFO_KEY_TVOUT_SUPPORTED, /**< Indicates whether the device supports TV-out */
	SYSTEM_INFO_KEY_WIFI_SUPPORTED, /**< Indicates whether the device supports Wi-Fi */
	SYSTEM_INFO_KEY_HAPTIC_SUPPORTED, /**< Indicates whether the device supports Haptic */
	SYSTEM_INFO_KEY_WIFI_DIRECT_SUPPORTED, /**< Indicates whether the device supports Wi-Fi Direct */
	SYSTEM_INFO_KEY_CPS_SUPPORTED, /**< Indicates whether the device supports CPS */
	SYSTEM_INFO_KEY_WPS_SUPPORTED, /**< Indicates whether the device supports WPS */
	SYSTEM_INFO_KEY_USB_HOST_SUPPORTED, /**< Indicates whether the device supports USB host */
	SYSTEM_INFO_KEY_USB_ACCESSORY_SUPPORTED, /**< Indicates whether the device supports USB accessory */
	SYSTEM_INFO_KEY_FRONT_CAMERA_SUPPORTED, /**< Indicates whether the device supports front camera */
	SYSTEM_INFO_KEY_FRONT_CAMERA_AF_SUPPORTED, /**< Indicates whether the device supports front camera auto focus */
	SYSTEM_INFO_KEY_FRONT_CAMERA_FLASH_SUPPORTED, /**< Indicates whether the device supports front camera flash */
	SYSTEM_INFO_KEY_BACK_CAMERA_SUPPORTED, /**< Indicates whether the device supports back camera */
	SYSTEM_INFO_KEY_BACK_CAMERA_AF_SUPPORTED, /**< Indicates whether the device supports back camera auto focus */
	SYSTEM_INFO_KEY_BACK_CAMERA_FLASH_SUPPORTED, /**< Indicates whether the device supports back camera flash */
	SYSTEM_INFO_KEY_HDMI_SUPPORTED, /**< Indicates whether the device supports HDMI */
	SYSTEM_INFO_KEY_RCA_SUPPORTED, /**< Indicates whether the device supports RCA */
	SYSTEM_INFO_KEY_SIP_VOIP_SUPPORTED, /**< Indicates whether the device supports SIP VOIP */
	SYSTEM_INFO_KEY_MICROPHONE_SUPPORTED, /**< Indicates whether the device supports Microphone */
	SYSTEM_INFO_KEY_SPEECH_RECOGNITION_SUPPORTED, /**< Indicates whether the device supports stt */
	SYSTEM_INFO_KEY_BAROMETER_SENSOR_SUPPORTED, /**< Indicates whether the device supports barometer sensor */
	SYSTEM_INFO_KEY_MANUFACTURER, /**< The manufacturer of the device */
	SYSTEM_INFO_KEY_CP_INTERFACE, /**< The method of interface with CP */
	SYSTEM_INFO_KEY_MMS_SUPPORTED, /**< Indicates whether the device supports MMS */
	SYSTEM_INFO_KEY_SMS_SUPPORTED, /**< Indicates whether the device supports SMS */
	SYSTEM_INFO_KEY_CBS_SUPPORTED, /**< Indicates whether the device supports CBS */
	SYSTEM_INFO_KEY_NFC_RESERVED_PUSH_SUPPORTED, /**< Indicates whether the device supports nfc-reserved push */
	SYSTEM_INFO_KEY_TETHERING_SUPPORTED, /**< Indicates whether the device supports tethering */
	SYSTEM_INFO_KEY_SPEECH_SYNTHESIS_SUPPORTED, /**< Indicates whether the device supports tts */
	SYSTEM_INFO_KEY_GRAPHICS_HWACCEL_SUPPORTED, /**< Indicates whether the device supports graphics hardware acceleration */
	SYSTEM_INFO_KEY_FEATURE_AUTO_ROTATION_SUPPORTED, /**< Indicates whether the device supports native auto rotation feature */
	SYSTEM_INFO_KEY_FEATURE_PINCH_ZOOM_SUPPORTED, /**< Indicates whether the device supports pinch zoom feature */
} system_info_key_e;

/**
 * @brief   Gets the integer value of the system information
 * @param[in] key The name of the system information to get
 * @param[out] value The value of the given system information
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from system
 */
int system_info_get_value_int(system_info_key_e key, int *value);

/**
 * @brief   Gets the boolean value of the system information
 * @param[in] key The name of the system information to get
 * @param[out] value The value of the given system information
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from system
 */
int system_info_get_value_bool(system_info_key_e key, bool *value);

/**
 * @brief   Gets the double value of the system information
 * @param[in] key The name of the system information to get
 * @param[out] value The value of the given system information
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from system
 */
int system_info_get_value_double(system_info_key_e key, double *value);

/**
 * @brief   Gets the string value of the system information
 * @remarks The @a value must be released with free() by you.
 * @param[in] key The name of the system information to get
 * @param[out] value The value of the given system information
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SYSTEM_INFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from system
 */
int system_info_get_value_string(system_info_key_e key, char **value);

int system_info_get_external_bool(const char *key, bool *value);
int system_info_get_external_int(const char *key, int *value);
int system_info_get_external_double(const char *key, double *value);
int system_info_get_external_string(const char *key, char **value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_H__ */
