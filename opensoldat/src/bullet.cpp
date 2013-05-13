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
#include "bullet.h"
#include "vehicle.h"
#include "player.h"

extern GLuint shaderProgram;

Bullet::Bullet(Player *owner) {
	// TODO Auto-generated constructor stub
	//create bullet
	this->owner = owner;
	force = 80;
	radius = 0.015f;
	bullet = NULL;
	time(&t);
	init();
}

void Bullet::init() {
	//TODO: player textures

	// Create Polygons Array Object
	glGenVertexArrays(1, &b_vao_poly);
	glBindVertexArray(b_vao_poly);

	// Create a Polygons Buffer Object and copy the vertex data to it
	glGenBuffers(1, &b_vbo_poly);
	glBindBuffer(GL_ARRAY_BUFFER, b_vbo_poly);

	int size = 4 * 8;

	//fill Polygons array
	polygons = new float[size];

	for (int i = 0; i < 1; i++) {
		int m = 0;
		//bullet
		polygons[i * 4 * 8 + m * 8 + 0] = -radius * worldScale; //x
		polygons[i * 4 * 8 + m * 8 + 1] = radius * worldScale; //y

		polygons[i * 4 * 8 + m * 8 + 2] = 1; //r
		polygons[i * 4 * 8 + m * 8 + 3] = 1; //g
		polygons[i * 4 * 8 + m * 8 + 4] = 1; //b
		polygons[i * 4 * 8 + m * 8 + 5] = 1; //a

		polygons[i * 4 * 8 + m * 8 + 6] = 1.0f;
		polygons[i * 4 * 8 + m * 8 + 7] = 0.0f;

		m++;
		polygons[i * 4 * 8 + m * 8 + 0] = radius * worldScale; //x
		polygons[i * 4 * 8 + m * 8 + 1] = radius * worldScale; //y

		polygons[i * 4 * 8 + m * 8 + 2] = 1; //r
		polygons[i * 4 * 8 + m * 8 + 3] = 1; //g
		polygons[i * 4 * 8 + m * 8 + 4] = 1; //b
		polygons[i * 4 * 8 + m * 8 + 5] = 1; //a

		polygons[i * 4 * 8 + m * 8 + 6] = 1.0f;
		polygons[i * 4 * 8 + m * 8 + 7] = 0.0f;

		m++;
		polygons[i * 4 * 8 + m * 8 + 0] = radius * worldScale; //x
		polygons[i * 4 * 8 + m * 8 + 1] = -radius * worldScale; //y

		polygons[i * 4 * 8 + m * 8 + 2] = 1; //r
		polygons[i * 4 * 8 + m * 8 + 3] = 1; //g
		polygons[i * 4 * 8 + m * 8 + 4] = 1; //b
		polygons[i * 4 * 8 + m * 8 + 5] = 1; //a

		polygons[i * 4 * 8 + m * 8 + 6] = 1.0f;
		polygons[i * 4 * 8 + m * 8 + 7] = 0.0f;

		m++;
		polygons[i * 4 * 8 + m * 8 + 0] = -radius * worldScale; //x
		polygons[i * 4 * 8 + m * 8 + 1] = -radius * worldScale; //y

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

void Bullet::addToWorld(float32 x, float32 y) {
	b2CircleShape bulletShape;
	bulletShape.m_radius = radius;

	b2FixtureDef bulletFixture;
	bulletFixture.density = 50;
	bulletFixture.friction = 0.5;
	bulletFixture.restitution = 0.0;
	//bulletFixture.filter.groupIndex = 0;
	bulletFixture.shape = &bulletShape;

	b2BodyDef bulletDef;
	bulletDef.type = b2_dynamicBody;
	bulletDef.bullet = true;

	bulletDef.position.Set(x, y);
	bullet = owner->getWorld()->CreateBody(&bulletDef);
	bullet->CreateFixture(&bulletFixture);
	bullet->ApplyForce(
			b2Vec2(std::sin(owner->getAngle()) * force,
					std::cos(owner->getAngle()) * force), b2Vec2(x, y));
}

void Bullet::draw() {

	glBindVertexArray(b_vao_poly);
	glBindBuffer(GL_ARRAY_BUFFER, b_vbo_poly);
	glm::mat4 model;
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	//draw polygons
	model = glm::translate(model,
			glm::vec3(bullet->GetPosition().x * worldScale,
					bullet->GetPosition().y * worldScale, 0));
	model = glm::rotate(model, bullet->GetAngle() / 0.0174532925f,
			glm::vec3(0, 0, 1));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glBindTexture(GL_TEXTURE_2D, 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

	glDrawArrays(GL_QUADS, 0, 4);
}

time_t Bullet::getTime() {
	return t;
}

Bullet::~Bullet() {
	// TODO Auto-generated destructor stub
	bullet->GetWorld()->DestroyBody(bullet);
	delete polygons;
	glDeleteBuffers(1, &b_vbo_poly);
	glDeleteVertexArrays(1, &b_vao_poly);
}

