//
// Created by bentoo on 12.02.17.
//

#ifndef YAGE_GAME_H
#define YAGE_GAME_H

#include "Core/Engine.h"

namespace Yage
{
	/*
	 * Basic boilerplate for game.
	 * Handles engine initialization, window creation and proper clean up.
	 *
	 * You dont have to use it, feel free to write appropriate code yourself.
	 */
	class Game
	{
		float	lastTime;
		float	lagAccumulator;
		float	fpsTime;
		int		frames;

		Memory::IMemoryBlock* persistentBlock;
		Memory::IMemoryBlock* frameBlock;

	protected:

		std::uint32_t persistentBlockSize;
		std::uint32_t frameBlockSize;

		Core::Engine* engine;
		Core::Window* window;

		virtual void OnInit();
		virtual void OnCleanUp();
		virtual void OnUpdate(Core::GameTime &gameTime);

		Memory::IMemoryBlock& getPersistentBlock();
		Memory::IMemoryBlock& getFrameBlock();

	public:
		         Game();
		virtual ~Game();

		// Initializes engine and starts main loop. Cleans up after itself
		void Run(const char* name = "Yage");

		// Initializes engine
		void Init(Core::Engine* engineInstance);

		// Starts and processes main loop
		void Loop();

		// Single iteration of main loop
		void Update(Core::GameTime& time);

		// Cleans up after itself
		void CleanUp();
	};
}

#endif // YAGE_GAME_H
