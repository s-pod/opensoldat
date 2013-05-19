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
#include "event.h"

Event::Event() {
}

Event::~Event() {
}

void Event::onEvent(SDL_Event* event) {
	switch (event->type) {
	case SDL_ACTIVEEVENT: {
		switch (event->active.state) {
		case SDL_APPMOUSEFOCUS: {
			if (event->active.gain) {
				onMouseFocus();
			} else {
				onMouseBlur();
			}
			break;
		}
		case SDL_APPINPUTFOCUS: {
			if (event->active.gain) {
				onInputFocus();
			} else {
				onInputBlur();
			}
			break;
		}
		case SDL_APPACTIVE: {
			if (event->active.gain) {
				onRestore();
			} else {
				onMinimize();
			}
			break;
		}
		}
		break;
	}

	case SDL_KEYDOWN: {
		onKeyDown(event->key.keysym.sym, event->key.keysym.mod,
				event->key.keysym.unicode);
		break;
	}

	case SDL_KEYUP: {
		onKeyUp(event->key.keysym.sym, event->key.keysym.mod,
				event->key.keysym.unicode);
		break;
	}

	case SDL_MOUSEMOTION: {
		onMouseMove(event->motion.x, event->motion.y, event->motion.xrel,
				event->motion.yrel,
				(event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))!= 0,
				(event->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
				(event->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);break;
        }

		case SDL_MOUSEBUTTONDOWN:
		{
			switch(event->button.button)
			{
				case SDL_BUTTON_LEFT:
				{
					onLButtonDown(event->button.x, event->button.y);
					break;
				}
				case SDL_BUTTON_RIGHT:
				{
					onRButtonDown(event->button.x, event->button.y);
					break;
				}
				case SDL_BUTTON_MIDDLE:
				{
					onMButtonDown(event->button.x, event->button.y);
					break;
				}
				case SDL_BUTTON_WHEELUP:
				{
					onMouseWheel(true,false);
					break;
				}
				case SDL_BUTTON_WHEELDOWN:
				{
					onMouseWheel(false,true);
					break;
				}

			}
			break;
		}

		case SDL_MOUSEBUTTONUP:
		{
			switch(event->button.button)
			{
				case SDL_BUTTON_LEFT:
				{
					onLButtonUp(event->button.x, event->button.y);
					break;
				}
				case SDL_BUTTON_RIGHT:
				{
					onRButtonUp(event->button.x, event->button.y);
					break;
				}
				case SDL_BUTTON_MIDDLE:
				{
					onMButtonUp(event->button.x, event->button.y);
					break;
				}
			}
			break;
		}

		case SDL_JOYAXISMOTION:
		{
			onJoyAxis(event->jaxis.which, event->jaxis.axis, event->jaxis.value);
			break;
		}

		case SDL_JOYBALLMOTION:
		{
			onJoyBall(event->jball.which, event->jball.ball, event->jball.xrel, event->jball.yrel);
			break;
		}

		case SDL_JOYHATMOTION:
		{
			onJoyHat(event->jhat.which, event->jhat.hat, event->jhat.value);
			break;
		}
		case SDL_JOYBUTTONDOWN:
		{
			onJoyButtonDown(event->jbutton.which, event->jbutton.button);
			break;
		}

		case SDL_JOYBUTTONUP:
		{
			onJoyButtonUp(event->jbutton.which, event->jbutton.button);
			break;
		}

		case SDL_QUIT:
		{
			onExit();
			break;
		}

		case SDL_SYSWMEVENT:
		{
			break;
		}

		case SDL_VIDEORESIZE:
		{
			onResize(event->resize.w, event->resize.h);
			break;
		}

		case SDL_VIDEOEXPOSE:
		{
			onExpose();
			break;
		}

		default:
		{
			onUser(event->user.type, event->user.code, event->user.data1, event->user.data2);
			break;
		}
	}
}

void Event::onInputFocus() {
}

void Event::onInputBlur() {
}

void Event::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
}

void Event::onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
}

void Event::onMouseFocus() {
}

void Event::onMouseBlur() {
}

void Event::onMouseMove(int mX, int mY, int relX, int relY, bool left,
		bool right, bool middle) {
}

void Event::onMouseWheel(bool up, bool down) {
}

void Event::onLButtonDown(int mX, int mY) {
}

void Event::onLButtonUp(int mX, int mY) {
}

void Event::onRButtonDown(int mX, int mY) {
}

void Event::onRButtonUp(int mX, int mY) {
}

void Event::onMButtonDown(int mX, int mY) {
}

void Event::onMButtonUp(int mX, int mY) {
}

void Event::onJoyAxis(Uint8 which, Uint8 axis, Sint16 value) {
}

void Event::onJoyButtonDown(Uint8 which, Uint8 button) {
}

void Event::onJoyButtonUp(Uint8 which, Uint8 button) {
}

void Event::onJoyHat(Uint8 which, Uint8 hat, Uint8 value) {
}

void Event::onJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel) {
}

void Event::onMinimize() {
}

void Event::onRestore() {
}

void Event::onResize(int w, int h) {
}

void Event::onExpose() {
}

void Event::onExit() {
}

void Event::onUser(Uint8 type, int code, void* data1, void* data2) {
}
