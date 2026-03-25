/**************************************************************************/
/*  openxr_ext_plane_detector.h                                           */
/**************************************************************************/
/*                       This file is part of:                            */
/*                              GODOT XR                                  */
/*                      https://godotengine.org                           */
/**************************************************************************/
/* Copyright (c) 2022-present Godot XR contributors (see CONTRIBUTORS.md) */
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
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include "classes/openxr_ext_plane_detector_settings.h"
#include "classes/openxr_ext_plane_tracker.h"

namespace godot {

class OpenXRExtPlaneDetector : public Node {
	GDCLASS(OpenXRExtPlaneDetector, Node);

public:
	// similar to the enum XrPlaneDetectionStateEXT from XR_EXT_plane_detection
	enum Status {
		PLANE_DETECTION_STATE_NONE,
		PLANE_DETECTION_STATE_PENDING,
		PLANE_DETECTION_STATE_DONE,
		PLANE_DETECTION_STATE_ERROR,
		PLANE_DETECTION_STATE_FATAL,
		PLANE_DETECTION_STATE_MAX_ENUM
	};

private:
	Status status = Status::PLANE_DETECTION_STATE_NONE;
	TypedArray<OpenXRExtPlaneTracker> plane_trackers;
	XrPlaneDetectorEXT plane_detector = nullptr;
	bool detector_settings_dirty = false;
	bool query_running = false;
	bool detection_enabled = false;
	bool contour_bit = true;
	Ref<OpenXRExtPlaneDetectorSettings> settings;

	void _on_updating_plane_detectors(XrTime time);
	void _on_settings_changed();
	void _create_plane_detector();
	void _destroy_plane_detector();
	void _update_plane_detector(XrTime time);
	void _notification(int p_what);
	Ref<OpenXRExtPlaneTracker> get_or_create_plane_tracker_by_detector_location(XrPlaneDetectorLocationEXT p_detector_location);

protected:
	static void _bind_methods();

public:
	Status get_status() const;
	TypedArray<OpenXRExtPlaneTracker> get_plane_trackers() const;

	void set_settings(const Ref<OpenXRExtPlaneDetectorSettings> &p_settings);
	Ref<OpenXRExtPlaneDetectorSettings> get_settings() const;

	void set_detection_enabled(bool p_detection_enabled);
	bool get_detection_enabled() const;

	void set_contour_bit(bool p_contour_bit);
	bool get_contour_bit() const;
};

} // namespace godot

VARIANT_ENUM_CAST(OpenXRExtPlaneDetector::Status);