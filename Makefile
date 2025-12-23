# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

# Include Directories
INCLUDES = -Iinclude/core -Iinclude/factory

# Source Files
SRC = src/main.cpp \
      src/core/Address.cpp \
      src/core/Appliance.cpp \
      src/core/Employee.cpp \
      src/core/Fridge.cpp \
      src/core/RepairRequest.cpp \
      src/core/TV.cpp \
      src/core/WashingMachine.cpp \
      src/factory/AddressFactory.cpp \
      src/factory/ApplianceFactory.cpp \
      src/factory/RepairRequestFactory.cpp

# Object Files
OBJ = $(SRC:.cpp=.o)

# Output Executable
TARGET = app

# Default Rule
all: $(TARGET)

# Link Object Files to create Executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean Build Files
clean:
	rm -f $(OBJ) $(TARGET)

# Run the application
run: all
	./$(TARGET)

.PHONY: all clean run