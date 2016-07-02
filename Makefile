# ----------------------------------------------------------------------------
# CMake project wrapper Makefile
# Heavily modified, from:
#     https://code.google.com/archive/p/cpp-project-template/
# BSD-3 License http://opensource.org/licenses/BSD-3-Clause
# ----------------------------------------------------------------------------

SHELL ?= /bin/sh
RM := rm -rf
MKDIR := mkdir -p
BUILDDIR := build
DEBUG :=
SILENT :=
ifeq ($(findstring s,$(MAKEFLAGS)),s)
    SILENT := >/dev/null
endif

.PHONY: just-exe
just-exe: | all clean-cmake

.PHONY: all
all: $(BUILDDIR)/Makefile
	$(MAKE) -C $(BUILDDIR)

.PHONY: debug
debug: DEBUG := -DCMAKE_BUILD_TYPE=Debug
debug: all

$(BUILDDIR)/Makefile:
	$(MKDIR) $(BUILDDIR)
	(cd $(BUILDDIR) && cmake .. $(SILENT) $(DEBUG))

.PHONY: distclean
distclean: | $(BUILDDIR)/Makefile .my-clean clean-cmake

.PHONY: .my-clean
.my-clean:
	$(MAKE) -C $(BUILDDIR) clean

.PHONY: clean-cmake
clean-cmake:
	$(RM) ./$(BUILDDIR)/Makefile
	$(RM) ./$(BUILDDIR)/CMakeCache.txt
	$(RM) ./$(BUILDDIR)/CMakeFiles
	$(RM) ./$(BUILDDIR)/cmake_install.cmake
	$(RM) ./$(BUILDDIR)/version.c
	@- rmdir --ignore-fail-on-non-empty ./$(BUILDDIR)

ifeq (,$(filter $(MAKECMDGOALS),$(BUILDDIR)/Makefile just-exe all debug distclean .my-clean clean-cmake))
.PHONY: $(MAKECMDGOALS)
$(MAKECMDGOALS): $(BUILDDIR)/Makefile
	$(MAKE) -C $(BUILDDIR) $(MAKECMDGOALS)
endif
