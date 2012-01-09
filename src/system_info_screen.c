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

#include <X11/X.h>
#include <X11/Xlib.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZEN_N_SYSTEM_INFO"

typedef enum {
	SCREEN_METRICS_BPP,
	SCREEN_METRICS_WIDTH,
	SCREEN_METRICS_HEIGHT
} screen_metrics_e;

static const int SCREEN_BPP = 32;

static int system_info_get_default_screen_metrics(screen_metrics_e key, int *value)
{
	static Display *display = NULL;
	
	if (display == NULL)
	{
		display = XOpenDisplay(":0");

		if (display == NULL)
		{
			return -1;
		}
	}

	switch (key)
	{
	case SCREEN_METRICS_BPP:
	{
		*value = SCREEN_BPP;
		break;
	}

	case SCREEN_METRICS_WIDTH:
		*value = DisplayWidth(display, DefaultScreen(display));
		break;

	case SCREEN_METRICS_HEIGHT:
		*value = DisplayHeight(display, DefaultScreen(display));
		break;

	default:
		return -1;
	}

	return 0;
}

int system_info_get_screen_bits_per_pixel(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *bpp;

	bpp = (int*)value;

	if (system_info_get_default_screen_metrics(SCREEN_METRICS_BPP, bpp))
	{
		LOGE("[%s] IO_ERROR(0x%08x) : failed to get the screen metrics", __FUNCTION__, SYSTEM_INFO_ERROR_IO_ERROR);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_screen_width(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *width;

	width = (int*)value;

	if (system_info_get_default_screen_metrics(SCREEN_METRICS_WIDTH, width))
	{
		LOGE("[%s] IO_ERROR(0x%08x) : failed to get the screen metrics", __FUNCTION__, SYSTEM_INFO_ERROR_IO_ERROR);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_screen_height(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *height;

	height = (int*)value;

	if (system_info_get_default_screen_metrics(SCREEN_METRICS_HEIGHT, height))
	{
		LOGE("[%s] IO_ERROR(0x%08x) : failed to get the screen metrics", __FUNCTION__, SYSTEM_INFO_ERROR_IO_ERROR);
		return SYSTEM_INFO_ERROR_IO_ERROR;
	}

	return SYSTEM_INFO_ERROR_NONE;
}

