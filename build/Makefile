#!/bin/make
CXX = g++
CXXFLAGS = -std=c++20
CXXFLAGS += $(shell pkg-config --cflags opencv4 gtkmm-3.0)
LDFLAGS += $(shell pkg-config --libs opencv4 gtkmm-3.0)

EXEC = main

SOURCES = main.cpp display.cpp algorithms.cpp display_helper.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	-rm -f $(OBJECTS) $(EXEC)

#eof

# g++ -std=c++20 main.cpp -o main `pkgs opencv4 gtkmm-3.0` -o app