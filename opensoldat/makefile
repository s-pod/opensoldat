CFLAGS+= -Wall -g
CFLAGS+= -I/home/stas/programs/engines/Box2D_v2.2.1/
LDFLAGS+= -L/home/stas/programs/engines/Box2D_v2.2.1/Box2D/ -lBox2D 
LDFLAGS+= -lSDLmain -lSDL -lGLU -lGL -lfreeimage -lm -lGLEW  -lsfml-window -lsfml-system
BUILD_DIR = ./build
SRC_DIR = ./src
all: opensoldat

opensoldat : game.o map.o player.o animations.o weapon.o bullet.o vehicle.o
	g++ -o opensoldat $(BUILD_DIR)/game.o $(BUILD_DIR)/map.o $(BUILD_DIR)/player.o $(BUILD_DIR)/animations.o $(BUILD_DIR)/weapon.o $(BUILD_DIR)/bullet.o $(BUILD_DIR)/vehicle.o $(CFLAGS) $(LDFLAGS)

game.o : $(SRC_DIR)/game.cpp $(SRC_DIR)/global.h $(SRC_DIR)/map.h
	g++ -o $(BUILD_DIR)/game.o -c $(SRC_DIR)/game.cpp $(CFLAGS)
	
map.o : $(SRC_DIR)/map.cpp $(SRC_DIR)/global.h $(SRC_DIR)/map.h
	g++ -o $(BUILD_DIR)/map.o -c $(SRC_DIR)/map.cpp $(CFLAGS)
	
player.o : $(SRC_DIR)/player.cpp $(SRC_DIR)/player.h $(SRC_DIR)/global.h
	g++ -o $(BUILD_DIR)/player.o -c $(SRC_DIR)/player.cpp $(CFLAGS)
	
vehicle.o : $(SRC_DIR)/vehicle.cpp $(SRC_DIR)/vehicle.h $(SRC_DIR)/global.h
	g++ -o $(BUILD_DIR)/vehicle.o -c $(SRC_DIR)/vehicle.cpp $(CFLAGS)

animations.o : $(SRC_DIR)/animations.cpp $(SRC_DIR)/animations.h $(SRC_DIR)/global.h
	g++ -o $(BUILD_DIR)/animations.o -c $(SRC_DIR)/animations.cpp $(CFLAGS)

weapon.o : $(SRC_DIR)/weapon.cpp $(SRC_DIR)/weapon.h $(SRC_DIR)/global.h
	g++ -o $(BUILD_DIR)/weapon.o -c $(SRC_DIR)/weapon.cpp $(CFLAGS)
	
bullet.o : $(SRC_DIR)/bullet.cpp $(SRC_DIR)/bullet.h $(SRC_DIR)/player.h
	g++ -o $(BUILD_DIR)/bullet.o -c $(SRC_DIR)/bullet.cpp $(CFLAGS)
clean:
	rm opensoldat $(BUILD_DIR)/game.o $(BUILD_DIR)/map.o $(BUILD_DIR)/player.o $(BUILD_DIR)/animations.o $(BUILD_DIR)/weapon.o $(BUILD_DIR)/bullet.o $(BUILD_DIR)/vehicle.o
