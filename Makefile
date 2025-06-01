EXE ?= clockwork


# Debug object files
ifeq ($(strip $(debug)),) # If debug is empty or whitespace only
	OBJDIR := release
else
	OBJDIR := debug
endif


# Source and object files
SOURCES  := src/main.cpp src/uci.cpp
BUILDDIR := build
OBJDIR   := $(BUILDDIR)/$(OBJDIR)
OBJECTS  := $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SOURCES))


# Compilation flags
CXX      ?= clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -fno-exceptions -fno-rtti $(CXXFLAGS)
LDFLAGS  := $(LDFLAGS)


# Debug flags
ifeq ($(strip $(debug)),) # If debug is empty or whitespace only
	CXXFLAGS += -O3 -flto -DNDEBUG
	LDFLAGS  += -flto
else
	CXXFLAGS += -O0 -g
	LDFLAGS  += -g
endif


# Handle windows exe extensions
ifeq ($(OS), Windows_NT)
	SUFFIX := .exe
else
	SUFFIX :=
endif

EXE := $(EXE)$(SUFFIX)


# Rules
.PHONY: all clean

all: $(EXE)



$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) -r $(BUILDDIR) $(EXE)


# Include generated dependencies
-include $(OBJECTS:.o=.d)
