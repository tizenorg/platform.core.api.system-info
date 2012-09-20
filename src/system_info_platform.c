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

#include <GLES/gl.h>
#include <GLES/glext.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZEN_N_SYSTEM_INFO"

#define LIB_GLES_V1 "/usr/lib/libGLESv1_CM.so"
#define LIB_GLES_V2 "/usr/lib/libGLESv2.so"

#define SIZE_OF_MODEL_NAME 8

int system_info_get_model(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	FILE *info = NULL;
	char *name = NULL;
	char *MODEL = NULL;
	char *token = NULL;
	char tmpStr[MAXBUFSIZE];
	char str[MAXBUFSIZE];
	extern char *strcasestr(const char *s, const char *find);

	if (system_info_get_system_info_model_type() != SYSTEM_INFO_MODEL_TYPE_EMULATOR) {
		info = fopen(INFO_FILE_PATH, "r");
		if (NULL == info) {
			LOGE("[%s] cannot file open %s file!!!", __func__, INFO_FILE_PATH);
			return SYSTEM_INFO_ERROR_IO_ERROR;
		} else {
			while (fgets(str, MAXBUFSIZE, info)) {
				if (strncmp("Build", str, strlen("Build")))
					continue;
				else {
					/* Open SDK Target*/
					if (strcasestr(str, "Tizen")) {
						strcpy(tmpStr, str);
						token = strtok(tmpStr, "_");
						token = strtok(NULL, "_");
						MODEL = strdup(token);
						if (MODEL == NULL) {
							LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
							fclose(info);
							return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
						}
					} else {
						/* DEV, SEL, REL Target */
						name = strchr(str, '=');
						strncpy(tmpStr, name+1, SIZE_OF_MODEL_NAME);
						tmpStr[SIZE_OF_MODEL_NAME] = '\0';
						MODEL = strdup(tmpStr);
						if (MODEL == NULL) {
							LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
							fclose(info);
							return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
						}
					}
					break;
				}
			}
			fclose(info);
		}
	} else {
		/* Emulator */
		MODEL = strdup("Emulator");
		if (MODEL == NULL) {
			LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
			return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
		}
	}

	*value = MODEL;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_tizen_version(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *TIZEN_VERSION = NULL;
	char *name = NULL;
	char *major = NULL;
	char *minor = NULL;
	char *order = NULL;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	FILE *info;


	info = fopen(INFO_FILE_PATH, "r");
	if (NULL == info) {
		LOGE("[%s] cannot file open %s file!!!", __func__, INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp("Major", str, strlen("Major"))) {
				name = strchr(str, '=');
				name++;
				strncpy(tmpStr, name, strlen(name)-2);
				tmpStr[strlen(name)-2] = '\0';
				major = strdup(tmpStr);
				continue;
			} else if (!strncmp("Minor", str, strlen("Minor"))) {
				name = strchr(str, '=');
				name++;
				strncpy(tmpStr, name, strlen(name)-2);
				tmpStr[strlen(name)-2] = '\0';
				minor = strdup(tmpStr);
				continue;
			} else if (!strncmp("Order", str, strlen("Order"))) {
				name = strchr(str, '=');
				name++;
				strncpy(tmpStr, name, strlen(name)-2);
				tmpStr[strlen(name)-2] = '\0';
				if (!strcmp(tmpStr, ""))
					order = strdup("0");
				else
					order = strdup(tmpStr);
				continue;
			} else
				continue;
		}
	}
	fclose(info);

	TIZEN_VERSION = (char *)malloc(strlen(major)+strlen(minor)+strlen(order)+3);

	if (TIZEN_VERSION == NULL) {
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		free(major);
		free(minor);
		free(order);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	memset(TIZEN_VERSION, 0, strlen(major)+strlen(minor)+strlen(order)+3);

	sprintf(TIZEN_VERSION, "%s.%s.%s", major, minor, order);
	TIZEN_VERSION[strlen(TIZEN_VERSION)] = '\0';

	*value = TIZEN_VERSION;

	free(major);
	free(minor);
	free(order);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_platform_name(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *PLATFORM_NAME = NULL;

	PLATFORM_NAME = strdup("TIZEN");
	if (PLATFORM_NAME == NULL) {
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = PLATFORM_NAME;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_tizen_version_name(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *TIZEN_VERSION_NAME = NULL;
	char *name = NULL;
	char str[MAXBUFSIZE];
	FILE *info = NULL;

	info = fopen(INFO_FILE_PATH, "r");
	if (NULL == info) {
		LOGE("[%s] cannot file open %s file!!!", __func__, INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp("Major", str, strlen("Major"))) {
				name = strchr(str, '=');
				name++;
				if (!strncmp("1", name, 1))
					TIZEN_VERSION_NAME = strdup("Larkspur");
				else if (!strncmp("2", name, 1))
					TIZEN_VERSION_NAME = strdup("Magnolia");
				else
					TIZEN_VERSION_NAME = strdup("Unknown Version Name");

				if (TIZEN_VERSION_NAME == NULL) {
					LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
					fclose(info);
					return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
				}

				break;
			} else
				continue;
		}
	}
	*value = TIZEN_VERSION_NAME;
	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_opengles_version(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *version;
	char *OPENGLES_VERSION = NULL;
	int version_supported = 0;

	if (!access(LIB_GLES_V1, R_OK))
		version_supported += 1;

	if (!access(LIB_GLES_V2, R_OK))
		version_supported += 2;

	switch (version_supported) {
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

	if (version != NULL) {
		OPENGLES_VERSION = strdup(version);

		if (OPENGLES_VERSION == NULL) {
			LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
			return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
		}
	}

	*value = OPENGLES_VERSION;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_core_cpu_arch(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	FILE *cpuinfo;
	char *CORE_CPU_ARCH = NULL;
	char *name;
	char str[MAXBUFSIZE];

	if (system_info_get_system_info_model_type() != SYSTEM_INFO_MODEL_TYPE_EMULATOR) {
		cpuinfo = fopen(CPU_INFO_FILE_PATH, "r");
		if (NULL == cpuinfo) {
			LOGE("[%s] cannot file open %s file!!!", __func__, CPU_INFO_FILE_PATH);
			return SYSTEM_INFO_ERROR_IO_ERROR;
		} else {
			while (fgets(str, MAXBUFSIZE, cpuinfo)) {
				if (!strncmp("Processor", str, strlen("Processor"))) {
					name = strchr(str, ':');
					if (!(strncmp("ARMv7", name+2, strlen("ARMv7"))))
						CORE_CPU_ARCH = strdup("ARMv7");
					else if (!(strncmp("ARMv6", name+2, strlen("ARMv6"))))
						CORE_CPU_ARCH = strdup("ARMv6");
					else if (!(strncmp("x86", name+2, strlen("x86"))))
						CORE_CPU_ARCH = strdup("x86");
				} else
					continue;
			}
			fclose(cpuinfo);
		}
	} else {
		/* Emulator */
		CORE_CPU_ARCH = NULL;
	}

	*value = CORE_CPU_ARCH;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_core_fpu_arch(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	FILE *cpuinfo;
	char *CORE_FPU_ARCH = NULL;
	char *name;
	char str[MAXBUFSIZE];

	if (system_info_get_system_info_model_type() != SYSTEM_INFO_MODEL_TYPE_EMULATOR) {
		cpuinfo = fopen(CPU_INFO_FILE_PATH, "r");
		if (NULL == cpuinfo) {
			LOGE("[%s] cannot file open %s file!!!", __func__, CPU_INFO_FILE_PATH);
			return SYSTEM_INFO_ERROR_IO_ERROR;
		} else {
			while (fgets(str, MAXBUFSIZE, cpuinfo)) {
				if (!strncmp("Features", str, strlen("Features"))) {
					name = strchr(str, ':');
					if (strstr(name+2, "vfpv3"))
						CORE_FPU_ARCH = strdup("vfpv3");
					else if (strstr(name+2, "vfpv2"))
						CORE_FPU_ARCH = strdup("vfpv2");
					else if (strstr(name+2, "sse2"))
						CORE_FPU_ARCH = strdup("sse2");
					else if (strstr(name+2, "sse3"))
						CORE_FPU_ARCH = strdup("sse3");
					else if (strstr(name+2, "ssse3"))
						CORE_FPU_ARCH = strdup("ssse3");
				} else
					continue;
			}
			fclose(cpuinfo);
		}
	} else {
		/* Emulator */
		CORE_FPU_ARCH = NULL;
	}

	*value = CORE_FPU_ARCH;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_core_cpu_freq(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	double *count;
	FILE *cpuinfo, *cpuinfo_max_freq;
	double max_freq;
	char *name;
	char str[MAXBUFSIZE];

	if (system_info_get_system_info_model_type() != SYSTEM_INFO_MODEL_TYPE_EMULATOR) {
		cpuinfo_max_freq = fopen(CPU_INFO_MAX_FREQ_PATH, "r");
		if (NULL == cpuinfo_max_freq) {
			LOGE("[%s] cannot file open %s file!!!", __func__, CPU_INFO_MAX_FREQ_PATH);
			return SYSTEM_INFO_ERROR_IO_ERROR;
		} else {
			if (fscanf(cpuinfo_max_freq, "%lf", &max_freq) < 1) {
				fclose(cpuinfo_max_freq);
				return SYSTEM_INFO_ERROR_IO_ERROR;
			}
			max_freq = max_freq / 1024;
		}
	} else {
		/* Emulator */
		cpuinfo = fopen(CPU_INFO_FILE_PATH, "r");
		if (NULL == cpuinfo) {
			LOGE("[%s] cannot file open %s file!!!", __func__, CPU_INFO_FILE_PATH);
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
	}

	count = (double *)value;

	*count = max_freq;

	return SYSTEM_INFO_ERROR_NONE;
}


int system_info_get_opengles_texture_format(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int cnt = 0;
	char *OPENGLES_TEXTURE_FORMAT;
	char textureformat[MAXBUFSIZE];

	memset(textureformat, 0, MAXBUFSIZE);

	if (!access(LIB_GLES_V1, R_OK)) {
		strncpy(textureformat+cnt, "utc ", strlen("utc "));
		cnt += strlen("utc ");
	}
	if (GL_OES_compressed_paletted_texture) {
		strncpy(textureformat+cnt, "| ptc ", strlen("| ptc "));
		cnt += strlen("| ptc ");
	}
	if (GL_OES_compressed_ETC1_RGB8_texture) {
		strncpy(textureformat+cnt, "| etc ", strlen("| etc "));
		cnt += strlen("| etc ");
	}
	if (GL_AMD_compressed_3DC_texture) {
		strncpy(textureformat+cnt, "| 3dc ", strlen("| 3dc "));
		cnt += strlen("| 3dc ");
	}
	if (GL_AMD_compressed_ATC_texture) {
		strncpy(textureformat+cnt, "| atc ", strlen("| atc "));
		cnt += strlen("| atc ");
	}
	if (GL_IMG_texture_compression_pvrtc) {
		strncpy(textureformat+cnt, "| pvrtc", strlen("| pvrtc"));
		cnt += strlen("| pvrtc");
	}

	OPENGLES_TEXTURE_FORMAT = strdup(textureformat);

	if (OPENGLES_TEXTURE_FORMAT == NULL) {
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*value = OPENGLES_TEXTURE_FORMAT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_build_string(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *BUILD_STRING = NULL;
	char *name = NULL;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	FILE *info = NULL;

	info = fopen(INFO_FILE_PATH, "r");
	if (NULL == info) {
		LOGE("[%s] cannot file open %s file!!!", __func__, INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp("Build", str, strlen("Build"))) {
				name = strchr(str, '=');
				name++;
				strncpy(tmpStr, name, strlen(name)-2);
				tmpStr[strlen(name)-2] = '\0';

				BUILD_STRING = strdup(tmpStr);
				if (BUILD_STRING == NULL) {
					LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
					fclose(info);
					return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
				}
				break;
			} else
				continue;
		}
	}
	*value = BUILD_STRING;
	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_build_date(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *BUILD_DATE = NULL;
	char *name = NULL;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	FILE *info = NULL;

	info = fopen(INFO_FILE_PATH, "r");
	if (NULL == info) {
		LOGE("[%s] cannot file open %s file!!!", __func__, INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp("Date", str, strlen("Date"))) {
				name = strchr(str, '=');
				name++;
				strncpy(tmpStr, name, strlen(name)-2);
				tmpStr[strlen(name)-2] = '\0';

				BUILD_DATE = strdup(tmpStr);
				if (BUILD_DATE == NULL) {
					LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
					fclose(info);
					return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
				}
				break;
			} else
				continue;
		}
	}
	*value = BUILD_DATE;
	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_build_time(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *BUILD_TIME = NULL;
	char *name = NULL;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	FILE *info = NULL;

	info = fopen(INFO_FILE_PATH, "r");
	if (NULL == info) {
		LOGE("[%s] cannot file open %s file!!!", __func__, INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (!strncmp("Time", str, strlen("Time"))) {
				name = strchr(str, '=');
				name++;
				strncpy(tmpStr, name, strlen(name)-2);
				tmpStr[strlen(name)-2] = '\0';

				BUILD_TIME = strdup(tmpStr);
				if (BUILD_TIME == NULL) {
					LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __func__, SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
					fclose(info);
					return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
				}
				break;
			} else
				continue;
		}
	}
	*value = BUILD_TIME;
	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}
