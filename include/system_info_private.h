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


#ifndef __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__
#define __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
	SYSTEM_INFO_DATA_TYPE_STRING,
	SYSTEM_INFO_DATA_TYPE_INT,
	SYSTEM_INFO_DATA_TYPE_DOUBLE,
	SYSTEM_INFO_DATA_TYPE_BOOL
} system_info_data_type_e;

typedef int (*system_info_get_value_cb) (system_info_key_e key, system_info_data_type_e data_type, void **value);

int system_info_get_model(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_tizen_version(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_bluetooth_supported(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_camera_count(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_fmradio_supported(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_gps_supported(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_keyboard_type(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_multi_point_touch_count(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_network_type(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_nfc_supported(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_opengles_version(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_screen_bits_per_pixel(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_screen_height(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_screen_width(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_tvout_supported(system_info_key_e key, system_info_data_type_e data_type, void **value);
int system_info_get_wifi_supported(system_info_key_e key, system_info_data_type_e data_type, void **value);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_PRIVATE_H__ */
