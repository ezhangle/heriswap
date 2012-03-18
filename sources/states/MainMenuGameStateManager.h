#pragma once

#include "base/EntityManager.h"
#include "base/TouchInputManager.h"

#include "systems/ButtonSystem.h"
#include "systems/TextRenderingSystem.h"
#include "systems/TransformationSystem.h"
#include "systems/RenderingSystem.h"

#include "Game.h"
#include "GameStateManager.h"
#include "../modes/GameModeManager.h"

class MainMenuGameStateManager : public GameStateManager {
	public:
		MainMenuGameStateManager();
		~MainMenuGameStateManager();
		void Setup();
		void Enter();
		GameState Update(float dt);
		void Exit();
	private:
		Entity start[2], score, eScore, eStart[2], bStart[2], bScore;
		float elapsedTime;
		
	public:
		GameMode choosenGameMode;
};
