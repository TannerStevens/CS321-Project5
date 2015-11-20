NAME: Tanner Stevens
PROJECT: Project5
COURSE: CS321 - Intro to Comp Graphics
INSTRUCTOR: Beomjin Kim

**NOTE FOR INSTRUCTOR:
	I urge you to play with the OBJ Group selection via Mouse Click, the Mouse and OBJ Group Collision Detection is what 
	I poured most of my time into on this project. Clicking on some part of an Object should select that Rendering Group
	and then hitting Insert will allow you to modify the Rendering Groups Material Coeffiencts in real time in the CMD Prompt.

	Side Note- The Collision Detection currently doesn't have support for 'Hidden Surface Removal' so it is very likely that when you attempt to click 
	on something that an OBJ Group behind it will be selected instead. This would be relatively simply fixed but my current fix involves creating my
	own std::list iterator that sorts the OBJ Groups by distance, or something along the lines of that. Which given the quickly approaching deadline
	for the Final Project I believe I should focus my attention on to the Final Project.
**

DESCRIPTION:
	- Prompts User for the Name of an OBJ File
	- Loads in that OBJ File
	- Renders the OBJ File
		* Supported Tags:
			- v, vn, k<adse>, se, o, g
	- Allows the User to apply Geometric Transformations to the OBJ Model
	- Makes use of 2 Lights (GL_LIGHT0 & GL_LIGHT1)
		* Refer to CONTROLS for activating them
	- Allows the User to click on OBJ Groups and modify their Material Coefficients in Real Time via input through the CMD Prompt
		* The User may also enable 'debugging' mode which visualizes the Click Group Collision Detection process
		* Refer to CONTROLS

CONTROLS:
	Lights:
		1: Turn on the directional light.
		2: Turn on the positional light.
		0: Turn off the directional and positional light.

		A: Increase the ambient light of both the Directional and Positional Lights.
		a: Decrease the ambient light of both the Directional and Positional Lights.
	
		D: Increase the diffuse light of both the Directional and Positional Lights.
		d: Decrease the diffuse light of both the Directional and Positional Lights.

		S: Increase the specular component of the positional light.
		s: Decrease the specular component of the positional light.

		+: Increase the cutoff angle of the positional light.
		-: Decrease the cutoff angle of the positional light.

	Material:
		H: Increase the shininess of the object.
		h: Decrease the shininess of the object.

		E: Increase the emissive light of the object.
		e: Decrease the emissive light of the object.

		Insert:
			* If NO OBJ Group is selected then this does nothing
			* If an OBJ Group is Selected
				- The CMD Prompt will Prompt the User for a Command
					* Commands: ka, kd, ks, ke, se, or c
				- Based on the Command it will either ask for the RGB or SE Value(s), or terminate the prompt

	Geometric Transformations (Lights & Object):
		P, p: Rotate the positional light about the Y-axis.

		8, O, o: Rotate the object around the Y-axis by +15 Degrees.
		5: Rotate the object around the Y-axis by -15 Degrees.

		4: Rotate the object around the X-axis by +15 Degrees.
		6: Rotate the object around the X-axis by -15 Degrees.

		7: Rotate the object around the Z-axis by +15 Degrees.
		9: Rotate the object around the Z-axis by -15 Degrees.

		*: Scale the Object Up by 10%
		/: Scale the Object Down by 10%

	OBJ Group Selection:
		Left Mouse Button: Selects an OBJ Group located under your mouse cursor

		`: Enable Mouse Click Collision Detection Debugging Visualization

	Q,q: Exit Program
