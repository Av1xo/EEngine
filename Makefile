CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I. -I$(CORE_DIR) -I$(CORE_DIR)/core -I$(CORE_DIR)/platform -I$(CORE_DIR)/rendering -I$(CORE_DIR)/scene
LDFLAGS = -lglfw -lGL -lGLEW -ldl -lpthread 

BUILD_DIR = build
CORE_DIR = EverEngineCore
SANDBOX_DIR = Sandbox

CORE_SOURCES = $(wildcard $(CORE_DIR)/core/*.cpp) \
               $(wildcard $(CORE_DIR)/platform/*.cpp) \
               $(wildcard $(CORE_DIR)/rendering/*.cpp) \
               $(wildcard $(CORE_DIR)/scene/*.cpp)

SANDBOX_SOURCES = $(wildcard $(SANDBOX_DIR)/*.cpp)

ALL_SOURCES = $(CORE_SOURCES) $(SANDBOX_SOURCES)

CORE_OBJECTS = $(CORE_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
SANDBOX_OBJECTS = $(SANDBOX_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
ALL_OBJECTS = $(CORE_OBJECTS) $(SANDBOX_OBJECTS)

TARGET = $(BUILD_DIR)/main

DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

.PHONY: all
all: release

.PHONY: release
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

.PHONY: debug
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

$(TARGET): $(ALL_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(ALL_OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	@echo "Clean complete"

.PHONY: run
run: release
	./$(TARGET)

.PHONY: run-debug
run-debug: debug
	./$(TARGET)

.PHONY: info
info:
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "SOURCES: $(ALL_SOURCES)"
	@echo "OBJECTS: $(ALL_OBJECTS)"
	@echo "TARGET: $(TARGET)"

.PHONY: rebuild
rebuild: clean all