/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <gdbm.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dlog.h>

#include <system_info.h>
#include <iniparser.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "system_info_private.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "SYSTEM_INFO"
#define _D(fmt, args...)   SLOGD(fmt, ##args)
#define _E(fmt, args...)   SLOGE(fmt, ##args)
#define _I(fmt, args...)   SLOGI(fmt, ##args)


#define ARRAY_SIZE(name) (sizeof(name)/sizeof(name[0]))

#define KEY_MAX 256

#define MODEL_CONFIG_TAG "model-config"
#define KEY_PREFIX "http://"

static int db_set_value(GDBM_FILE *db, char *tag, char *name, char *type, char *value)
{
	char key[KEY_MAX];
	datum d_key;
	datum d_data;
	int ret;

	if (!db || !*db || !tag || !name || !type || !value)
		return -EINVAL;

	if (name == strstr(name, KEY_PREFIX))
		snprintf(key, sizeof(key), "%s:%s:%s", name, type, tag);
	else
		snprintf(key, sizeof(key), "%s%s:%s:%s", KEY_PREFIX, name, type, tag);

	d_key.dptr = key;
	d_key.dsize = strlen(key) + 1;

	d_data.dptr = value;
	d_data.dsize = strlen(value) + 1;

	ret = gdbm_store(*db, d_key, d_data, GDBM_REPLACE);
	if (ret != 0) {
		_E("Failed to store key (%s, %s, %d)", key, type, gdbm_errno);
		return -gdbm_errno;
	}

	gdbm_sync(*db);

	_I("DB: value (key:%s,value:%s) is stored", key, value);

	return 0;
}

static int system_info_get_values_config_xml(GDBM_FILE *db)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlNode *cur_node, *tag_node;
	char *tag, *name, *type, *value;
	int ret;

	if (!db || !*db)
		return -EINVAL;

	doc = xmlParseFile(CONFIG_FILE_PATH);
	if (!doc) {
		_E("cannot file open %s file!!!", CONFIG_FILE_PATH);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	cur = xmlDocGetRootElement(doc);
	if (!cur) {
		_E("empty document %s file!!!", CONFIG_FILE_PATH);
		ret = -ENOENT;
		goto out;
	}

	for (cur_node = cur; cur_node ; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur->name, (const xmlChar*)MODEL_CONFIG_TAG))
			break;
	}
	if (!cur_node) {
		_E("cannot find %s root element file!!!", MODEL_CONFIG_TAG);
		ret = -ENOENT;
		goto out;
	}

	cur = cur_node->xmlChildrenNode;
	for (tag_node = cur; tag_node; tag_node = tag_node->next) {
		if (!xmlStrcmp(tag_node->name, (const xmlChar *)TAG_TYPE_PLATFORM_STR))
			tag = TAG_TYPE_PLATFORM_STR;
		else if (!xmlStrcmp(tag_node->name, (const xmlChar *)TAG_TYPE_CUSTOM_STR))
			tag = TAG_TYPE_CUSTOM_STR;
		else
			continue;

		cur = tag_node->xmlChildrenNode;
		for (cur_node = cur; cur_node ; cur_node = cur_node->next) {
			if (cur_node->type != XML_ELEMENT_NODE)
				continue;

			name = (char *)xmlGetProp(cur_node, (const xmlChar*)"name");
			if (!name)
				continue;

			type = (char *)xmlGetProp(cur_node, (const xmlChar*)"type");
			if (!type) {
				xmlFree(name);
				continue;
			}

			value = (char *)xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
			if (!value) {
				xmlFree(name);
				xmlFree(type);
				continue;
			}

			ret = db_set_value(db, tag, name, type, value);
			if (ret < 0)
				_E("Failed to set value (%d)", ret);

			xmlFree(name);
			xmlFree(type);
			xmlFree(value);
		}
	}

	ret = 0;

out:
	if (doc)
		xmlFreeDoc(doc);
	return ret;
}

static struct build_ini_keys {
	char *info;
	char *key;
} ini_keys[] = {
	{ "version:model",   "http://tizen.org/system/build.model"   },
	{ "version:build",   "http://tizen.org/system/build.string"  },
	{ "version:release", "http://tizen.org/system/build.release" },
	{ "build:type",      "http://tizen.org/system/build.type"    },
	{ "build:date",      "http://tizen.org/system/build.date"    },
	{ "build:time",      "http://tizen.org/system/build.time"    },
	{ "build:variant",   "http://tizen.org/system/build.variant" },
	{ "build:id",        "http://tizen.org/system/build.id"      },
};

static int system_info_get_values_ini(GDBM_FILE *db)
{
	dictionary *ini;
	int i, ret;
	char *value;

	if (!db || !*db)
		return -EINVAL;

	ini = iniparser_load(INFO_FILE_PATH);
	if (!ini) {
		_E("cannot file open %s file!!!", INFO_FILE_PATH);
		return -ENOENT;
	}

	for (i = 0 ; i < ARRAY_SIZE(ini_keys) ; i++) {
		value = iniparser_getstring(ini, ini_keys[i].info, NULL);
		if (!value) {
			_E("NOT found %s", ini_keys[i].info);
			continue;
		}

		ret = db_set_value(db, TAG_TYPE_PLATFORM_STR, ini_keys[i].key, STR_TYPE, value);
		if (ret < 0)
			_E("Failed to set value (%d)", ret);
	}

	iniparser_freedict(ini);

	return 0;
}

static int system_info_create_db(void)
{
	int ret;
	GDBM_FILE db;

	db = gdbm_open(SYSTEM_INFO_DB_PATH, 0, GDBM_WRCREAT, S_IRUSR | S_IRGRP | S_IROTH, NULL);
	if (!db) {
		_E("Failed to open db (%d, %s)", gdbm_errno, gdbm_strerror(gdbm_errno));
		return -ENOENT;
	}

	ret = system_info_get_values_config_xml(&db);
	if (ret < 0)
		_E("Failed to get keys and values from xml(%d)", ret);

	ret = system_info_get_values_ini(&db);
	if (ret < 0)
		_E("Failed to get keys and values from ini(%d)", ret);

	gdbm_close(db);

	return 0;
}

static void show_help(void)
{
	printf("system_info_init_db [OPTIONS]\n");
	printf("\t-h --help\t\tShow this help\n");
	printf("\t-k --key=KEY\tSystem info key to update\n");
	printf("\t-t --type=TYPE\tSystem info type to update (int/bool/double/string)\n");
	printf("\t-g --tag=TAGE\tSystem info tag to update (platform/custom)\n");
	printf("\t-v --value=VALUE\tSystem info value to update\n");
}

static int system_info_update_db(int argc, char *argv[])
{
	int ret;
	GDBM_FILE db;
	int opt;
	bool failed = false;
	char key[KEY_MAX] = { 0, };
	char type[KEY_MAX] = { 0, };
	char tag[KEY_MAX] = { 0, };
	char value[KEY_MAX] = { 0, };
	struct option long_options[] = {
		{ "key",   required_argument, 0, 0 },
		{ "type",  required_argument, 0, 0 },
		{ "tag",   required_argument, 0, 0 },
		{ "value", required_argument, 0, 0 },
		{ "help",  no_argument,       0, 0 },
		{ 0,       0,                 0, 0 },
	};

	while (1) {
		opt = getopt_long(argc, argv, "k:t:g:v:h",
				long_options, NULL);
		if (opt < 0)
			break;
		switch (opt) {
		case 'k':
			snprintf(key, sizeof(key), "%s", optarg);
			break;
		case 't':
			snprintf(type, sizeof(type), "%s", optarg);
			break;
		case 'g':
			snprintf(tag, sizeof(tag), "%s", optarg);
			break;
		case 'v':
			snprintf(value, sizeof(value), "%s", optarg);
			break;
		case 'h':
		default:
			show_help();
			return 0;
		}
	}

	failed = false;
	if (key[0] == '\0') {
		printf("Invalid Parameter: no key\n");
		failed = true;
	}
	if (type[0] == '\0') {
		printf("Invalid Parameter: no type\n");
		failed = true;
	}
	if (tag[0] == '\0') {
		printf("Invalid Parameter: no tag\n");
		failed = true;
	}
	if (value[0] == '\0') {
		printf("Invalid Parameter: no value\n");
		failed = true;
	}

	if (failed)
		return -EINVAL;

	_I("Request to update: key(%s), type(%s), tag(%s), value(%s)",
			key, type, tag, value);

	db = gdbm_open(SYSTEM_INFO_DB_PATH, 0, GDBM_WRCREAT, S_IRUSR | S_IRGRP | S_IROTH, NULL);
	if (!db) {
		_E("Failed to open db (%d, %s)", gdbm_errno, gdbm_strerror(gdbm_errno));
		return -ENOENT;
	}

	ret = db_set_value(&db, tag, key, type, value);
	if (ret != 0)
		_E("Failed to set value (%d)", ret);

	gdbm_close(db);
	return ret;
}

int main(int argc, char *argv[])
{
	if (argc == 1)
		return system_info_create_db();

	return system_info_update_db(argc, argv);
}
