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
#include "gameloop.h"

float getTime() {
	return static_cast<float>(SDL_GetTicks()) / 1000.0f;
}

float getTimeSince(float& startTime, bool updateStart) {
	const float newTime = getTime();
	float diff = newTime - startTime;
	if (updateStart) {
		startTime = newTime;
	}
	return diff;
}

void delay(float seconds) {
	SDL_Delay(static_cast<int>(ceil(seconds * 1000.0f)));
}

GameLoop::GameLoop(float stateFps, bool limitFps) :
		dt(1.0f / stateFps), stateFps(stateFps), limitFps(limitFps) {
}

void GameLoop::runLoop() {
	float currentTime = getTime();
	printf("current time: %f",currentTime);
	float frameTimeLeft = 0.0f;
	while (continueLoop()) {
		frameTimeLeft -= getTimeSince(currentTime, true);
		float physicsFrames = 0.0f;
		while (frameTimeLeft <= 0.0f) {
			frameTimeLeft += dt;
			physicsFrames += 1.0f;
		}
		update(dt, physicsFrames);
		float timeLeft = frameTimeLeft - getTimeSince(currentTime, false);
		if (limitFps) {
			if (timeLeft > 0.0f) {
				render(1.0f);
			}
			timeLeft = frameTimeLeft - getTimeSince(currentTime, false);
			if (timeLeft > 0.0f) {
				delay(timeLeft);
			}
		} else {
//			while (timeLeft > 0.0f) {
//				render(1.0f - (timeLeft / dt));
//				timeLeft = frameTimeLeft - getTimeSince(currentTime, false);
//			}
		}
	}
}
GameLoop::~GameLoop(){}
