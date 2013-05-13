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

#include "global.h"
#include "player.h"
#include "vehicle.h"
#include "map.h"
#include "animations.h"
#include "weapon.h"
#include "bullet.h"
void reshape(int width, int height);

//global variables
animations *anims;
Map *map;
std::vector<Bullet*> *bullets;
std::vector<Player*> *players;
int screen_w = 1920, screen_h = 1080, screen_depth = 32;

// Shader sources
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

GLuint shaderProgram;

int main(int argc, char **argv) {
	// init sdl
	SDL_Surface *screen;

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

	SDL_WM_SetCaption("SoldatX", NULL);
	glDisable(GL_DEPTH_TEST);
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
	b2World world(gravity);
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	//init map
	map = new Map();
	char mapname[] = "maps/ZS_JungleRumble.PMS";
	std::cout << map->getMapDir() << std::endl;
	map->load(mapname);
	map->addToWorld(&world);

	//init animations
	anims = new animations();
	anims->loadAnimations();

	//add players
	players = new std::vector<Player*>();
	std::string name = "Rambo";
	std::vector<tagPMS_SPAWNPOINT> spawnPoints = map->getSpawnPoints();
	Player *p1 = new Player(name, &world, 0, 0);
	p1->setSpawnPoint(spawnPoints[1]);
	p1->addToWorld();

	std::string name3 = "Rambo";
	Player *p2 = new Player(name3, &world, 0, 0);
	p2->setSpawnPoint(spawnPoints[3]);
	p2->addToWorld();

	//init bullets
	bullets = new std::vector<Bullet*>();

	//active player
	Player *ap = p1;

	int done = 0, mousex = 0, mousey = 0;
	bool drag = false;
	float mx = 0, my = 0, sfactor = 0.002f;
	Uint8 *keys;

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	char buffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	glm::mat4 view;
	GLint viewTrans = glGetUniformLocation(shaderProgram, "view");

	while (!done) {
		world.Step(timeStep, velocityIterations, positionIterations);
		SDL_Event event;
		SDL_GetMouseState(&mousex, &mousey);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_VIDEORESIZE:
				screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32,
						SDL_OPENGL | SDL_RESIZABLE);
				if (screen) {
					//reshape(screen->w, screen->h);
				} else {
					/* Uh oh, we couldn't set the new video mode?? */;
				}
				break;

			case SDL_QUIT:
				done = 1;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == 4)
					sfactor *= 1.1;
				if (event.button.button == 5)
					sfactor *= 0.9;
				if (event.button.button == 1) {
					ap->shoot();
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == 1)
					drag = false;
				break;

			case SDL_MOUSEMOTION:
				if (drag) {
					mx += event.motion.xrel * sfactor * map->getBottomffs()
							* 900 * 900 / 376 / 1075 / screen->w;
					my -= event.motion.yrel * sfactor * map->getBottomffs()
							* 900 * 900 / 376 / 1075 / screen->w;
				}
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_TAB:
					if (ap == p1) {
						ap = p2;
					} else {
						ap = p1;
					}
					break;
				case SDLK_d:
					p1->changeState(running);
					break;

				case SDLK_UP:
					ap->getBody()->SetBullet(true);
					ap->getBody()->ApplyForce(b2Vec2(0, 600.0f),
							b2Vec2(ap->getX(), ap->getY()));
					break;

				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_d:
					p1->changeState(standing);

					break;
				default:
					break;
				}
				break;
			}
		}
		keys = SDL_GetKeyState(NULL);
		if (keys[SDLK_LEFT]) {
			ap->getBody()->ApplyForce(b2Vec2(-100.0f, 0.0f),
					b2Vec2(ap->getX(), ap->getY()));
			if (ap->getBody()->GetLinearVelocity().x <= -10.0f) {
				ap->getBody()->SetLinearVelocity(
						b2Vec2(-10.0f, ap->getBody()->GetLinearVelocity().y));
			}
		} else if (keys[SDLK_RIGHT]) {

			ap->getBody()->ApplyForce(b2Vec2(100.0f, 0.0f),
					b2Vec2(ap->getX(), ap->getY()));
			if (ap->getBody()->GetLinearVelocity().x >= 10.0f) {
				ap->getBody()->SetLinearVelocity(
						b2Vec2(10.0f, ap->getBody()->GetLinearVelocity().y));
			}
		} else {
		}
		if (keys[SDLK_UP]) {

		}
		if (keys[SDLK_ESCAPE]) {
			done = 1;
		}
		if (keys[SDLK_w]) {
			my = (my - 0.01 / sfactor);
		}
		if (keys[SDLK_s]) {
			my = (my + 0.01 / sfactor);
		}
		if (keys[SDLK_a]) {
			mx = (mx + 0.01 / sfactor);
		}
		if (keys[SDLK_d]) {
			mx = (mx - 0.01 / sfactor);
		}
		if (keys[SDLK_q]) {
			sfactor *= 1.01;
		}
		if (keys[SDLK_e]) {
			sfactor *= 0.99;
		}
		if (keys[SDLK_SPACE]) {
			ap->shoot();
		}

		map->render(&view);

		//draw player
		p1->update();
		p2->update();
		p1->draw();
		p2->draw();
		//draw bullets
		time_t now;
		time(&now);
		for (std::vector<Bullet*>::size_type i = 0; i < bullets->size(); i++) {

			Bullet *b = bullets->at(i);
			if (difftime(now, b->getTime()) > 1) {
				bullets->erase(bullets->begin() + i);
				delete b;
				i--;
			} else {
				b->draw();
			}
		}
		double factor = 1 / (sfactor * 1000.0f);
		//camera
		float x = (float) -mousex * factor + (float) screen->w / 2.0f * factor
				- ap->getX() * worldScale;
		float y = (float) mousey * factor - (float) screen->h / 2.0f * factor
				- (float) ap->getY() * worldScale;
		view = glm::mat4();
		view = glm::scale(view, glm::vec3(sfactor, sfactor, 0));
		view = glm::translate(view, glm::vec3(x, y, 0));
		glUniformMatrix4fv(viewTrans, 1, GL_FALSE, glm::value_ptr(view));
		SDL_Flip(screen);
		SDL_GL_SwapBuffers();
		//set FPS
		SDL_Delay(1000 / 100.0f);
	}
	glDeleteProgram( shaderProgram );
	glDeleteShader( fragmentShader );
	glDeleteShader( vertexShader );
	delete (anims);
	delete (map);

	SDL_Quit();
}
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	//projection
	float aspect = (float) width / (float) height;
	glm::mat4 proj = glm::scale(glm::mat4(), glm::vec3(1.0f, aspect, 0.0f));
	GLint projTrans = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(projTrans, 1, GL_FALSE, glm::value_ptr(proj));
}

