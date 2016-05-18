/*
 * Copyright (c) 2011 - 2015 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__
#define __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <dlog.h>
#include "system_info_type.h"

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

#undef LOG_TAG
#define LOG_TAG "CAPI_SYSTEM_INFO"
#define _D(fmt, args...)   SLOGD(fmt, ##args)
#define _E(fmt, args...)   SLOGE(fmt, ##args)
#define _I(fmt, args...)   SLOGI(fmt, ##args)

#define TAG_TYPE_PLATFORM_STR	"platform"
#define TAG_TYPE_CUSTOM_STR		"custom"

#define BOOL_TYPE	"bool"
#define INT_TYPE	"int"
#define DBL_TYPE	"double"
#define STR_TYPE	"string"

int system_info_get_file(const char *key, char *value, size_t len);
int system_info_get_type_file(const char *key, system_info_type_e *type);

int external_get_value(const char *tag, const char *key, const char *type, char **value);
int external_get_type(const char *tag, const char *key, char **type);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__ */
