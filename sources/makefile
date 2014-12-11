CFLAGS = -O4 -Wall -pedantic -std=c99 -F /usr/local/lib/ -I /usr/local/include -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer  -lsmpeg
#INCDIR = ../includes/
#INCS = $(INCDIR)parser.h
#		$(INCDIR)gameProperties.h	
#		$(INCDIR)actionQueueDataStructure.h
#		$(INCDIR)debug.h\
#		$(INCDIR)tower.h\
#		$(INCDIR)terminalwindow.h\
#		$(INCDIR)sput.h              
TARGET =  main
SOURCES = levelController.c Sound.c Display.c Information_Window.c tower.c enemy.c  gameProperties.c actionQueueDataStructure.c parser.c $(TARGET).c
#	  		sTower2.c\
#	  		tower2.c\
#      		enemy2.c\
#          	terminalwindow.c\
 
LIBS = -lm
CC =gcc 

all: $(TARGET)

$(TARGET) $(SOURCES): $(INCS)
	$(CC) $(SOURCES) -o gameOut $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
