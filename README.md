UBISOFT OpenGL project
A lightweight real-time rendering framework and scene editor built with Modern OpenGL.

Features:
	- mesh loading DONE
		load meshes with textures from files (obj, stl) dynamically. gITF - as bonus DONE
		- dynamic adding/removing meshes from scene DONE
		- load mesh from file DONE
		- load mesh textures DONE
		- load embedded textures DONE
	- dynamic scene DONE
		- move objects across the scene DONE
		- scale and rotate objects on the scene DONE
		- move camera across the scene DONE
	- lights
		- types: directoinal, point, spot, ambient
		- all lights can be edited dynamically
			- intensity
			- type
			- color
		- user can add light object to the scene (behaves like any object, but produces light of selected type)
	- camera DONE
        - behavior (movement type)
            - orbit camera DONE
            - free camera DONE
        - projection
            - orthographic DONE
            - perspective DONE
	- render statistics
		- FPS DONE
		- frame time DONE
		- cpu time
		- gpu time (possibly)
		- total vertices count DONE
		- total triangles count DONE
		- objects hierarcy tree (imgui control) DONE
	- shaders (possibly) DONE
		hot shader reload. Select a shader file during runtime and use it (compile and run in app). I think, this can be used for each mesh: you load mesh with generic shader, but can select a specific one
	- shadow mapping
	- using imgui as a wrap for a project and as lib for generic controllers DONE

	- switch on/off from ui:
		- main axes (rgb - xyz)
		- xz face in wireframe style (net)