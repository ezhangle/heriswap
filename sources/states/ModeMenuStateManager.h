#pragma once

#include "api/NameInputAPI.h"
#include "SuccessManager.h"
#include "states/GameStateManager.h"
#include "Game.h"
#include "AnimedEntity.h"

class ModeMenuStateManager : public GameStateManager {
	public:
		ModeMenuStateManager(ScoreStorage* storag, NameInputAPI* nameInputAPI, SuccessManager* successMgr, LocalizeAPI* lAPI);
		~ModeMenuStateManager();
		void Setup();
		void Enter();
		GameState Update(float dt);
		void Exit();
        void LateExit();

		void LoadScore(int mode, int dif);

		Entity title, menufg, menubg;
		AnimatedActor* herisson;
	private:
		ScoreStorage* storage;
		NameInputAPI* nameInputAPI;
		Entity play, playButton, scoresPoints[5], scoresName[5], scoresLevel[5], back, openfeint, scoreTitle;
		Entity yourScore, fond;
		std::string playerName;
		SuccessManager* successMgr;
		LocalizeAPI* localizeAPI;

		//variables for perso mode
		int difficulty;
		Entity eDifficulty, bDifficulty;

        void submitScore(const std::string& playerName);
        bool isCurrentScoreAHighOne();
    public:
        enum GameOverState {
            NoGame,
            GameEnded,
            AskingPlayerName
        } gameOverState;

};
