
CXXA ?= clang++
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
	CXX=$(CXXA) cmake -DCMAKE_BUILD_TYPE=Relase -B build-release -S . && cmake --build build-release -j
	cp build-release/clockwork $(EXE)

debug:
	export CXX=$CXX
	cmake -DCMAKE_BUILD_TYPE=Debug -B build-debug -S . && cmake --build build-debug -j
	cp build-debug/clockwork $(EXE)

clean:
	rm -rf build-debug build-release
	rm ./clockwork
