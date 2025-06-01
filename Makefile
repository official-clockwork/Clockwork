
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

ifeq ($(OS), Windows_NT)
release:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(CXX) -B build-release -S . && cmake --build build-release -j
	copy "build-release\clockwork.exe" $(EXE)
else
release:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(CXX) -B build-release -S . && cmake --build build-release -j
	cp "build-release/clockwork" $(EXE)
endif

ifeq ($(OS), Windows_NT)
debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$(CXX) -B build-debug -S . && cmake --build build-debug -j
	copy "build-debug\clockwork.exe" $(EXE)
else
debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$(CXX) -B build-debug -S . && cmake --build build-debug -j
	cp "build-debug/clockwork" $(EXE)
endif

ifeq ($(OS), Windows_NT)
clean:
		rd /s /q build-debug build-release
		rd $(EXE)
else
clean:
		rm -rf build-debug build-release
		rm $(EXE)
endif
