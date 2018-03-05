main:		sample.cpp
		make glew
install:
		sudo apt-get install build-essential libgl1-mesa-dev
		sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev
		sudo apt install mesa-utils
		glxinfo | grep OpenGL
		pkg-config --libs --static glew
glew:
		g++ -o out  sample.cpp glslprogram.cpp loader.cpp SimpleAI.cpp bmptotexture.cpp -DGLEW_STATIC -lGL  -lGLU /usr/lib/libglut.so  -lm -lGLEW -lglfw -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp -fpermissive -lalut -lopenal -w
		./out 1
