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

#include "map.h"
#include "global.h"

using namespace std;
extern GLuint shaderProgram;
Map::~Map() {
	delete pmap;
	glDeleteTextures(1, &texture);
	glDeleteTextures(pmap->scenery.size(), scenery_tex);
	delete (polygons);
	glDeleteBuffers(1, &map_vbo_poly);
	glDeleteVertexArrays(1, &map_vao_poly);
}

void Map::render(glm::mat4 *view) {
	// Clear the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glm::mat4 model;
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(map_vao_scenery);
	glBindBuffer(GL_ARRAY_BUFFER, map_vbo_scenery);

	//draw the background
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 1);
	glDrawArrays(GL_QUADS, 0, 4);
	glEnable(GL_BLEND);


	//draw scenery on the back
	for (unsigned int i = 0; i < pmap->prop.size(); i++) {
		if (pmap->prop[i].level != dbBEHIND_NONE) {
			glBindTexture(GL_TEXTURE_2D, scenery_tex[pmap->prop[i].style - 1]);
			glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

			float angle = (pmap->prop[i].rotation) * (180.0f / M_PI);

			model = glm::translate(model,
					glm::vec3(pmap->prop[i].x, -pmap->prop[i].y, 0));

			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::scale(model,
					glm::vec3(pmap->prop[i].scaleX, pmap->prop[i].scaleY, 0));

			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_QUADS, i * 4 + 4, 4);
			model = glm::mat4();
		}
	}

	//draw polygons
	model = glm::mat4();
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
	glBindVertexArray(map_vao_poly);
	glBindBuffer(GL_ARRAY_BUFFER, map_vbo_poly);
	glDrawArrays(GL_TRIANGLES, 0, pmap->polygonCount * 3);

	//draw scenery on the front
	glBindVertexArray(map_vao_scenery);
	glBindBuffer(GL_ARRAY_BUFFER, map_vbo_scenery);
	for (unsigned int i = 0; i < pmap->prop.size(); i++) {
		if (pmap->prop[i].level == dbBEHIND_NONE) {
			glBindTexture(GL_TEXTURE_2D, scenery_tex[pmap->prop[i].style - 1]);
			glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
			float angle = (pmap->prop[i].rotation) * (180.0f / M_PI);
			model = glm::translate(model,
					glm::vec3(pmap->prop[i].x, -pmap->prop[i].y, 0));
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model,
					glm::vec3(pmap->prop[i].scaleX, pmap->prop[i].scaleY, 0));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_QUADS, i * 4 + 4, 4);
			model = glm::mat4();
		}
	}
}

bool Map::load(char filename[]) {

	scenery_tex = NULL;
	// open the map
	std::ifstream mapf(filename, ios::binary);
	pms map;

	mapf >> map;
	mapf.close();
	pmap = new pms(map);

	mapdir = "";
	mapdir += filename;
	mapdir = mapdir.substr(0, mapdir.rfind("/"));
	init();
	return true;
}

void Map::init(void) {
	// Create Polygons Array Object
	glGenVertexArrays(1, &map_vao_poly);
	glBindVertexArray(map_vao_poly);

	// Create a Polygons Buffer Object and copy the vertex data to it
	glGenBuffers(1, &map_vbo_poly);
	glBindBuffer(GL_ARRAY_BUFFER, map_vbo_poly);

	int size = pmap->polygonCount * 3 * 8;
	printf("num of polygons: %d\n", pmap->polygonCount);

	//fill Polygons array
	polygons = new float[size];

	for (int i = 0; i < pmap->polygonCount; i++) {
		for (int m = 0; m < 3; m++) {
			polygons[i * 24 + m * 8 + 0] = pmap->polygon[i].vertex[m].x;
			polygons[i * 24 + m * 8 + 1] = -pmap->polygon[i].vertex[m].y;
			polygons[i * 24 + m * 8 + 2] =
					(float) pmap->polygon[i].vertex[m].color.red / 255.0f;
			polygons[i * 24 + m * 8 + 3] =
					(float) pmap->polygon[i].vertex[m].color.green / 255.0f;
			polygons[i * 24 + m * 8 + 4] =
					(float) pmap->polygon[i].vertex[m].color.blue / 255.0f;
			polygons[i * 24 + m * 8 + 5] =
					(float) pmap->polygon[i].vertex[m].color.alpha / 255.0f;
			polygons[i * 24 + m * 8 + 6] = pmap->polygon[i].vertex[m].tu;
			polygons[i * 24 + m * 8 + 7] = -pmap->polygon[i].vertex[m].tv;
		}

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

	//init scenery
	// Create Scenery Array Object
	glGenVertexArrays(1, &map_vao_scenery);
	glBindVertexArray(map_vao_scenery);

	// Create a Scenery Buffer Object and copy the vertex data to it
	glGenBuffers(1, &map_vbo_scenery);
	glBindBuffer(GL_ARRAY_BUFFER, map_vbo_scenery);

	int j = 0, k = 0;
	//fill scenery array
	scenery_arr = new float[pmap->prop.size() * 4 * 8 + 4 * 8];
	//add the background
	scenery_arr[j + 0] = pmap->leftoffs;
	scenery_arr[j + 1] = -pmap->topoffs;
	scenery_arr[j + 2] = pmap->bgColorTop.red / 255.0f;
	scenery_arr[j + 3] = pmap->bgColorTop.green / 255.0f;
	scenery_arr[j + 4] = pmap->bgColorTop.blue / 255.0f;
	scenery_arr[j + 5] = pmap->bgColorTop.alpha / 255.0f;
	j += 8;
	scenery_arr[j + 0] = pmap->rightoffs;
	scenery_arr[j + 1] = -pmap->topoffs;
	scenery_arr[j + 2] = pmap->bgColorTop.red / 255.0f;
	scenery_arr[j + 3] = pmap->bgColorTop.green / 255.0f;
	scenery_arr[j + 4] = pmap->bgColorTop.blue / 255.0f;
	scenery_arr[j + 5] = pmap->bgColorTop.alpha / 255.0f;
	j += 8;
	scenery_arr[j + 0] = pmap->rightoffs;
	scenery_arr[j + 1] = -pmap->bottomoffs;
	scenery_arr[j + 2] = pmap->bgColorBottom.red / 255.0f;
	scenery_arr[j + 3] = pmap->bgColorBottom.green / 255.0f;
	scenery_arr[j + 4] = pmap->bgColorBottom.blue / 255.0f;
	scenery_arr[j + 5] = pmap->bgColorBottom.alpha / 255.0f;
	j += 8;
	scenery_arr[j + 0] = pmap->leftoffs;
	scenery_arr[j + 1] = -pmap->bottomoffs;
	scenery_arr[j + 2] = pmap->bgColorBottom.red / 255.0f;
	scenery_arr[j + 3] = pmap->bgColorBottom.green / 255.0f;
	scenery_arr[j + 4] = pmap->bgColorBottom.blue / 255.0f;
	scenery_arr[j + 5] = pmap->bgColorBottom.alpha / 255.0f;

	for (size_t i = 0; i < pmap->prop.size(); i++) {
		//x,y
		scenery_arr[32 + i * 32 + k + 0] = 0;
		scenery_arr[32 + i * 32 + k + 1] = 0;
		//color
		scenery_arr[32 + i * 32 + k + 2] = pmap->prop[i].color.red / 255.0;
		scenery_arr[32 + i * 32 + k + 3] = pmap->prop[i].color.green / 255.0;
		scenery_arr[32 + i * 32 + k + 4] = pmap->prop[i].color.blue / 255.0;
		scenery_arr[32 + i * 32 + k + 5] = pmap->prop[i].alpha / 255.0;
		//texture mapping
		scenery_arr[32 + i * 32 + k + 6] = 0.0;
		scenery_arr[32 + i * 32 + k + 7] = 1.0;

		k += 8;
		scenery_arr[32 + i * 32 + k + 0] = pmap->prop[i].width;
		scenery_arr[32 + i * 32 + k + 1] = 0;

		scenery_arr[32 + i * 32 + k + 2] = pmap->prop[i].color.red / 255.0;
		scenery_arr[32 + i * 32 + k + 3] = pmap->prop[i].color.green / 255.0;
		scenery_arr[32 + i * 32 + k + 4] = pmap->prop[i].color.blue / 255.0;
		scenery_arr[32 + i * 32 + k + 5] = pmap->prop[i].alpha / 255.0;

		scenery_arr[32 + i * 32 + k + 6] = 1.0;
		scenery_arr[32 + i * 32 + k + 7] = 1.0;

		k += 8;
		scenery_arr[32 + i * 32 + k + 0] = pmap->prop[i].width;
		scenery_arr[32 + i * 32 + k + 1] = -pmap->prop[i].height;

		scenery_arr[32 + i * 32 + k + 2] = pmap->prop[i].color.red / 255.0;
		scenery_arr[32 + i * 32 + k + 3] = pmap->prop[i].color.green / 255.0;
		scenery_arr[32 + i * 32 + k + 4] = pmap->prop[i].color.blue / 255.0;
		scenery_arr[32 + i * 32 + k + 5] = pmap->prop[i].alpha / 255.0;

		scenery_arr[32 + i * 32 + k + 6] = 1.0;
		scenery_arr[32 + i * 32 + k + 7] = 0.0;

		k += 8;
		scenery_arr[32 + i * 32 + k + 0] = 0;
		scenery_arr[32 + i * 32 + k + 1] = -pmap->prop[i].height;

		scenery_arr[32 + i * 32 + k + 2] = pmap->prop[i].color.red / 255.0;
		scenery_arr[32 + i * 32 + k + 3] = pmap->prop[i].color.green / 255.0;
		scenery_arr[32 + i * 32 + k + 4] = pmap->prop[i].color.blue / 255.0;
		scenery_arr[32 + i * 32 + k + 5] = pmap->prop[i].alpha / 255.0;

		scenery_arr[32 + i * 32 + k + 6] = 0.0;
		scenery_arr[32 + i * 32 + k + 7] = 0.0;
		k = 0;
	}

	glBufferData(GL_ARRAY_BUFFER,
			(pmap->prop.size() * 4 * 8 + 4 * 8) * sizeof(float), scenery_arr,
			GL_STATIC_DRAW);

	// Specify the layout of the posititon data
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			0);

	// Specify the layout of the color data
	colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*) (2 * sizeof(float)));

	// Specify the layout of the texture data
	texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*) (6 * sizeof(float)));

	//init textures
	texture = load_texture();

	scenery_tex = new GLuint[pmap->scenery.size()];
	for (size_t i = 0; i < pmap->scenery.size(); i++)
		scenery_tex[i] = load_scenery(pmap->scenery[i].name.c_str());

}

bool Map::addToWorld(b2World *world) {
	for (int i = 0; i < pmap->polygonCount; i++) {
		if (pmap->polygon[i].polyType == ptNO_COLLIDE
				|| pmap->polygon[i].polyType == ptONLY_BULLETS_COLLIDE)
			continue;
		b2Vec2 vertices[3];
		for (int m = 0; m < 3; m++) {
			vertices[m].Set(
					(pmap->polygon[i].vertex[m].x - pmap->polygon[i].vertex[0].x)
							/ worldScale,
					-(pmap->polygon[i].vertex[m].y
							+ pmap->polygon[i].vertex[0].y) / worldScale);
		}
		for (int m = 0; m < 3; m++) {
			b2BodyDef edgeBodyDef;
			edgeBodyDef.position.Set(pmap->polygon[i].vertex[0].x / worldScale,
					pmap->polygon[i].vertex[0].y / worldScale);
			b2Body *edgeBody = world->CreateBody(&edgeBodyDef);
			b2EdgeShape edgeShape;
			if (m < 2) {
				edgeShape.Set(vertices[m], vertices[m + 1]);
			} else {
				edgeShape.Set(vertices[m], vertices[0]);
			}
			b2FixtureDef edgeFixDef;
			edgeFixDef.density = 0.0f;
			edgeFixDef.friction = 0.3f;
			edgeFixDef.restitution = 0.1f;
			edgeFixDef.shape = &edgeShape;
			edgeBody->CreateFixture(&edgeFixDef);

		}

	}

	return true;
}

// converts from 0-255 notation to opengl's [0,1]
void Map::glColor4fb(float r, float g, float b, float a) {
	glColor4f(r / 255.0, g / 255.0, b / 255, a / 255.0);
}

FIBITMAP* Map::load_image(const char* lpszPathName, FREE_IMAGE_FORMAT &fif) {
	fif = FIF_UNKNOWN;

	fif = FreeImage_GetFileType(lpszPathName, 0);
	if (fif == FIF_UNKNOWN) {
		cout << "Unknown image format." << endl;
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
	}

	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		FIBITMAP *dib = FreeImage_Load(fif, lpszPathName);
		return dib;
	}

	return NULL;
}

GLuint Map::load_gl_texture(const char* lpszPathName, bool wrap = true) {
	cout << "Loading texture: " + string(lpszPathName) << endl;

	GLuint t = 0;

	FREE_IMAGE_FORMAT fif;
	FIBITMAP *data, *dataOld = load_image(lpszPathName, fif);
	int bpp = FreeImage_GetBPP(dataOld);

	if (bpp != 32) {
		data = FreeImage_ConvertTo32Bits(dataOld);
		FreeImage_Unload(dataOld);
	} else {
		data = dataOld;
	}

	if (data == NULL) {
		cout << "Error loading image." << endl;
		return t;
	}

	unsigned int w = FreeImage_GetWidth(data);
	unsigned int h = FreeImage_GetHeight(data);

	// get a pointer to FreeImage's data.
	BYTE *pixels = (BYTE*) FreeImage_GetBits(data);
	BYTE v;
	// Iterate through the pixels, copying the data
	// from 'pixels' to 'bits' except in RGB format.
	for (unsigned int pix = 0; pix < (w * h); pix++) {
		if (pixels[pix * 4 + 0] == 0 && pixels[pix * 4 + 1] == 255
				&& pixels[pix * 4 + 2] == 0) {
			pixels[pix * 4 + 0] = 0;
			pixels[pix * 4 + 1] = 0;
			pixels[pix * 4 + 2] = 0;
			pixels[pix * 4 + 3] = 0;
		} else if (bpp < 32 || fif == FIF_BMP)
			pixels[pix * 4 + 3] = 255;

		v = pixels[pix * 4 + 0];
		pixels[pix * 4 + 0] = pixels[pix * 4 + 2];
		pixels[pix * 4 + 2] = v;
	}

	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			wrap ? GL_NEAREST : GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			wrap ? GL_NEAREST : GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE,
			pixels);

	FreeImage_Unload(data);

	return t;
}

GLuint Map::load_texture(void) {
	return load_gl_texture((mapdir + "/../Textures/" + pmap->texture).c_str(),
			true);
}

GLuint Map::load_scenery(const char* lpszPathName) {
	string fname = mapdir + "/../Scenery-gfx/" + lpszPathName;
	if (FILE *f = fopen(fname.c_str(), "r")) {
		fclose(f);
		return load_gl_texture(fname.c_str(), false);
	} else {
		cout << "File not found: " << fname;
	}

	return 0;
}
