#pragma once

namespace NDjinn {

	class FpsLimiter {
	private:
		float _fps;
		float _maxFps;
		float _frameTime;
		unsigned int _startTicks;

		void calcFps();
	public:
		FpsLimiter();
		~FpsLimiter();

		void init(float maxFps);

		void setMaxFps(float maxFps);

		void begin();
		float end(); // returns the FPS
	};

}