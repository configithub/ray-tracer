CC=gcc
CFLAGS=-g -O3 -std=c99 --float-store
INCLUDES=-I/usr/include
LDFLAGS=-lm -lSDLmain -lSDL -lSDL_image -lGL -L/usr/lib/x86_64-linux-gnu   
SRC_DIR=src
SOURCES=src/main.c src/render_tools.c src/ray_tracer.c src/data.c
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h)
MAIN=bin/app

all: $(MAIN)
	
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c 
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) 

$(MAIN): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@



