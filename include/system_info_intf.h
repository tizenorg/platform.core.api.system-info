/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_SYSTEM_INFO_INTF_H__
#define __TIZEN_SYSTEM_SYSTEM_INFO_INTF_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

#define TAG_CUSTOM	"custom"

#define TYPE_BOOL	"bool"
#define TYPE_INT	"int"
#define TYPE_DBL	"double"
#define TYPE_STR	"string"

typedef struct {
	int (*get_value_external)(const char *tag,
			const char *key, const char *type,
			char *buf, unsigned int len);
	int (*get_type_external)(const char *tag,
			const char *key, char *buf, unsigned int len);

} system_info_external_plugin_interface;

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__ */