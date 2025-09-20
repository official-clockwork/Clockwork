EXE  ?= clockwork
ARCH ?= native
PARALLEL_BUILD ?= yes

ifeq ($(origin CXX), default)
	CXX = clang++
endif

ifdef MSYSTEM
	SUFFIX := .exe
	SUFFIX :=
	COPY := cp
	RM := rm -f
	RM_DIR := rm -rf
	MK_PATH = $(1)
else
	ifeq ($(OS), Windows_NT)
	    SUFFIX := .exe
	    COPY := copy
	    RM := del
	    RM_DIR := rd /s /q
	    MK_PATH = $(subst /,\,$(1))
	else
	    SUFFIX :=
	    COPY := cp
	    RM := rm -f
	    RM_DIR := rm -rf
	    MK_PATH = $(1)
	endif
endif

CMAKE_FLAGS := -DCMAKE_CXX_COMPILER=$(CXX) -DCLOCKWORK_MARCH_TARGET=$(ARCH)
ifeq ($(PARALLEL_BUILD),yes)
	CMAKE_BUILD_FLAGS := -j
endif

.PHONY: all release debug evaltune test clean format

all: release

release:
	cmake -DCMAKE_BUILD_TYPE=Release $(CMAKE_FLAGS) -B build-release -S . && cmake --build build-release $(CMAKE_BUILD_FLAGS)
	$(COPY) $(call MK_PATH,"build-release/clockwork$(SUFFIX)") $(EXE)$(SUFFIX)

debug:
	cmake -DCMAKE_BUILD_TYPE=Debug $(CMAKE_FLAGS) -B build-debug -S . && cmake --build build-debug $(CMAKE_BUILD_FLAGS)
	$(COPY) $(call MK_PATH,"build-debug/clockwork$(SUFFIX)") $(EXE)$(SUFFIX)

evaltune:
	cmake -DCMAKE_BUILD_TYPE=Release $(CMAKE_FLAGS) -DCLOCKWORK_ENABLE_EVALTUNE=ON -B build-evaltune -S . && cmake --build build-evaltune $(CMAKE_BUILD_FLAGS)
	$(COPY) $(call MK_PATH,"build-evaltune/clockwork-evaltune$(SUFFIX)") $(EXE)-evaltune$(SUFFIX)

evaltune-debug:
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo $(CMAKE_FLAGS) -DCLOCKWORK_ENABLE_EVALTUNE=ON -B build-evaltune-debug -S . && cmake --build build-evaltune-debug $(CMAKE_BUILD_FLAGS)
	$(COPY) $(call MK_PATH,"build-evaltune-debug/clockwork-evaltune$(SUFFIX)") $(EXE)-evaltune$(SUFFIX)

bench: release
	./$(EXE)$(SUFFIX) bench

test: release
	ctest --test-dir build-release

clean:
	-$(RM_DIR) build-debug build-release build-evaltune build-evaltune-debug
	-$(RM) $(EXE)$(SUFFIX)

format:
	./scripts/auto_format.sh
