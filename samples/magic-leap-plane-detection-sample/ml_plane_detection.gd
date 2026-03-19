extends OpenXRMlPlaneDetector

@export var plane_display_scene: PackedScene = null
var plane_displays: Dictionary[OpenXRMlPlaneTracker, Node] = {}


func _ready() -> void:
	print("ml_plane_detection::_ready: OpenXRMlPlaneDetectionExtension.is_plane_detection_supported: ", OpenXRMlPlaneDetectionExtension.is_plane_detection_supported())


func _process(delta: float):
	update_plane_displays()


func update_plane_displays():
	var plane_trackers = get_plane_trackers()
	if plane_display_scene != null:
		# LuNo: debug/testing
		print("ml_plane_detection::update_plane_displays: Instantiating new scene based on scene template!")
		for plane_tracker in plane_trackers:
			if not plane_displays.has(plane_tracker):
				var display = plane_display_scene.instantiate()
				display.setup_scene(plane_tracker)
				add_child(display)
				plane_displays[plane_tracker] = display
	# LuNo: debug/testing
	#else:
		#print("ml_plane_detection::update_plane_displays: There is no plane display scene template defined, cannot instantiate new scene!")

	# Free PlaneDisplays for markers that no longer exist
	for key in plane_displays.keys():
		if key not in plane_trackers:
			plane_displays[key].queue_free()
			plane_displays.erase(key)
