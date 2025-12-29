# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

# Directories
SRC_DIR = src
BUILD_DIR = build
INC_DIR = include

# Automatically find all subdirectories in include/ to use for -I flags
# This allows VS Code to find headers in core/, factory/, and manager/
INCLUDE_DIRS = $(shell find $(INC_DIR) -type d | sed 's/^/-I/')

# Automatically find all .cpp files in src/ and its subdirectories
# This ensures EmployeeFactory.cpp and others are included in the project
SRC = $(shell find $(SRC_DIR) -name "*.cpp")

# Object Files mapping: transforms 'src/path/file.cpp' to 'build/path/file.o'
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

# Output Executable
TARGET = app

# Default Rule
all: $(TARGET)

# Link Object Files to create the Executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

# Compile .cpp files to .o files in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Clean Build Files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run the application
run: all
	./$(TARGET)

.PHONY: all clean run