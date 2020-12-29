main:		sample.cpp
		#make glew
		make jetson
install:
		sudo apt-get install build-essential libgl1-mesa-dev
		sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev
		sudo apt install mesa-utils
		sudo apt-get install libopenal1 libopenal-dev
		sudo apt-get install libalut-dev
		sudo apt-get install libglfw3 libglfw3-dev
		glxinfo | grep OpenGL
		pkg-config --libs --static glew
glew:
		g++ -o out -g sample.cpp glslprogram.cpp loader.cpp SimpleAI.cpp bmptotexture.cpp -DGLEW_STATIC -lGL  -lGLU /usr/lib/libglut.so -lm -lGLEW -lglfw -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp -fpermissive -lalut -lopenal -w
		./out 1
jetson:
		#/usr/lib/aarch64-linux-gnu/
		g++ -o out -g sample.cpp glslprogram.cpp loader.cpp SimpleAI.cpp bmptotexture.cpp -DGLEW_STATIC -lGL  -lGLU /usr/lib/aarch64-linux-gnu/libglut.so -lm -lGLEW -lglfw -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp -fpermissive -lalut -lopenal -w
		./out 1
