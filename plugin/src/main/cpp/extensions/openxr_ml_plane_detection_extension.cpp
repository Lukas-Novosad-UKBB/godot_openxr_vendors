/**************************************************************************/
/*  openxr_ml_plane_detection_extension.cpp                               */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "extensions/openxr_ml_plane_detection_extension.h"

#include <godot_cpp/classes/open_xrapi_extension.hpp>
#include <godot_cpp/core/binder_common.hpp>

using namespace godot;

OpenXRMlPlaneDetectionExtension *OpenXRMlPlaneDetectionExtension::singleton = nullptr;

OpenXRMlPlaneDetectionExtension *OpenXRMlPlaneDetectionExtension::get_singleton() {
	if (singleton == nullptr) {
		singleton = memnew(OpenXRMlPlaneDetectionExtension());
	}

	return singleton;
}

OpenXRMlPlaneDetectionExtension::OpenXRMlPlaneDetectionExtension() : OpenXRExtensionWrapper() {

	ERR_FAIL_COND_MSG(singleton != nullptr, "An OpenXRMlPlaneDetectionExtension singleton already exists.");

	request_extensions[XR_EXT_PLANE_DETECTION_EXTENSION_NAME] = &ml_plane_detection_ext;

	singleton = this;
}

OpenXRMlPlaneDetectionExtension::~OpenXRMlPlaneDetectionExtension() {
	cleanup();
	singleton = nullptr;
}

void OpenXRMlPlaneDetectionExtension::_bind_methods() {

	ClassDB::bind_method(D_METHOD("is_plane_detection_supported"), &OpenXRMlPlaneDetectionExtension::is_plane_detection_supported);

	ADD_SIGNAL(MethodInfo("updating_plane_detectors", PropertyInfo(Variant::INT, "time")));
}

godot::Dictionary OpenXRMlPlaneDetectionExtension::_get_requested_extensions(uint64_t p_xr_version) {
	godot::Dictionary result;
	for (auto ext : request_extensions) {
		uint64_t value = reinterpret_cast<uint64_t>(ext.value);
		result[ext.key] = (Variant)value;
	}
	
	return result;
}

void OpenXRMlPlaneDetectionExtension::_on_instance_created(const uint64_t p_instance) {

	if (ml_plane_detection_ext) {
		bool result = initialize_ml_plane_detection_extension((XrInstance)p_instance);
		if (!result) {
			UtilityFunctions::print("OpenXRMlPlaneDetectionExtension::_on_instance_created: Failed to initialize ml_plane_detection extension");
			ml_plane_detection_ext = false;
		}
	}
}

void OpenXRMlPlaneDetectionExtension::_on_instance_destroyed() {
	cleanup();
}

bool OpenXRMlPlaneDetectionExtension::initialize_ml_plane_detection_extension(const XrInstance &p_instance) {
	GDEXTENSION_INIT_XR_FUNC_V(xrCreatePlaneDetectorEXT);
	GDEXTENSION_INIT_XR_FUNC_V(xrDestroyPlaneDetectorEXT);
	GDEXTENSION_INIT_XR_FUNC_V(xrBeginPlaneDetectionEXT);
	GDEXTENSION_INIT_XR_FUNC_V(xrGetPlaneDetectionStateEXT);
	GDEXTENSION_INIT_XR_FUNC_V(xrGetPlaneDetectionsEXT);
	GDEXTENSION_INIT_XR_FUNC_V(xrGetPlanePolygonBufferEXT);

	return true;
}

void OpenXRMlPlaneDetectionExtension::_on_process() {

	if (!is_plane_detection_supported())
	{
		// not supported
		print_line("OpenXRMlPlaneDetectionExtension::_on_process: The plane detection extension is not supported, returning!");
		return;
	}

	const XrTime time = get_openxr_api()->get_predicted_display_time();
	if (time == 0) {
		// we don't have timing info yet, or we're skipping a frame...
		return;
	}

	emit_signal("updating_plane_detectors", time);
}

void OpenXRMlPlaneDetectionExtension::cleanup() {
	ml_plane_detection_ext = false;
}

XrPlaneDetectorEXT OpenXRMlPlaneDetectionExtension::create_plane_detector(const XrPlaneDetectorCreateInfoEXT *p_create_info) {
	XrPlaneDetectorEXT plane_detector;

	XrResult result = xrCreatePlaneDetectorEXT((XrSession)get_openxr_api()->get_session(), p_create_info, &plane_detector);
	if (XR_FAILED(result)) {
		print_line("OpenXRMlPlaneDetectionExtension::create_plane_detector: Failed to create plane detector [", get_openxr_api()->get_error_string(result), "]");
		return XR_NULL_HANDLE;
	}

	return plane_detector;
}

bool OpenXRMlPlaneDetectionExtension::begin_plane_detection(XrPlaneDetectorEXT p_plane_detector, const XrPlaneDetectorBeginInfoEXT *p_begin_info) {
	XrResult result = xrBeginPlaneDetectionEXT(p_plane_detector, p_begin_info);
	if (XR_FAILED(result)) {
		print_line("OpenXRMlPlaneDetectionExtension::begin_plane_detection: Failed to begin plane detection [", get_openxr_api()->get_error_string(result), "]");
		return false;
	}
	// LuNo: debug/testing
	//print_line("OpenXRMlPlaneDetectionExtension::begin_plane_detection: Begun plane detection!");
	return true;
}

XrPlaneDetectionStateEXT OpenXRMlPlaneDetectionExtension::get_plane_detector_status(XrPlaneDetectorEXT p_plane_detector) {
	XrPlaneDetectionStateEXT plane_detection_state;
	XrResult result = xrGetPlaneDetectionStateEXT(p_plane_detector, &plane_detection_state);
	if (XR_FAILED(result)) {
		print_line("OpenXRMlPlaneDetectionExtension::get_plane_detector_status: Failed to get the plane detection state [", get_openxr_api()->get_error_string(result), "]");
		return XR_PLANE_DETECTION_STATE_FATAL_EXT;
	}
	return plane_detection_state;
}

bool OpenXRMlPlaneDetectionExtension::get_plane_detections(XrPlaneDetectorEXT p_plane_detector, const XrPlaneDetectorGetInfoEXT *p_plane_get_info, XrPlaneDetectorLocationsEXT *p_plane_locations) {
	XrResult result = xrGetPlaneDetectionsEXT(p_plane_detector, p_plane_get_info, p_plane_locations);
	if (XR_FAILED(result)) {
		print_line("OpenXRMlPlaneDetectionExtension::get_plane_detections: Failed to query the plane detections [", get_openxr_api()->get_error_string(result), "]");
		return false;
	}
	return true;
}

bool OpenXRMlPlaneDetectionExtension::get_plane_polygon_buffer(XrPlaneDetectorEXT p_plane_detector, uint64_t p_plane_id, uint32_t p_plane_polygon_buffer_index, XrPlaneDetectorPolygonBufferEXT* p_plane_polygon_buffer) {
	XrResult result = xrGetPlanePolygonBufferEXT(p_plane_detector, p_plane_id, p_plane_polygon_buffer_index, p_plane_polygon_buffer);
	if (XR_FAILED(result)) {
		print_line("OpenXRMlPlaneDetectionExtension::get_plane_polygon_buffer: Failed to query the plane polygon buffer [", get_openxr_api()->get_error_string(result), "]");
		return false;
	}
	return true;
}

void OpenXRMlPlaneDetectionExtension::destroy_plane_detector(XrPlaneDetectorEXT p_plane_detector) {
	XrResult result = xrDestroyPlaneDetectorEXT(p_plane_detector);
	if (XR_FAILED(result)) {
		print_line("OpenXRMlPlaneDetectionExtension::destroy_plane_detector: Failed to destroy plane detector [", get_openxr_api()->get_error_string(result), "]");
	}
	// LuNo: debug/testing
	else {
		print_line("OpenXRMlPlaneDetectionExtension::destroy_plane_detector: Destroyed plane detector!");
	}
}

// LuNo: remove once done?
String OpenXRMlPlaneDetectionExtension::get_plane_detection_state_string(XrPlaneDetectionStateEXT state) {
	switch (state) {
		case  XR_PLANE_DETECTION_STATE_NONE_EXT: {
			return "XR_PLANE_DETECTION_STATE_NONE_EXT";
		}
		case XR_PLANE_DETECTION_STATE_PENDING_EXT: {
			return "XR_PLANE_DETECTION_STATE_PENDING_EXT";
		}
		case XR_PLANE_DETECTION_STATE_DONE_EXT: {
			return "XR_PLANE_DETECTION_STATE_DONE_EXT";
		}
		case XR_PLANE_DETECTION_STATE_ERROR_EXT: {
			return "XR_PLANE_DETECTION_STATE_ERROR_EXT";
		}
		case XR_PLANE_DETECTION_STATE_FATAL_EXT: {
			return "XR_PLANE_DETECTION_STATE_FATAL_EXT";
		}
		case XR_PLANE_DETECTION_STATE_MAX_ENUM_EXT: {
			return "XR_PLANE_DETECTION_STATE_MAX_ENUM_EXT";
		}
		default: {
			return "XR_PLANE_DETECTION_STATE_NONE_EXT";
		}
	}
}
