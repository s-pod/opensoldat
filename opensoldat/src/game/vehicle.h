///*
// Copyright (c) 2013, Stanislav Podolsky
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies,
// either expressed or implied, of the FreeBSD Project.
// */
//#ifndef VEHICLE_H_
//#define VEHICLE_H_
//#include "global.h"
//#include "weapon.h"
//#include "map.h"
//#include "player.h"
//class Bullet;
//
//class vehicle {
//public:
//	vehicle(std::string name, b2World *world, float posX, float posY,
//			Player *driver);
//	~vehicle(void);
//	void draw();
//	float getX(void) {
//		return cart->GetPosition().x;
//	}
//	float getY(void) {
//		return cart->GetPosition().y;
//	}
//	void addToWorld();
//	void setSpawnPoint(tagPMS_SPAWNPOINT sp);
//	bool setPrimaryWeap(weapon *weap);
//	bool setSecondaryWeap(weapon *weap);
//	void shoot();
//	b2World *getWorld();
//	b2RevoluteJoint* getMotor(int n);
//	Player * getDriver();
//	b2Body* getBody();
//	double getAngle();
//	void update();
//private:
//	std::string name;
//	b2World *world;
//	b2Body *playerBody;
//	tagPMS_SPAWNPOINT spawn_point;
//	float health;
//	float jet;
//	float vest;
//	int ammo;
//	int nades;
//	int activeWeap;
//	double angle; //in radians
//	int cur_frame;
//
//	Player *driver;
//	b2Body* cart;
//	b2Body* wheel1;
//	b2Body* wheel2;
//	b2RevoluteJoint* motor1;
//	b2RevoluteJoint* motor2;
//
//	float carWidth;
//	float carHeight;
//	float wheelRadius;
//
//};
//
//extern Map *map;
//extern int screen_w, screen_h;
//extern std::vector<Bullet*> *bullets;
//#endif /* VEHICLE_H_ */
