#pragma once
#include "Iscene.h"
#include "CJEngine.h"
#include "Input.h"
#include "Audio.h"
#include "GlobalVariables.h"
#include "TextureManager.h"

//sceneInclude
#include "GameTitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"

class SceneManager {
public:
	//シーン一覧、シーン追加時はここに追加する
	enum SCENE {
		TITLE_SCENE,
		GAME_SCENE,
		SCENE_MAX
	};

	void Run();

	void Initialize();
	void Update();
	//void Draw();
	void Finalize();

private:
	CitrusJunosEngine* CJEngine_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	TextureManager* textureManager = nullptr;

	std::unique_ptr<Iscene>scene_[SCENE_MAX];
};