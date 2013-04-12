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
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dlog.h>
#include <vconf.h>

#include <system_info.h>
#include <system_info_private.h>

#include <GLES/gl.h>
#include <GLES/glext.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

#define LIB_GLES_V1 "/usr/lib/libGLESv1_CM.so"
#define LIB_GLES_V2 "/usr/lib/libGLESv2.so"

#define SIZE_OF_MODEL_NAME 8

#define MESSAGE_INFO_FILE_PATH "/etc/config/sysinfo-message.xml"
#define GRAPHICS_INFO_FILE_PATH "/etc/config/graphics/sysinfo-graphics.xml"
#define SCREEN_INFO_FILE_PATH "/etc/config/screen/sysinfo-screen.xml"

int system_info_get_value_from_xml(char *xml_file_path, char *model, char *id_field, char **value)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr cur = NULL;
	xmlNodePtr default_node = NULL;
	xmlNodePtr model_node = NULL;
	xmlNode *cur_node = NULL;
	char *id = NULL;
	char *string = NULL;

	doc = xmlParseFile(xml_file_path);

	if (doc == NULL) {
		LOGE("cannot file open %s file!!!", xml_file_path);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		LOGE("empty document %s file!!!", xml_file_path);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	for (cur_node = cur; cur_node; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur->name, (const xmlChar*)"sys-info"))
			break;
	}

	if (cur == NULL) {
		LOGE("cannot find %s root element file!!!", "sys-info");
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	cur = cur->xmlChildrenNode;

	for (cur_node = cur; cur_node; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur_node->name, (const xmlChar*)"default"))
			default_node = cur_node;
		if (strcmp(model, "default") && !xmlStrcmp(cur_node->name, (const xmlChar*)model))
			model_node = cur_node;
	}

	if (model_node) {
		cur = model_node->xmlChildrenNode;

		for (cur_node = cur; cur_node; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE) {
				id = (char *)xmlGetProp(cur_node, (const xmlChar*)"id");
				string = (char *) xmlGetProp(cur_node, (const xmlChar*)"string");

				if (!strncmp(id, id_field, strlen(id_field))) {
					if (string) {
						*value = strdup(string);
						break;
					}
				}
			}
		}
	}

	if (*value == NULL && default_node) {
		cur = default_node->xmlChildrenNode;

		for (cur_node = cur; cur_node; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE) {
				id = (char *)xmlGetProp(cur_node, (const xmlChar*)"id");
				string = (char *) xmlGetProp(cur_node, (const xmlChar*)"string");

				if (!strncmp(id, id_field, strlen(id_field))) {
					if (string) {
						*value = strdup(string);
						break;
					}
				}
			}
		}
	}

	if (!cur_node) {
		LOGE("cannot find %s field from %s file!!!", id_field, xml_file_path);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (*value == NULL) {
		LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	xmlFreeDoc(doc);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_model(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *MODEL = NULL;
	char *name = NULL;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	int tmpStrlen = 0;
	FILE *info;
	extern char *strcasestr(const char *s, const char *find);

	info = fopen(INFO_FILE_PATH, "r");
	if (NULL == info) {
		LOGE("cannot file open %s file!!!", INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (strcasestr(str, "Model")) {
				name = strchr(str, '=');
				name++;
				tmpStrlen = strlen(name);

				strncpy(tmpStr, name, tmpStrlen-2);
				tmpStr[tmpStrlen-2] = '\0';

				MODEL = strdup(tmpStr);
				if (MODEL == NULL) {
					LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
					fclose(info);
					return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
				}
				break;
			} else
				continue;
		}
	}
	*value = MODEL;
	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_tizen_version(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *TIZEN_VERSION = NULL;
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
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (strcasestr(str, "VERSION_ID")) {
				name = strchr(str, '=');
				name += 2;
				tmpStrlen = strlen(name);

				strncpy(tmpStr, name, tmpStrlen-2);
				tmpStr[tmpStrlen-2] = '\0';

				TIZEN_VERSION = strdup(tmpStr);
				if (TIZEN_VERSION == NULL) {
					LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
					fclose(info);
					return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
				}
				break;
			} else
				continue;
		}
	}
	*value = TIZEN_VERSION;
	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_platform_name(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *PLATFORM_NAME = NULL;
	char *name = NULL;
	char str[MAXBUFSIZE];
	char tmpStr[MAXBUFSIZE];
	int tmpStrlen = 0;
	int i = 0;
	FILE *info;
	extern char *strcasestr(const char *s, const char *find);

	info = fopen(OS_RELEASE_FILE_PATH, "r");
	if (NULL == info) {
		LOGE("cannot file open %s file!!!", OS_RELEASE_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, info)) {
			if (strcasestr(str, "NAME")) {
				name = strchr(str, '=');
				name += 2;
				tmpStrlen = strlen(name);

				strncpy(tmpStr, name, tmpStrlen-2);
				tmpStr[tmpStrlen-2] = '\0';

				tmpStrlen = strlen(tmpStr);

				for (i = 0; i < tmpStrlen; i++)
						tmpStr[i] = (char)toupper((int)tmpStr[i]);

				PLATFORM_NAME = strdup(tmpStr);
				if (PLATFORM_NAME == NULL) {
					LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
					fclose(info);
					return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
				}
				break;
			} else
				continue;
		}
	}
	*value = PLATFORM_NAME;
	fclose(info);
	return SYSTEM_INFO_ERROR_NONE;
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
	} else {
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

int system_info_get_core_cpu_arch(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	FILE *cpuinfo = NULL;
	char *CORE_CPU_ARCH = NULL;
	char *name = NULL;
	char str[MAXBUFSIZE] = "";
	char tmpStr[MAXBUFSIZE] = "";

	cpuinfo = fopen(CPU_INFO_FILE_PATH, "r");
	if (NULL == cpuinfo) {
		LOGE("cannot file open %s file!!!", CPU_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		while (fgets(str, MAXBUFSIZE, cpuinfo)) {
			if (!strncmp("Processor", str, strlen("Processor"))) {
				name = strchr(str, ':');
				if (!(strncmp("ARMv7", name+2, strlen("ARMv7")))) {
					strncpy(tmpStr, name+2, strlen("ARMv7"));
					tmpStr[strlen("ARMv7")] = '\0';
				} else if (!(strncmp("ARMv6", name+2, strlen("ARMv6")))) {
					strncpy(tmpStr, name+2, strlen("ARMv6"));
					tmpStr[strlen("ARMv6")] = '\0';
				} else if (!(strncmp("x86", name+2, strlen("x86")))) {
					strncpy(tmpStr, name+2, strlen("x86"));
					tmpStr[strlen("x86")] = '\0';
				}
			} else
				continue;
		}

		/* x86 emulator doesn't have "Processor" field in /proc/cpuinfo */
		if (strlen(tmpStr) == 0 && system_info_get_system_info_model_type() == SYSTEM_INFO_MODEL_TYPE_EMULATOR) {
			strncpy(tmpStr, "x86", strlen("x86"));
			tmpStr[strlen("x86")] = '\0';
		} else if (strlen(tmpStr) == 0) {
			strncpy(tmpStr, "Unkown", strlen("Unkown"));
			tmpStr[strlen("Unkown")] = '\0';
		}

		CORE_CPU_ARCH = strdup(tmpStr);

		if (CORE_CPU_ARCH == NULL) {
			LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
			fclose(cpuinfo);
			return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
		}
	}
	fclose(cpuinfo);

	*value = CORE_CPU_ARCH;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_core_fpu_arch(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	FILE *cpuinfo = NULL;
	char *CORE_FPU_ARCH = NULL;
	char *name = NULL;
	char str[MAXBUFSIZE] = "";
	char tmpStr[MAXBUFSIZE] = "";

	cpuinfo = fopen(CPU_INFO_FILE_PATH, "r");
	if (NULL == cpuinfo) {
		LOGE("cannot file open %s file!!!", CPU_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	} else {
		/* The target and the emulator uses the field "Features" and "flags" in /proc/cpuinfo */
		if (system_info_get_system_info_model_type() != SYSTEM_INFO_MODEL_TYPE_EMULATOR)
			snprintf(tmpStr, strlen("Features"), "Features");
		else
			snprintf(tmpStr, strlen("flags"), "flags");

		while (fgets(str, MAXBUFSIZE, cpuinfo)) {
			if (!strncmp(tmpStr, str, strlen(tmpStr))) {
				name = strchr(str, ':');
				if (strstr(name+2, "vfpv3"))
					CORE_FPU_ARCH = strdup("vfpv3");
				else if (strstr(name+2, "vfpv2"))
					CORE_FPU_ARCH = strdup("vfpv2");
				else if (strstr(name+2, "ssse3"))
					CORE_FPU_ARCH = strdup("ssse3");
				else if (strstr(name+2, "sse3"))
					CORE_FPU_ARCH = strdup("sse3");
				else if (strstr(name+2, "sse2"))
					CORE_FPU_ARCH = strdup("sse2");
				else if (strstr(name+2, "sse"))
					CORE_FPU_ARCH = strdup("sse");
				else
					CORE_FPU_ARCH = strdup("Unkown");

				if (CORE_FPU_ARCH == NULL) {
					LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
					fclose(cpuinfo);
					return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
				}

				break;
			} else
				continue;
		}
	}
	fclose(cpuinfo);

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


int system_info_get_opengles_texture_format(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	char *OPENGLES_TEXTURE_FORMAT;
	char textureformat[MAXBUFSIZE];

	textureformat[0] = '\0';

	if (!access(LIB_GLES_V2, R_OK)) {
		strcat(textureformat, "utc ");
	}
	if (GL_OES_compressed_paletted_texture) {
		strcat(textureformat, "| ptc ");
	}
	if (GL_OES_compressed_ETC1_RGB8_texture) {
		strcat(textureformat, "| etc ");
	}
	if (GL_AMD_compressed_3DC_texture) {
		strcat(textureformat, "| 3dc ");
	}
	if (GL_AMD_compressed_ATC_texture) {
		strcat(textureformat, "| atc ");
	}
	if (GL_IMG_texture_compression_pvrtc) {
		strcat(textureformat, "| pvrtc");
	}

	OPENGLES_TEXTURE_FORMAT = strdup(textureformat);

	if (OPENGLES_TEXTURE_FORMAT == NULL) {
		LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
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
		LOGE("cannot file open %s file!!!", INFO_FILE_PATH);
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
					LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
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
		LOGE("cannot file open %s file!!!", INFO_FILE_PATH);
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
					LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
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
		LOGE("cannot file open %s file!!!", INFO_FILE_PATH);
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
					LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
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

int system_info_get_mms_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (access(MESSAGE_INFO_FILE_PATH, R_OK)) {
	*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(MESSAGE_INFO_FILE_PATH, model, "mms-support", &string)) {
		LOGE("cannot get mms-support info from %s!!!", MESSAGE_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
	*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_sms_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (access(MESSAGE_INFO_FILE_PATH, R_OK)) {
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(MESSAGE_INFO_FILE_PATH, model, "sms-support", &string)) {
		LOGE("cannot get sms-support info from %s!!!", MESSAGE_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_cbs_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (access(MESSAGE_INFO_FILE_PATH, R_OK)) {
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(MESSAGE_INFO_FILE_PATH, model, "cbs-support", &string)) {
		LOGE("cannot get cbs-support info from %s!!!", MESSAGE_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_graphics_hwaccel_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (access(GRAPHICS_INFO_FILE_PATH, R_OK)) {
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(GRAPHICS_INFO_FILE_PATH, model, "acceleration-support", &string)) {
		LOGE("cannot get acceleration-support info from %s!!!", GRAPHICS_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_feature_auto_rotation_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)

{
	bool *supported;
	char *string = NULL;
	char *model = "default";

	supported = (bool *)value;

	if (access(SCREEN_INFO_FILE_PATH, R_OK)) {
		*supported = false;
		return SYSTEM_INFO_ERROR_NONE;
	}

	if (system_info_get_value_from_xml(SCREEN_INFO_FILE_PATH, model, "auto-rotation-support", &string)) {
		LOGE("cannot get auto-rotation-support info from %s!!!", SCREEN_INFO_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	if (!strcmp(string, "true") || !strcmp(string, "TRUE"))
		*supported = true;
	else
		*supported = false;

	free(string);

	return SYSTEM_INFO_ERROR_NONE;
}
