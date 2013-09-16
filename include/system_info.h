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
	SYSTEM_INFO_KEY_PLATFORM_NAME, /**< The name of platform */
	SYSTEM_INFO_KEY_TIZEN_VERSION_NAME, /**< The name of tizen version  */
	SYSTEM_INFO_KEY_MANUFACTURER, /**< The manufacturer of the device */
	SYSTEM_INFO_KEY_CORE_CPU_ARCH, /**< The CORE CPU architecture of model */
	SYSTEM_INFO_KEY_CORE_CPU_FREQ, /**< The CORE CPU frequency of model */
	SYSTEM_INFO_KEY_BUILD_STRING, /**< The build string of platform binary */
	SYSTEM_INFO_KEY_BUILD_DATE, /**< The build date of platform binary */
	SYSTEM_INFO_KEY_BUILD_TIME, /**< The build time of platform binary */
	SYSTEM_INFO_KEY_SCREEN_HEIGHT, /**< The height of the screen in pixels */
	SYSTEM_INFO_KEY_SCREEN_WIDTH, /**< The width of the screen in pixels */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_HEIGHT, /**< The physical screen height in millimeters */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_WIDTH, /**< The physical screen width in millimeters */
	SYSTEM_INFO_KEY_TETHERING_SUPPORTED, /**< Indicates whether the device supports tethering */
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

/**
 * @brief   Gets the boolean value of the platform feature
 * @param[in] key The name of the platform feature to get
 * @param[out] value The value of the given platform feature
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER cannot find key in model config file
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from model config file
 */
int system_info_get_platform_bool(const char *key, bool *value);

/**
 * @brief   Gets the integer value of the platform feature
 * @param[in] key The name of the platform feature to get
 * @param[out] value The value of the given platform feature
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER cannot find key in model config file
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from model config file
 */
int system_info_get_platform_int(const char *key, int *value);

/**
 * @brief   Gets the string value of the platform feature
 * @remarks The @a value must be released with free() by you.
 * @param[in] key The name of the platform feature to get
 * @param[out] value The value of the given platform feature
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER cannot find key in model config file
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from model config file
 */
int system_info_get_platform_double(const char *key, double *value);

/**
 * @brief   Gets the string value of the platform feature
 * @remarks The @a value must be released with free() by you.
 * @param[in] key The name of the platform feature to get
 * @param[out] value The value of the given platform feature
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER cannot find key in model config file
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from model config file
 */
int system_info_get_platform_string(const char *key, char **value);


/**
 * @brief   Gets the boolean value of the custom feature
 * @param[in] key The name of the custom feature to get
 * @param[out] value The value of the given custom feature
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER cannot find key in model config file
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from model config file
 */
int system_info_get_custom_bool(const char *key, bool *value);

/**
 * @brief   Gets the string value of the custom feature
 * @remarks The @a value must be released with free() by you.
 * @param[in] key The name of the custom feature to get
 * @param[out] value The value of the given custom feature
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER cannot find key in model config file
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from model config file
 */
int system_info_get_custom_int(const char *key, int *value);

/**
 * @brief   Gets the string value of the custom feature
 * @remarks The @a value must be released with free() by you.
 * @param[in] key The name of the custom feature to get
 * @param[out] value The value of the given custom feature
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER cannot find key in model config file
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from model config file
 */
int system_info_get_custom_double(const char *key, double *value);

/**
 * @brief   Gets the string value of the custom feature
 * @remarks The @a value must be released with free() by you.
 * @param[in] key The name of the custom feature to get
 * @param[out] value The value of the given custom feature
 * @return  0 on success, otherwise a negative error value.
 * @retval  #SYSTEM_INFO_ERROR_NONE Successful
 * @retval  #SYSTEM_INFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval  #SYSTEM_INFO_ERROR_INVALID_PARAMETER cannot find key in model config file
 * @retval  #SYSTEM_INFO_ERROR_IO_ERROR An input/output error occurred when read value from model config file
 */
int system_info_get_custom_string(const char *key, char **value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_H__ */
