/**************************************************************************/
/*  openxr_ext_plane_detector_settings.h                                  */
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
#include <godot_cpp/classes/resource.hpp>

#include "classes/openxr_ext_plane_tracker.h"

namespace godot {

class OpenXRExtPlaneDetectorSettings : public Resource {
	GDCLASS(OpenXRExtPlaneDetectorSettings, Resource);

private:
	uint32_t orientation_flags = 0;
	Vector<OpenXRExtPlaneTracker::PlaneOrientation> orientations;
	uint32_t semantic_type_flags = 0;
	Vector<OpenXRExtPlaneTracker::PlaneSemanticType> semantic_types;
	uint32_t max_planes = 50;
	float min_area = 0.05;
	//XrPosef bounding_box_pose = ;
	//XrExtent3DfEXT bounding_box_extent;

protected:
	static void _bind_methods();

public:
	OpenXRExtPlaneDetectorSettings() = default;

	void set_orientation_flags(uint32_t p_orientation_flags);
	uint32_t get_orientation_flags() const;
	Vector<OpenXRExtPlaneTracker::PlaneOrientation> get_orientations();

	void set_semantic_type_flags(uint32_t p_semantic_type_flags);
	uint32_t get_semantic_type_flags() const;
	Vector<OpenXRExtPlaneTracker::PlaneSemanticType> get_semantic_types();

	void set_max_planes(uint32_t p_max_planes);
	uint32_t get_max_planes() const;

	void set_min_area(float p_min_area);
	float get_min_area() const;
};

} // namespace godot