# ============================================================================
# EverEngine Makefile
# ============================================================================

# Directories
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin

# Build type (Release or Debug)
BUILD_TYPE ?= Release

# Number of parallel jobs
JOBS ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Colors for output
COLOR_RESET = \033[0m
COLOR_BOLD = \033[1m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_BLUE = \033[34m

# ============================================================================
# Main Targets
# ============================================================================

.PHONY: all
all: build

# Build project (Release by default)
.PHONY: build
build:
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)Building EverEngine ($(BUILD_TYPE))...$(COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..
	@cmake --build $(BUILD_DIR) --config $(BUILD_TYPE) -j$(JOBS) --verbose
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)✓ Build complete!$(COLOR_RESET)"

# Rebuild (clean + build)
.PHONY: rebuild
rebuild: clean build
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)✓ Rebuild complete!$(COLOR_RESET)"

# Run the application
.PHONY: run
run: build
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)Running EverEngine...$(COLOR_RESET)"
	@$(BIN_DIR)/main

# Debug build
.PHONY: debug
debug:
	@echo "$(COLOR_BOLD)$(COLOR_YELLOW)Building EverEngine (Debug)...$(COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug ..
	@cmake --build $(BUILD_DIR) --config Debug -j$(JOBS) --verbose
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)✓ Debug build complete!$(COLOR_RESET)"

# Run debug version
.PHONY: run-debug
run-debug: debug
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)Running EverEngine (Debug)...$(COLOR_RESET)"
	@$(BIN_DIR)/main

# Run with GDB debugger
.PHONY: gdb
gdb: debug
	@echo "$(COLOR_BOLD)$(COLOR_YELLOW)Starting GDB...$(COLOR_RESET)"
	@gdb $(BIN_DIR)/main

# Run with Valgrind (memory leak detection)
.PHONY: valgrind
valgrind: debug
	@echo "$(COLOR_BOLD)$(COLOR_YELLOW)Running Valgrind...$(COLOR_RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BIN_DIR)/main

# ============================================================================
# Clean Targets
# ============================================================================

# Clean build files
.PHONY: clean
clean:
	@echo "$(COLOR_BOLD)$(COLOR_YELLOW)Cleaning build files...$(COLOR_RESET)"
	@rm -rf $(BUILD_DIR)
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)✓ Clean complete!$(COLOR_RESET)"

# Clean all (including CMake cache)
.PHONY: clean-all
clean-all: clean
	@echo "$(COLOR_BOLD)$(COLOR_YELLOW)Cleaning all generated files...$(COLOR_RESET)"
	@find . -name "CMakeCache.txt" -delete
	@find . -name "CMakeFiles" -type d -exec rm -rf {} + 2>/dev/null || true
	@find . -name "cmake_install.cmake" -delete
	@find . -name "*.a" -delete
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)✓ Deep clean complete!$(COLOR_RESET)"

# ============================================================================
# Utility Targets
# ============================================================================

# Configure only (don't build)
.PHONY: configure
configure:
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)Configuring CMake...$(COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)✓ Configuration complete!$(COLOR_RESET)"

# Show configuration info
.PHONY: info
info:
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)=== EverEngine Build Info ===$(COLOR_RESET)"
	@echo "Build Directory: $(BUILD_DIR)"
	@echo "Binary Directory: $(BIN_DIR)"
	@echo "Build Type: $(BUILD_TYPE)"
	@echo "Parallel Jobs: $(JOBS)"
	@echo "CMake Version: $$(cmake --version | head -n1)"
	@echo "Compiler: $$(gcc --version | head -n1 2>/dev/null || clang --version | head -n1 2>/dev/null || echo 'Unknown')"
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)================================$(COLOR_RESET)"

# Format code (requires clang-format)
.PHONY: format
format:
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)Formatting code...$(COLOR_RESET)"
	@find EverEngineCore Sandbox -name "*.cpp" -o -name "*.h" | xargs clang-format -i 2>/dev/null || echo "clang-format not found, skipping..."
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)✓ Formatting complete!$(COLOR_RESET)"

# Static analysis (requires cppcheck)
.PHONY: check
check:
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)Running static analysis...$(COLOR_RESET)"
	@cppcheck --enable=all --suppress=missingIncludeSystem EverEngineCore/ Sandbox/ 2>/dev/null || echo "cppcheck not found, skipping..."

# Generate compile_commands.json for IDE
.PHONY: compile-commands
compile-commands:
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)Generating compile_commands.json...$(COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
	@ln -sf $(BUILD_DIR)/compile_commands.json . 2>/dev/null || true
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)✓ compile_commands.json generated!$(COLOR_RESET)"

# Install dependencies (Ubuntu/Debian)
.PHONY: install-deps
install-deps:
	@echo "$(COLOR_BOLD)$(COLOR_YELLOW)Installing dependencies...$(COLOR_RESET)"
	@sudo apt update
	@sudo apt install -y cmake build-essential libglfw3-dev libglew-dev libfmt-dev libspdlog-dev
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)✓ Dependencies installed!$(COLOR_RESET)"

# ============================================================================
# Help
# ============================================================================

.PHONY: help
help:
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)EverEngine Makefile Commands:$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BOLD)Build Commands:$(COLOR_RESET)"
	@echo "  make build          - Build project (Release)"
	@echo "  make rebuild        - Clean and rebuild"
	@echo "  make debug          - Build in Debug mode"
	@echo "  make clean          - Remove build files"
	@echo "  make clean-all      - Deep clean (remove all generated files)"
	@echo ""
	@echo "$(COLOR_BOLD)Run Commands:$(COLOR_RESET)"
	@echo "  make run            - Build and run (Release)"
	@echo "  make run-debug      - Build and run (Debug)"
	@echo "  make gdb            - Run with GDB debugger"
	@echo "  make valgrind       - Run with Valgrind (memory check)"
	@echo ""
	@echo "$(COLOR_BOLD)Utility Commands:$(COLOR_RESET)"
	@echo "  make configure      - Configure CMake only"
	@echo "  make info           - Show build configuration"
	@echo "  make format         - Format code with clang-format"
	@echo "  make check          - Run static analysis"
	@echo "  make compile-commands - Generate compile_commands.json"
	@echo "  make install-deps   - Install dependencies (Ubuntu/Debian)"
	@echo "  make help           - Show this help"
	@echo ""
	@echo "$(COLOR_BOLD)Examples:$(COLOR_RESET)"
	@echo "  make                # Build release"
	@echo "  make run            # Build and run"
	@echo "  make debug run-debug # Build debug and run"
	@echo "  make rebuild        # Clean rebuild"
	@echo "  make JOBS=8 build   # Build with 8 parallel jobs"
	@echo ""

.DEFAULT_GOAL := help