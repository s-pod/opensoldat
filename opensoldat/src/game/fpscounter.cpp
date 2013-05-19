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

#include "fpscounter.h"

void FpsCounter::countFrame() {
	float time = getTime();
	FrameData* f = new FrameData();
	f->time = time;
	while (!counted.empty()) {
		FrameData* e = counted.back();
		if (e->time + 1.0f > time) {
			break;
		}
		delete e;
		counted.pop_back();
	}
	counted.push_front(f);
	f->fps = static_cast<float>(counted.size());
}

float FpsCounter::getCurrentFps() {
	if (counted.empty()) {
		return 0.0f;
	}
	return counted.front()->fps;
}

float FpsCounter::getAverageFps() {
	if (counted.empty()) {
		return 0.0f;
	}
	float fps = 0.0f;
	std::list<FrameData*>::iterator it;
	for (it = counted.begin(); it != counted.end(); ++it) {
		fps += (*it)->fps;
	}
	return fps / static_cast<float>(counted.size());
}
