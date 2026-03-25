/**************************************************************************/
/*  openxr_ext_plane_tracker.cpp                                          */
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

#include "classes/openxr_ext_plane_tracker.h"

using namespace godot;

void OpenXRExtPlaneTracker::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_plane_id", "plane_id"), &OpenXRExtPlaneTracker::set_plane_id);
	ClassDB::bind_method(D_METHOD("get_plane_id"), &OpenXRExtPlaneTracker::get_plane_id);

	ClassDB::bind_method(D_METHOD("set_plane_space_location_flags", "plane_space_location_flags"), &OpenXRExtPlaneTracker::set_plane_space_location_flags);
	ClassDB::bind_method(D_METHOD("get_place_space_location_flags"), &OpenXRExtPlaneTracker::get_plane_space_location_flags);

	ClassDB::bind_method(D_METHOD("set_plane_position", "plane_position"), &OpenXRExtPlaneTracker::set_plane_position);
	ClassDB::bind_method(D_METHOD("get_plane_position"), &OpenXRExtPlaneTracker::get_plane_position);

	ClassDB::bind_method(D_METHOD("set_plane_rotation", "plane_rotation"), &OpenXRExtPlaneTracker::set_plane_rotation);
	ClassDB::bind_method(D_METHOD("get_plane_rotation"), &OpenXRExtPlaneTracker::get_plane_rotation);

	ClassDB::bind_method(D_METHOD("set_plane_width", "plane_width"), &OpenXRExtPlaneTracker::set_plane_width);
	ClassDB::bind_method(D_METHOD("get_plane_width"), &OpenXRExtPlaneTracker::get_plane_width);

	ClassDB::bind_method(D_METHOD("set_plane_height", "plane_height"), &OpenXRExtPlaneTracker::set_plane_height);
	ClassDB::bind_method(D_METHOD("get_plane_height"), &OpenXRExtPlaneTracker::get_plane_height);

	ClassDB::bind_method(D_METHOD("set_plane_orientation", "plane_orientation"), &OpenXRExtPlaneTracker::set_plane_orientation);
	ClassDB::bind_method(D_METHOD("get_plane_orientation"), &OpenXRExtPlaneTracker::get_plane_orientation);

	ClassDB::bind_method(D_METHOD("set_plane_semantic_type", "plane_semantic_type"), &OpenXRExtPlaneTracker::set_plane_semantic_type);
	ClassDB::bind_method(D_METHOD("get_plane_semantic_type"), &OpenXRExtPlaneTracker::get_plane_semantic_type);

	ClassDB::bind_method(D_METHOD("set_plane_polygon_buffer_count", "plane_polygon_buffer_count"), &OpenXRExtPlaneTracker::set_plane_polygon_buffer_count);
	ClassDB::bind_method(D_METHOD("get_plane_polygon_buffer_count"), &OpenXRExtPlaneTracker::get_plane_polygon_buffer_count);

	ClassDB::bind_method(D_METHOD("get_plane_polygon_buffers"), &OpenXRExtPlaneTracker::get_plane_polygon_buffers);

	BIND_ENUM_CONSTANT(PLANE_ORIENTATION_HORIZONTAL_UPWARD);
	BIND_ENUM_CONSTANT(PLANE_ORIENTATION_HORIZONTAL_DOWNWARD);
	BIND_ENUM_CONSTANT(PLANE_ORIENTATION_VERTICAL);
	BIND_ENUM_CONSTANT(PLANE_ORIENTATION_ARBITRARY);
	BIND_ENUM_CONSTANT(PLANE_ORIENTATION_MAX_ENUM);

	BIND_ENUM_CONSTANT(PLANE_SEMANTIC_TYPE_UNDEFINED);
	BIND_ENUM_CONSTANT(PLANE_SEMANTIC_TYPE_CEILING);
	BIND_ENUM_CONSTANT(PLANE_SEMANTIC_TYPE_FLOOR);
	BIND_ENUM_CONSTANT(PLANE_SEMANTIC_TYPE_WALL);
	BIND_ENUM_CONSTANT(PLANE_SEMANTIC_TYPE_PLATFORM);
	BIND_ENUM_CONSTANT(PLANE_SEMANTIC_TYPE_MAX_ENUM);

	BIND_ENUM_CONSTANT(PLANE_SPACE_LOCATION_ORIENTATION_VALID_BIT);
	BIND_ENUM_CONSTANT(PLANE_SPACE_LOCATION_POSITION_VALID_BIT);
	BIND_ENUM_CONSTANT(PLANE_SPACE_LOCATION_ORIENTATION_TRACKED_BIT);
	BIND_ENUM_CONSTANT(PLANE_SPACE_LOCATION_POSITION_TRACKED_BIT);

	// LuNo: required?
	ADD_SIGNAL(MethodInfo("openxr_plane_updated"));
	ADD_SIGNAL(MethodInfo("openxr_plane_polygon_updated"));
}

void OpenXRExtPlaneTracker::set_plane_id(uint64_t p_plane_id) {
	plane_id = p_plane_id;
}

uint64_t OpenXRExtPlaneTracker::get_plane_id() const {
	return plane_id;
}

void OpenXRExtPlaneTracker::set_plane_space_location_flags(BitField<PlaneSpaceLocationFlags> p_plane_space_locations_flags) {
	plane_space_location_flags = p_plane_space_locations_flags;
}

BitField<OpenXRExtPlaneTracker::PlaneSpaceLocationFlags> OpenXRExtPlaneTracker::get_plane_space_location_flags() const {
	return plane_space_location_flags;
}

void OpenXRExtPlaneTracker::set_plane_position(Vector3 p_plane_position) {
	plane_position = p_plane_position;
}

Vector3 OpenXRExtPlaneTracker::get_plane_position() const {
	return plane_position;
}

void OpenXRExtPlaneTracker::set_plane_rotation(Quaternion p_plane_rotation) {
	plane_rotation = p_plane_rotation;
}

Quaternion OpenXRExtPlaneTracker::get_plane_rotation() const {
	return plane_rotation;
}

void OpenXRExtPlaneTracker::set_plane_width(float p_plane_width) {
	plane_width = p_plane_width;
}

float OpenXRExtPlaneTracker::get_plane_width() const {
	return plane_width;
}

void OpenXRExtPlaneTracker::set_plane_height(float p_plane_height) {
	plane_height = p_plane_height;
}

float OpenXRExtPlaneTracker::get_plane_height() const {
	return plane_height;
}

void OpenXRExtPlaneTracker::set_plane_orientation(PlaneOrientation p_plane_orientation) {
	plane_orientation = p_plane_orientation;
}

OpenXRExtPlaneTracker::PlaneOrientation OpenXRExtPlaneTracker::get_plane_orientation() const {
	return plane_orientation;
}

void OpenXRExtPlaneTracker::set_plane_semantic_type(PlaneSemanticType p_plane_semantic_type) {
	plane_semantic_type = p_plane_semantic_type;
}

OpenXRExtPlaneTracker::PlaneSemanticType OpenXRExtPlaneTracker::get_plane_semantic_type() const {
	return plane_semantic_type;
}

void OpenXRExtPlaneTracker::set_plane_polygon_buffer_count(uint32_t p_plane_polygon_buffer_count) {
	plane_polygon_buffer_count = p_plane_polygon_buffer_count;
}

uint32_t OpenXRExtPlaneTracker::get_plane_polygon_buffer_count() const {
	return plane_polygon_buffer_count;
}

Array OpenXRExtPlaneTracker::get_plane_polygon_buffers() const {
	return plane_polygon_buffers;
}

void OpenXRExtPlaneTracker::update_plane(XrPlaneDetectorLocationEXT p_detector_location) {
	// LuNo: debug/testing
	//print_line("OpenXRExtPlaneTracker::update_plane: The location flags for plane id", plane_id, " are ", p_detector_location.locationFlags);

	if (p_detector_location.locationFlags & XR_SPACE_LOCATION_POSITION_TRACKED_BIT) {
		plane_space_location_flags.set_flag(PLANE_SPACE_LOCATION_POSITION_TRACKED_BIT);
	} else {
		plane_space_location_flags.clear_flag(PLANE_SPACE_LOCATION_POSITION_TRACKED_BIT);
	}
	if (p_detector_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT) {
		plane_space_location_flags.set_flag(PLANE_SPACE_LOCATION_ORIENTATION_TRACKED_BIT);
	} else {
		plane_space_location_flags.clear_flag(PLANE_SPACE_LOCATION_ORIENTATION_TRACKED_BIT);
	}
	if (p_detector_location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) {
		plane_space_location_flags.set_flag(PLANE_SPACE_LOCATION_POSITION_VALID_BIT);
	} else {
		plane_space_location_flags.clear_flag(PLANE_SPACE_LOCATION_POSITION_VALID_BIT);
	}
	if (p_detector_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) {
		plane_space_location_flags.set_flag(PLANE_SPACE_LOCATION_ORIENTATION_VALID_BIT);
	} else {
		plane_space_location_flags.clear_flag(PLANE_SPACE_LOCATION_ORIENTATION_VALID_BIT);
	}

	if (!(plane_space_location_flags.has_flag(PLANE_SPACE_LOCATION_POSITION_TRACKED_BIT)) ||
		!(plane_space_location_flags.has_flag(PLANE_SPACE_LOCATION_ORIENTATION_TRACKED_BIT)) ||
		!(plane_space_location_flags.has_flag(PLANE_SPACE_LOCATION_POSITION_VALID_BIT)) ||
		!(plane_space_location_flags.has_flag(PLANE_SPACE_LOCATION_ORIENTATION_VALID_BIT))) {
		print_line("OpenXRExtPlaneTracker::update_plane: Position and/or orientation are NOT tracked and/or valid, will not update any plane attributes!");

		Ref<XRPose> default_pose = get_pose("default");
		if (default_pose.is_valid()) {
			// Set the tracking confidence to none, while maintaining the existing transform.
			default_pose->set_tracking_confidence(XRPose::XR_TRACKING_CONFIDENCE_NONE);
		} else {
			set_pose("default", Transform3D(), Vector3(), Vector3(), XRPose::XR_TRACKING_CONFIDENCE_NONE);
		}

		// keep the existing plane attributes and hide the plane until it is tracked and valid again
	} else {
		// LuNo: debug/testing
		//print_line("OpenXRExtPlaneTracker::update_plane: Plane position and orientation are both tracked and valid! ");

		// position
		XrVector3f pos = p_detector_location.pose.position;
		plane_position.x = pos.x;
		plane_position.y = pos.y;
		plane_position.z = pos.z;

		// rotation
		XrQuaternionf rot = p_detector_location.pose.orientation;
		plane_rotation.x = rot.x;
		plane_rotation.y = rot.y;
		plane_rotation.z = rot.z;
		plane_rotation.w = rot.w;

		// extents
		plane_width = p_detector_location.extents.width;
		plane_height = p_detector_location.extents.height;

		// orientation (enum)
		plane_orientation = (PlaneOrientation)p_detector_location.orientation;

		// semantic type (enum)
		plane_semantic_type = (PlaneSemanticType)p_detector_location.semanticType;

		// number of polygon buffers - 0 corresponds the the contour, any index >0 represents a hole
		plane_polygon_buffer_count = p_detector_location.polygonBufferCount;

		// pose
		Quaternion quat = Quaternion(p_detector_location.pose.orientation.x, p_detector_location.pose.orientation.y, p_detector_location.pose.orientation.z, p_detector_location.pose.orientation.w);
		quat.normalize(); // normalize to be on the safe side
		Transform3D transform{ Basis(quat),
							   Vector3(plane_position.x, plane_position.y, plane_position.z) };
		set_pose("default", transform, Vector3(), Vector3(), XRPose::XR_TRACKING_CONFIDENCE_HIGH);

		// prepare polygon buffers
		plane_polygon_buffers.clear();
		plane_polygon_buffers.resize(plane_polygon_buffer_count);
	}

	// LuNo: required?
	emit_signal("openxr_plane_updated");
}

void OpenXRExtPlaneTracker::update_plane_polygon(uint32_t p_polygon_buffer_index, XrPlaneDetectorPolygonBufferEXT p_polygon_buffer) {
	if (p_polygon_buffer_index >= plane_polygon_buffers.size()) {
		print_line("OpenXRExtPlaneTracker::update_plane_polyogon: Polygon buffer index ", p_polygon_buffer_index, " is larger than the number of buffers ", plane_polygon_buffers.size(), "for plane ID ", plane_id);
		return;
	}

	PackedVector2Array vertices;
	Vector2 *vertices_dst = nullptr;

	vertices.resize(p_polygon_buffer.vertexCountOutput);
	vertices_dst = vertices.ptrw();

	const XrVector2f *vertices_src = p_polygon_buffer.vertices;

	for (uint32_t i = 0; i < p_polygon_buffer.vertexCountOutput; i++) {
		vertices_dst[i].x = vertices_src[i].x;
		vertices_dst[i].y = vertices_src[i].y;
	}

	// polygon_buffer_index = 0 -> outside contour CCW
	// polygon_buffer_index > 0 -> holes CW
	if (p_polygon_buffer_index == 0) {
		vertices.reverse();
	}
	plane_polygon_buffers[p_polygon_buffer_index] = vertices;

	// LuNo: required?
	emit_signal("openxr_plane_polygon_updated");
}

OpenXRExtPlaneTracker::OpenXRExtPlaneTracker(XrPlaneDetectorLocationEXT p_detector_location) {
	set_tracker_type(XRServer::TrackerType::TRACKER_ANCHOR);
	plane_id = p_detector_location.planeId;
	// LuNo: debug/testing
	print_line("OpenXRExtPlaneTracker::OpenXRExtPlaneTracker: Created tracker for plane ID", plane_id);
}