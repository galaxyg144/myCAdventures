#>------------------------------------------=== INFO ===-----------------------------------------<#
#                                                                                                 #
#   To compile:                                  make -j$(nproc)                                  #
#     or:                                        make -j$(nproc) build                            #
#   To compile and run:                          make -j$(nproc) run                              #
#   To clean up the object files:                make clean                                       #
#   To clean up the object files and the binary: make distclean                                   #
#                                                                                                 #
#   To compile with debug mode and the address sanitizer enabled, pass DEBUG=y ASAN=y             #
#     Example:                                   make -j$(nproc) DEBUG=y ASAN=y                   #
#                                                                                                 #
#>-----------------------------------------------------------------------------------------------<#

# === SETUP AND PREP ===

# name of the executable to output
# !!! REPLACE 'myprogram' WITH YOUR PROGRAM'S NAME !!!

# directory containing the source files
SRCDIR := src
# directory to output the object files
OBJDIR := obj
# directory to output the executable
OUTDIR := .

# adds "release" or "debug" to the object files path
ifneq ($(DEBUG),y)
    OBJDIR := $(OBJDIR)/release
else
    OBJDIR := $(OBJDIR)/debug
endif
# adds "asan" to the object files path
ifeq ($(ASAN),y)
    OBJDIR := $(OBJDIR)_asan
endif

# finds all .c files in $(SRCDIR)
SOURCES := $(wildcard $(SRCDIR)/*.c)
# replaces the .c extensions with .o and $(SRCDIR) with $(OBJDIR)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# a variable to hold the full path of the executable to output
TARGET := $(OUTDIR)/$(BIN)

# set up some vars for utils (some of these are defined already by make)
CC ?= gcc
LD := $(CC)
_CC := $(TOOLCHAIN)$(CC)
_LD := $(TOOLCHAIN)$(LD)

# flags for the C compiler (change the -std=... if you want)
CFLAGS += -std=c11 -Wall -Wextra -Wuninitialized -Wundef
# flags for the C preprocessor (put things like -DMYMACRO=... here)
CPPFLAGS += -D_DEFAULT_SOURCE
# flags for the linker
LDFLAGS += 
# libraries to link to (put things like -lmylib here)
LDLIBS += 

# if called with DEBUG=y
ifeq ($(DEBUG),y)
    # enable debug symbols, switch to the debug optimization mode, enable the address sanitizer, and add some more warning flags
    CFLAGS += -g -Og -fsanitize=address -Wdouble-promotion
    #CFLAGS += -Wconversion
    LDFLAGS += -fsanitize=address
# otherwise
else
    # enable a stable optimization mode (O2),
    CFLAGS += -O2
    # and define the standard practice macro for indicating that this is not a debug build
    CPPFLAGS += -DNDEBUG
endif
# if called with ASAN=y
ifeq ($(ASAN),y)
    # enable the address sanitizer during compiling
    CFLAGS += -fsanitize=address
    # and during linking
    LDFLAGS += -fsanitize=address
endif

# === BUILDING THE PROGRAM ===

# enables some GNU make specific features to help with some things
.SECONDEXPANSION:

# file management helper funcs
define mkdir
if [ ! -d '$(1)' ]; then echo 'Creating $(1)/...'; mkdir -p '$(1)'; fi; true
endef
define rm
if [ -f '$(1)' ]; then echo 'Removing $(1)/...'; rm -f '$(1)'; fi; true
endef
define rmdir
if [ -d '$(1)' ]; then echo 'Removing $(1)/...'; rm -rf '$(1)'; fi; true
endef

# recompile a .c file if a .h it includes changes
deps.filter := %.c %.h
deps.option := -MM
define deps
$$(filter $$(deps.filter),,$$(shell $(_CC) $(CFLAGS) $(CPPFLAGS) -E $(deps.option) $(1)))
endef

# phony rule as a shortcut to build the executable (first rule because make uses the first rule as the default)
build: $(TARGET)
	@:

# phony rule to build and run the executable
run: build
	@echo Running $(BIN)...
	@'$(dir $(BIN))$(notdir $(BIN))' $(RUNFLAGS)

# create the output dir if it doesn't exist
$(OUTDIR):
	@$(call mkdir,$@)

# create the object dir if it doesn't exist
$(OBJDIR):
	@$(call mkdir,$@)

# compile the .c files into .o object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(call deps,$(SRCDIR)/%.c) | $(OBJDIR) $(OUTDIR)
	@echo Compiling $<...
	@$(_CC) $(CFLAGS) -Wall -Wextra -I$(PSRCDIR) -DPSRC_REUSABLE $(CPPFLAGS) $< -c -o $@
	@echo Compiled $<

# link the .o object files into an executable
$(TARGET): $(OBJECTS) | $(OUTDIR)
	@echo Linking $@...
	@$(_LD) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo Linked $@

# phony rule to clean up the object files
clean:
	@$(call rmdir,$(OBJDIR))

# phony rule to clean up the object files and executable
distclean: clean
	@$(call rm,$(TARGET))

# specify the phony rules (this means they don't correlate to actual files like real rules)
.PHONY: build run clean distclean
