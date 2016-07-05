/*
 * tizenid
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <dlog.h>
#include <glib.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZENID"

#define _D(fmt, args...)   SLOGD(fmt, ##args)
#define _E(fmt, args...)   SLOGE(fmt, ##args)
#define _I(fmt, args...)   SLOGI(fmt, ##args)

#define KEY_MAX 20

#define CHAR_POOL "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!"

static int get_pw_key(char *pw, unsigned int len)
{
	int i, val, poolsize;
	char *charpool = CHAR_POOL;
	unsigned int seed;

	if (!pw)
		return -EINVAL;

	poolsize = strlen(charpool);
	seed = time(NULL);

	for (i = 0 ; i < len ; i++) {
		val = rand_r(&seed) % poolsize;
		pw[i] = *(charpool + val);
	}

	return 0;
}

static int get_salt_by_model(char *salt, unsigned int len)
{
	int i;
	char *str = NULL, *start, *stop;
	char buf[256];
	char *key = "tizen.org/system/model_name";
	FILE *fp;

	if (!salt)
		return -EINVAL;

	fp = fopen(CONFIG_FILE_PATH, "r");
	if (!fp) {
		_E("Failed to open (%s)", CONFIG_FILE_PATH);
		return -ENOENT;
	}

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		str = strstr(buf, key);
		if (!str)
			continue;

		start = strchr(str, '>');
		if (!start)
			continue;
		stop = strchr(str, '<');
		if (!stop)
			continue;
		if (start >= stop)
			continue;

		for (start += 1, i = 0; (start < stop) && (i < len-1) ; start++, i++)
			salt[i] = start[i];

		salt[i] = '\0';

		break;
	}

	fclose(fp);

	return 0;
}

static int store_tizen_id(char *id)
{
	FILE *fp;
	int ret;

	if (!id)
		return -EINVAL;

	fp = fopen(TIZEN_ID_PATH, "w");
	if (!fp) {
		_E("Failed to open file (%s)", TIZEN_ID_PATH);
		return -ENOMEM;
	}

	ret = fputs(id, fp);
	fclose(fp);
	if (ret == EOF) {
		_E("Failed to store id");
		return -ENOMEM;
	}

	ret = chmod(TIZEN_ID_PATH, 0554);
	if (ret == -1) {
		_E("chmod() failed(errno:%d)", errno);
		return -errno;
	}

	return 0;
}

static int make_tizen_id(void)
{
	char salt[KEY_MAX], pw_key[KEY_MAX];
	char *id = NULL;
	gchar *id_64 = NULL;
	int ret;

	ret = get_salt_by_model(salt, sizeof(salt));
	if (ret < 0) {
		_E("Failed to get salt value (%d)", ret);
		return ret;
	}

	ret = get_pw_key(pw_key, sizeof(pw_key));
	if (ret < 0) {
		_E("Failed to get pw key value (%d)", ret);
		return ret;
	}

	id = (char *)malloc(sizeof(char) * KEY_MAX);
	if (!id) {
		_E("malloc() failed");
		return -ENOMEM;
	}

	ret = PKCS5_PBKDF2_HMAC_SHA1(pw_key, strlen(pw_key),	/* pw key */
			(unsigned char *)salt, sizeof(salt),			/*salt*/
			1,												/* nr of iteration */
			KEY_MAX, (unsigned char *)id);				/* output */
	if (ret == 0) {	/* 0: failed, 1: successful */
		_E("PKCS5_PBKDF2_HMAC_SHA1() failed");
		ret = -ENOENT;
		goto out;
	}

	id[KEY_MAX-1] = '\0';
	_I("ID: (%s)", id);

	id_64 = g_base64_encode((const guchar *)id, KEY_MAX);

	ret = store_tizen_id(id_64);
	if (ret < 0) {
		_E("Failed to store tizen id to file (%d)", ret);
		goto out;
	}

	ret = 0;

out:
	if (id)
		free(id);
	if (id_64)
		g_free(id_64);

	return ret;
}

static int check_tizen_id_content(void)
{
	FILE *fp;
	int ret;
	char id[KEY_MAX] = {0, };

	fp = fopen(TIZEN_ID_PATH, "r");
	if (!fp) {
		_E("Failed to open tizen id");
		return -ENOMEM;
	}

	if (fgets(id, sizeof(id), fp) == NULL) {
		if (errno == 0) {
			_E("Invalid Tizen ID (empty)");
			ret = -ENOENT;
		} else {
			_E("Failed to get tizen id (errno:%d)", errno);
			ret = -errno;
		}
		fclose(fp);
		return ret;
	}

	fclose(fp);

	if (strlen(id) == 0) {
		_E("Invalid Tizen ID (empty)");
		return -ENOENT;
	}

	return 0;
}

static int check_tizen_id(void)
{
	struct stat buf;
	int ret;
	mode_t mode;

	ret = lstat(TIZEN_ID_PATH, &buf);
	if (ret != 0) {
		ret = errno;
		if (ret == ENOENT)
			_I("Tizen ID does not exist");
		else
			_E("Failed to get info of Tizen ID (errno:%d)", ret);
		return -ret;
	}

	mode = buf.st_mode & S_IFMT;
	switch (mode) {
	case S_IFREG:
		if (check_tizen_id_content() < 0) {
			_I("Tizen ID is empty");
			break;
		}
		_I("Tizen ID already exists");
		return 0;
	case S_IFLNK:
		_E("Tizen ID is invalid (Symbolic link)");
		break;
	default:
		_E("Tizen ID is invalid");
		break;
	}

	if (unlink(TIZEN_ID_PATH) < 0)
		_E("Failed to remove invalid tizenid (errno:%d)", errno);

	return -ENOENT;
}

int main(int argc, char *argv[])
{
	if (check_tizen_id() == 0)
		return 0;

	return make_tizen_id();
}
