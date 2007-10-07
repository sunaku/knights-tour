CG_FLG=-lCg -lCgGL
GL_FLG=-IGL -lglut -lGLEW
CFLAGS=-Wall -g -std=c99 -lpthread $(GL_FLG) $(CG_FLG)

all: run

naive.cg: naive.cg.erb
	erb $< > $@

build: main.c naive.cg
	cc $(CFLAGS) $< -o $@

run: build
	./$<

clean:
	rm -f build naive.cg
