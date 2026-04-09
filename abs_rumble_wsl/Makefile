
# Directories
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -I$(INC_DIR)

# Target executable
TARGET = client

# Find all .cpp files in the source directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Map .cpp files to .o files in the build directory
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default rule
all: $(BUILD_DIR)/$(TARGET)

# Rule to link the final executable
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Mark rules that don't represent physical files
.PHONY: all clean