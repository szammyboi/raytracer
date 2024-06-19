# Building
The project uses premake5 as the build system manager. I have already generated a Makefile that can be used to build the project on Windows. Before building, you need to set the compiler by using "set CC=gcc" in your command line (with gcc being whatever compiler you use). If you use GCC, then you can just run the included run.bat file which will build the project and then run the executable. The project is built to build\bin\release. 

# Dependencies
I used Glad instead of Glew in this project but all of the dependencies are built into the project. It should compile GLFW, Glad, and ImGui before finally building and linking the application. 

# Interface
Upon opening the app, you will be greeted with a blank screen and no pre-built scene. If you click on the settings button in the top left corner, it will open up the menu with the ability to move the camera around, add objects, edit their materials, and create lights. The scene will be rendered live at a lower resolution. If you uncheck the "live editor" option it will render the scene at a very high resolution (warning because it may have a very high CPU usage). The render loop of this app is split across all the threads on the computer so that I could render the movies quickly but it is very intensive. 

A demo video of the UI can be found here: https://www.youtube.com/watch?v=aSaYAw-S-_8

# Switching the Camera
One of the requirements for this project was being able to switch between perspective and orthographic cameras. This ability is also inside the UI on the left. If you open the Camera dropdown, there will be another dropdown for changing the camera type as well as its position. Note: you may need to readjust the size of the objects in the scene to get them visible in the orthographic camera. 

# Animating
If you check off the Animate checkbox, it will start rotating the Camera around the scene. This will use your current z position as the orbital distance but it may change the framing of the objects slightly. 
