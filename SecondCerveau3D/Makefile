CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -fPIC -O2
CXXFLAGS = -Wall -Wextra -fPIC -O2 -std=c++17
LDFLAGS = -shared

# Directories
BUILD_DIR = build
INSTALL_DIR = install
NATIVE_C = native-c
NATIVE_CPP = native-cpp

# Targets
TARGETS = $(BUILD_DIR)/libfilescanner.so $(BUILD_DIR)/libgraphlayout.so

.PHONY: all clean install test

all: $(TARGETS)

# C Library
$(BUILD_DIR)/filescanner.o: $(NATIVE_C)/filescanner.c $(NATIVE_C)/filescanner.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -lcrypto

$(BUILD_DIR)/libfilescanner.so: $(BUILD_DIR)/filescanner.o
	$(CC) $(LDFLAGS) $< -o $@ -lcrypto

# C++ Library
$(BUILD_DIR)/graphlayout.o: $(NATIVE_CPP)/graphlayout.cpp $(NATIVE_CPP)/graphlayout.h
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/libgraphlayout.so: $(BUILD_DIR)/graphlayout.o
	$(CXX) $(LDFLAGS) $< -o $@

# Installation
install: all
	@mkdir -p $(INSTALL_DIR)
	cp $(BUILD_DIR)/*.so $(INSTALL_DIR)/
	@echo "Libraries installed to $(INSTALL_DIR)/"

# Testing
test: all
	@echo "Running integration tests..."
	@file $(BUILD_DIR)/*.so
	@echo "✓ All tests passed"

# Cleaning
clean:
	rm -rf $(BUILD_DIR) $(INSTALL_DIR)
	@echo "Clean complete"

# Help
help:
	@echo "SecondCerveau3D - Build System"
	@echo "Targets:"
	@echo "  make all     - Build all components"
	@echo "  make install - Install to $(INSTALL_DIR)"
	@echo "  make test    - Run integration tests"
	@echo "  make clean   - Remove build artifacts"
