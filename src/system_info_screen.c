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
#include <math.h>

#include <vconf.h>
#include <dlog.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <system_info.h>
#include <system_info_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZEN_N_SYSTEM_INFO"

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

static int RCA_SUPPORTED;
static int HDMI_SUPPORTED;
static int SCREEN_DPI;
static int BITS_PER_PIXEL;
static int SCREEN_WIDTH;
static int SCREEN_HEIGHT;
static int PHYSICAL_SCREEN_WIDTH;
static int PHYSICAL_SCREEN_HEIGHT;
int system_info_screen_initialized;

int system_info_screen_init()
{
	int i, n;
	XPixmapFormatValues *pmf;

	memset(&g_pinfo, 0x0, sizeof(ProgInfo));

	g_pinfo.dpy = XOpenDisplay(NULL);
	if (NULL == g_pinfo.dpy)
		return -1;

	if (0 > g_pinfo.screen)
		g_pinfo.screen = DefaultScreen(g_pinfo.dpy);
	g_pinfo.root = RootWindow(g_pinfo.dpy, g_pinfo.screen);

	if (!XRRQueryExtension(g_pinfo.dpy, &g_pinfo.event_base, &g_pinfo.error_base) ||
		!XRRQueryVersion(g_pinfo.dpy, &g_pinfo.major, &g_pinfo.minor)) {
		return -1;
	}

	g_pinfo.res = XRRGetScreenResources(g_pinfo.dpy, g_pinfo.root);

	if (!g_pinfo.res)
		return -1;

	pmf = XListPixmapFormats(g_pinfo.dpy, &n);

	for (i = 0; i < n; i++) {
		if (BITS_PER_PIXEL < pmf->bits_per_pixel)
			BITS_PER_PIXEL = pmf->bits_per_pixel;
		pmf++;
	}

	for (i = 0; i < g_pinfo.res->noutput; i++) {
		XRROutputInfo *output_info = XRRGetOutputInfo(g_pinfo.dpy, g_pinfo.res, g_pinfo.res->outputs[i]);
		if (!output_info)
			return -1;

		/* find target lcd */
		if (!strcmp(output_info->name, "LVDS1")) {
			/* XRRCrtcInfo information */
			XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(g_pinfo.dpy, g_pinfo.res, output_info->crtc);
			if (!crtc_info)
				break;

			SCREEN_WIDTH = crtc_info->width;
			SCREEN_HEIGHT = crtc_info->height;
			PHYSICAL_SCREEN_WIDTH = output_info->mm_width;
			PHYSICAL_SCREEN_HEIGHT = output_info->mm_height;

			XRRFreeCrtcInfo(crtc_info);
		} else if (!strcmp(output_info->name, "HDMI1"))
			HDMI_SUPPORTED = true;
		XRRFreeOutputInfo(output_info);
	}

	if (BITS_PER_PIXEL == 0)
		return -1;

	if (!SCREEN_WIDTH)
		SCREEN_WIDTH = DisplayWidth(g_pinfo.dpy, DefaultScreen(g_pinfo.dpy));

	if (!SCREEN_WIDTH)
		return -1;

	if (!SCREEN_HEIGHT)
		SCREEN_HEIGHT = DisplayHeight(g_pinfo.dpy, DefaultScreen(g_pinfo.dpy));

	if (!SCREEN_HEIGHT)
		return -1;

	if (system_info_get_system_info_model_type() == SYSTEM_INFO_MODEL_TYPE_EMULATOR) {
		FILE *cmdline;
		char *dpi;
		char str[MAXBUFSIZE];

		cmdline = fopen(CMDLINE_PATH, "r");
		if (NULL == cmdline) {
			LOGE("[%s] cannot file open %s file!!!", __func__, CPU_INFO_FILE_PATH);
			return SYSTEM_INFO_ERROR_IO_ERROR;
		} else {
			while (fgets(str, MAXBUFSIZE, cmdline)) {
				dpi = strstr(str, "dpi=");
				SCREEN_DPI = atoi(dpi+4) / 10;
				fclose(cmdline);
				break;
			}
		}
	} else {
		int max_resolution;
		double ratio, width_inch;

		width_inch = PHYSICAL_SCREEN_WIDTH / 10 / 2.54;
		max_resolution = SCREEN_WIDTH + SCREEN_HEIGHT;
		ratio = SCREEN_WIDTH / SCREEN_HEIGHT;

		SCREEN_DPI = max_resolution / (width_inch + (ratio * width_inch)) + 0.5;
	}

	XCloseDisplay(g_pinfo.dpy);

	system_info_screen_initialized = 1;

	return 0;
}

int system_info_get_screen_bits_per_pixel(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *bpp;
	int ret_val;

	bpp = (int *)value;

	if (0 == system_info_screen_initialized) {
		ret_val = system_info_screen_init();
		if (ret_val)
			return ret_val;
	}

	*bpp = BITS_PER_PIXEL;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_screen_width(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *width;
	int ret_val;

	width = (int *)value;

	if (0 == system_info_screen_initialized) {
		ret_val = system_info_screen_init();
		if (ret_val)
			return ret_val;
	}

	*width = SCREEN_WIDTH;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_screen_height(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *height;
	int ret_val;

	height = (int *)value;

	if (0 == system_info_screen_initialized) {
		ret_val = system_info_screen_init();
		if (ret_val)
			return ret_val;
	}

	*height = SCREEN_HEIGHT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_screen_DPI(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *bpp;
	int ret_val;

	bpp = (int *)value;

	if (0 == system_info_screen_initialized) {
		ret_val = system_info_screen_init();
		if (ret_val)
			return ret_val;
	}

	*bpp = SCREEN_DPI;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_hdmi_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;
	int ret_val;

	if (0 == system_info_screen_initialized) {
		ret_val = system_info_screen_init();
		if (ret_val)
			return ret_val;
	}

	supported = (bool *)value;

	*supported = HDMI_SUPPORTED;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_rca_supported(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	bool *supported;

	supported = (bool *)value;

	*supported = RCA_SUPPORTED;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_physical_screen_height(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *bpp;
	int ret_val;

	bpp = (int *)value;

	if (0 == system_info_screen_initialized) {
		ret_val = system_info_screen_init();
		if (ret_val)
			return ret_val;
	}

	*bpp = PHYSICAL_SCREEN_HEIGHT;

	return SYSTEM_INFO_ERROR_NONE;
}

int system_info_get_physical_screen_width(system_info_key_e key, system_info_data_type_e data_type, void **value)
{
	int *bpp;
	int ret_val;

	bpp = (int *)value;

	if (0 == system_info_screen_initialized) {
		ret_val = system_info_screen_init();
		if (ret_val)
			return ret_val;
	}

	*bpp = PHYSICAL_SCREEN_WIDTH;

	return SYSTEM_INFO_ERROR_NONE;
}

