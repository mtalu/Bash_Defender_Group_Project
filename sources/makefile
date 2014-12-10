CFLAGS = -O4 -Wall -pedantic -std=c99 -F /usr/local/lib/ -I /usr/local/include -lSDL2 -lSDL2_ttf -lSDL2_image
TARGET = main
SOURCES = levelController.c Display.c Information_Window.c tower.c enemy.c gameProperties.c actionQueueDataStructure.c parser.c $(TARGET).c
CC =gcc
LIBS = -lm
all: $(TARGET)
$(TARGET): $(SOURCES) $(INCS)
		 $(CC) -I. $(SOURCES) -o gameOut $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
run: all
	$(TARGET) 
