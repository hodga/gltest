cppsrc = $(wildcard *.cpp) \
	$(wildcard renderEngine/*.cpp) \
	$(wildcard gameObjects/*.cpp) \
	$(wildcard renderEngine/textureLib/*.cpp) \
	$(wildcard gameEngine/*.cpp)

OBJECTS = $(cppsrc:.cpp=.o)

LIBPATH = -L"/System/Library/Frameworks/OpenGL.framework/Libraries"

FRAMEWORK = -framework OpenGL 
FRAMEWORK += -framework Cocoa 
FRAMEWORK += -framework IOKit
FRAMEWORK += -framework CoreVideo

CXX=clang++
COMPILERFLAGS = -Wall -std=c++11 
CXXFLAGS = $(COMPILERFLAGS)
LIBRARIES = -lGLEW -lGL -lglfw3 -lm -lstdc++


OpenGLTest: $(OBJECTS)
	$(CXX) $(FRAMEWORK) -o $@ $^ $(LIBPATH) $(LIBRARIES)

.PHONY: clean
clean:
	rm -f $(OBJECTS) OpenGLTest