/*
* Copyright (c) 2011 - 2015 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __SYSTEM_INFO_TIZEN_PROP_H__
#define __SYSTEM_INFO_TIZEN_PROP_H__
#define	TIZEN_PROP_TRUE	1
#define	TIZEN_PROP_FALSE	0

struct tizen_prop
{
	char tizen_org_feature_camera;
	char tizen_org_feature_camera_back;
	char tizen_org_feature_camera_back_flash;
	char tizen_org_feature_camera_front;
	char tizen_org_feature_camera_front_flash;
	char tizen_org_feature_fmradio;
	char tizen_org_feature_graphics_acceleration;
	char tizen_org_feature_input_keyboard;
	char* tizen_org_feature_input_keyboard_layout;
	char tizen_org_feature_input_rotating_bezel;
	char tizen_org_feature_led;
	char tizen_org_feature_location;
	char tizen_org_feature_location_geofence;
	char tizen_org_feature_location_gps;
	char tizen_org_feature_location_gps_satellite;
	char tizen_org_feature_location_wps;
	char tizen_org_feature_microphone;
	char tizen_org_feature_multimedia_transcoder;
	char tizen_org_feature_multi_point_touch_pinch_zoom;
	int tizen_org_feature_multi_point_touch_point_count;
	char tizen_org_feature_network_bluetooth;
	char tizen_org_feature_network_bluetooth_audio_call;
	char tizen_org_feature_network_bluetooth_audio_media;
	char tizen_org_feature_network_bluetooth_health;
	char tizen_org_feature_network_bluetooth_hid;
	char tizen_org_feature_network_bluetooth_le;
	char tizen_org_feature_network_bluetooth_opp;
	char tizen_org_feature_network_ethernet;
	char tizen_org_feature_network_internet;
	char tizen_org_feature_network_nfc;
	char tizen_org_feature_network_nfc_card_emulation;
	char tizen_org_feature_network_nfc_card_emulation_hce;
	char tizen_org_feature_network_nfc_p2p;
	char tizen_org_feature_network_nfc_reserved_push;
	char tizen_org_feature_network_nfc_tag;
	char tizen_org_feature_network_push;
	char tizen_org_feature_network_secure_element;
	char tizen_org_feature_network_secure_element_ese;
	char tizen_org_feature_network_secure_element_uicc;
	char tizen_org_feature_network_telephony;
	char tizen_org_feature_network_telephony_mms;
	char tizen_org_feature_network_telephony_sms;
	char tizen_org_feature_network_tethering;
	char tizen_org_feature_network_tethering_bluetooth;
	char tizen_org_feature_network_tethering_usb;
	char tizen_org_feature_network_tethering_wifi;
	char tizen_org_feature_network_wifi;
	char tizen_org_feature_network_wifi_direct;
	char tizen_org_feature_network_wifi_direct_display;
	char tizen_org_feature_network_wifi_direct_service_discovery;
	char tizen_org_feature_opengles;
	char tizen_org_feature_opengles_texture_format_3dc;
	char tizen_org_feature_opengles_texture_format_atc;
	char tizen_org_feature_opengles_texture_format_etc;
	char tizen_org_feature_opengles_texture_format_ptc;
	char tizen_org_feature_opengles_texture_format_pvrtc;
	char tizen_org_feature_opengles_texture_format_utc;
	char tizen_org_feature_opengles_version_1_1;
	char tizen_org_feature_opengles_version_2_0;
	char tizen_org_feature_opengles_version_3_0;
	char tizen_org_feature_platform_core_cpu_arch_armv6;
	char tizen_org_feature_platform_core_cpu_arch_armv7;
	char tizen_org_feature_platform_core_cpu_arch_x86;
	char tizen_org_feature_platform_core_fpu_arch_sse2;
	char tizen_org_feature_platform_core_fpu_arch_sse3;
	char tizen_org_feature_platform_core_fpu_arch_ssse3;
	char tizen_org_feature_platform_core_fpu_arch_vfpv2;
	char tizen_org_feature_platform_core_fpu_arch_vfpv3;
	char* tizen_org_feature_platform_native_api_version;
	char* tizen_org_feature_platform_version;
	char* tizen_org_feature_platform_web_api_version;
	char* tizen_org_feature_profile;
	char tizen_org_feature_screen_auto_rotation;
	int tizen_org_feature_screen_bpp;
	int tizen_org_feature_screen_dpi;
	int tizen_org_feature_screen_height;
	char tizen_org_feature_screen_coordinate_system_size_large;
	char tizen_org_feature_screen_coordinate_system_size_normal;
	char tizen_org_feature_screen_output_hdmi;
	char tizen_org_feature_screen_output_rca;
	char tizen_org_feature_screen_shape_circle;
	char tizen_org_feature_screen_shape_rectangle;
	char tizen_org_feature_screen_size_large;
	char tizen_org_feature_screen_size_normal;
	char tizen_org_feature_screen_size_all;
	char tizen_org_feature_screen_size_normal_240_400;
	char tizen_org_feature_screen_size_normal_320_320;
	char tizen_org_feature_screen_size_normal_320_480;
	char tizen_org_feature_screen_size_normal_360_480;
	char tizen_org_feature_screen_size_normal_480_800;
	char tizen_org_feature_screen_size_normal_540_960;
	char tizen_org_feature_screen_size_normal_600_1024;
	char tizen_org_feature_screen_size_normal_720_1280;
	char tizen_org_feature_screen_size_normal_1080_1920;
	int tizen_org_feature_screen_width;
	char tizen_org_feature_sensor_accelerometer;
	char tizen_org_feature_sensor_accelerometer_wakeup;
	char tizen_org_feature_sensor_activity_recognition;
	char tizen_org_feature_sensor_barometer;
	char tizen_org_feature_sensor_barometer_wakeup;
	char tizen_org_feature_sensor_geomagnetic_rotation_vector;
	char tizen_org_feature_sensor_gesture_recognition;
	char tizen_org_feature_sensor_gravity;
	char tizen_org_feature_sensor_gyroscope;
	char tizen_org_feature_sensor_gyroscope_rotation_vector;
	char tizen_org_feature_sensor_gyroscope_uncalibrated;
	char tizen_org_feature_sensor_gyroscope_wakeup;
	char tizen_org_feature_sensor_heart_rate_monitor;
	char tizen_org_feature_sensor_heart_rate_monitor_led_green;
	char tizen_org_feature_sensor_heart_rate_monitor_led_ir;
	char tizen_org_feature_sensor_heart_rate_monitor_led_red;
	char tizen_org_feature_sensor_humidity;
	char tizen_org_feature_sensor_linear_acceleration;
	char tizen_org_feature_sensor_magnetometer;
	char tizen_org_feature_sensor_magnetometer_uncalibrated;
	char tizen_org_feature_sensor_magnetometer_wakeup;
	char tizen_org_feature_sensor_pedometer;
	char tizen_org_feature_sensor_photometer;
	char tizen_org_feature_sensor_photometer_wakeup;
	char tizen_org_feature_sensor_proximity;
	char tizen_org_feature_sensor_proximity_wakeup;
	char tizen_org_feature_sensor_rotation_vector;
	char tizen_org_feature_sensor_temperature;
	char tizen_org_feature_sensor_tiltmeter;
	char tizen_org_feature_sensor_tiltmeter_wakeup;
	char tizen_org_feature_sensor_ultraviolet;
	char tizen_org_feature_sensor_wrist_up;
	char tizen_org_feature_shell_appwidget;
	char tizen_org_feature_sip_voip;
	char tizen_org_feature_speech_control;
	char tizen_org_feature_speech_recognition;
	char tizen_org_feature_speech_synthesis;
	char tizen_org_feature_usb_accessory;
	char tizen_org_feature_usb_host;
	char tizen_org_feature_vision_barcode_detection;
	char tizen_org_feature_vision_barcode_generation;
	char tizen_org_feature_vision_face_recognition;
	char tizen_org_feature_vision_image_recognition;
	char tizen_org_feature_vision_qrcode_generation;
	char tizen_org_feature_vision_qrcode_recognition;
	char* tizen_org_system_build_date;
	char* tizen_org_system_build_string;
	char* tizen_org_system_build_time;
	char* tizen_org_system_manufacturer;
	char* tizen_org_system_model_name;
	char* tizen_org_system_platform_communication_processor;
	char* tizen_org_system_platform_name;
	char* tizen_org_system_platform_processor;
};

struct tizen_prop_type
{
	char* tizen_org_feature_camera;
	char* tizen_org_feature_camera_back;
	char* tizen_org_feature_camera_back_flash;
	char* tizen_org_feature_camera_front;
	char* tizen_org_feature_camera_front_flash;
	char* tizen_org_feature_fmradio;
	char* tizen_org_feature_graphics_acceleration;
	char* tizen_org_feature_input_keyboard;
	char* tizen_org_feature_input_keyboard_layout;
	char* tizen_org_feature_input_rotating_bezel;
	char* tizen_org_feature_led;
	char* tizen_org_feature_location;
	char* tizen_org_feature_location_geofence;
	char* tizen_org_feature_location_gps;
	char* tizen_org_feature_location_gps_satellite;
	char* tizen_org_feature_location_wps;
	char* tizen_org_feature_microphone;
	char* tizen_org_feature_multimedia_transcoder;
	char* tizen_org_feature_multi_point_touch_pinch_zoom;
	char* tizen_org_feature_multi_point_touch_point_count;
	char* tizen_org_feature_network_bluetooth;
	char* tizen_org_feature_network_bluetooth_audio_call;
	char* tizen_org_feature_network_bluetooth_audio_media;
	char* tizen_org_feature_network_bluetooth_health;
	char* tizen_org_feature_network_bluetooth_hid;
	char* tizen_org_feature_network_bluetooth_le;
	char* tizen_org_feature_network_bluetooth_opp;
	char* tizen_org_feature_network_ethernet;
	char* tizen_org_feature_network_internet;
	char* tizen_org_feature_network_nfc;
	char* tizen_org_feature_network_nfc_card_emulation;
	char* tizen_org_feature_network_nfc_card_emulation_hce;
	char* tizen_org_feature_network_nfc_p2p;
	char* tizen_org_feature_network_nfc_reserved_push;
	char* tizen_org_feature_network_nfc_tag;
	char* tizen_org_feature_network_push;
	char* tizen_org_feature_network_secure_element;
	char* tizen_org_feature_network_secure_element_ese;
	char* tizen_org_feature_network_secure_element_uicc;
	char* tizen_org_feature_network_telephony;
	char* tizen_org_feature_network_telephony_mms;
	char* tizen_org_feature_network_telephony_sms;
	char* tizen_org_feature_network_tethering;
	char* tizen_org_feature_network_tethering_bluetooth;
	char* tizen_org_feature_network_tethering_usb;
	char* tizen_org_feature_network_tethering_wifi;
	char* tizen_org_feature_network_wifi;
	char* tizen_org_feature_network_wifi_direct;
	char* tizen_org_feature_network_wifi_direct_display;
	char* tizen_org_feature_network_wifi_direct_service_discovery;
	char* tizen_org_feature_opengles;
	char* tizen_org_feature_opengles_texture_format_3dc;
	char* tizen_org_feature_opengles_texture_format_atc;
	char* tizen_org_feature_opengles_texture_format_etc;
	char* tizen_org_feature_opengles_texture_format_ptc;
	char* tizen_org_feature_opengles_texture_format_pvrtc;
	char* tizen_org_feature_opengles_texture_format_utc;
	char* tizen_org_feature_opengles_version_1_1;
	char* tizen_org_feature_opengles_version_2_0;
	char* tizen_org_feature_opengles_version_3_0;
	char* tizen_org_feature_platform_core_cpu_arch_armv6;
	char* tizen_org_feature_platform_core_cpu_arch_armv7;
	char* tizen_org_feature_platform_core_cpu_arch_x86;
	char* tizen_org_feature_platform_core_fpu_arch_sse2;
	char* tizen_org_feature_platform_core_fpu_arch_sse3;
	char* tizen_org_feature_platform_core_fpu_arch_ssse3;
	char* tizen_org_feature_platform_core_fpu_arch_vfpv2;
	char* tizen_org_feature_platform_core_fpu_arch_vfpv3;
	char* tizen_org_feature_platform_native_api_version;
	char* tizen_org_feature_platform_version;
	char* tizen_org_feature_platform_web_api_version;
	char* tizen_org_feature_profile;
	char* tizen_org_feature_screen_auto_rotation;
	char* tizen_org_feature_screen_bpp;
	char* tizen_org_feature_screen_dpi;
	char* tizen_org_feature_screen_height;
	char* tizen_org_feature_screen_coordinate_system_size_large;
	char* tizen_org_feature_screen_coordinate_system_size_normal;
	char* tizen_org_feature_screen_output_hdmi;
	char* tizen_org_feature_screen_output_rca;
	char* tizen_org_feature_screen_shape_circle;
	char* tizen_org_feature_screen_shape_rectangle;
	char* tizen_org_feature_screen_size_large;
	char* tizen_org_feature_screen_size_normal;
	char* tizen_org_feature_screen_size_all;
	char* tizen_org_feature_screen_size_normal_240_400;
	char* tizen_org_feature_screen_size_normal_320_320;
	char* tizen_org_feature_screen_size_normal_320_480;
	char* tizen_org_feature_screen_size_normal_360_480;
	char* tizen_org_feature_screen_size_normal_480_800;
	char* tizen_org_feature_screen_size_normal_540_960;
	char* tizen_org_feature_screen_size_normal_600_1024;
	char* tizen_org_feature_screen_size_normal_720_1280;
	char* tizen_org_feature_screen_size_normal_1080_1920;
	char* tizen_org_feature_screen_width;
	char* tizen_org_feature_sensor_accelerometer;
	char* tizen_org_feature_sensor_accelerometer_wakeup;
	char* tizen_org_feature_sensor_activity_recognition;
	char* tizen_org_feature_sensor_barometer;
	char* tizen_org_feature_sensor_barometer_wakeup;
	char* tizen_org_feature_sensor_geomagnetic_rotation_vector;
	char* tizen_org_feature_sensor_gesture_recognition;
	char* tizen_org_feature_sensor_gravity;
	char* tizen_org_feature_sensor_gyroscope;
	char* tizen_org_feature_sensor_gyroscope_rotation_vector;
	char* tizen_org_feature_sensor_gyroscope_uncalibrated;
	char* tizen_org_feature_sensor_gyroscope_wakeup;
	char* tizen_org_feature_sensor_heart_rate_monitor;
	char* tizen_org_feature_sensor_heart_rate_monitor_led_green;
	char* tizen_org_feature_sensor_heart_rate_monitor_led_ir;
	char* tizen_org_feature_sensor_heart_rate_monitor_led_red;
	char* tizen_org_feature_sensor_humidity;
	char* tizen_org_feature_sensor_linear_acceleration;
	char* tizen_org_feature_sensor_magnetometer;
	char* tizen_org_feature_sensor_magnetometer_uncalibrated;
	char* tizen_org_feature_sensor_magnetometer_wakeup;
	char* tizen_org_feature_sensor_pedometer;
	char* tizen_org_feature_sensor_photometer;
	char* tizen_org_feature_sensor_photometer_wakeup;
	char* tizen_org_feature_sensor_proximity;
	char* tizen_org_feature_sensor_proximity_wakeup;
	char* tizen_org_feature_sensor_rotation_vector;
	char* tizen_org_feature_sensor_temperature;
	char* tizen_org_feature_sensor_tiltmeter;
	char* tizen_org_feature_sensor_tiltmeter_wakeup;
	char* tizen_org_feature_sensor_ultraviolet;
	char* tizen_org_feature_sensor_wrist_up;
	char* tizen_org_feature_shell_appwidget;
	char* tizen_org_feature_sip_voip;
	char* tizen_org_feature_speech_control;
	char* tizen_org_feature_speech_recognition;
	char* tizen_org_feature_speech_synthesis;
	char* tizen_org_feature_usb_accessory;
	char* tizen_org_feature_usb_host;
	char* tizen_org_feature_vision_barcode_detection;
	char* tizen_org_feature_vision_barcode_generation;
	char* tizen_org_feature_vision_face_recognition;
	char* tizen_org_feature_vision_image_recognition;
	char* tizen_org_feature_vision_qrcode_generation;
	char* tizen_org_feature_vision_qrcode_recognition;
	char* tizen_org_system_build_date;
	char* tizen_org_system_build_string;
	char* tizen_org_system_build_time;
	char* tizen_org_system_manufacturer;
	char* tizen_org_system_model_name;
	char* tizen_org_system_platform_communication_processor;
	char* tizen_org_system_platform_name;
	char* tizen_org_system_platform_processor;
};

int system_info_init_tizen_prop(void);
struct tizen_prop * system_info_get_tizen_prop(void);
#endif
