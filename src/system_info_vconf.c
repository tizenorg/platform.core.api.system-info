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

#define LOG_TAG "CAPI_SYSTEM_INFO"

int system_info_vconf_get_value_int(const char *vconf_key, int *value)
{
	return vconf_get_int(vconf_key, value);
}

int system_info_vconf_get_value_bool(const char *vconf_key, bool *value)
{
	return vconf_get_bool(vconf_key, (int *)value);
}

int system_info_vconf_get_value_double(const char *vconf_key, double *value)
{
	return vconf_get_dbl(vconf_key, value);
}

int system_info_vconf_get_value_string(const char *vconf_key, char **value)
{
	char *str_value = NULL;

	str_value = vconf_get_str(vconf_key);

	if (str_value != NULL) {
		*value = str_value;
		return 0;
	} else {
		return -1;
	}
}
