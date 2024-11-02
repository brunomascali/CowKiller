OBJ_DIR := ./obj
SRC_DIR := ./src
INCLUDE_DIRS := -I./include -I./include/GLFW/ -I./include/glad
LIB_DIR := -L./lib/GLFW
CXXFLAGS := -std=c++20 -g

EXECUTABLE := main.exe
LIBS := -lglfw3 -lopengl32 -lgdi32

SRC_FILES := $(wildcard $(SRC_DIR)/*.c*)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(OBJ_FILES))

all: $(EXECUTABLE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	g++ -c $(CXXFLAGS) $(INCLUDE_DIRS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	g++ -c $(CXXFLAGS) $(INCLUDE_DIRS) $< -o $@

$(EXECUTABLE): $(OBJ_FILES)
	g++ $(CXXFLAGS) $(LIB_DIR) $(OBJ_FILES) $(LIBS) -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

.PHONY: all clean