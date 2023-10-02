#pragma once
#include "Iscene.h"
#include "CJEngine.h"
#include "components/input/Input.h"
#include "components/audio/Audio.h"
#include "components/utilities/GlobalVariables/GlobalVariables.h"

//sceneInclude
#include "gamedata/scenes/GameTitleScene.h"
#include "gamedata/scenes/GamePlayScene.h"
#include "gamedata/scenes/GameClearScene.h"

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

	std::unique_ptr<Iscene>scene_[SCENE_MAX];
};