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
#ifndef EVENT_H
#define EVENT_H

#include "global.h"

class Event
{
public:
    Event();
    virtual ~Event();

    virtual void onEvent(SDL_Event* event);

    virtual void onInputFocus();
    virtual void onInputBlur();

    virtual void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    virtual void onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

    virtual void onMouseFocus();
    virtual void onMouseBlur();
    virtual void onMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
    virtual void onMouseWheel(bool Up, bool Down);    // not implemented

    virtual void onLButtonDown(int mX, int mY);
    virtual void onLButtonUp(int mX, int mY);
    virtual void onRButtonDown(int mX, int mY);
    virtual void onRButtonUp(int mX, int mY);
    virtual void onMButtonDown(int mX, int mY);
    virtual void onMButtonUp(int mX, int mY);

    virtual void onJoyAxis(Uint8 which, Uint8 axis, Sint16 value);
    virtual void onJoyButtonDown(Uint8 which, Uint8 button);
    virtual void onJoyButtonUp(Uint8 which, Uint8 button);
    virtual void onJoyHat(Uint8 which, Uint8 hat, Uint8 value);
    virtual void onJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);

    virtual void onMinimize();
    virtual void onRestore();
    virtual void onResize(int w, int h);
    virtual void onExpose();
    virtual void onExit();

    virtual void onUser(Uint8 type, int code, void* data1, void* data2);

/*
    
    // TODO: integrate static keystate buffers

    // set up static keystate buffers
    // these keystate buffers can be accessed anytime 
    // independent of when events happen to determine keyboard input
    static BYTE keyStateOld[SDLK_LAST] = {0};
    static BYTE keyPressed[SDLK_LAST] = {0};
    static BYTE keyReleased[SDLK_LAST] = {0};
    static int keyStateCount;
    static BYTE *keyState = SDL_GetKeyState( &keyStateCount ); 
    for (int i = 0; i < keyStateCount; i++){
        keyPressed[i] = keyState[i] && (!keyStateOld[i]);
        keyReleased[i] = (!keyState[i]) && keyStateOld[i];
        keyStateOld[i] = keyState[i];
        }

    SDL_GetMouseState(&mouseScreen.x, &mouseScreen.y);

    if(keyPressed[SDLK_w])
    {
        player.jump();
    }
    if(keyState[SDLK_a])
    {
        player.moveLeft();
    }
    if(keyState[SDLK_d])
    {
        player.moveRight();
    }
*/

};

#endif
