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
#include "vehicle.h"
#include "bullet.h"

#define _USE_MATH_DEFINES

vehicle::vehicle(std::string name, b2World *world, float posX, float posY,
		Player *driver) {
	this->name.empty();
	this->name.append(name);
	this->world = world;
	this->ammo = 0;
	this->health = 100;
	this->activeWeap = 0;
	this->jet = 0;
	this->vest = 0;
	this->nades = 0;
	this->playerBody = NULL;
	this->driver = driver;
	cur_frame = 0;
	cart = NULL;
	wheel1 = NULL;
	wheel2 = NULL;
	motor1 = NULL;
	motor2 = NULL;
	carHeight = carWidth = wheelRadius = 0;
	angle = 0;
}
void vehicle::shoot() {
	Bullet *b = new Bullet(this->driver);
	bullets->push_back(b);
	b->addToWorld(cart->GetPosition().x + 1.0f * std::sin(angle),
			cart->GetPosition().y + 1.0f * std::cos(angle));
}

b2World* vehicle::getWorld() {
	return world;
}

double vehicle::getAngle() {
	return angle;
}

b2RevoluteJoint* vehicle::getMotor(int n) {
	if (n == 1) {
		return motor1;
	} else {
		return motor2;
	}
}
b2Body* vehicle::getBody() {
	return cart;
}

void vehicle::update() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	angle = std::atan2((double) ((double) -screen_w / 2 + x),
			(double) ((double) screen_h / 2 - y));
}

Player *vehicle::getDriver() {
	return driver;
}

void vehicle::draw() {
	//draw cart
	glPushMatrix();
	glTranslatef(cart->GetPosition().x * worldScale,
			cart->GetPosition().y * worldScale, 0.0f);
	glRotatef(cart->GetAngle() / 0.0174532925f, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 1);
	glVertex3f((-carWidth / 2.0f) * worldScale, (carHeight / 2.0f) * worldScale,
			0.0f);
	glVertex3f((carWidth / 2.0f) * worldScale, (carHeight / 2.0f) * worldScale,
			0.0f);
	glVertex3f((carWidth / 2.0f) * worldScale, (-carHeight / 2.0f) * worldScale,
			0.0f);
	glVertex3f((-carWidth / 2.0f) * worldScale,
			(-carHeight / 2.0f) * worldScale, 0.0f);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(cart->GetPosition().x * worldScale,
			cart->GetPosition().y * worldScale, 0.0f);
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0.0f);
	glVertex3f(1.0f * std::sin(angle) * worldScale,
			1.0f * std::cos(angle) * worldScale, 0.0f);
	glEnd();
	glPopMatrix();
	//draw wheels
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float degInRad = i * 0.0174532925f;
		glVertex2f(
				wheel1->GetPosition().x * worldScale
						+ cos(degInRad) * wheelRadius * worldScale,
				(wheel1->GetPosition().y) * worldScale
						+ sin(degInRad) * wheelRadius * worldScale);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float degInRad = i * 0.0174532925f;
		glVertex2f(
				wheel2->GetPosition().x * worldScale
						+ cos(degInRad) * wheelRadius * worldScale,
				(wheel2->GetPosition().y) * worldScale
						+ sin(degInRad) * wheelRadius * worldScale);
	}

	glEnd();

}
void vehicle::setSpawnPoint(tagPMS_SPAWNPOINT sp) {
	this->spawn_point = sp;
}

void vehicle::addToWorld() {
	int ind;
	ind = random();
//cart
	b2BodyDef cartDef;
	cartDef.position.Set(spawn_point.x / worldScale,
			spawn_point.y / worldScale);
	cartDef.type = b2_dynamicBody;

	cart = world->CreateBody(&cartDef);

	carWidth = 2, carHeight = 0.5;
	wheelRadius = 0.5;

	b2PolygonShape cartShape;
	cartShape.SetAsBox(carWidth / 2.0f, carHeight / 2.0f);

	b2FixtureDef cartFixture;
	cartFixture.density = 4;
	cartFixture.friction = 0.5;
	cartFixture.restitution = 0.2;
	cartFixture.filter.groupIndex = ind;
	cartFixture.shape = &cartShape;

	cart->CreateFixture(&cartFixture);

//wheels
	b2BodyDef wheelDef;
	wheelDef.type = b2_dynamicBody;

	wheelDef.position.Set((spawn_point.x - 1 * worldScale) / worldScale,
			(spawn_point.y) / worldScale);
	wheel1 = world->CreateBody(&wheelDef);

	wheelDef.position.Set((spawn_point.x + 1 * worldScale) / worldScale,
			(spawn_point.y) / worldScale);
	wheel2 = world->CreateBody(&wheelDef);

	b2CircleShape wheelShape;
	wheelShape.m_radius = wheelRadius;

	b2FixtureDef wheelFixture;
	wheelFixture.density = 0.2;
	wheelFixture.friction = 5;
	wheelFixture.restitution = 0.2;
	wheelFixture.filter.groupIndex = ind;
	wheelFixture.shape = &wheelShape;

	wheel1->CreateFixture(&wheelFixture);
	wheel2->CreateFixture(&wheelFixture);

	b2RevoluteJointDef wheelJointDef;
	wheelJointDef.enableMotor = true;
	wheelJointDef.maxMotorTorque = 1000;

	wheelJointDef.Initialize(wheel1, cart, wheel1->GetWorldCenter());
	motor1 = (b2RevoluteJoint*) (world->CreateJoint(&wheelJointDef));

	wheelJointDef.Initialize(wheel2, cart, wheel2->GetWorldCenter());
	motor2 = (b2RevoluteJoint*) (world->CreateJoint(&wheelJointDef));
///////////////////////////////////////////////////
}

vehicle::~vehicle() {

}
