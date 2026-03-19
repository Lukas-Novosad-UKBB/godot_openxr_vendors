/**************************************************************************/
/*  openxr_ml_plane_detection_extension.h                                 */
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

#pragma once

#include <openxr/openxr.h>
#include <godot_cpp/classes/open_xr_extension_wrapper.hpp>

#include "util.h"

#include <map>

using namespace godot;

// Wrapper for the Magic Leap Plane Detection extension.
class OpenXRMlPlaneDetectionExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRMlPlaneDetectionExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods();

public:
	static OpenXRMlPlaneDetectionExtension *get_singleton();

	OpenXRMlPlaneDetectionExtension();
	~OpenXRMlPlaneDetectionExtension();

	godot::Dictionary _get_requested_extensions(uint64_t p_xr_version) override;

	bool is_plane_detection_supported() const {
		return ml_plane_detection_ext;
	}

	// extension wrapper
	void _on_instance_created(uint64_t p_instance) override;
	void _on_instance_destroyed() override;

	//uint64_t _set_system_properties_and_get_next_pointer(void *p_next_pointer) override;
	void _on_process() override;

	XrPlaneDetectorEXT create_plane_detector(const XrPlaneDetectorCreateInfoEXT *p_create_info);
	bool begin_plane_detection(XrPlaneDetectorEXT p_plane_detector, const XrPlaneDetectorBeginInfoEXT *p_begin_info);
	XrPlaneDetectionStateEXT get_plane_detector_status(XrPlaneDetectorEXT p_plane_detector);
	bool get_plane_detections(XrPlaneDetectorEXT p_plane_detector, const XrPlaneDetectorGetInfoEXT* p_plane_get_info, XrPlaneDetectorLocationsEXT* p_plane_locations);
	bool get_plane_polygon_buffer(XrPlaneDetectorEXT p_plane_detector, uint64_t p_plane_id, uint32_t p_plane_polygon_buffer_index, XrPlaneDetectorPolygonBufferEXT* p_plane_polygon_buffer);
	void destroy_plane_detector(XrPlaneDetectorEXT p_plane_detector);

	String get_plane_detection_state_string(XrPlaneDetectionStateEXT state);

private:
	static OpenXRMlPlaneDetectionExtension *singleton;

	// state
	XrSystemPlaneDetectionPropertiesEXT plane_detection_system_properties;

	HashMap<String, bool *> request_extensions;

	bool ml_plane_detection_ext = false;
	bool query_running = false;

	// functions
	bool initialize_ml_plane_detection_extension(const XrInstance &p_instance);
	void cleanup();

	// OpenXR API call wrappers
	EXT_PROTO_XRRESULT_FUNC3(xrCreatePlaneDetectorEXT, (XrSession), p_session, (const XrPlaneDetectorCreateInfoEXT *), p_createInfo, (XrPlaneDetectorEXT *), p_planeDetector);
	EXT_PROTO_XRRESULT_FUNC1(xrDestroyPlaneDetectorEXT, (XrPlaneDetectorEXT), p_planeDetector);
	EXT_PROTO_XRRESULT_FUNC2(xrBeginPlaneDetectionEXT, (XrPlaneDetectorEXT), p_planeDetector, (const XrPlaneDetectorBeginInfoEXT *), p_beginInfo);
	EXT_PROTO_XRRESULT_FUNC2(xrGetPlaneDetectionStateEXT, (XrPlaneDetectorEXT), p_planeDetector, (XrPlaneDetectionStateEXT *), p_state);
	EXT_PROTO_XRRESULT_FUNC3(xrGetPlaneDetectionsEXT, (XrPlaneDetectorEXT), p_planeDetector, (const XrPlaneDetectorGetInfoEXT *), p_info, (XrPlaneDetectorLocationsEXT *), p_locations);
	EXT_PROTO_XRRESULT_FUNC4(xrGetPlanePolygonBufferEXT, (XrPlaneDetectorEXT), p_planeDetector, (uint64_t), planeId, (uint32_t), polygonBufferIndex, (XrPlaneDetectorPolygonBufferEXT *), p_polygonBuffer);

};
