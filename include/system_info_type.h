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


#ifndef __TIZEN_SYSTEM_SYSTEM_INFO_TYPE_H__
#define __TIZEN_SYSTEM_SYSTEM_INFO_TYPE_H__

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
 * @brief Enumeration of error codes for system information
 */
typedef enum {
	SYSTEM_INFO_ERROR_NONE              = TIZEN_ERROR_NONE,          /**< Successful */
	SYSTEM_INFO_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	SYSTEM_INFO_ERROR_OUT_OF_MEMORY     = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	SYSTEM_INFO_ERROR_IO_ERROR          = TIZEN_ERROR_IO_ERROR,	 /**< An input/output error occurred when reading value from system */
	SYSTEM_INFO_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< No permission to use the API */
} system_info_error_e;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_TYPE_H__ */
