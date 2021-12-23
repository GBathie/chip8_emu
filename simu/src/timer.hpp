#pragma once

#include <SDL.h>

constexpr int TIMER_DELAY = 17;

class Timer
{
public:
	Timer()
	{
		id = SDL_AddTimer(TIMER_DELAY, static_step, this);
	}

	virtual ~Timer()
	{
		SDL_RemoveTimer(id);
	}

	inline static Uint32 static_step(Uint32 interval, void* param)
	{
		Timer *t = (Timer*) param;
		t->step();		
		return interval;
	}

	inline uint8_t get() const 			{ return count; }
	virtual inline void set(uint8_t c) 	{ count = c; }
	virtual inline void step() 			{ if (count > 0) --count; }

protected:
	SDL_TimerID id;
	uint8_t count;
};

#include <SDL_mixer.h>

class SoundTimer: public Timer
{
public:
	SoundTimer(): Timer() 
	{

		// Mix_PlayMusic(beep_sound, 0);
	}

	~SoundTimer()
	{
		// Mix_FreeMusic(beep_sound);
	}

	inline void play_sound() 
	{
		if (beep_sound == nullptr)
		{
			beep_sound = Mix_LoadMUS("./res/beep.wav");
			if (!beep_sound)
				throw runtime_error("Could not load beep sound! " + string(Mix_GetError()));
			Mix_PlayMusic(beep_sound, -1);
		}
		Mix_ResumeMusic();
	}

	inline void stop_sound() 
	{
		// Mix_HaltChannel(-1);
		Mix_PauseMusic();
	}

	inline void set(uint8_t c)
	{
		count = c;
		if (c > 0)
			play_sound();
	}

	inline void step() final
	{ 
		if (count == 1)
			stop_sound();
		if (count > 0) --count; 
	}
private:
	Mix_Music *beep_sound = nullptr;
	// Mix_Chunk *beep_sound = nullptr;
};