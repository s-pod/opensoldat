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

#include "player.h"
#include "bullet.h"

#define _USE_MATH_DEFINES
Player::Player(std::string name, float posX, float posY, Game *game) {
	this->game = game;
	this->name.empty();
	this->name.append(name);
	this->world = game->getWorld();
	this->ammo = 0;
	this->health = 100;
	this->activeWeap = 0;
	this->jet = 0;
	this->vest = 0;
	this->nades = 0;
	this->playerBody = NULL;
	this->shaderProgram = game->getShaderProg();
	screen_h = game->getScreen_h();
	screen_w = game->getScreen_w();
	state = standing;
	direction = left;
	cur_frame = 0;
	angle = 0;
	height = 0.8f;
	width = 0.25f;
	moveRight = moveDown = moveLeft = moveUp = 0;
	init();
}

void Player::shoot() {
	Bullet *b = new Bullet(this);
	game->addBullet(b);
	b->addToWorld(playerBody->GetPosition().x + 1.0f * std::sin(angle),
			playerBody->GetPosition().y + 1.0f * std::cos(angle));
}

b2World* Player::getWorld() {
	return world;
}

double Player::getAngle() {
	return angle;
}

b2Body* Player::getBody() {
	return playerBody;
}

void Player::setShooting(bool shoot) {
	shooting = shoot;
}

void Player::setMoveRight(bool right) {
	moveRight = right;
}
void Player::setMoveLeft(bool left) {
	moveLeft = left;
}
void Player::setMoveUp(bool up) {
	moveUp = up;
}
void Player::setMoveDown(bool down) {
	moveDown = down;
}

void Player::update(int x, int y) {
	angle = std::atan2((double) ((double) -screen_w / 2 + x),
			(double) ((double) screen_h / 2 - y));
	if (std::sin(angle) < 0) {
		direction = left;
		//std::cout << "left" << std::endl;
	} else {
		direction = right;
		//std::cout << "right" << std::endl;
	}
	if (moveLeft) {
		playerBody->ApplyForce(b2Vec2(-100.0f, 0.0f), b2Vec2(getX(), getY()));
		if (playerBody->GetLinearVelocity().x <= -10.0f) {
			playerBody->SetLinearVelocity(
					b2Vec2(-10.0f, playerBody->GetLinearVelocity().y));
		}
	}
	if (moveRight) {
		playerBody->ApplyForce(b2Vec2(100.0f, 0.0f), b2Vec2(getX(), getY()));
		if (playerBody->GetLinearVelocity().x >= 10.0f) {
			playerBody->SetLinearVelocity(
					b2Vec2(10.0f, playerBody->GetLinearVelocity().y));
		}
	}
	if (moveUp) {
		playerBody->SetBullet(true);
		playerBody->ApplyForce(b2Vec2(0, 100.0f), b2Vec2(getX(), getY()));
	}
	if (shooting) {
		shoot();
	}
}
void Player::init() {
	//TODO: player textures

	// Create Polygons Array Object
	glGenVertexArrays(1, &p_vao_poly);
	glBindVertexArray(p_vao_poly);

	// Create a Polygons Buffer Object and copy the vertex data to it
	glGenBuffers(1, &p_vbo_poly);
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo_poly);

	int size = 4 * 8;

	//fill Polygons array
	polygons = new float[size];

	for (int i = 0; i < 1; i++) {
		int m = 0;
		//add torso
		polygons[i * 4 * 8 + m * 8 + 0] = width / -2.0f * worldScale; //x
		polygons[i * 4 * 8 + m * 8 + 1] = height / 2.0f * worldScale; //y

		polygons[i * 4 * 8 + m * 8 + 2] = 1; //r
		polygons[i * 4 * 8 + m * 8 + 3] = 1; //g
		polygons[i * 4 * 8 + m * 8 + 4] = 1; //b
		polygons[i * 4 * 8 + m * 8 + 5] = 1; //a

		polygons[i * 4 * 8 + m * 8 + 6] = 1.0f;
		polygons[i * 4 * 8 + m * 8 + 7] = 0.0f;

		m++;
		polygons[i * 4 * 8 + m * 8 + 0] = width / 2.0f * worldScale; //x
		polygons[i * 4 * 8 + m * 8 + 1] = height / 2.0f * worldScale; //y

		polygons[i * 4 * 8 + m * 8 + 2] = 1; //r
		polygons[i * 4 * 8 + m * 8 + 3] = 1; //g
		polygons[i * 4 * 8 + m * 8 + 4] = 1; //b
		polygons[i * 4 * 8 + m * 8 + 5] = 1; //a

		polygons[i * 4 * 8 + m * 8 + 6] = 1.0f;
		polygons[i * 4 * 8 + m * 8 + 7] = 0.0f;

		m++;
		polygons[i * 4 * 8 + m * 8 + 0] = width / 2.0f * worldScale; //x
		polygons[i * 4 * 8 + m * 8 + 1] = height / -2.0f * worldScale; //y

		polygons[i * 4 * 8 + m * 8 + 2] = 1; //r
		polygons[i * 4 * 8 + m * 8 + 3] = 1; //g
		polygons[i * 4 * 8 + m * 8 + 4] = 1; //b
		polygons[i * 4 * 8 + m * 8 + 5] = 1; //a

		polygons[i * 4 * 8 + m * 8 + 6] = 1.0f;
		polygons[i * 4 * 8 + m * 8 + 7] = 0.0f;

		m++;
		polygons[i * 4 * 8 + m * 8 + 0] = width / -2.0f * worldScale; //x
		polygons[i * 4 * 8 + m * 8 + 1] = height / -2.0f * worldScale; //y

		polygons[i * 4 * 8 + m * 8 + 2] = 1; //r
		polygons[i * 4 * 8 + m * 8 + 3] = 1; //g
		polygons[i * 4 * 8 + m * 8 + 4] = 1; //b
		polygons[i * 4 * 8 + m * 8 + 5] = 1; //a

		polygons[i * 4 * 8 + m * 8 + 6] = 1.0f;
		polygons[i * 4 * 8 + m * 8 + 7] = 0.0f;
	}

	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), polygons,
			GL_STATIC_DRAW);

	// Specify the layout of the posititon data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			0);

	// Specify the layout of the color data
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*) (2 * sizeof(float)));

	// Specify the layout of the texture data
	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*) (6 * sizeof(float)));

}

void Player::draw() {
	glBindVertexArray(p_vao_poly);
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo_poly);
	glm::mat4 model;
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	//draw polygons
	model = glm::translate(model,
			glm::vec3(playerBody->GetPosition().x * worldScale,
					playerBody->GetPosition().y * worldScale, 0));
	model = glm::rotate(model, playerBody->GetAngle() / 0.0174532925f,
			glm::vec3(0, 0, 1));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glBindTexture(GL_TEXTURE_2D, 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

	glDrawArrays(GL_QUADS, 0, 4);
}

void Player::setSpawnPoint(tagPMS_SPAWNPOINT sp) {
	this->spawn_point = sp;
}

void Player::changeState(pSTATE state) {
	this->state = state;
}

void Player::addToWorld() {
	int ind = random();
	b2BodyDef playerDef;
	playerDef.position.Set(spawn_point.x / worldScale,
			spawn_point.y / worldScale);
	playerDef.type = b2_dynamicBody;

	playerBody = world->CreateBody(&playerDef);

	b2PolygonShape playerShape;
	playerShape.SetAsBox(width / 2.0f, height / 2.0f);

	b2FixtureDef playerFixture;
	playerFixture.density = 10;
	playerFixture.friction = 5;
	playerFixture.restitution = 0.2;
	playerFixture.filter.groupIndex = ind;
	playerFixture.shape = &playerShape;

	playerBody->CreateFixture(&playerFixture);

}

Game *Player::getGame() {
	return game;
}

Player::~Player() {
	playerBody->GetWorld()->DestroyBody(playerBody);
	delete polygons;
	glDeleteBuffers(1, &p_vbo_poly);
	glDeleteVertexArrays(1, &p_vao_poly);
}
