#pragma once
#include <SDL\SDL.h>
#include "Timing.h"

using namespace NDjinn;

FpsLimiter::FpsLimiter() {}
FpsLimiter::~FpsLimiter(){}

void FpsLimiter::init(float maxFps) {
	setMaxFps(maxFps);
}

void FpsLimiter::setMaxFps(float maxFps) {
	_maxFps = maxFps;
}

void FpsLimiter::begin() {
	_startTicks = SDL_GetTicks();
}

float FpsLimiter::end() {
	calcFps();

	//FPS limiter
	Uint32 frameTicks = SDL_GetTicks() - _startTicks;
	if (1000.0f / (Uint32)_maxFps > frameTicks) {
		SDL_Delay(1000 / (Uint32)_maxFps - frameTicks);
	}

	return _fps;
}

void FpsLimiter::calcFps() {
	static const int NUM_SAMPLES = 10;
	static Uint32 frameTimeBuffer[NUM_SAMPLES];
	static Uint32 frameNum = 0;
	static Uint32 previousTicks = 0;

	Uint32 currentTicks = SDL_GetTicks();
	_frameTime = currentTicks - previousTicks;

	frameTimeBuffer[frameNum % NUM_SAMPLES] = _frameTime;

	float frameTimeAvg = 0.0f;
	if (frameNum > NUM_SAMPLES) {
		for (int i = 0; i < NUM_SAMPLES; i++) {
			frameTimeAvg += frameTimeBuffer[i];
		}
		frameTimeAvg /= NUM_SAMPLES;

		if (frameTimeAvg > 0) {
			_fps = 1000.0f / frameTimeAvg;
		}
	}
	else {
		_fps = 0.0f;
	}

	previousTicks = currentTicks;
	frameNum++;
}

