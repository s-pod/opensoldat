/*
 Copyright (c) 2013, Stanislav Podolsky
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 */
#ifndef GAME_H
#define GAME_H

#include "global.h"
#include "map.h"
#include "event.h"
#include "gameloop.h"
#include "fpscounter.h"
#include "player.h"
#include "bullet.h"


class Game: public Event, public GameLoop {
public:
	Game(int argc, char** argv);
	~Game();

	bool init();
	int run();

	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onMouseMove(int mX, int mY, int relX, int relY, bool left, bool right,
			bool middle);
	void onLButtonDown(int mX, int mY);
	void onLButtonUp(int mX, int mY);
	void onRButtonDown(int mX, int mY);
//	void onMButtonDown(int mX, int mY);
	void onMouseWheel(bool Up, bool Down);
//	void onInputFocus();
//	void onInputBlur();
	void onExit();

	bool continueLoop();
	void update(float frameTime, float frameCount);
	void render(float interpolation);
	void renderWorld(float interpolation);
	void renderUI();
	void reshape(int width, int height);
	void cleanUp();

	b2World *getWorld(void);
	GLuint getShaderProg(void);
	void addBullet(Bullet *b);
	std::vector<Player*> *getPlayers(void);
	int getScreen_h();
	int getScreen_w();

private:
	bool running;
	int argc;
	char** argv;
	FpsCounter fpsCounter;

	SDL_Surface* screen;

	std::string title;
	Map *map;
	b2World *world;
	std::vector<Bullet*> *bullets;
	std::vector<Player*> *players;
	Player* player;
	double sfactor;
	GLint viewTrans;
	int mousex, mousey;
	GLuint vertexShader;
	GLuint fragmentShader;
	int32 velocityIterations;
	int32 positionIterations;
	GLuint shaderProgram;
	glm::mat4 view;
	// TODO: implement EntityManager
	int screen_w, screen_h, screen_depth;

};

#endif
