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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define KEY_MAX 256

static const char *db_update_arg[] = {
	"/usr/bin/system_info_init_db",
	"-k", NULL,
	"-t", NULL,
	"-g", NULL,
	"-v", NULL,
	NULL,
};

static void show_help(void)
{
	printf("system_info_update_db [OPTIONS]\n");
	printf("  -h --help         Show this help\n");
	printf("  -r --root=ROOT    Root path for chroot operation\n");
	printf("  -k --key=KEY      System info key to update\n");
	printf("  -t --type=TYPE    System info type to update (int/bool/double/string)\n");
	printf("  -g --tag=TAGE     System info tag to update (platform/custom)\n");
	printf("  -v --value=VALUE  System info value to update\n");
}

int main(int argc, char *argv[])
{
	pid_t pid;
	int status, ret;
	int opt;
	bool failed;
	char root[KEY_MAX] = { 0, };
	char key[KEY_MAX] = { 0, };
	char type[KEY_MAX] = { 0, };
	char tag[KEY_MAX] = { 0, };
	char value[KEY_MAX] = { 0, };
	struct option long_options[] = {
		{ "root",  required_argument, 0, 0 },
		{ "key",   required_argument, 0, 0 },
		{ "type",  required_argument, 0, 0 },
		{ "tag",   required_argument, 0, 0 },
		{ "value", required_argument, 0, 0 },
		{ "help",  no_argument,       0, 0 },
		{ 0,       0,                 0, 0 },
	};

	while (1) {
		opt = getopt_long(argc, argv, "r:k:t:g:v:h",
				long_options, NULL);
		if (opt < 0)
			break;
		switch (opt) {
		case 'r':
			snprintf(root, sizeof(root), "%s", optarg);
			break;
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
	if (root[0] == '\0') {
		printf("Invalid Parameter: no root path\n");
		failed = true;
	}
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

	printf("Root Path(%s)\n", root);
	printf("Request to update: key(%s), type(%s), tag(%s), value(%s)\n",
			key, type, tag, value);

	db_update_arg[2] = key;
	db_update_arg[4] = type;
	db_update_arg[6] = tag;
	db_update_arg[8] = value;

	ret = chroot(root);
	if (ret < 0) {
		printf("Failed to change root dir (%s, errno:%d)\n", root, errno);
		return -errno;
	}

	pid = fork();
	if (pid < 0) {
		printf("Failed to update db (%d)\n", errno);
		return -errno;
	} else if (pid == 0) {
		ret = execv(db_update_arg[0], (char **)db_update_arg);
		if (ret < 0)
			exit(EXIT_FAILURE);
		return ret;
	} else {
		ret = waitpid(pid, &status, 0);
		if (ret == -1) {
			printf("%d waitpid() failed : %d\n", pid, errno);
			return -EAGAIN;
		}

		if (WIFEXITED(status)) {
			printf("%d terminated by exit(%d)\n", pid, WEXITSTATUS(status));
			return WEXITSTATUS(status);
		}

		if (WIFSIGNALED(status))
			printf("%d terminated by signal %d\n", pid, WTERMSIG(status));
		else if (WIFSTOPPED(status))
			printf("%d stopped by signal %d\n", pid, WSTOPSIG(status));
		return -EAGAIN;
	}
}
