
CXX ?= clang++
EXE ?= clockwork

ifeq ($(OS), Windows_NT)
	SUFFIX := .exe
else
	SUFFIX :=
endif


EXE := "$(EXE)$(SUFFIX)"

.PHONY: all release debug clean

all: release

release:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(CXX) -B build-release -S . && cmake --build build-release -j
	cp "build-release/clockwork$(SUFFIX)" $(EXE)

debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$(CXX) -B build-debug -S . && cmake --build build-debug -j
	cp "build-debug/clockwork$(SUFFIX)" $(EXE)

clean:
	rm -rf build-debug build-release
	rm $(EXE)
