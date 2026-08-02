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
		- type
			- directional
			- point
			- spot
		- diplay data in imgui
			- position
			- color
			- components: ambient, diffuse, specular
				- attenuation: constant, linear, quadrantic
				- spot light: direction, cut_off angle
		- edited dynamically from imgui
			- position
			- intensity
			- color
			- components (for each light type separately)
		- passing and processing arbitrary amount of lights of differenty types to shaders
		- user can add light object to the scene (behaves like any object, but produces light of selected type)
			the light renderer as a sphere of light's color
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
		- cpu time DONE
		- gpu time (possibly) DONE
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
		- faces' normals (short red lines)