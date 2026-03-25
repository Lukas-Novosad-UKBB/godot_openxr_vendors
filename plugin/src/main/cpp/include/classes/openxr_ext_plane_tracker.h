/**************************************************************************/
/*  openxr_ext_plane_tracker.h                                            */
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

#include <godot_cpp/classes/xr_positional_tracker.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace godot {

class OpenXRExtPlaneTracker : public XRPositionalTracker {
	GDCLASS(OpenXRExtPlaneTracker, XRPositionalTracker);

public:
	// similar to enum XrPlaneDetectorOrientationEXT from XR_EXT_plane_detection
	enum PlaneOrientation {
		PLANE_ORIENTATION_HORIZONTAL_UPWARD,
		PLANE_ORIENTATION_HORIZONTAL_DOWNWARD,
		PLANE_ORIENTATION_VERTICAL,
		PLANE_ORIENTATION_ARBITRARY,
		PLANE_ORIENTATION_MAX_ENUM,
	};

	// similar to enum XrPlaneDetectorSemanticTypeEXT from XR_EXT_plane_detection
	enum PlaneSemanticType {
		PLANE_SEMANTIC_TYPE_UNDEFINED,
		PLANE_SEMANTIC_TYPE_CEILING,
		PLANE_SEMANTIC_TYPE_FLOOR,
		PLANE_SEMANTIC_TYPE_WALL,
		PLANE_SEMANTIC_TYPE_PLATFORM,
		PLANE_SEMANTIC_TYPE_MAX_ENUM,
	};

	/* typedef XrFlags64 XrSpaceLocationFlags;

	// Flag bits for XrSpaceLocationFlags
	static const XrSpaceLocationFlags XR_SPACE_LOCATION_ORIENTATION_VALID_BIT = 0x00000001;
	static const XrSpaceLocationFlags XR_SPACE_LOCATION_POSITION_VALID_BIT = 0x00000002;
	static const XrSpaceLocationFlags XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT = 0x00000004;
	static const XrSpaceLocationFlags XR_SPACE_LOCATION_POSITION_TRACKED_BIT = 0x00000008; */

	// Flag bits similar for XrSpaceLocationFlags
	enum PlaneSpaceLocationFlags {
		PLANE_SPACE_LOCATION_ORIENTATION_VALID_BIT = 1,
		PLANE_SPACE_LOCATION_POSITION_VALID_BIT = 2,
		PLANE_SPACE_LOCATION_ORIENTATION_TRACKED_BIT = 4,
		PLANE_SPACE_LOCATION_POSITION_TRACKED_BIT = 8,
	};

private:
	// order of members roughly follows XrPlaneDetectorLocationEXT
	uint64_t plane_id = 0;
	BitField<PlaneSpaceLocationFlags> plane_space_location_flags = 0;
	Vector3 plane_position;
	Quaternion plane_rotation;
	float plane_width = 0;
	float plane_height = 0;
	PlaneOrientation plane_orientation = PLANE_ORIENTATION_MAX_ENUM;
	PlaneSemanticType plane_semantic_type = PLANE_SEMANTIC_TYPE_MAX_ENUM;
	uint32_t plane_polygon_buffer_count = 0;

	Array plane_polygon_buffers;

protected:
	static void _bind_methods();

public:
	OpenXRExtPlaneTracker() = default;
	OpenXRExtPlaneTracker(XrPlaneDetectorLocationEXT plane_location);

	void set_plane_id(uint64_t p_plane_id);
	uint64_t get_plane_id() const;

	void set_plane_space_location_flags(BitField<PlaneSpaceLocationFlags> p_plane_space_locations_flags);
	BitField<PlaneSpaceLocationFlags> get_plane_space_location_flags() const;

	void set_plane_position(Vector3 p_plane_position);
	Vector3 get_plane_position() const;

	void set_plane_rotation(Quaternion p_plane_rotation);
	Quaternion get_plane_rotation() const;

	void set_plane_width(float p_plane_width);
	float get_plane_width() const;

	void set_plane_height(float p_plane_height);
	float get_plane_height() const;

	void set_plane_orientation(PlaneOrientation p_orientation);
	PlaneOrientation get_plane_orientation() const;

	void set_plane_semantic_type(PlaneSemanticType p_semantic_type);
	PlaneSemanticType get_plane_semantic_type() const;

	void set_plane_polygon_buffer_count(uint32_t p_plane_polygon_buffer_count);
	uint32_t get_plane_polygon_buffer_count() const;

	Array get_plane_polygon_buffers() const;

	void update_plane(XrPlaneDetectorLocationEXT p_detector_location);
	void update_plane_polygon(uint32_t p_polygon_buffer_index, XrPlaneDetectorPolygonBufferEXT p_polygon_buffer);
};
} // namespace godot

VARIANT_ENUM_CAST(OpenXRExtPlaneTracker::PlaneOrientation)
VARIANT_ENUM_CAST(OpenXRExtPlaneTracker::PlaneSemanticType)
VARIANT_BITFIELD_CAST(OpenXRExtPlaneTracker::PlaneSpaceLocationFlags)