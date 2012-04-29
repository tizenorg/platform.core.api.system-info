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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dlog.h>
#include <vconf.h>
#include <iniparser.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZEN_N_SYSTEM_INFO"

#define TIZEN_VERSION "1.0"
#define DEVICE_MODEL "SDK"
#define LIB_GLES_V1 "/usr/lib/libGLESv1_CM.so"
#define LIB_GLES_V2 "/usr/lib/libGLESv2.so"

int system_info_get_model(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *model_dup;

	model_dup = strdup(DEVICE_MODEL);

	if (model_dup == NULL)
	{
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __FUNCTION__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = model_dup;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_tizen_version(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *version_dup;

	version_dup = strdup(TIZEN_VERSION);

	if (version_dup == NULL)
	{
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __FUNCTION__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = version_dup;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_opengles_version(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *version;
	int version_supported = 0;
	char *version_dup = NULL;
	
	if (!access(LIB_GLES_V1, R_OK))
	{
		version_supported += 1;
	}

	if (!access(LIB_GLES_V2, R_OK))
	{
		version_supported += 2;
	}

	switch (version_supported)
	{
	case 1:
		version = "1.1";
		break;

	case 2:
		version = "2.0";
		break;

	case 3:
		version = "1.0/2.0";
		break;
	default:
		version = NULL;
		break;
	}

	if (version != NULL)
	{
		version_dup = strdup(version);

		if (version_dup == NULL)
		{
			LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __FUNCTION__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
			return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
		}
	}

	*value = version_dup;

	return SYSTEM_INFO_ERROR_NONE;
}

