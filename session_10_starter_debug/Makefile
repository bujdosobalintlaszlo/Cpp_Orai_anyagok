UNAME_S := $(shell uname -s)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -fPIC -Wno-sign-compare

ifeq ($(UNAME_S),Linux)
    PLUGIN_EXT = .so
    LDFLAGS = -ldl
    PLUGIN_FLAGS = -shared
endif
ifeq ($(UNAME_S),Darwin)
    PLUGIN_EXT = .dylib
    LDFLAGS =
    PLUGIN_FLAGS = -dynamiclib
endif
ifeq ($(OS),Windows_NT)
    PLUGIN_EXT = .dll
    LDFLAGS =
    PLUGIN_FLAGS = -shared
endif

PLUGIN_DIR = plugins
GAME = game
PLUGINS = $(PLUGIN_DIR)/scrib_plugin$(PLUGIN_EXT) $(PLUGIN_DIR)/netch_plugin$(PLUGIN_EXT)

all: $(GAME) $(PLUGINS)

$(GAME): main.cpp display.cpp world.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(PLUGIN_DIR):
	mkdir -p $(PLUGIN_DIR)

$(PLUGIN_DIR)/scrib_plugin$(PLUGIN_EXT): scrib_plugin.cpp | $(PLUGIN_DIR)
	$(CXX) $(CXXFLAGS) $(PLUGIN_FLAGS) -o $@ $<

$(PLUGIN_DIR)/netch_plugin$(PLUGIN_EXT): netch_plugin.cpp | $(PLUGIN_DIR)
	$(CXX) $(CXXFLAGS) $(PLUGIN_FLAGS) -o $@ $<

clean:
	rm -f $(GAME)
	rm -rf $(PLUGIN_DIR)

run: all
	./$(GAME)

.PHONY: all clean run
