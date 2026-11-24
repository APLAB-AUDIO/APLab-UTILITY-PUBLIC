# =============================================================================
#  HOTKEY (plugin "APLab Utility", formerly "Encoder") — build recipe
#  A standalone utility plugin: use the push-dial on your computer keyboard
#  as a real encoder for any module in VCV Rack. NOT part of the Orbital family.
#
#  You normally never edit this. Run `make` to build, `make install` to build
#  AND copy the plugin into VCV Rack's plugins folder.
# =============================================================================

# Where the Rack SDK lives (shared with Orbital, one folder up).
RACK_DIR ?= ../Rack-SDK

FLAGS +=
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

# Every .cpp file in src/ gets compiled — new files are picked up automatically.
SOURCES += $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk
