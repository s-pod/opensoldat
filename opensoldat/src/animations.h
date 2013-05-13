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
#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_
typedef struct tagVERTEX {
	double x;
	double y;
	double z;
} VERTEX;

typedef struct taganimFRAME {
	std::vector<VERTEX> *bodyParts;
} animFRAME;

typedef enum BODYPARTS {
	left_foot = 0,
	right_foot = 1,
	right_leg_lower = 2,
	left_leg_lower = 3,
	left_leg_upper = 4,
	right_leg_upper = 5,
	right_torso = 6,
	left_torso = 7,
	neck_bottom = 8,
	left_shoulder = 9,
	right_shoulder = 10,
	head = 11,
	left_arm = 12,
	right_arm = 13,
	right_hand = 14,
	left_hand = 15,
	left_ground_point = 16,
	right_ground_point = 17,
	right_special = 18,
	left_special = 19
} anim_BODYPARTS;



class animations {
public:
	animations();
	virtual ~animations();
	void loadAnimations();
private:
	int loadAnimation(const char *filename, std::vector<animFRAME> *anim);
	std::map<std::string,std::vector<animFRAME> > anim_arr;
};

#endif /* ANIMATIONS_H_ */
