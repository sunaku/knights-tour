# the path to Cg installation directory.
# this is usually the root of the file system ('/')
CG_DIR=$(APPS)/Cg-1.5_x86

CG_INC=$(CG_DIR)/usr/include
CG_LIB=$(CG_DIR)/usr/lib

CG_FLG=-I$(CG_INC) -L$(CG_LIB) -lCg -lCgGL
GL_FLG=-IGL -lglut -lGLEW

CFLAGS=-Wall -g $(GL_FLG) $(CG_FLG)

all: run

build: main.c
	cc $(CFLAGS) $^ -o $@

run: build
	$<

clean:
	rm -f build
