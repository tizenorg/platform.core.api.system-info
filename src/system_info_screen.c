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

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_SYSTEM_INFO"

typedef struct _progInfo ProgInfo;

/* globals */
ProgInfo g_pinfo;

struct _progInfo {
	Display *dpy;
	Window root;
	int screen;
	int event_base, error_base;
	int major, minor;
	XRRScreenResources *res;
};

static int PHYSICAL_SCREEN_WIDTH;
static int PHYSICAL_SCREEN_HEIGHT;
int system_info_screen_initialized;

int system_info_screen_init()
{
	int i;

	memset(&g_pinfo, 0x0, sizeof(ProgInfo));

	g_pinfo.dpy = XOpenDisplay(NULL);
	if (NULL == g_pinfo.dpy) {
		LOGE("XOpenDisplay Failed");
		return -1;
	}

	if (0 > g_pinfo.screen)
		g_pinfo.screen = DefaultScreen(g_pinfo.dpy);
	g_pinfo.root = RootWindow(g_pinfo.dpy, g_pinfo.screen);

	if (!XRRQueryExtension(g_pinfo.dpy, &g_pinfo.event_base, &g_pinfo.error_base) ||
		!XRRQueryVersion(g_pinfo.dpy, &g_pinfo.major, &g_pinfo.minor)) {
		LOGE("XRRQuery Failed");
		XCloseDisplay(g_pinfo.dpy);
		return -1;
	}

	g_pinfo.res = XRRGetScreenResources(g_pinfo.dpy, g_pinfo.root);

	if (!g_pinfo.res) {
		LOGE("XRRGetScreenResources Failed");
		XCloseDisplay(g_pinfo.dpy);
		return -1;
	}

	for (i = 0; i < g_pinfo.res->noutput; i++) {
		XRROutputInfo *output_info = XRRGetOutputInfo(g_pinfo.dpy, g_pinfo.res, g_pinfo.res->outputs[i]);
		if (!output_info) {
			LOGE("XRRGetOutputInfo Failed");
			XCloseDisplay(g_pinfo.dpy);
			return -1;
		}

		/* find target lcd */
		if (!strcmp(output_info->name, "LVDS1")) {
			/* XRRCrtcInfo information */
			XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(g_pinfo.dpy, g_pinfo.res, output_info->crtc);
			if (!crtc_info)
				break;

			PHYSICAL_SCREEN_WIDTH = output_info->mm_width;
			PHYSICAL_SCREEN_HEIGHT = output_info->mm_height;

			XRRFreeCrtcInfo(crtc_info);
		}
		XRRFreeOutputInfo(output_info);
	}

	XCloseDisplay(g_pinfo.dpy);

	system_info_screen_initialized = 1;

	return 0;
}

int system_info_get_screen_bits_per_pixel(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_int("tizen.org/feature/screen.bpp", (int *)value);
}

int system_info_get_screen_width(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_int("tizen.org/feature/screen.width", (int *)value);
}

int system_info_get_screen_height(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_int("tizen.org/feature/screen.height", (int *)value);
}

int system_info_get_screen_DPI(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_int("tizen.org/feature/screen.dpi", (int *)value);
}

int system_info_get_hdmi_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/screen.output.hdmi", (bool *)value);
}

int system_info_get_rca_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	return system_info_get_platform_bool("tizen.org/feature/screen.output.rca", (bool *)value);
}

int system_info_get_physical_screen_height(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *height;
	int ret_val;

	height = (int *)value;

	if (0 == system_info_screen_initialized) {
		ret_val = system_info_screen_init();
		if (ret_val)
			return ret_val;
	}

	*height = PHYSICAL_SCREEN_HEIGHT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_physical_screen_width(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *width;
	int ret_val;

	width = (int *)value;

	if (0 == system_info_screen_initialized) {
		ret_val = system_info_screen_init();
		if (ret_val)
			return ret_val;
	}

	*width = PHYSICAL_SCREEN_WIDTH;

	return SYSTEM_INFO_ERROR_NONE;
}
