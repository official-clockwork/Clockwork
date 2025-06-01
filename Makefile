CXX ?= clang++
EXE ?= clockwork

ifeq ($(OS), Windows_NT)
    SUFFIX := .exe
    COPY := copy
    RM := del
    RM_DIR := rd /s /q
    MK_PATH = $(subst /,\,$(1))
else
    SUFFIX :=
    COPY := cp
    RM := rm
    RM_DIR := rm -rf
    MK_PATH = $(1)
endif



EXE := "$(EXE)$(SUFFIX)"

.PHONY: all release debug clean

all: release

release:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(CXX) -B build-release -S . && cmake --build build-release -j
	$(COPY) $(call MK_PATH,"build-release/clockwork$(SUFFIX)") $(EXE)

debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$(CXX) -B build-debug -S . && cmake --build build-debug -j
	$(COPY) $(call MK_PATH,"build-debug/clockwork$(SUFFIX)") $(EXE)

clean:
	-$(RM_DIR) build-debug build-release
	-$(RM) $(EXE)