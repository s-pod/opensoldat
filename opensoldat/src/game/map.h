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

#ifndef MAP_H
#define MAP_H

#include "global.h"

//prims
template<class T> T read_bin(std::istream& is) {
	T tmp;
	is.read(reinterpret_cast<char*>(&tmp), sizeof(T));
	return tmp;
}

inline std::string read_string(std::istream& is, int full_length) {
	int length = read_bin<char>(is);
	std::vector<char> tmp(full_length);
	is.read(&tmp[0], tmp.size());
	return std::string(tmp.begin(), tmp.begin() + length);
}

// Basic aux structures
/*typedef enum TRUEFALSE
 {
 FALSE = 0,
 TRUE
 } bool;*/

typedef enum POLYTYPE {
	ptNORMAL = 0,
	ptONLY_BULLETS_COLLIDE,
	ptONLY_PLAYERS_COLLIDE,
	ptNO_COLLIDE,
	ptICE,
	ptDEADLY,
	ptBLOODY_DEADLY,
	ptHURTS,
	ptREGENERATES,
	ptLAVA
} PMS_POLYTYPE;

typedef enum DRAWBEHIND {
	dbBEHIND_ALL = 0, dbBEHIND_MAP, dbBEHIND_NONE
} PMS_DRAWBEHIND;

typedef enum SPECIALACTIONS {
	saNONE = 0,
	saSTOP_AND_CAMP,
	saWAIT_1_SECOND,
	saWAIT_5_SECONDS,
	saWAIT_10_SECONDS,
	saWAIT_15_SECONDS,
	saWAIT_20_SECONDS
} PMS_SPECIALACTIONS;

typedef enum WEATHERTYPE {
	wtNONE = 0, wtRAIN, wtSANDSTORM, wtSNOW
} PMS_WEATHERTYPE;

typedef enum STEPSTYPE {
	stHARD_GROUND = 0, stSOFT_GROUND, stNONE
} PMS_STEPSTYPE;

typedef enum SPAWNTEAM {
	stGENERAL = 0,
	stALPHA,
	stBRAVO,
	stCHARLIE,
	stDELTA,
	stALPHA_FLAG,
	stBRAVO_FLAG,
	stGRENADES,
	stMEDKITS,
	stCLUSTERS,
	stVEST,
	stFLAMER,
	stBERSERKER,
	stPREDATOR,
	stYELLOW_FLAG,
	stRAMBO_BOW,
	stSTAT_GUN
} PMS_SPAWNTEAM;

typedef struct tagPMS_VECTOR {
	FLOAT x;
	FLOAT y;
	FLOAT z;
} PMS_VECTOR;

typedef struct tagPMS_COLOR {
	UBYTE blue;
	UBYTE green;
	UBYTE red;
	UBYTE alpha;
} PMS_COLOR;

// more stuff

typedef struct tagPMS_VERTEX {
	float x;
	float y;
	float z;
	float rhw;
	PMS_COLOR color;
	float tu;
	float tv;
} PMS_VERTEX;

typedef struct tagPMS_POLYGON {
	PMS_VERTEX vertex[3];
	PMS_VECTOR perpendicular[3];
	PMS_POLYTYPE polyType;
} PMS_POLYGON;

typedef struct tagPMS_SECTOR {
	WORD polyCount;
	std::vector<WORD> polys;
} PMS_SECTOR;

typedef struct tagPMS_PROP {
	bool active;
	UBYTE filler1;
	WORD style;
	LONG width;
	LONG height;
	float x;
	float y;
	float rotation;
	float scaleX;
	float scaleY;
	UBYTE alpha;
	UBYTE filler2[3];
	PMS_COLOR color;
	PMS_DRAWBEHIND level;
	UBYTE filler3[3];
} PMS_PROP;

typedef struct tagDOSTIME {
	WORD second :5;
	WORD minute :6;
	WORD hour :5;
} DOSTIME;

typedef struct tagDOSDATE {
	WORD day :5;
	WORD month :4;
	WORD year :7;
} DOSDATE;

typedef struct tagPMS_TIMESTAMP {
	DOSTIME time;
	DOSDATE date;
} PMS_TIMESTAMP;

typedef struct tagPMS_SCENERY {
	std::string name;
	PMS_TIMESTAMP timestamp;
} PMS_SCENERY;

typedef struct tagPMS_COLLIDER {
	bool active;
	UBYTE filler[3];
	float x;
	float y;
	float radius;
} PMS_COLLIDER;

typedef struct tagPMS_SPAWNPOINT {
	bool active;
	UBYTE filler[3];
	LONG x;
	LONG y;
	PMS_SPAWNTEAM team;
} PMS_SPAWNPOINT;

typedef struct tagPMS_WAYPOINT {
	bool active;
	UBYTE filler1[3];
	LONG id;
	LONG x;
	LONG y;
	bool left;
	bool right;
	bool up;
	bool down;
	bool jet;
	UBYTE path;
	PMS_SPECIALACTIONS specialAction;
	UBYTE c2;
	UBYTE c3;
	UBYTE filler2[3];
	LONG numConnections;
	LONG connections[20];
} PMS_WAYPOINT;

// Pms structure

struct pms {
	LONG version;
	std::string name;
	std::string texture;
	PMS_COLOR bgColorTop;
	PMS_COLOR bgColorBottom;
	LONG jetAmount;
	UBYTE grenades;
	UBYTE medikits;
	PMS_WEATHERTYPE weather;
	PMS_STEPSTYPE steps;
	LONG randID;
	LONG polygonCount;
	std::vector<PMS_POLYGON> polygon;
	LONG sectorDivisions;
	LONG numSectors;
	std::vector<PMS_SECTOR> sector;
	LONG propCount;
	std::vector<PMS_PROP> prop;
	LONG sceneryCount;
	std::vector<PMS_SCENERY> scenery;
	LONG colliderCount;
	std::vector<PMS_COLLIDER> collider;
	LONG spawnpointCount;
	std::vector<PMS_SPAWNPOINT> spawnpoint;
	LONG waypointCount;
	std::vector<PMS_WAYPOINT> waypoint;
	FLOAT leftoffs;
	FLOAT rightoffs;
	FLOAT topoffs;
	FLOAT bottomoffs;
};

inline std::istream& operator>>(std::istream& is, pms& p) {
	p.version = read_bin<int>(is);
	p.name = read_string(is, 38);
	p.texture = read_string(is, 24);
	p.bgColorTop = read_bin<PMS_COLOR>(is);
	p.bgColorBottom = read_bin<PMS_COLOR>(is);
	p.jetAmount = read_bin<LONG>(is);
	p.grenades = read_bin<UBYTE>(is);
	p.medikits = read_bin<UBYTE>(is);
	p.weather = PMS_WEATHERTYPE(read_bin<UBYTE>(is));
	p.steps = PMS_STEPSTYPE(read_bin<UBYTE>(is));
	p.randID = read_bin<LONG>(is);
	p.polygonCount = read_bin<LONG>(is);

	for (int i = 0; i < p.polygonCount; i++) {
		PMS_POLYGON temp; // cheap solution
		temp.vertex[0] = read_bin<PMS_VERTEX>(is);
		temp.vertex[1] = read_bin<PMS_VERTEX>(is);
		temp.vertex[2] = read_bin<PMS_VERTEX>(is);
		temp.perpendicular[0] = read_bin<PMS_VECTOR>(is);
		temp.perpendicular[1] = read_bin<PMS_VECTOR>(is);
		temp.perpendicular[2] = read_bin<PMS_VECTOR>(is);
		temp.polyType = PMS_POLYTYPE(read_bin<UBYTE>(is));
		p.polygon.push_back(temp);
	}
	p.sectorDivisions = read_bin<LONG>(is);
	p.numSectors = read_bin<LONG>(is);

	p.topoffs = p.sectorDivisions * -p.numSectors;
	p.bottomoffs = p.sectorDivisions * p.numSectors;
	p.leftoffs = p.sectorDivisions * -p.numSectors;
	p.rightoffs = p.sectorDivisions * p.numSectors;

	for (int i = 0; i < ((p.numSectors * 2) + 1) * ((p.numSectors * 2) + 1);
			i++) {
		PMS_SECTOR temp;
		temp.polyCount = read_bin<WORD>(is);
		for (int m = 0; m < temp.polyCount; m++) {
			temp.polys.push_back(read_bin<WORD>(is));
		}
		p.sector.push_back(temp);
	}
	p.propCount = read_bin<LONG>(is);
	for (int i = 0; i < p.propCount; i++) {
		PMS_PROP temp;
		temp.active = bool(read_bin<UBYTE>(is));
		temp.filler1 = read_bin<UBYTE>(is);
		temp.style = read_bin<WORD>(is);
		temp.width = read_bin<LONG>(is);
		temp.height = read_bin<LONG>(is);
		temp.x = read_bin<FLOAT>(is);
		temp.y = read_bin<FLOAT>(is);
		temp.rotation = read_bin<FLOAT>(is);
		temp.scaleX = read_bin<FLOAT>(is);
		temp.scaleY = read_bin<FLOAT>(is);
		temp.alpha = read_bin<UBYTE>(is);
		temp.filler2[0] = read_bin<UBYTE>(is);
		temp.filler2[1] = read_bin<UBYTE>(is);
		temp.filler2[2] = read_bin<UBYTE>(is);
		temp.color = read_bin<PMS_COLOR>(is);
		temp.level = PMS_DRAWBEHIND(read_bin<UBYTE>(is));
		temp.filler3[0] = read_bin<UBYTE>(is);
		temp.filler3[1] = read_bin<UBYTE>(is);
		temp.filler3[2] = read_bin<UBYTE>(is);
		p.prop.push_back(temp);
	}
	p.sceneryCount = read_bin<LONG>(is);
	for (int i = 0; i < p.sceneryCount; i++) {
		PMS_SCENERY temp;
		//temp.nameLen = read_bin<UBYTE>( is );
		temp.name = read_string(is, 50);
		temp.timestamp = read_bin<PMS_TIMESTAMP>(is);
		p.scenery.push_back(temp);
	}
	p.colliderCount = read_bin<LONG>(is);
	for (int i = 0; i < p.colliderCount; i++) {
		PMS_COLLIDER temp;
		temp.active = bool(read_bin<UBYTE>(is));
		temp.filler[0] = read_bin<UBYTE>(is);
		temp.filler[1] = read_bin<UBYTE>(is);
		temp.filler[2] = read_bin<UBYTE>(is);
		temp.x = read_bin<FLOAT>(is);
		temp.y = read_bin<FLOAT>(is);
		temp.radius = read_bin<FLOAT>(is);
		p.collider.push_back(temp);
	}
	p.spawnpointCount = read_bin<LONG>(is);
	for (int i = 0; i < p.spawnpointCount; i++) {
		PMS_SPAWNPOINT temp;
		temp.active = bool(read_bin<UBYTE>(is));
		temp.filler[0] = read_bin<UBYTE>(is);
		temp.filler[1] = read_bin<UBYTE>(is);
		temp.filler[2] = read_bin<UBYTE>(is);
		temp.x = read_bin<LONG>(is);
		temp.y = read_bin<LONG>(is);
		temp.team = PMS_SPAWNTEAM(read_bin<ULONG>(is));
		p.spawnpoint.push_back(temp);
	}
	p.waypointCount = read_bin<LONG>(is);
	for (int i = 0; i < p.waypointCount; i++) {
		PMS_WAYPOINT temp;
		temp.active = bool(read_bin<UBYTE>(is));
		temp.filler1[0] = read_bin<UBYTE>(is);
		temp.filler1[1] = read_bin<UBYTE>(is);
		temp.filler1[2] = read_bin<UBYTE>(is);
		temp.id = read_bin<LONG>(is);
		temp.x = read_bin<LONG>(is);
		temp.y = read_bin<LONG>(is);
		temp.left = bool(read_bin<UBYTE>(is));
		temp.right = bool(read_bin<UBYTE>(is));
		temp.up = bool(read_bin<UBYTE>(is));
		temp.down = bool(read_bin<UBYTE>(is));
		temp.jet = bool(read_bin<UBYTE>(is));
		temp.path = read_bin<UBYTE>(is);
		temp.specialAction = PMS_SPECIALACTIONS(read_bin<UBYTE>(is));
		temp.c2 = read_bin<UBYTE>(is);
		temp.c3 = read_bin<UBYTE>(is);
		temp.filler2[0] = read_bin<UBYTE>(is);
		temp.filler2[1] = read_bin<UBYTE>(is);
		temp.filler2[2] = read_bin<UBYTE>(is);
		temp.numConnections = read_bin<LONG>(is);
		for (int c = 0; c < 20; c++) {
			temp.connections[c] = read_bin<LONG>(is);
		}
		p.waypoint.push_back(temp);
	}
	return is;
}

class Map {
public:
	Map(GLuint shaderProgram);
	~Map();
	void renderOnBack();
	bool load(char filename[]);
	bool addToWorld(b2World *world);
	void renderOnFront();
	std::string getName() {
		return pmap->name;
	}

	std::string getMapDir() {
		return mapdir;
	}
	std::vector<tagPMS_SPAWNPOINT> getSpawnPoints(void) {
		return pmap->spawnpoint;
	}
	FLOAT getLeftoffs() {
		return pmap->leftoffs;
	}
	FLOAT getRightoffs() {
		return pmap->rightoffs;
	}
	FLOAT getTopoffs() {
		return pmap->topoffs;
	}
	FLOAT getBottomffs() {
		return pmap->bottomoffs;
	}

private:
	pms *pmap;
	GLuint texture;
	GLuint *scenery_tex;
	std::string mapdir;
	void init(void);
	GLuint load_texture(void);
	void glColor4fb(float, float, float, float);
	FIBITMAP* load_image(const char*, FREE_IMAGE_FORMAT &);
	GLuint load_gl_texture(const char*, bool);
	GLuint load_scenery(const char*);
	GLuint map_vao_poly;
	GLuint map_vbo_poly;
	GLuint map_vbo_scenery;
	GLuint map_vao_scenery;
	float *polygons;
	float *scenery_arr;
	GLuint shaderProgram;
};
#endif
