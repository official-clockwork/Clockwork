
CXX ?= clang++
EXE ?= clockwork

ifeq ($(OS), Windows_NT)
	SUFFIX := .exe
	COPY := copy
	RM := rd /q
	RM_DIR := rd /s /q
else
	SUFFIX :=
	COPY := cp
	RM := rm
	RM_DIR := rm -rf
endif


EXE := "$(EXE)$(SUFFIX)"

.PHONY: all release debug clean

all: release

release:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(CXX) -B build-release -S . && cmake --build build-release -j
	$(COPY) "build-release/clockwork$(SUFFIX)" $(EXE)

debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$(CXX) -B build-debug -S . && cmake --build build-debug -j
	$(COPY) "build-debug/clockwork$(SUFFIX)" $(EXE)

clean:
	$(RM_DIR) build-debug build-release
	$(RM) $(EXE)
