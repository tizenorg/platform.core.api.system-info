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

#include <system_info.h>
#include <system_info_private.h>

#include <iniparser.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define MODEL_CONFIG_TAG "model-config"

int system_info_ini_get_string(char *ini_file, char *key, char **output)
{
	dictionary	*ini;
	char *str;
	char *tmp;

	ini = iniparser_load(ini_file);

	if (ini == NULL) {
		_E("cannot file open %s file!!!", ini_file); //LCOV_EXCL_LINE
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	str = iniparser_getstring(ini, key, NULL);

	if (str == NULL) {
//LCOV_EXCL_START System Error
		_E("NOT found %s(0x%08x)", key, SYSTEM_INFO_ERROR_IO_ERROR);
		iniparser_freedict(ini);
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	tmp = strdup(str);

	if (tmp == NULL) {
//LCOV_EXCL_START System Error
		_E("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		iniparser_freedict(ini);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
//LCOV_EXCL_STOP
	}

	*output = tmp;
	iniparser_freedict(ini);

	return SYSTEM_INFO_ERROR_NONE;
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
//LCOV_EXCL_START System Error
		_E("cannot file open %s file!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
//LCOV_EXCL_START System Error
		_E("empty document %s file!!!", CONFIG_FILE_PATH);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	for (cur_node = cur; cur_node; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur->name, (const xmlChar*)MODEL_CONFIG_TAG))
			break;
	}

	if (cur == NULL) {
//LCOV_EXCL_START System Error
		_E("cannot find %s root element file!!!", MODEL_CONFIG_TAG);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_IO_ERROR;
//LCOV_EXCL_STOP
	}

	cur = cur->xmlChildrenNode;

	for (cur_node = cur; cur_node; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur_node->name, (const xmlChar*)feature_tag))
			model_node = cur_node;
	}

	if (model_node == NULL) {
//LCOV_EXCL_START System Error
		_E("cannot find %s field from %s file!!!", name_field, CONFIG_FILE_PATH);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
//LCOV_EXCL_STOP
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
//LCOV_EXCL_START System Error 
							_E("INVALID_PARAMETER(0x%08x) : invalid output param", SYSTEM_INFO_ERROR_INVALID_PARAMETER);
							free(name);
							free(type);
							xmlFreeDoc(doc);
							return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
//LCOV_EXCL_STOP
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
		_E("cannot find %s field from %s file!!!", name_field, CONFIG_FILE_PATH);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	}

	if (*value == NULL) {
//LCOV_EXCL_START System Error
		_E("OUT_OF_MEMORY(0x%08x)", SYSTEM_INFO_ERROR_OUT_OF_MEMORY);
		xmlFreeDoc(doc);
		return SYSTEM_INFO_ERROR_OUT_OF_MEMORY;
//LCOV_EXCL_STOP
	}

	xmlFreeDoc(doc);
	return SYSTEM_INFO_ERROR_NONE;
}

//LCOV_EXCL_START Not used function TODO Will make iUTC
int system_info_get_type_from_config_xml(const char *feature_tag,
		const char *name_field, char *type_field, size_t len)
{
	xmlDocPtr doc;
	xmlNodePtr cur, model_node;
	xmlNode *cur_node;
	char *name, *p_name, *type;
	int ret;

	doc = xmlParseFile(CONFIG_FILE_PATH);
	if (!doc) {
		_E("cannot file open %s file!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	cur = xmlDocGetRootElement(doc);
	if (!cur) {
		_E("empty document %s file!!!", CONFIG_FILE_PATH);
		ret = SYSTEM_INFO_ERROR_IO_ERROR;
		goto out;
	}

	for (cur_node = cur; cur_node ; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur->name, (const xmlChar*)MODEL_CONFIG_TAG))
			break;
	}
	if (!cur_node) {
		_E("cannot find %s root element file!!!", MODEL_CONFIG_TAG);
		ret = SYSTEM_INFO_ERROR_IO_ERROR;
		goto out;
	}

	cur = cur->xmlChildrenNode;

	model_node = NULL;
	for (cur_node = cur; cur_node; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur_node->name, (const xmlChar*)feature_tag))
			model_node = cur_node;
	}
	if (!model_node) {
		_E("cannot find %s field from %s file!!!", name_field, CONFIG_FILE_PATH);
		ret = SYSTEM_INFO_ERROR_INVALID_PARAMETER;
		goto out;
	}

	ret = SYSTEM_INFO_ERROR_INVALID_PARAMETER;
	cur = model_node->xmlChildrenNode;
	for (cur_node = cur; cur_node ; cur_node = cur_node->next) {
		if (cur_node->type != XML_ELEMENT_NODE)
			continue;

		name = (char *)xmlGetProp(cur_node, (const xmlChar*)"name");

		p_name = strstr(name_field, "http://");
		if (p_name && p_name == name_field)
			p_name = (char *)name_field + strlen("http://");
		else
			p_name = (char *)name_field;

		ret = strncmp(name, p_name, strlen(name) + 1);
		xmlFree(name);
		if (ret != 0)
			continue;

		type = (char *)xmlGetProp(cur_node, (const xmlChar*)"type");
		if (!type) {
			_E("Failed to get type of key (%s)", name_field);
			ret = SYSTEM_INFO_ERROR_IO_ERROR;
			goto out;
		}

		snprintf(type_field, len, "%s", type);
		xmlFree(type);
		ret = SYSTEM_INFO_ERROR_NONE;
		break;
	}

out:
	if (doc)
		xmlFreeDoc(doc);
	return ret;
}
//LCOV_EXCL_STOP
