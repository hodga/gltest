VPATH = renderEngine:renderEngine/textureLib
LIBPATH = -L"/System/Library/Frameworks/OpenGL.framework/Libraries"


FRAMEWORK = -framework OpenGL 
FRAMEWORK += -framework Cocoa 
FRAMEWORK += -framework IOKit
FRAMEWORK += -framework CoreVideo

COMPILERFLAGS = -Wall -std=c++11 -stdlib=libc++
CFLAGS = $(COMPILERFLAGS)
LIBRARIES = -lGLEW -lGL -lglfw3 -lm -lobjc -lstdc++

OBJECTS = main.o renderEngine.o testObject.o instance.o shaderBase.o Texture.o texturePacker.o renderBatch.o lodepng.o
All: OpenGLTest


OpenGLTest: $(OBJECTS)
	$(CC) $(FRAMEWORK) $(CFLAGS) -o $@ $(LIBPATH) $(LIBRARIES) $(OBJECTS)

main.o : main.cpp renderEngine.h testObject.h

renderEngine.o : renderEngine.cpp renderTrait.h renderBatch.h instance.h texturePacker.h

testObject.o : testObject.cpp renderTrait.h

instance.o : instance.cpp

shaderBase.o : shaderBase.cpp

texturePacker.o : texturePacker.cpp lodepng.h
	$(CC) $(CFLAGS) -c renderEngine/textureLib/texturePacker.cpp

Texture.o : Texture.cpp lodepng.h

renderBatch.o : renderBatch.cpp shaderBase.h Texture.h instance.h

lodepng.o : lodepng.cpp

.PHONY : clean
clean :
	rm *.o