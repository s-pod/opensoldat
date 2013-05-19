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
#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "global.h"



float getTime();
float getTimeSince(float& startTime, bool updateStart);
void delay(float seconds);

/**
 * A Dual-mode Gameloop class.
 * It can either function as a frame-limiter loop,
 * or as a max framerate via Interpolation.
 * This class should be inherited by your main App class.
 */
class GameLoop
{
public:

    /** The amount of time for each game state frame */
    float dt;

    /** Game State Frequency. The rate at which the physics and game logic is updated. */
    float stateFps;

    /** Weather to limit FPS */
    bool limitFps;

    /**
     * Initialize the loop.
     * @param stateFps The frequency at which the game state will be updated.
     * @param limitFps To limit gives better system performance.
     *                 To not limits give better framerates.
     */
    GameLoop(float stateFps, bool limitFps);
    virtual ~GameLoop();

    /**
     * Must return weather the loop may continue running or not.
     */
    virtual bool continueLoop()=0;

    /**
     * Will be called when the physics and other game logic needs to be updated.
     * @param frameTime  The timestep for a Single frame.
     * @param frameCount The amount of frames that need to be processed.
     *                   Should always be 1.0 unless the computer hardware
     *                   is slower than the game requirements, in which case
     *                   this value can be used to determine how much time the
     *                   game needs to regain.(this is when we start taking
     *                   sub-optimal shortcuts to try and keep up.)
     */
    virtual void update(float frameTime, float frameCount)=0;

    /**
     * Will be called when a frame is to be displayed.
     * @param interpolation The interpolation since the last frame.
     *                      Range 0.0(at previous frame) - 1.0(at current frame).
     *                      For Frame Limiter this will always be 1.0.
     */
    virtual void render(float interpolation)=0;

    /**
     * Starts running the loop!
     */
    void runLoop();

};

#endif
