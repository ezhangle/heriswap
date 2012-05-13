#pragma once

#include "base/EntityManager.h"

#include "systems/TransformationSystem.h"
#include "systems/TextRenderingSystem.h"
#include "systems/ContainerSystem.h"
#include "systems/ButtonSystem.h"
#include "systems/SoundSystem.h"

#include "modes/GameModeManager.h"
#include "states/GameStateManager.h"
#include "Game.h"
#include "DepthLayer.h"

class HelpStateManager : public GameStateManager {
	public:

	HelpStateManager() { };
	~HelpStateManager();
	void Setup();
	void Enter();
	GameState Update(float dt);
	void Exit();

	GameState oldState;
	GameMode mode;
	private:
		Entity eHelp;
};