/**************************************************************************/
/*  openxr_ml_plane_detector_settings.cpp                                 */
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

#include "classes/openxr_ml_plane_detector_settings.h"

#include <godot_cpp/classes/open_xrapi_extension.hpp>

using namespace godot;

void OpenXRMlPlaneDetectorSettings::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_orientation_flags", "orientation_flags"), &OpenXRMlPlaneDetectorSettings::set_orientation_flags);
	ClassDB::bind_method(D_METHOD("get_orientation_flags"), &OpenXRMlPlaneDetectorSettings::get_orientation_flags);

	ClassDB::bind_method(D_METHOD("set_semantic_type_flags", "semantic_type_flags"), &OpenXRMlPlaneDetectorSettings::set_semantic_type_flags);
	ClassDB::bind_method(D_METHOD("get_semantic_type_flags"), &OpenXRMlPlaneDetectorSettings::get_semantic_type_flags);

	ClassDB::bind_method(D_METHOD("set_max_planes", "max_planes"), &OpenXRMlPlaneDetectorSettings::set_max_planes);
	ClassDB::bind_method(D_METHOD("get_max_planes"), &OpenXRMlPlaneDetectorSettings::get_max_planes);

	ClassDB::bind_method(D_METHOD("set_min_area", "min_area"), &OpenXRMlPlaneDetectorSettings::set_min_area);
	ClassDB::bind_method(D_METHOD("get_min_area"), &OpenXRMlPlaneDetectorSettings::get_min_area);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "orientation_flags", PROPERTY_HINT_FLAGS, "HOR UPWARD:1,HOR DOWNWARD:2,VERTICAL:4,ARBITRARY:8"), "set_orientation_flags", "get_orientation_flags");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "semantic_type_flags", PROPERTY_HINT_FLAGS, "UNDEFINED:1,CEILING:2,FLOOR:4,WALL:8,PLATFORM:16"), "set_semantic_type_flags", "get_semantic_type_flags");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_planes", PROPERTY_HINT_RANGE, "50,1000,1"), "set_max_planes", "get_max_planes");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_area", PROPERTY_HINT_RANGE, "0.01,100.0,0.01"), "set_min_area", "get_min_area");
}

void OpenXRMlPlaneDetectorSettings::set_orientation_flags(uint32_t p_orientation_flags) {
	if (orientation_flags == p_orientation_flags) {
		return;
	}

	orientation_flags = p_orientation_flags;

	orientations.clear();
	/* The mapping between the flags and the enum are as follows
		PLANE_DETECTOR_ORIENTATION_UPWARD = 0		-->  1
    	PLANE_DETECTOR_ORIENTATION_DOWNWARD = 1		-->  2
    	PLANE_DETECTOR_ORIENTATION_VERTICAL = 2		-->  4
    	PLANE_DETECTOR_ORIENTATION_ARBITATRY = 3	-->  8
    	PLANE_DETECTOR_ORIENTATION_MAX_ENUM = 4 	--> 16
	*/
	if (p_orientation_flags & 1) {
		orientations.push_back(OpenXRMlPlaneTracker::PLANE_ORIENTATION_HORIZONTAL_UPWARD);
	}
	if (p_orientation_flags & 2) {
		orientations.push_back(OpenXRMlPlaneTracker::PLANE_ORIENTATION_HORIZONTAL_DOWNWARD);
	}
	if (p_orientation_flags & 4) {
		orientations.push_back(OpenXRMlPlaneTracker::PLANE_ORIENTATION_VERTICAL);
	}
	if (p_orientation_flags & 8) {
		orientations.push_back(OpenXRMlPlaneTracker::PLANE_ORIENTATION_ARBITRARY);
	}
	if (p_orientation_flags & 16 ) {
		print_line("OpenXRMlPlaneDetectorSettings::set_orientation_flags: The plane detection orientation [PLANE_DETECTOR_ORIENTATION_MAX_ENUM] cannot be set, will ignore it.");
	}
	emit_changed();
}

uint32_t OpenXRMlPlaneDetectorSettings::get_orientation_flags() const {
	return orientation_flags;
}

Vector<OpenXRMlPlaneTracker::PlaneOrientation> OpenXRMlPlaneDetectorSettings::get_orientations() {
	return orientations;
}

uint32_t OpenXRMlPlaneDetectorSettings::get_orientations_count() const {
	return (uint32_t)orientations.size();
}

void OpenXRMlPlaneDetectorSettings::set_semantic_type_flags(uint32_t p_semantic_type_flags) {
	if (semantic_type_flags == p_semantic_type_flags) {
		return;
	}

	semantic_type_flags = p_semantic_type_flags;
	/* The mapping between the flags and the enum are as follows
		PLANE_DETECTOR_SEMANTIC_TYPE_UNDEFINED = 0	-->  1
    	PLANE_DETECTOR_SEMANTIC_TYPE_CEILING = 1	-->  2
    	PLANE_DETECTOR_SEMANTIC_TYPE_FLOOR = 2		-->  4
    	PLANE_DETECTOR_SEMANTIC_TYPE_WALL = 3		-->  8
    	PLANE_DETECTOR_SEMANTIC_TYPE_PLATFORM = 4	--> 16
    	PLANE_DETECTOR_SEMANTIC_TYPE_MAX_ENUM = 5 --> 32
	*/
	semantic_types.clear();
	if (p_semantic_type_flags & 1) {
		semantic_types.push_back(OpenXRMlPlaneTracker::PLANE_SEMANTIC_TYPE_UNDEFINED);
	}
	if (p_semantic_type_flags & 2) {
		semantic_types.push_back(OpenXRMlPlaneTracker::PLANE_SEMANTIC_TYPE_CEILING);
	}
	if (p_semantic_type_flags & 4) {
		semantic_types.push_back(OpenXRMlPlaneTracker::PLANE_SEMANTIC_TYPE_FLOOR);
	}
	if (p_semantic_type_flags & 8) {
		semantic_types.push_back(OpenXRMlPlaneTracker::PLANE_SEMANTIC_TYPE_WALL);
	}
	if (p_semantic_type_flags & 16) {
		semantic_types.push_back(OpenXRMlPlaneTracker::PLANE_SEMANTIC_TYPE_PLATFORM);
	}
	if (p_semantic_type_flags & 32) {
		print_line("OpenXRMlPlaneDetectorSettings::set_semantic_type_flags: The plane detection semantic type [PLANE_DETECTOR_SEMANTIC_TYPE_MAX_ENUM] cannot be set, will ignore it.");
	}

	emit_changed();
}

uint32_t OpenXRMlPlaneDetectorSettings::get_semantic_type_flags() const {
	return semantic_type_flags;
}

Vector<OpenXRMlPlaneTracker::PlaneSemanticType> OpenXRMlPlaneDetectorSettings::get_semantic_types() {
	return semantic_types;
}

uint32_t OpenXRMlPlaneDetectorSettings::get_semantic_types_count() const {
	return (uint32_t)semantic_types.size();
}

void OpenXRMlPlaneDetectorSettings::set_max_planes(uint32_t p_max_planes) {
	if (max_planes == p_max_planes) {
		return;
	}
	max_planes = p_max_planes;
	emit_changed();
}

uint32_t OpenXRMlPlaneDetectorSettings::get_max_planes() const {
	return max_planes;
}

void OpenXRMlPlaneDetectorSettings::set_min_area(float p_min_area) {
	if (min_area == p_min_area) {
		return;
	}
	min_area = p_min_area;
	emit_changed();
}

float OpenXRMlPlaneDetectorSettings::get_min_area() const {
	return min_area;
}
