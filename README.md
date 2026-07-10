UBISOFT OpenGL project
A lightweight real-time rendering framework and scene editor built with Modern OpenGL.

Features:
	- mesh loading
		load meshes with textures from files (obj, stl) dynamically. gITF - as bonus
		- dynamic adding/removing meshes from scene
		- load mesh from file
	- dynamic scene
		- move objects across the scene
		- scale and rotate objects on the scene
		- move camera across the scene
	- lights
		- types: directoinal, point, spot, ambient
		- all lights can be edited dynamically
			- intensity
			- type
			- color
		- user can add light object to the scene (behaves like any object, but produces light of selected type)
	- camera
        - behavior (movement type)
            - orbit camera
            - free camera
        - projection
            - orthographic camera
            - perspective
	- render statistics
		- FPS
		- frame time
		- cpu time
		- gpu time (possibly)
		- total vertices count
		- total triangles count
		- objects hierarcy tree (imgui control)
	- shaders (possibly)
		hot shader reload. Select a shader file during runtime and use it (compile and run in app). I think, this can be used for each mesh: you load mesh with generic shader, but can select a specific one
	- shadow mapping
	- using imgui as a wrap for a project and as lib for generic controllers

