# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_linux
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_linux)
  VictAsteroids_config = debug_linux
endif
ifeq ($(config),debug_windows)
  VictAsteroids_config = debug_windows
endif
ifeq ($(config),release_linux)
  VictAsteroids_config = release_linux
endif
ifeq ($(config),release_windows)
  VictAsteroids_config = release_windows
endif

PROJECTS := VictAsteroids

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

VictAsteroids:
ifneq (,$(VictAsteroids_config))
	@echo "==== Building VictAsteroids ($(VictAsteroids_config)) ===="
	@${MAKE} --no-print-directory -C VictAsteroids -f Makefile config=$(VictAsteroids_config)
endif

clean:
	@${MAKE} --no-print-directory -C VictAsteroids -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_linux"
	@echo "  debug_windows"
	@echo "  release_linux"
	@echo "  release_windows"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   VictAsteroids"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"