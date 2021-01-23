MAKEFILE_PATH=/dfact/common/makefiles


EXE             =       tfv


USER_LIBRARIES  =

USE_LIBCURSES=YES

CPP_SRCS        =   App.cpp                     \
                    bindable_functions.cpp      \
                    common.cpp                  \
                    Console.cpp                 \
                    Display.cpp                 \
                    Keyboard.cpp                \
                    Finder.cpp                  \
                    tfv.cpp                     \

HEADERS         =   App.hpp                     \
                    bindable_functions.hpp      \
                    common.hpp                  \
                    Console.hpp                 \
                    Display.hpp                 \
                    Keyboard.hpp                \
                    Finder.hpp                  \


EXTRA_CFLAGS=-std=c++11
EXTRA_DBG_CFLAGS=-std=c++11

OBJECTS = $(SOURCES:%.cpp=%.o)

TARGET = myExecutable




$(EXE): $(OBJECTS)
    g++ $^ -o $@

%.o: %.cpp
    g++ -c $< -o $@

