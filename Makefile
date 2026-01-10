CXX = g++
CXXFLAGS = -std=c++17 -g -Wall \
           -I. \
           -Isrc/app/encryptDecrypt \
           -Isrc/app/fileHandling \
           -Isrc/app/processes

# Directories
OBJ_DIR = obj
BIN_DIR = bin

# Targets
MAIN_TARGET = $(BIN_DIR)/encrypt_decrypt
CRYPTION_TARGET = $(BIN_DIR)/testCryption

# Source files
MAIN_SRC = main.cpp \
           src/app/processes/ProcessManagement.cpp \
           src/app/fileHandling/IO.cpp \
           src/app/fileHandling/ReadEnv.cpp \
           src/app/encryptDecrypt/Cryption.cpp

CRYPTION_SRC = src/app/encryptDecrypt/testCryption.cpp \
               src/app/encryptDecrypt/Cryption.cpp \
               src/app/fileHandling/IO.cpp \
               src/app/fileHandling/ReadEnv.cpp

# Object files (mirror directory structure inside obj/)
MAIN_OBJ = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRC))
CRYPTION_OBJ = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(CRYPTION_SRC))

# Default target
all: $(MAIN_TARGET) $(CRYPTION_TARGET)

# Create binaries
$(MAIN_TARGET): $(MAIN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CRYPTION_TARGET): $(CRYPTION_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile .cpp → obj/.o
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean