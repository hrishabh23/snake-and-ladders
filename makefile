CXX      := g++
CXXFLAGS := -Wall -O3
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := build
PACKAGE := `pkg-config --cflags --libs opencv`
TARGET   := SnakeAndLadders
SRC      :=\

$(TARGET):
	@mkdir -p $(@D)
	$(CXX) game.cpp $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(PACKAGE)

.PHONY: all build clean

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf build/*
