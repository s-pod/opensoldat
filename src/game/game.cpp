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
#include "game.h"
// Shader sources
//TODO: make normal shader manager with external shader sources
const char* vertexSource = "#version 130\n"
		"in vec2 position;"
		"in vec4 color;"
		"in vec2 texcoord;"
		"out vec4 Color;"
		"out vec2 Texcoord;"
		"uniform mat4 proj;"
		"uniform mat4 view;"
		"uniform mat4 model;"
		"void main() {"
		"	Color = color;"
		"	Texcoord = texcoord;"
		"	gl_Position = proj * view * model * vec4( position, 0.0f, 1.0f );"
		"}";

const char* fragmentSource = "#version 130\n"
		"in vec4 Color;"
		"in vec2 Texcoord;"
		"out vec4 outColor;"
		"uniform sampler2D tex;"
		"void main() {"
		"	outColor =  Color * texture(tex, Texcoord);"
		"}";
Game::Game(int argc, char** argv) :
		GameLoop(60.0f, true), running(true), argc(argc), argv(argv), screen(
				NULL), title("openSoldat 0.0.0.0 pre-alpha (stage-0)") {
	screen_w = 1920, screen_h = 1080, screen_depth = 32, shaderProgram = 0, world =
			NULL, viewTrans = 0, mousex = 0, mousey = 0, map = NULL, sfactor =
			0.002f, velocityIterations = 6, positionIterations = 2, bullets = NULL, players =
			NULL, vertexShader = 0, fragmentShader = 0;
	player = NULL;
	// for testing purposes
}

bool Game::init() {
	// init sdl

	SDL_Init(SDL_INIT_EVERYTHING);
	//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	screen = SDL_SetVideoMode(screen_w, screen_h, screen_depth,
			SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN);
	if (!screen) {
		fprintf(stderr, "Couldn't set GL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	//SDL_ShowCursor(SDL_DISABLE);

	// init image library
	FreeImage_Initialise();

	SDL_WM_SetCaption("OpenSoldat", NULL);
	glDisable(GL_DEPTH_TEST);
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create and compile the vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	reshape(screen->w, screen->h);

	//create world
	b2Vec2 gravity(0.0f, -15.0f);
	world = new b2World(gravity);

	//init map
	map = new Map(shaderProgram);
	char mapname[] = "maps/ZS_JungleRumble.PMS";
	std::cout << map->getMapDir() << std::endl;
	map->load(mapname);
	map->addToWorld(world);

	//init animations
//	anims = new animations();
//	anims->loadAnimations();

	//add players
	players = new std::vector<Player*>();
	std::string name = "Rambo";
	std::vector<tagPMS_SPAWNPOINT> spawnPoints = map->getSpawnPoints();
	player = new Player(name, 0, 0, this);
	player->setSpawnPoint(spawnPoints[1]);
	player->addToWorld();

//	std::string name3 = "Rambo";
//	Player *p2 = new Player(name3, world, 0, 0);
//	p2->setSpawnPoint(spawnPoints[3]);
//	p2->addToWorld();

	//init bullets
	bullets = new std::vector<Bullet*>();

	//active player
	//Player *ap = player;

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	char buffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	std::cout << buffer << std::endl;
	viewTrans = glGetUniformLocation(shaderProgram, "view");

	return true;
}

int Game::run() {
	if (!init()) {
		// TODO: write some sort of error message
		printf("init error in Game::run\n");
		return -1;
	}

	// we don't leave this loop until we quit the game
	runLoop();

	cleanUp();

	return 0;
}

bool Game::continueLoop() {
	return running;
}

void Game::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_q:
	case SDLK_ESCAPE:
		running = false;
		break;
	case SDLK_d:
		player->setMoveRight(true);
		break;
	case SDLK_a:
		player->setMoveLeft(true);
		break;
	case SDLK_s:
		player->setMoveDown(true);
		break;
	case SDLK_w:
		player->setMoveUp(true);
		break;
	default:
		break;
	}
}

void Game::onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_d:
		player->setMoveRight(false);
		break;
	case SDLK_a:
		player->setMoveLeft(false);
		break;
	case SDLK_s:
		player->setMoveDown(false);
		break;
	case SDLK_w:
		player->setMoveUp(false);
		break;
	default:
		break;
	}
}

void Game::onMouseMove(int mX, int mY, int relX, int relY, bool left,
		bool right, bool middle) {
	// TODO: weapon angle adjustment
}

// temporary solution
// TODO: put into MouseInput class
void bindMouseToWindow(bool enable) {
	// TODO: implement Input class and MouseInput class
	// temporary solution
	SDL_GrabMode grabMode = enable ? SDL_GRAB_ON : SDL_GRAB_OFF;
	if (SDL_WM_GrabInput(SDL_GRAB_QUERY) != grabMode) {
		std::cout << "SDL_GRAB_" << (enable ? "ON" : "OFF") << std::endl;
		SDL_WM_GrabInput(grabMode);
	}
}

void Game::onRButtonDown(int mX, int mY) {

}

void Game::onMouseWheel(bool Up, bool Down) {
	if (Up) {
		sfactor *= 1.1;
	}
	if (Down) {
		sfactor *= 0.9;
	}

}

void Game::onLButtonDown(int mX, int mY) {
	bindMouseToWindow(true);
	player->setShooting(true);
}

void Game::onLButtonUp(int mX, int mY) {
	player->setShooting(false);
}

void Game::update(float frameTime, float frameCount) {
	// process window message queue here
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		onEvent(&event);
	}
	world->Step(frameTime * frameCount, velocityIterations, positionIterations);
	SDL_GetMouseState(&mousex, &mousey);
	player->update(mousex, mousey);
}

void Game::render(float interpolation) {
	fpsCounter.countFrame();
	map->renderOnBack();
	player->draw();
	//draw bullets
	float now = SDL_GetTicks();
	for (std::vector<Bullet*>::size_type i = 0; i < bullets->size(); i++) {

		Bullet *b = bullets->at(i);
		if ((now - b->getTime()) > 1000) {
			bullets->erase(bullets->begin() + i);
			delete b;
			i--;
		} else {
			b->draw();
		}
	}

	double factor = 1 / (sfactor * 1000.0f);
	map->renderOnFront();
	//camera
	float x = (float) -mousex * factor + (float) screen->w / 2.0f * factor
			- player->getX() * worldScale;
	float y = (float) mousey * factor - (float) screen->h / 2.0f * factor
			- (float) player->getY() * worldScale;
	view = glm::mat4();
	view = glm::scale(view, glm::vec3(sfactor, sfactor, 0));
	view = glm::translate(view, glm::vec3(x, y, 0));
	glUniformMatrix4fv(viewTrans, 1, GL_FALSE, glm::value_ptr(view));
	renderUI();
	SDL_Flip(screen);
	SDL_GL_SwapBuffers();
}

void Game::renderWorld(float interpolation) {

}

void Game::renderUI() {
	std::cout << "FPS: " << fpsCounter.getCurrentFps() << std::endl;
//	hud.render();
}

void Game::reshape(int width, int height) {
	glViewport(0, 0, width, height);
	//projection
	float aspect = (float) width / (float) height;
	glm::mat4 proj = glm::scale(glm::mat4(), glm::vec3(1.0f, aspect, 0.0f));
	GLint projTrans = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(projTrans, 1, GL_FALSE, glm::value_ptr(proj));
}

void Game::cleanUp() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
//	delete anims;
	delete map;
	delete world;
	SDL_FreeSurface(screen);
	SDL_Quit();
}

b2World *Game::getWorld(void) {
	return world;
}
GLuint Game::getShaderProg(void) {
	return shaderProgram;
}
void Game::addBullet(Bullet *b) {
	bullets->push_back(b);
}
std::vector<Player*> *Game::getPlayers(void) {
	return players;
}
int Game::getScreen_h() {
	return screen_h;
}
int Game::getScreen_w() {
	return screen_w;
}

void Game::onExit() {
	running = false;
}

Game::~Game() {

}
