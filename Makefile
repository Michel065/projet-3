CXX      = x86_64-w64-mingw32-g++
CXXFLAGS = -Wall -std=c++17 -MMD -MP

SRC_DIR   := .
BUILD_DIR := build

# Tous les .cpp du dossier courant
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# build/Fichier.o pour chaque .cpp
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

DEPS := $(OBJS:.o=.d)

# cible par défaut
all: main

main: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Création de build/ si besoin (version Windows)
$(BUILD_DIR):
	if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

# Règle de compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) main.exe main

run: main
	./main

r: run

-include $(DEPS)
