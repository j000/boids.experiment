# ----------------------------------------------------------------------------
# CMake project wrapper Makefile
# Heavily modified, from:
#     https://code.google.com/archive/p/cpp-project-template/
# BSD-3 License http://opensource.org/licenses/BSD-3-Clause
# ----------------------------------------------------------------------------

SHELL  := /bin/sh
RM     := rm -rf
MKDIR  := mkdir -p
BUILDDIR := build
SILENT :=
ifeq ($(findstring s,$(MAKEFLAGS)),s)
    SILENT := >/dev/null
endif

all: $(BUILDDIR)/Makefile
	$(MAKE) -C $(BUILDDIR)

$(BUILDDIR)/Makefile:
	$(MKDIR) $(BUILDDIR)
	(cd $(BUILDDIR) && cmake .. $(SILENT))

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

#$(info MAKECMDGOALS = $(filter $(MAKECMDGOALS),$(BUILDDIR)/Makefile all distclean .my-clean clean-cmake))
ifeq (,$(filter $(MAKECMDGOALS),$(BUILDDIR)/Makefile all distclean .my-clean clean-cmake))
.PHONY: $(MAKECMDGOALS)
$(MAKECMDGOALS): $(BUILDDIR)/Makefile
	$(MAKE) -C $(BUILDDIR) $(MAKECMDGOALS)
endif
