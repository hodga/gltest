LIBPATH = -L"/System/Library/Frameworks/OpenGL.framework/Libraries"


FRAMEWORK = -framework OpenGL 
FRAMEWORK += -framework Cocoa 
FRAMEWORK += -framework IOKit
FRAMEWORK += -framework CoreVideo

COMPILERFLAGS = -Wall -Wc++11-extensions
CC = g++
CFLAGS = $(COMPILERFLAGS)
LIBRARIES = -lGLEW -lGL -lglfw3 -lm -lobjc -lstdc++


OBJECTS = main.o instance.o asset.o shaderBase.o Texture.o lodepng.o
All: OpenGLTest


OpenGLTest: $(OBJECTS)
	$(CC) $(FRAMEWORK) $(CFLAGS) -o $@ $(LIBPATH) $(LIBRARIES) $(OBJECTS)

main.o : main.cpp instance.h asset.h

instance.o : instance.cpp asset.h

asset.o : asset.cpp shaderBase.h 

shaderBase.o : shaderBase.cpp

Texture.o : Texture.cpp lodepng.h

lodepng.o : lodepng.cpp
