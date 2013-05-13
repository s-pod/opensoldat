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
#ifndef PLAYER_H_
#define PLAYER_H_
#include "global.h"
#include "weapon.h"
#include "map.h"
class Bullet;

typedef enum playerSTATE {
	standing = 0,
	running = 1,
	up_jump = 2,
	forward_jump = 3,
	falling = 4,
	jet = 5,
	prone = 6,
	lying = 7,
	reloading = 8,
	weap_switch = 9,
	forward_roll = 10,
	backward_roll = 11
} pSTATE;

typedef enum playerDIRECTION {
	left = 0, right = 1
} pDIRECTION;

class Player {
public:
	Player(std::string name, b2World *world, float posX, float posY);
	virtual ~Player(void);
	virtual void draw();
	virtual float getX(void) {
		return playerBody->GetPosition().x;
	}
	virtual float getY(void) {
		return playerBody->GetPosition().y;
	}
	virtual void changeState(pSTATE state);
	virtual void addToWorld();
	void setSpawnPoint(tagPMS_SPAWNPOINT sp);
	bool setPrimaryWeap(weapon *weap);
	bool setSecondaryWeap(weapon *weap);
	virtual void shoot();
	b2World *getWorld();
	virtual b2Body* getBody();
	double getAngle();
	virtual void update();
private:
	void init();
	std::string name;
	b2World *world;
	b2Body *playerBody;
	tagPMS_SPAWNPOINT spawn_point;
	float health;
	float jet;
	float vest;
	int ammo;
	int nades;
	int activeWeap;
	double angle; //in radians
	pDIRECTION direction;
	pSTATE state;
	int cur_frame;
	float height, width;

	GLuint p_vao_poly;
	GLuint p_vbo_poly;
	float *polygons;

};

extern Map *map;
extern int screen_w, screen_h;
extern std::vector<Bullet*> *bullets;
#endif /* PLAYER_H_ */
