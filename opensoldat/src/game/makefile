OBJECTS = game.o map.o player.o animations.o weapon.o bullet.o vehicle.o gameloop.o fpscounter.o event.o main.o
CC= g++
CFLAGS+= -Wall -g
CFLAGS+= -I/home/stas/programs/engines/Box2D_v2.2.1/
LDFLAGS+= -L/home/stas/programs/engines/Box2D_v2.2.1/Box2D/ -lBox2D
LDFLAGS+= -lSDLmain -lSDL -lGLU -lGL -lfreeimage -lm -lGLEW
all: opensoldat clean

opensoldat: $(OBJECTS)
	$(CC) $(OBJECTS) -o ../../$@ $(CFLAGS) $(LDFLAGS)
	
.PHONY: clean cleanest

clean:
	rm *.o
	
cleanest: clean
	rm ../../opensoldat
	
%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
