extends Node3D
class_name PlaneTrackerDisplay

@onready var description: Label3D = $Description
@onready var planeMesh: ImmediateMesh = $Plane.mesh
var tracker_entity: OpenXRExtPlaneTracker


func setup_scene(p_tracker: OpenXRExtPlaneTracker):
	tracker_entity = p_tracker

func _process(deltaTime):
	# update the info label
	var desc = ""
	desc += "ID: " + str(tracker_entity.get_plane_id()) + "\n"
	desc += "Width: " + str(tracker_entity.get_plane_width()) + " m\n"
	desc += "Height: " + str(tracker_entity.get_plane_height()) + " m\n"
	desc += "Orientation: " + str(tracker_entity.get_plane_orientation()) + "\n"
	desc += "Semantic Type: " + str(tracker_entity.get_plane_semantic_type()) + "\n"
	desc += "Tracking Confidence: " + str(tracker_entity.get_pose("default").tracking_confidence)
	description.text = desc
	
	# update the geometry
	# draw a quad based on the four corners
	var h_2 = tracker_entity.get_plane_height() / 2.0
	var w_2 = tracker_entity.get_plane_width() / 2.0
	var nor = Vector3.BACK
	
	# create materials
	#var col = Color(randf(),randf(),randf(),1.0)
	var mat_norm = StandardMaterial3D.new()
	var mat_rect = StandardMaterial3D.new()
	var mat_poly = StandardMaterial3D.new()
	mat_norm.albedo_color = Color(0.686, 0.314, 0.816, 1.0)
	mat_rect.albedo_color = Color(0.961, 0.525, 0.075, 1.0)
	mat_poly.albedo_color = Color(0.008, 0.902, 0.086, 1.0)
	
	planeMesh.clear_surfaces()
	# draw plane normal
	planeMesh.surface_begin(Mesh.PRIMITIVE_LINES, mat_norm)
	planeMesh.surface_add_vertex(Vector3(0,0,0))
	planeMesh.surface_add_vertex(nor*0.25)
	planeMesh.surface_end()
	# draw plane
	planeMesh.surface_begin(Mesh.PRIMITIVE_LINE_STRIP, mat_rect)
	planeMesh.surface_set_normal(nor)
	planeMesh.surface_add_vertex(Vector3(w_2, h_2, 0.0))
	planeMesh.surface_add_vertex(Vector3(w_2, -h_2, 0.0))
	planeMesh.surface_add_vertex(Vector3(-w_2, -h_2, 0.0))
	planeMesh.surface_add_vertex(Vector3(-w_2, h_2, 0.0))
	planeMesh.surface_add_vertex(Vector3(w_2, h_2, 0.0))
	planeMesh.surface_end()
	
	# loop through all polygon buffers and add a surface for each buffer
	var polygon_buffers = tracker_entity.get_plane_polygon_buffers()
	for i in tracker_entity.get_plane_polygon_buffer_count():
		var polygon_buffer = polygon_buffers[i]
		planeMesh.surface_begin(Mesh.PRIMITIVE_LINE_STRIP, mat_poly)
		for j in polygon_buffer.size():
			var vert = polygon_buffer[j]
			planeMesh.surface_add_vertex(Vector3(vert.x, vert.y, 0))
		# close the line strip
		if polygon_buffer.size() > 0:
			planeMesh.surface_add_vertex(Vector3(polygon_buffer[0].x, polygon_buffer[0].y, 0))
		planeMesh.surface_end()
