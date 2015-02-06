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

#include <dlog.h>

#include <system_info.h>
#include <system_info_private.h>

#include <iniparser.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

#define MODEL_CONFIG_TAG "model-config"

int system_info_ini_get_string(char *ini_file, char *key, char **output)
{
	dictionary	*ini;
	char *str;
	char *tmp;

	ini = iniparser_load(ini_file);

	if (ini == NULL) {
		LOGE("cannot file open %s file!!!", ini_file);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	str = iniparser_getstr(ini, key);

	if (str == NULL) {
		LOGE("NOT found %s(0x%08x)", key, SYSTEM_INFO_ERROR_IO_ERROR);
		iniparser_freedict(ini);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	tmp = strdup(str);

	if (tmp == NULL) {
		LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		iniparser_freedict(ini);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	*output = tmp;
	iniparser_freedict(ini);

	return SYSTEM_INFO_ERROR_NONE;
}

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

				if (!strncmp(id, id_field, strlen(id))) {
					if (!strncmp(id, id_field, strlen(id_field))) {
						if (!string) {
							free(id);
							continue;
						}

						*value = strdup(string);
						free(id);
						free(string);
						xmlFreeDoc(doc);
						if (*value == NULL) {
								LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
								xmlFreeDoc(doc);
								return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
						}
						return SYSTEM_INFO_ERROR_NONE;
					}
				}
				free(id);
				free(string);
			}
		}
	}

	if (*value == NULL && default_node) {
		cur = default_node->xmlChildrenNode;

		for (cur_node = cur; cur_node; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE) {
				id = (char *)xmlGetProp(cur_node, (const xmlChar*)"id");
				string = (char *) xmlGetProp(cur_node, (const xmlChar*)"string");

				if (!strncmp(id, id_field, strlen(id))) {
					if (!strncmp(id, id_field, strlen(id_field))) {
						if (!string) {
							free(id);
							continue;
						}

						*value = strdup(string);
						free(id);
						free(string);
						xmlFreeDoc(doc);
						if (*value == NULL) {
								LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
								xmlFreeDoc(doc);
								return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
						}
						return SYSTEM_INFO_ERROR_NONE;
					}
				}
				free(id);
				free(string);
			}
		}
	}

	LOGE("cannot find %s field from %s file!!!", id_field, xml_file_path);
	xmlFreeDoc(doc);
	return SYSTEM_INFO_ERROR_IO_ERROR;
}

int system_info_get_value_from_config_xml(char *feature_tag, const char *name_field, char *type_field, char **value)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr cur = NULL;
	xmlNodePtr model_node = NULL;
	xmlNode *cur_node = NULL;
	char *name = NULL, *p_name = NULL;
	char *type = NULL;
	char *string = NULL;

	doc = xmlParseFile(CONFIG_FILE_PATH);

	if (doc == NULL) {
		LOGE("cannot file open %s file!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		LOGE("empty document %s file!!!", CONFIG_FILE_PATH);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	for (cur_node = cur; cur_node; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur->name, (const xmlChar*)MODEL_CONFIG_TAG))
			break;
	}

	if (cur == NULL) {
		LOGE("cannot find %s root element file!!!", MODEL_CONFIG_TAG);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	cur = cur->xmlChildrenNode;

	for (cur_node = cur; cur_node; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur_node->name, (const xmlChar*)feature_tag))
			model_node = cur_node;
	}

	if (model_node == NULL) {
		LOGE("cannot find %s field from %s file!!!", name_field, CONFIG_FILE_PATH);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	if (model_node) {
		cur = model_node->xmlChildrenNode;

		for (cur_node = cur; cur_node; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE) {
				name = (char *)xmlGetProp(cur_node, (const xmlChar*)"name");
				type = (char *)xmlGetProp(cur_node, (const xmlChar*)"type");

				p_name = strstr(name_field, "http://");
				if (p_name && p_name == name_field)
					p_name = (char *)name_field + strlen("http://");
				else
					p_name = (char *)name_field;

				if (!strncmp(name, p_name, strlen(name))) {
					if (!strncmp(name, p_name, strlen(p_name))) {
						if (strncmp(type, type_field, strlen(type_field))) {
							LOGE("INVALID_PARAMETER(0x%08x) : invalid output param", SYSTEM_INFO_ERROR_INVALID_PARAMETER);
							free(name);
							free(type);
							xmlFreeDoc(doc);
							return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
						}
						string = (char *)xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
						if (string) {
							*value = strdup(string);
							free(name);
							free(type);
							free(string);
							break;
						}
					}
				}
				free(name);
				free(type);
			}
		}
	}

	if (!cur_node) {
		LOGE("cannot find %s field from %s file!!!", name_field, CONFIG_FILE_PATH);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	if (*value == NULL) {
		LOGE("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
	}

	xmlFreeDoc(doc);
	return SYSTEM_INFO_ERROR_NONE;
}
