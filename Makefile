CXX      = x86_64-w64-mingw32-g++
CXXFLAGS = -Wall -std=c++17 -MMD -MP -Iinclude

SRC_DIR   := src
BUILD_DIR := build

# Tous les .cpp dans src/ ET dans src/*/
SRCS := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp)

# build/xxx.o pour chaque src/xxx.cpp (en gardant les sous-dossiers)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

DEPS := $(OBJS:.o=.d)

all: main

main: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
	if exist main.exe del /Q main.exe
	cls

c: clean

run: main
	./main

r: run

-include $(DEPS)
