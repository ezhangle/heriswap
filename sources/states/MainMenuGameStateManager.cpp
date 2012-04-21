#include "MainMenuGameStateManager.h"

MainMenuGameStateManager::~MainMenuGameStateManager() {
	for (int i=0; i<3; i++) {
		theTextRenderingSystem.DestroyLocalEntity(eStart[i]);
		theEntityManager.DeleteEntity(bStart[i]);
	}
}

void MainMenuGameStateManager::Setup() {
	Color green = Color(3.0/255.0, 99.0/255, 71.0/255);
	
	//Creating text entities
	for (int i=0; i<3; i++)
		eStart[i] = theTextRenderingSystem.CreateLocalEntity(7);
	//Settings
	for (int i=0; i<3; i++) {
		TRANSFORM(eStart[i])->z = DL_MainMenuUITxt;
		TEXT_RENDERING(eStart[i])->hide = true;
		TEXT_RENDERING(eStart[i])->positioning = TextRenderingComponent::LEFT;
		TEXT_RENDERING(eStart[i])->color = green;
		TEXT_RENDERING(eStart[i])->charHeight = PlacementHelper::GimpHeightToScreen(75);
		theMorphingSystem.Add(eStart[i]);
	    TypedMorphElement<Vector2>* posMorph = new TypedMorphElement<Vector2>(&TRANSFORM(eStart[i])->position, TRANSFORM(eStart[i])->position, Vector2(PlacementHelper::GimpXToScreen(444),PlacementHelper::GimpYToScreen(100)));
	    TypedMorphElement<float>* colorMorph = new TypedMorphElement<float>(&TEXT_RENDERING(eStart[i])->charHeight, TEXT_RENDERING(eStart[i])->charHeight, PlacementHelper::GimpHeightToScreen(54));
	    MORPHING(eStart[i])->elements.push_back(colorMorph);
	    MORPHING(eStart[i])->timing = 0.2;
	    TRANSFORM(eStart[i])->position.X = PlacementHelper::GimpXToScreen(82);
	    
	    bStart[i] = theEntityManager.CreateEntity();
	    ADD_COMPONENT(bStart[i], Transformation);
	    TRANSFORM(bStart[i])->size = Vector2(PlacementHelper::GimpWidthToScreen(708), PlacementHelper::GimpHeightToScreen(147));
	    TRANSFORM(bStart[i])->position.X = 0;
	    TRANSFORM(bStart[i])->z = DL_MainMenuUIBg;
	    ADD_COMPONENT(bStart[i], Rendering);
	    RENDERING(bStart[i])->texture = theRenderingSystem.loadTextureFile("menu/fond_bouton.png"); 
	    RENDERING(bStart[i])->color.a = 0.3;
	}
	TEXT_RENDERING(eStart[0])->text = "Classique";
	TRANSFORM(eStart[0])->position.Y = TRANSFORM(bStart[0])->position.Y = PlacementHelper::GimpYToScreen(156);
	TEXT_RENDERING(eStart[1])->text = "Contre le temps";
	TRANSFORM(eStart[1])->position.Y = TRANSFORM(bStart[1])->position.Y = PlacementHelper::GimpYToScreen(339);
	TEXT_RENDERING(eStart[2])->text = "Contre le score";
	TRANSFORM(eStart[2])->position.Y = TRANSFORM(bStart[2])->position.Y = PlacementHelper::GimpYToScreen(522);

	//Adding containers
	for (int i=0; i<3; i++) {
		TypedMorphElement<Vector2>* posMorph = new TypedMorphElement<Vector2>(&TRANSFORM(eStart[i])->position, TRANSFORM(eStart[i])->position, Vector2(PlacementHelper::GimpXToScreen(444),PlacementHelper::GimpYToScreen(100)));
		MORPHING(eStart[i])->elements.push_back(posMorph);
		ADD_COMPONENT(bStart[i], Sound);
		SOUND(bStart[i])->type = SoundComponent::EFFECT;
		ADD_COMPONENT(bStart[i], Button);
		BUTTON(bStart[i])->enabled = false;
	}
	
	menubg = theEntityManager.CreateEntity();
	ADD_COMPONENT(menubg, Transformation);
	TRANSFORM(menubg)->size = Vector2(PlacementHelper::GimpWidthToScreen(800), PlacementHelper::GimpHeightToScreen(570));
	TransformationSystem::setPosition(TRANSFORM(menubg), Vector2(0, PlacementHelper::GimpYToScreen(542)), TransformationSystem::N);
	TRANSFORM(menubg)->z = DL_MainMenuBg;
	ADD_COMPONENT(menubg, Rendering);
	RENDERING(menubg)->texture = theRenderingSystem.loadTextureFile("menu/2emeplan.png");
	RENDERING(menubg)->hide = true;
	
	menufg = theEntityManager.CreateEntity();
	ADD_COMPONENT(menufg, Transformation);
	TRANSFORM(menufg)->size = Vector2(PlacementHelper::GimpWidthToScreen(800), PlacementHelper::GimpHeightToScreen(570));
	TransformationSystem::setPosition(TRANSFORM(menufg), Vector2(0, PlacementHelper::GimpYToScreen(1280)), TransformationSystem::S);
	TRANSFORM(menufg)->z = DL_MainMenuFg;
	ADD_COMPONENT(menufg, Rendering);
	RENDERING(menufg)->texture = theRenderingSystem.loadTextureFile("menu/1erplan.png");
	RENDERING(menufg)->hide = true;
	
	modeTitleToReset = 0;
}

void MainMenuGameStateManager::Enter() {
	LOGI("%s", __PRETTY_FUNCTION__);

	// preload sound effect
	theSoundSystem.loadSoundFile("audio/son_menu.ogg", false);
	
	for (int i=0; i<3; i++) {
		MORPHING(eStart[i])->active = false;
		RENDERING(bStart[i])->hide = false;
		TEXT_RENDERING(eStart[i])->hide = false;
		BUTTON(bStart[i])->enabled = true;
		RENDERING(menubg)->hide = false;
		RENDERING(menufg)->hide = false;
	}
	if (modeTitleToReset) {
		theMorphingSystem.reverse(MORPHING(modeTitleToReset));
		MORPHING(modeTitleToReset)->activationTime = 0;
		MORPHING(modeTitleToReset)->active = true;
	}
}

GameState MainMenuGameStateManager::Update(float dt) {
	if (BUTTON(bStart[0])->clicked) {
		choosenGameMode = Normal;
		SOUND(bStart[0])->sound = theSoundSystem.loadSoundFile("audio/son_menu.ogg", false);
		return ModeMenu;
	} else if (BUTTON(bStart[1])->clicked){
		choosenGameMode = ScoreAttack;
		SOUND(bStart[1])->sound = theSoundSystem.loadSoundFile("audio/son_menu.ogg", false);
		return ModeMenu;
	} else if (BUTTON(bStart[2])->clicked){
		choosenGameMode = StaticTime;
		SOUND(bStart[2])->sound = theSoundSystem.loadSoundFile("audio/son_menu.ogg", false);
		return ModeMenu;
	}
	return MainMenu;
}

void MainMenuGameStateManager::Exit() {
	LOGI("%s", __PRETTY_FUNCTION__);
	for (int i=0; i<3; i++) {
		if (i!=choosenGameMode-1) TEXT_RENDERING(eStart[i])->hide = true;
		RENDERING(bStart[i])->hide = true;
		BUTTON(bStart[i])->enabled = false;
	}

	if (modeTitleToReset) {
		theMorphingSystem.reverse(MORPHING(modeTitleToReset));
	}
    MORPHING(eStart[choosenGameMode-1])->active = true;
    modeTitleToReset = eStart[choosenGameMode-1];
}
