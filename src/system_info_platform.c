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

#include <dlog.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

int system_info_get_model(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "version:model", (char **)value);
}

int system_info_get_build_string(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "version:build", (char **)value);
}

int system_info_get_build_date(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "build:date", (char **)value);
}

int system_info_get_build_time(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_ini_get_string(INFO_FILE_PATH, "build:time", (char **)value);
}

int system_info_get_tizen_version(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_string("tizen.org/feature/platform.version", (char**)value);
}

int system_info_get_core_cpu_arch(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool cpu_arch;
	char *CORE_CPU_ARCH = NULL;

	if (system_info_get_platform_bool("tizen.org/feature/platform.core.cpu.arch.armv6", &cpu_arch) == SYSTEM_INFO_ERROR_NONE
		&& cpu_arch == true)
		CORE_CPU_ARCH = strdup("armv6");
	else if (system_info_get_platform_bool("tizen.org/feature/platform.core.cpu.arch.armv7", &cpu_arch) == SYSTEM_INFO_ERROR_NONE
		&& cpu_arch == true)
		CORE_CPU_ARCH = strdup("armv7");
	else if (system_info_get_platform_bool("tizen.org/feature/platform.core.cpu.arch.x86", &cpu_arch) == SYSTEM_INFO_ERROR_NONE
		&& cpu_arch == true)
		CORE_CPU_ARCH = strdup("x86");

		if (CORE_CPU_ARCH == NULL) {
		LOGE("Unknown cpu");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	*value = CORE_CPU_ARCH;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_core_fpu_arch(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool fpu_arch;
	char *CORE_FPU_ARCH = NULL;

	if (system_info_get_platform_bool("tizen.org/feature/platform.core.fpu.arch.sse2", &fpu_arch) == SYSTEM_INFO_ERROR_NONE
		&& fpu_arch == true)
					CORE_FPU_ARCH = strdup("sse2");
	else if (system_info_get_platform_bool("tizen.org/feature/platform.core.fpu.arch.sse3", &fpu_arch) == SYSTEM_INFO_ERROR_NONE
		&& fpu_arch == true)
		CORE_FPU_ARCH = strdup("sse3");
	else if (system_info_get_platform_bool("tizen.org/feature/platform.core.fpu.arch.ssse3", &fpu_arch) == SYSTEM_INFO_ERROR_NONE
		&& fpu_arch == true)
		CORE_FPU_ARCH = strdup("ssse3");
	else if (system_info_get_platform_bool("tizen.org/feature/platform.core.fpu.arch.vfpv2", &fpu_arch) == SYSTEM_INFO_ERROR_NONE
		&& fpu_arch == true)
		CORE_FPU_ARCH = strdup("vfpv2");
	else if (system_info_get_platform_bool("tizen.org/feature/platform.core.fpu.arch.vfpv3", &fpu_arch) == SYSTEM_INFO_ERROR_NONE
		&& fpu_arch == true)
		CORE_FPU_ARCH = strdup("vfpv3");

				if (CORE_FPU_ARCH == NULL) {
		LOGE("Unknown fpu");
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	*value = CORE_FPU_ARCH;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_core_cpu_freq(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	double *count;
	FILE *cpuinfo, *cpuinfo_max_freq;
	double max_freq = 0.0;
	char *name;
	char str[MAXBUFSIZE];

	if (system_info_get_system_info_model_type() != SYSTEM_INFO_MODEL_TYPE_EMULATOR) {
		cpuinfo_max_freq = fopen(CPU_INFO_MAX_FREQ_PATH, "r");
		if (NULL == cpuinfo_max_freq) {
			LOGE("cannot file open %s file!!!", CPU_INFO_MAX_FREQ_PATH);
			return SYSTEM_INFO_ERROR_IO_ERROR;
		} else {
			if (fscanf(cpuinfo_max_freq, "%lf", &max_freq) < 1) {
				fclose(cpuinfo_max_freq);
				return SYSTEM_INFO_ERROR_IO_ERROR;
			}
			max_freq = max_freq / 1024;
		}
		fclose(cpuinfo_max_freq);
	} else {
		/* Emulator */
		cpuinfo = fopen(CPU_INFO_FILE_PATH, "r");
		if (NULL == cpuinfo) {
			LOGE("cannot file open %s file!!!", CPU_INFO_FILE_PATH);
			return SYSTEM_INFO_ERROR_IO_ERROR;
		} else {
			while (fgets(str, MAXBUFSIZE, cpuinfo)) {
				if (!strncmp("cpu MHz", str, strlen("cpu MHz"))) {
					name = strchr(str, ':');
					max_freq = atof(name+2);
					break;
				}
			}
		}
		fclose(cpuinfo);
	}

	count = (double *)value;

	*count = max_freq;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_platform_name(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_string("tizen.org/system/platform.name", (char**)value);
	}

int system_info_get_tizen_version_name(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *TIZEN_VERSION_NAME = NULL;
	char *name = NULL;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	int tmpStrlen = 0;
	FILE *info;
	extern char *strcasestr(const char *s, const char *find);

	info = fopen(OS_RELEASE_FILE_PATH, "r");
	if (NULL == info) {
		LOGE("cannot file open %s file!!!", OS_RELEASE_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	while (fgets(str, MAXBUFSIZE, info)) {
		if (strcasestr(str, "VERSION")) {
			name = strchr(str, ',');
			name += 2;
			tmpStrlen = strlen(name);

			strncpy(tmpStr, name, tmpStrlen-2);
			tmpStr[tmpStrlen-2] = '\0';

			TIZEN_VERSION_NAME = strdup(tmpStr);
			if (TIZEN_VERSION_NAME == NULL) {
				LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
				fclose(info);
				return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
			}
			break;
		}
	}

	*value = TIZEN_VERSION_NAME;
	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_opengles_version(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool glversion = 0;
	char *version;
	char *OPENGLES_VERSION = NULL;
	int version_supported = 0;

	if (system_info_get_platform_bool("tizen.org/feature/opengles.version.1_1", &glversion) == SYSTEM_INFO_ERROR_NONE
		&& glversion == true)
		version_supported += 1;

	if (system_info_get_platform_bool("tizen.org/feature/opengles.version.2_0", &glversion) == SYSTEM_INFO_ERROR_NONE
		&& glversion == true)
		version_supported += 2;

	switch (version_supported) {
	case 1:
		version = "1.1";
		break;

	case 2:
		version = "2.0";
		break;

	case 3:
		version = "1.1/2.0";
		break;
	default:
		version = NULL;
				break;
}

	if (version != NULL) {
		OPENGLES_VERSION = strdup(version);

		if (OPENGLES_VERSION == NULL) {
					LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
					return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
				}
	}

	*value = OPENGLES_VERSION;

		return SYSTEM_INFO_ERROR_NONE;
	}

int system_info_get_opengles_texture_format(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool texture;
	char *OPENGLES_TEXTURE_FORMAT;
	char textureformat[MAXBUFSIZE];

	textureformat[0] = '\0';

	if (system_info_get_platform_bool("tizen.org/feature/opengles.texture_format.utc", &texture) == SYSTEM_INFO_ERROR_NONE
		&& texture == true)
		strcat(textureformat, "utc ");
	if (system_info_get_platform_bool("tizen.org/feature/opengles.texture_format.ptc", &texture) == SYSTEM_INFO_ERROR_NONE
		&& texture == true)
		strcat(textureformat, "| ptc ");
	if (system_info_get_platform_bool("tizen.org/feature/opengles.texture_format.etc", &texture) == SYSTEM_INFO_ERROR_NONE
		&& texture == true)
		strcat(textureformat, "| etc ");
	if (system_info_get_platform_bool("tizen.org/feature/opengles.texture_format.3dc", &texture) == SYSTEM_INFO_ERROR_NONE
		&& texture == true)
		strcat(textureformat, "| 3dc ");
	if (system_info_get_platform_bool("tizen.org/feature/opengles.texture_format.atc", &texture) == SYSTEM_INFO_ERROR_NONE
		&& texture == true)
		strcat(textureformat, "| atc ");
	if (system_info_get_platform_bool("tizen.org/feature/opengles.texture_format.pvrtc", &texture) == SYSTEM_INFO_ERROR_NONE
		&& texture == true)
		strcat(textureformat, "| pvrtc");

	OPENGLES_TEXTURE_FORMAT = strdup(textureformat);

	if (OPENGLES_TEXTURE_FORMAT == NULL) {
		LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = OPENGLES_TEXTURE_FORMAT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_mms_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/network.telephony.mms", (bool *)value);
	}

int system_info_get_sms_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/network.telephony.sms", (bool *)value);
	}

int system_info_get_cbs_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/network.telephony.cbs", (bool *)value);
}

int system_info_get_graphics_hwaccel_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/graphics.acceleration", (bool *)value);
}

int system_info_get_feature_pinch_zoom_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/multi_point_touch.pinch_zoom", (bool *)value);
}

int system_info_get_feature_auto_rotation_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/screen.auto_rotation", (bool *)value);
}
