#include "GamePlayScene.h"
#include "components/utilities/globalVariables/GlobalVariables.h"

void GamePlayScene::Initialize() {
	CJEngine_ = CitrusJunosEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//三角形
	triangleData_[0].position[0] = { -0.5f,-0.5f,0.0f,1.0f };
	triangleData_[0].position[1] = { 0.0f,0.5f,0.0f,1.0f };
	triangleData_[0].position[2] = { 0.5f,-0.5f,0.0f,1.0f };
	triangleData_[0].material = { 1.0f,1.0f,1.0f,1.0f };

	triangleData_[1].position[0] = { -0.5f,-0.5f,0.5f,1.0f };
	triangleData_[1].position[1] = { 0.0f,0.0f,0.0f,1.0f };
	triangleData_[1].position[2] = { 0.5f,-0.5f,-0.5f,1.0f };
	triangleData_[1].material = { 1.0f,1.0f,1.0f,1.0f };

	for (int i = 0; i < 2; i++) {
		triangle_[i] = new CreateTriangle();
		triangle_[i]->Initialize(dxCommon_, CJEngine_);
	}

	for (int i = 0; i < 2; i++) {
		triangleTransform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	}
	isTriangleDraw1_ = false;
	isTriangleDraw2_ = false;

	//スプライト
	spriteData_.positionLeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.positionRightDown[0] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.positionLeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.positionRightDown[1] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	SpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	for (int i = 0; i < 2; i++) {
		sprite_[i] = new CreateSprite();
		sprite_[i]->Initialize(dxCommon_, CJEngine_);
	}

	isSpriteDraw_ = false;

	//球体
	sphereTransform_ = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	sphere_ = new CreateSphere();
	sphere_->Initialize(dxCommon_, CJEngine_);

	isSphereDraw_ = false;

	//objモデル
	modelTransform_ = { {0.5f,0.5f,0.5f},{0.0f,2.5f,0.0f},{0.0f,0.0f,1.0f} };
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	modelResourceNum_ = 2;
	model_ = new Model();
	model_->Initialize(dxCommon_, CJEngine_, "project/gamedata/resources/axis", "axis.obj", modelResourceNum_);

	//ライト
	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };

	//テクスチャ
	texture_ = 0;
	uvResourceNum_ = 0;
	CJEngine_->SettingTexture("project/gamedata/resources/uvChecker.png", uvResourceNum_);

	monsterBallResourceNum_ = 1;
	CJEngine_->SettingTexture("project/gamedata/resources/monsterBall.png", monsterBallResourceNum_);

	//Input
	input_ = Input::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();
	soundData1_ = audio_->SoundLoadWave("project/gamedata/resources/fanfare.wav");
	//音声再生
	audio_->SoundPlayWave(soundData1_);

	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();

	//CollisionManager
	collisionManager_ = CollisionManager::GetInstance();

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "GamePlayScene";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Test", 90);
}

void GamePlayScene::Update() {

	ApplyGlobalVariables();

	collisionManager_->ClearColliders();
	collisionManager_->CheckAllCollision();

	if (input_->PressKey(DIK_A)) {
		OutputDebugStringA("Hit A\n");
	}

	directionalLight_.direction = Normalise(directionalLight_.direction);

	ImGui::Begin("debug");
	ImGui::Text("GamePlayScene");
	ImGui::Text("DebugCameraTranslate:ArrowKey");
	ImGui::Text("DebugCameraRotate:WASD");
	if (ImGui::TreeNode("Triangle")) {//三角形
		if (ImGui::Button("DrawTriangle1")) {
			if (isTriangleDraw1_ == false) {
				isTriangleDraw1_ = true;
			}
			else {
				isTriangleDraw1_ = false;
			}
		}
		if (ImGui::Button("DrawTriangle2")) {
			if (isTriangleDraw2_ == false) {
				isTriangleDraw2_ = true;
			}
			else {
				isTriangleDraw2_ = false;
			}
		}
		if (isTriangleDraw1_ == true) {
			if (ImGui::TreeNode("Triangle1")) {
				ImGui::DragFloat3("Translate", triangleTransform_[0].translate.num, 0.05f);
				ImGui::DragFloat3("Rotate", triangleTransform_[0].rotate.num, 0.05f);
				ImGui::DragFloat3("Scale", triangleTransform_[0].scale.num, 0.05f);
				ImGui::ColorEdit4("", triangleData_[0].material.num, 0);
				ImGui::TreePop();
			}
		}
		if (isTriangleDraw2_ == true) {
			if (ImGui::TreeNode("Triangle2")) {
				ImGui::DragFloat3("Translate2", triangleTransform_[1].translate.num, 0.05f);
				ImGui::DragFloat3("Rotate2", triangleTransform_[1].rotate.num, 0.05f);
				ImGui::DragFloat3("Scale2", triangleTransform_[1].scale.num, 0.05f);
				ImGui::ColorEdit4("", triangleData_[1].material.num, 0);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sphere")) {//球体
		if (ImGui::Button("DrawSphere")) {
			if (isSphereDraw_ == false) {
				isSphereDraw_ = true;
			}
			else {
				isSphereDraw_ = false;
			}
		}
		ImGui::DragFloat3("Translate", sphereTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", sphereTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", sphereTransform_.scale.num, 0.05f);
		ImGui::ColorEdit4("", sphereMaterial_.num, 0);
		ImGui::SliderInt("ChangeTexture", &texture_, 0, 1);
		ImGui::DragFloat4("LightColor", directionalLight_.color.num, 1.0f);
		ImGui::DragFloat3("lightDirection", directionalLight_.direction.num, 0.1f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sprite")) {//スプライト
		if (ImGui::Button("DrawSprite")) {
			if (isSpriteDraw_ == false) {
				isSpriteDraw_ = true;
			}
			else {
				isSpriteDraw_ = false;
			}
		}
		ImGui::DragFloat3("Translate", spriteTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", spriteTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", spriteTransform_.scale.num, 0.05f);
		ImGui::ColorEdit4("", spriteData_.material.num, 0);
		ImGui::DragFloat2("uvScale", SpriteuvTransform_.scale.num, 0.1f);
		ImGui::DragFloat3("uvTranslate", SpriteuvTransform_.translate.num, 0.1f);
		ImGui::DragFloat("uvRotate", &SpriteuvTransform_.rotate.num[2], 0.1f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Model")) {//objモデル
		if (ImGui::Button("DrawModel")) {
			if (isModelDraw_ == false) {
				isModelDraw_ = true;
			}
			else {
				isModelDraw_ = false;
			}
		}
		ImGui::DragFloat3("Translate", modelTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", modelTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", modelTransform_.scale.num, 0.05f);
		ImGui::TreePop();
	}

	ImGui::End();

	input_->Update();
	debugCamera_->Update();
}

void GamePlayScene::Draw() {
#pragma region 3Dオブジェクト描画
	if (isTriangleDraw1_) {//Triangle描画
		triangle_[0]->Draw(triangleData_[0], triangleTransform_[0], debugCamera_->GetViewMatrix(), uvResourceNum_, directionalLight_);
	}
	if (isTriangleDraw2_) {//Triangle描画
		triangle_[1]->Draw(triangleData_[1], triangleTransform_[1], debugCamera_->GetViewMatrix(), uvResourceNum_, directionalLight_);
	}

	if (isSphereDraw_) {
		sphere_->Draw(sphereMaterial_, sphereTransform_, texture_, debugCamera_->GetViewMatrix(), directionalLight_);
	}

	if (isModelDraw_) {
		model_->Draw(modelMaterial_, modelTransform_, modelResourceNum_, debugCamera_->GetViewMatrix(), directionalLight_);
	}
#pragma endregion

#pragma region 前景スプライト描画
	if (isSpriteDraw_) {
		for (int i = 0; i < 1; i++) {//Sprite描画
			sprite_[i]->Draw(spriteData_.positionLeftTop[i], spriteData_.positionRightDown[i], spriteTransform_, SpriteuvTransform_, spriteData_.material, uvResourceNum_, directionalLight_);
		}
	}
#pragma endregion
}

void GamePlayScene::Finalize() {
	for (int i = 0; i < 2; i++) {
		triangle_[i]->Finalize();
		delete triangle_[i];
	}

	for (int i = 0; i < 2; i++) {
		sprite_[i]->Finalize();
		delete sprite_[i];
	}

	sphere_->Finalize();
	delete sphere_;

	model_->Finalize();
	delete model_;

	audio_->SoundUnload(&soundData1_);
}

void GamePlayScene::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "GamePlayScene";
}