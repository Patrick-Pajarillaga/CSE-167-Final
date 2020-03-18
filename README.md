# CSE-167-Final
1. Download 
  Clouds: Cube.cpp, Cube.h
    Shader: skybox.frag, skybox.vert
    
  Rain Particles: ParticleController.cpp, ParticleController.h
    Shader: shader.frag, shader.vert
 
2. TO INTEGERATE
   - Look at Example Window.cpp for reference
   - Add camRight, camDir, and camUp vec3 objects (look at Window.cpp)
   - Load Shaders for each object
     One for skybox with skybox shaders
   - Instantiate Cube object and Particle Controller object
      Put a random float in Cube Constructor
      EX: cube = new Cube(1.0f);
   - Deallocate objects into cleanup()
      EX: delete cube;
   - In idle callback, call both update functions for each object
     Add a counter and put an if statement for the sky box so that it runs at a slower rate
     EX: if (counter % 100 == 0) {
		        skybox->update();
	       }
         // Make it update every 100 callbacks
   - In Display Callback make sure to call transforms (look at Window.cpp for which uniforms are in each shader)
   - Call draw functions for each object.
   
