/**************************************************************************/
/*  openxr_ext_plane_detector.cpp                                         */
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

#include "classes/openxr_ext_plane_detector.h"
#include "extensions/openxr_ext_plane_detection_extension.h"

#include <godot_cpp/classes/open_xrapi_extension.hpp>

#include <algorithm>
#include <vector>

using namespace godot;

void OpenXRExtPlaneDetector::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_status"), &OpenXRExtPlaneDetector::get_status);
	ClassDB::bind_method(D_METHOD("get_plane_trackers"), &OpenXRExtPlaneDetector::get_plane_trackers);

	ClassDB::bind_method(D_METHOD("set_settings", "settings"), &OpenXRExtPlaneDetector::set_settings);
	ClassDB::bind_method(D_METHOD("get_settings"), &OpenXRExtPlaneDetector::get_settings);

	ClassDB::bind_method(D_METHOD("set_detection_enabled", "detection_enabled"), &OpenXRExtPlaneDetector::set_detection_enabled);
	ClassDB::bind_method(D_METHOD("get_detection_enabled"), &OpenXRExtPlaneDetector::get_detection_enabled);

	ClassDB::bind_method(D_METHOD("set_contour_bit", "contour_bit"), &OpenXRExtPlaneDetector::set_contour_bit);
	ClassDB::bind_method(D_METHOD("get_contour_bit"), &OpenXRExtPlaneDetector::get_contour_bit);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "settings", PROPERTY_HINT_RESOURCE_TYPE, "OpenXRExtPlaneDetectorSettings"), "set_settings", "get_settings");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "detection_enabled", PROPERTY_HINT_NONE, "Enable or disable plane detection"), "set_detection_enabled", "get_detection_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "contour_bit", PROPERTY_HINT_NONE, "XR_PLANE_DETECTOR_ENABLE_CONTOUR_BIT_EXT - populate the plane contour information"), "set_contour_bit", "get_contour_bit");

	BIND_ENUM_CONSTANT(PLANE_DETECTION_STATE_NONE);
	BIND_ENUM_CONSTANT(PLANE_DETECTION_STATE_PENDING);
	BIND_ENUM_CONSTANT(PLANE_DETECTION_STATE_DONE);
	BIND_ENUM_CONSTANT(PLANE_DETECTION_STATE_ERROR);
	BIND_ENUM_CONSTANT(PLANE_DETECTION_STATE_FATAL);
	BIND_ENUM_CONSTANT(PLANE_DETECTION_STATE_MAX_ENUM);
}

OpenXRExtPlaneDetector::Status OpenXRExtPlaneDetector::get_status() const {
	return status;
}

TypedArray<OpenXRExtPlaneTracker> OpenXRExtPlaneDetector::get_plane_trackers() const {
	return plane_trackers;
}

void OpenXRExtPlaneDetector::set_settings(const Ref<OpenXRExtPlaneDetectorSettings> &p_settings) {
	if (settings == p_settings) {
		return;
	}

	_destroy_plane_detector();
	detector_settings_dirty = true;
	settings = p_settings;
}

Ref<OpenXRExtPlaneDetectorSettings> OpenXRExtPlaneDetector::get_settings() const {
	return settings;
}

void OpenXRExtPlaneDetector::set_detection_enabled(bool p_detection_enabled) {
	if (detection_enabled == p_detection_enabled) {
		return;
	}

	detection_enabled = p_detection_enabled;
	if (detection_enabled) {
		detector_settings_dirty = true;
	}
}

bool OpenXRExtPlaneDetector::get_detection_enabled() const {
	return detection_enabled;
}

void OpenXRExtPlaneDetector::set_contour_bit(bool p_contour_bit) {
	if (contour_bit == p_contour_bit) {
		return;
	}

	_destroy_plane_detector();
	detector_settings_dirty = true;
	contour_bit = p_contour_bit;
}

bool OpenXRExtPlaneDetector::get_contour_bit() const {
	return contour_bit;
}

void OpenXRExtPlaneDetector::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			OpenXRExtPlaneDetectionExtension *plane_detection_extension = OpenXRExtPlaneDetectionExtension::get_singleton();
			if (plane_detection_extension) {
				plane_detection_extension->connect("updating_plane_detectors", callable_mp(this, &OpenXRExtPlaneDetector::_on_updating_plane_detectors));
			}

			_create_plane_detector();
		} break;
		case NOTIFICATION_EXIT_TREE: {
			_destroy_plane_detector();

			OpenXRExtPlaneDetectionExtension *plane_detection_extension = OpenXRExtPlaneDetectionExtension::get_singleton();
			if (plane_detection_extension) {
				plane_detection_extension->disconnect("updating_plane_detectors", callable_mp(this, &OpenXRExtPlaneDetector::_on_updating_plane_detectors));
			}
		} break;
	}
}

void OpenXRExtPlaneDetector::_on_updating_plane_detectors(XrTime time) {
	if (detector_settings_dirty) {
		print_line("OpenXRExtPlaneDetector::_on_updating_plane_detectors: The plane detector settings are dirty, will re-create plane detector!");
		_destroy_plane_detector();
		_create_plane_detector();
	}

	if (detection_enabled) {
		_update_plane_detector(time);
	} else {
		_destroy_plane_detector();
		XRServer *xr_server = XRServer::get_singleton();
		for (int i = plane_trackers.size() - 1; i >= 0; --i) {
			Ref<OpenXRExtPlaneTracker> plane_tracker = plane_trackers.get(i);
			plane_trackers.remove_at(i);
			if (xr_server) {
				xr_server->remove_tracker(plane_tracker);
			}
		}
	}
}

void OpenXRExtPlaneDetector::_update_plane_detector(XrTime time) {
	if (plane_detector == XR_NULL_HANDLE) {
		print_line("OpenXRExtPlaneDetector::_update_plane_detector: The plane_detector instance is null, cannot update!");
		return;
	}

	OpenXRExtPlaneDetectionExtension *plane_detection_extension = OpenXRExtPlaneDetectionExtension::get_singleton();

	// begin plane detection
	if (!query_running) {
		XrPosef pose{};
		pose.orientation.w = 1.0f;
		XrExtent3DfEXT extents = { 10.0f, 10.0f, 10.0f };
		Vector<OpenXRExtPlaneTracker::PlaneOrientation> orientations = settings.ptr()->get_orientations();
		Vector<OpenXRExtPlaneTracker::PlaneSemanticType> semantic_types = settings.ptr()->get_semantic_types();
		XrPlaneDetectorBeginInfoEXT begin_info = {
			XR_TYPE_PLANE_DETECTOR_BEGIN_INFO_EXT,
			nullptr, // next
			(XrSpace)plane_detection_extension->get_openxr_api()->get_play_space(), // baseSpace
			time,
			(uint32_t)orientations.size(), // orientationCount
			(XrPlaneDetectorOrientationEXT *)orientations.ptrw(), // orientations
			(uint32_t)semantic_types.size(), // semanticTypeCount
			(XrPlaneDetectorSemanticTypeEXT *)semantic_types.ptrw(), // semanticTypes
			settings.ptr()->get_max_planes(), // maxPlanes
			settings.ptr()->get_min_area(), // minArea
			pose, //boundingBoxPose
			extents //boundingBoxExtent
		};

		if (plane_detection_extension->begin_plane_detection(plane_detector, &begin_info)) {
			query_running = true;
		} else {
			print_line("OpenXRExtPlaneDetector::_update_plane_detector: Could not start new plane detection query!");
		}

	}
	// wait for the query to finish
	else {
		status = (OpenXRExtPlaneDetector::Status)plane_detection_extension->get_plane_detector_status(plane_detector);
		switch (status) {
			case PLANE_DETECTION_STATE_DONE:
				// query has finished, process the results below
				break;
			case PLANE_DETECTION_STATE_ERROR:
				// something temporary went wrong, just restart the query
				print_line("OpenXRExtPlaneDetector::_update_plane_detector: The plane detection is in an error state [", plane_detection_extension->get_plane_detection_state_string((XrPlaneDetectionStateEXT)status), "]");
				query_running = false;
				return;
			case PLANE_DETECTION_STATE_FATAL:
				// there was something wrong with the query, do not retry and disable plane detection or just restart the query?
				print_line("OpenXRExtPlaneDetector::_update_plane_detector: The plane detection is in a fatal state [", plane_detection_extension->get_plane_detection_state_string((XrPlaneDetectionStateEXT)status), "]");
				query_running = false;
				return;
			case PLANE_DETECTION_STATE_PENDING:
				// query is still processing, come back in the next loop
				return;
			case PLANE_DETECTION_STATE_NONE:
			case PLANE_DETECTION_STATE_MAX_ENUM:
			default:
				// restart the query
				query_running = false;
				return;
		}

		// process the finished query
		XrPlaneDetectorGetInfoEXT plane_get_info = {
			XR_TYPE_PLANE_DETECTOR_GET_INFO_EXT,
			nullptr,
			(XrSpace)plane_detection_extension->get_openxr_api()->get_play_space(), // baseSpace
			time
		};

		// query the number of detected planes
		XrPlaneDetectorLocationsEXT plane_locations{};
		plane_locations.type = XR_TYPE_PLANE_DETECTOR_LOCATIONS_EXT;
		plane_locations.planeLocationCapacityInput = 0;
		plane_locations.planeLocations = nullptr;

		if (!plane_detection_extension->get_plane_detections(plane_detector, &plane_get_info, &plane_locations)) {
			query_running = false;
			print_line("OpenXRExtPlaneDetector::_update_plane_detector: Could not obtain PLANE DETECTIONS COUNT after plane detection query has finished, returning!");
			return;
		}
		// LuNo: debug/testing
		// else {
		// 	print_line("OpenXRExtPlaneDetector::_update_plane_detector: The PLANE DETECTIONS COUNT is ", plane_locations.planeLocationCountOutput);
		// }

		// obtain the detected planes
		if (plane_locations.planeLocationCountOutput > 0) {
			query_running = false;
			plane_locations.planeLocationCapacityInput = plane_locations.planeLocationCountOutput;
			// initialize array for return values
			LocalVector<XrPlaneDetectorLocationEXT> locations_buffer;
			locations_buffer.resize(plane_locations.planeLocationCountOutput);
			for (XrPlaneDetectorLocationEXT &location : locations_buffer) {
				location = {
					XR_TYPE_PLANE_DETECTOR_LOCATION_EXT, //type
					// nullptr, // next
					// 0, // planeId
					// 0, // locationFlags
					// {}, // pose
					// {}, // extends
					// PLANE_ORIENTATION_MAX_ENUM, // orientation
					// PLANE_SEMANTIC_TYPE_MAX_ENUM, // semanticType
					// 0, // polygonBufferCount
				};
			}
			plane_locations.planeLocations = locations_buffer.ptr();

			if (!plane_detection_extension->get_plane_detections(plane_detector, &plane_get_info, &plane_locations)) {
				query_running = false;
				print_line("OpenXRExtPlaneDetector::_update_plane_detector: Could not obtain PLANE DETECTIONS after plane detection query has finished, returning!");
				return;
			}

			// add new planes and update existing planes, build existing planes helper structure
			std::vector<uint64_t> planeIds;
			planeIds.resize(plane_locations.planeLocationCountOutput);
			for (uint32_t i = 0; i < plane_locations.planeLocationCountOutput; ++i) {
				XrPlaneDetectorLocationEXT detector_location = plane_locations.planeLocations[i];
				planeIds[i] = detector_location.planeId;
				Ref<OpenXRExtPlaneTracker> plane = get_or_create_plane_tracker_by_detector_location(detector_location);
				plane->update_plane(detector_location);
				// iterate through all polygon buffers
				for (uint32_t polygonBufferIndex = 0; polygonBufferIndex < detector_location.polygonBufferCount; polygonBufferIndex++) {
					// polygonBufferIndex = 0 -> outside contour CCW
					// polygonBufferIndex > 0 -> holes CW
					XrPlaneDetectorPolygonBufferEXT polygon_buffer{};
					polygon_buffer.type = XR_TYPE_PLANE_DETECTOR_POLYGON_BUFFER_EXT;
					polygon_buffer.vertexCapacityInput = 0;
					if (!plane_detection_extension->get_plane_polygon_buffer(plane_detector, detector_location.planeId, polygonBufferIndex, &polygon_buffer)) {
						query_running = false;
						print_line("OpenXRExtPlaneDetector::_update_plane_detector: Could not obtain PLANE POLYGON BUFFER SIZE after plane detection query has finished, returning!");
						return;
					}
					// allocate space
					polygon_buffer.vertexCapacityInput = polygon_buffer.vertexCountOutput;
					// initialize array for return values
					LocalVector<XrVector2f> vertex_buffer;
					vertex_buffer.resize(polygon_buffer.vertexCountOutput);
					for (XrVector2f &vertex : vertex_buffer) {
						vertex = XrVector2f();
					}
					polygon_buffer.vertices = vertex_buffer.ptr();
					// use buffer
					if (!plane_detection_extension->get_plane_polygon_buffer(plane_detector, detector_location.planeId, polygonBufferIndex, &polygon_buffer)) {
						query_running = false;
						print_line("OpenXRExtPlaneDetector::_update_plane_detector: Could not obtain PLANE POLYGON BUFFER VERTICES after plane detection query has finished, returning!");
						return;
					}
					// process vertices
					plane->update_plane_polygon(polygonBufferIndex, polygon_buffer);
				}
			}

			// LuNo: debug/testing
			// print_line("OpenXRExtPlaneDetector::_update_plane_detector: The currently active plane ids are:");
			// for (int i=0; i < planeIds.size(); i++) {
			// 	print_line("OpenXRExtPlaneDetector::_update_plane_detector: ", planeIds[i]);
			// }

			// remove obsolete planes
			XRServer *xr_server = XRServer::get_singleton();
			for (int i = plane_trackers.size() - 1; i >= 0; --i) {
				Ref<OpenXRExtPlaneTracker> plane_tracker = plane_trackers.get(i);
				// LuNo: debug/testing
				//print_line("OpenXRExtPlaneDetector::_update_plane_detector: Checking obsolence for plane id ", plane_tracker->get_plane_id());
				bool still_exists = std::find(planeIds.begin(), planeIds.end(), plane_tracker->get_plane_id()) != planeIds.end();
				if (!still_exists) {
					// LuNo: test order of commands?
					// LuNo: debug/testing
					print_line("OpenXRExtPlaneDetector::_update_plane_detector: Removing obsolete plane tracker for plane id ", plane_tracker->get_plane_id());
					plane_trackers.remove_at(i);
					// LuNo: debug/testing
					print_line("OpenXRExtPlaneDetector::_update_plane_detector: Removed from the plane_trackers!");
					if (xr_server) {
						xr_server->remove_tracker(plane_tracker);
						// LuNo: debug/testing
						print_line("OpenXRExtPlaneDetector::_update_plane_detector: Removed from XR server!");
					}
				}
				// LuNo: debug/testing
				else {
					//print_line("OpenXRExtPlaneDetector::_update_plane_detector: Still valid plane id ", plane_tracker->get_plane_id());
				}
			}
		} else {
			print_line("OpenXRExtPlaneDetector::_update_plane_detector: No planes have been detected by the plane detection query!");
		}
	}
}

Ref<OpenXRExtPlaneTracker> OpenXRExtPlaneDetector::get_or_create_plane_tracker_by_detector_location(XrPlaneDetectorLocationEXT p_detector_location) {
	for (int i = 0; i < plane_trackers.size(); i++) {
		Ref<OpenXRExtPlaneTracker> plane = plane_trackers.get(i);
		if (plane.ptr()->get_plane_id() == p_detector_location.planeId) {
			return plane;
		}
	}
	OpenXRExtPlaneTracker *plane_tracker = memnew(OpenXRExtPlaneTracker(p_detector_location));
	plane_tracker->set_tracker_name("/ml_plane/" + String::num_int64(plane_tracker->get_plane_id()));
	plane_trackers.push_back(plane_tracker);
	XRServer *xr_server = XRServer::get_singleton();
	if (xr_server) {
		XRServer::get_singleton()->add_tracker(plane_tracker);
		// LuNo: debug/testing
		print_line("OpenXRExtPlaneDetector::get_or_create_plane_tracker_by_detector_location: Added tracker to XR server for plane id ", plane_tracker->get_plane_id());
	}
	// LuNo: debug/testing
	print_line("OpenXRExtPlaneDetector::get_or_create_plane_tracker_by_detector_location: Created new plane tracker for plane id ", plane_tracker->get_plane_id());
	return plane_tracker;
}

void OpenXRExtPlaneDetector::_on_settings_changed() {
	_destroy_plane_detector();
	detector_settings_dirty = true;
}

void OpenXRExtPlaneDetector::_destroy_plane_detector() {
	if (plane_detector == XR_NULL_HANDLE) {
		print_line("OpenXRExtPlaneDetector::_destroy_plane_detector: There is no plane detector to be destroyed, returning!");
		return;
	}
	// LuNo: debug/testing
	else {
		print_line("OpenXRExtPlaneDetector::_destroy_plane_detector: Will call destroy plane detector...");
	}

	if (settings.is_valid()) {
		settings->disconnect("changed", callable_mp(this, &OpenXRExtPlaneDetector::_on_settings_changed));
	}

	OpenXRExtPlaneDetectionExtension *plane_detection_extension = OpenXRExtPlaneDetectionExtension::get_singleton();
	if (plane_detection_extension) {
		plane_detection_extension->destroy_plane_detector(plane_detector);
		// LuNo: debug/testing
		print_line("OpenXRExtPlaneDetector::_destroy_plane_detector: ... called destroy plane detector!");
	}
	XRServer *xr_server = XRServer::get_singleton();
	if (xr_server) {
		for (int i = 0; i < plane_trackers.size(); i++) {
			Ref<OpenXRExtPlaneTracker> plane = plane_trackers.get(i);
			xr_server->remove_tracker(plane);
		}
	}
	plane_trackers.clear();
	plane_detector = XR_NULL_HANDLE;
}

void OpenXRExtPlaneDetector::_create_plane_detector() {
	detector_settings_dirty = false;
	if (!settings.is_valid()) {
		print_line("OpenXRExtPlaneDetector::_create_plane_detector: The plane detector settings are a nullptr, returning!");
		return;
	}

	OpenXRExtPlaneDetectionExtension *plane_detection_extension = OpenXRExtPlaneDetectionExtension::get_singleton();
	if (!plane_detection_extension) {
		print_line("OpenXRExtPlaneDetector::_create_plane_detector: The plane detection extension does not exist, returning!");
		return;
	}

	if (!plane_detection_extension->is_plane_detection_supported()) {
		print_line("OpenXRExtPlaneDetector::_create_plane_detector: The plane detection extension is not supported, returning!");
		return;
	}

	XrPlaneDetectorCreateInfoEXT create_info = {
		XR_TYPE_PLANE_DETECTOR_CREATE_INFO_EXT,
		nullptr
	};
	if (contour_bit) {
		create_info.flags = XR_PLANE_DETECTOR_ENABLE_CONTOUR_BIT_EXT;
	}

	plane_detector = plane_detection_extension->create_plane_detector(&create_info);
	if (plane_detector != XR_NULL_HANDLE) {
		settings->connect("changed", callable_mp(this, &OpenXRExtPlaneDetector::_on_settings_changed));
	}
}
