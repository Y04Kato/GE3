#include "Player.h"
#include <cassert>
#define _USE_MATH_DEFINES

#include "GlobalVariables.h"
#include <math.h>

void Player::Initialize(const std::vector<Model*>& models) {
	// NULLポインタチェック
	BaseCharacter::Initialize(models);

	models_[kModelIndexBody] = models[kModelIndexBody];
	models_[kModelIndexHead] = models[kModelIndexHead];
	models_[kModelIndexL_arm] = models[kModelIndexL_arm];
	models_[kModelIndexR_arm] = models[kModelIndexR_arm];
	models_[kModelIndexWeapon] = models[kModelIndexWeapon];

	worldTransformHead_.translation_.num[1] = 1.8f;

	worldTransformL_arm_.translation_.num[0] = -0.5f;
	worldTransformL_arm_.translation_.num[1] = 1.8f;

	worldTransformR_arm_.translation_.num[0] = 0.5f;
	worldTransformR_arm_.translation_.num[1] = 1.8f;

	worldTransformWeapon_.translation_.num[0] = -0.2f;
	worldTransformWeapon_.translation_.num[1] = 1.6f;
	worldTransformWeapon_.scale_ = { 0.6f, 0.6f, 0.6f };

	InitializeFloatingGimmick();

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformWeapon_.Initialize();
	worldTransform_.Initialize();
	
	SetParent(&GetWorldTransformBody());

	input_ = Input::GetInstance();

	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{-0.2f,-1.5f,0.4f},1.0f };
	worldTransformBody_.translation_ = { 0.0f,0.1f,0.0f };

	isHit_ = false;
	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer);
	SetCollisionMask(~CollisionConfig::kCollisionAttributePlayer);

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Test", 90);
	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "L_Arm Translation", worldTransformL_arm_.translation_);
	globalVariables->AddItem(groupName, "R_Arm Translation", worldTransformR_arm_.translation_);
	globalVariables->AddItem(groupName, "floatingCycle", floatingCycle_);
	globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
}

void Player::Update() {

	ApplyGlobalVariables();

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
		GlobalVariables::GetInstance()->SaveFile("Player");
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
		behaviorRequest_ = Behavior::kAttack;
	}

	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case Player::Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;

		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Player::Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	ImGui::Begin("Player");
	ImGui::SliderFloat("Head Translation", &worldTransformHead_.translation_.num[0], -5.0f, 5.0f);
	ImGui::SliderFloat("ArmL Translation", &worldTransformL_arm_.translation_.num[0], -5.0f, 5.0f);
	ImGui::SliderFloat("ArmR Translation", &worldTransformR_arm_.translation_.num[0], -5.0f, 5.0f);
	ImGui::SliderInt("FloatingCycle", (int*)&floatingCycle_, 0, 240);
	ImGui::SliderFloat("FloatingAmplitude", &floatingAmplitude_, -1.0f, 1.0f);
	ImGui::Text("Attack : X");
	ImGui::End();

	ImGui::Begin("player2");
	ImGui::DragFloat3("translation", worldTransformBody_.translation_.num, 0.01f);
	ImGui::End();

	if (worldTransformBody_.translation_.num[1] < -10.0f) {
		gameOver = true;
	}
	if (!isHit_ || worldTransformBody_.GetWorldPos().num[1] < 0.0f) {
		IsFallStart();
	}
	else {
		worldTransformBody_.translation_.num[1] = objectPos_.translation_.num[1] + objectPos_.scale_.num[1] + worldTransformBody_.scale_.num[1] - 1.5f;
	}

	structSphere_.center = worldTransformBody_.GetWorldPos();
	structSphere_.radius = 1.5f;

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	worldTransformWeapon_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);

	if (behavior_ == Behavior::kAttack) {
		models_[kModelIndexWeapon]->Draw(worldTransformWeapon_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	}
}


void Player::SetParent(const WorldTransform* parent) {
	worldTransformBase_.parent_ = parent;
	worldTransformHead_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformWeapon_.parent_ = parent;
	worldTransform_.parent_ = parent;
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	const float step = 2.0f * (float)M_PI / floatingCycle_;

	floatingParameter_ += step;
	floatingParameter_ = (float)std::fmod(floatingParameter_, 2.0f * M_PI);

	//worldTransformBody_.translation_.num[1] = std::sin(floatingParameter_) * floatingAmplitude_;

	worldTransformL_arm_.rotation_.num[0] = std::sin(floatingParameter_) * 0.75f;
	worldTransformR_arm_.rotation_.num[0] = std::sin(floatingParameter_) * 0.75f;
}

void Player::BehaviorRootInitialize() {
	worldTransformL_arm_.rotation_.num[0] = 0.0f;
	worldTransformR_arm_.rotation_.num[0] = 0.0f;
	worldTransformWeapon_.rotation_.num[0] = 0.0f;

	InitializeFloatingGimmick();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformWeapon_.Initialize();
}

void Player::BehaviorRootUpdate() {
	XINPUT_STATE joystate;

	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		const float kCharacterSpeed = 0.3f;
		Vector3 move = {
			(float)joystate.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			(float)joystate.Gamepad.sThumbLY / SHRT_MAX };
		move = Multiply(kCharacterSpeed, Normalise(move));

		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		move = TransformNormal(move, rotateMatrix);

		worldTransformBody_.translation_ = Add(move, worldTransformBody_.translation_);

		worldTransformBody_.rotation_.num[1] = std::atan2(move.num[0], move.num[2]);
	}

	UpdateFloatingGimmick();
}

void Player::BehaviorAttackInitialize() {
	worldTransformL_arm_.rotation_.num[0] = (float)M_PI;
	worldTransformR_arm_.rotation_.num[0] = (float)M_PI;
	worldTransformWeapon_.rotation_.num[0] = 0.0f;
	animationFrame_ = 0;
}

void Player::BehaviorAttackUpdate() {
	if (animationFrame_ < 12) {
		worldTransformL_arm_.rotation_.num[0] -= 0.05f;
		worldTransformR_arm_.rotation_.num[0] -= 0.0f;

		worldTransformWeapon_.rotation_.num[0] -= 0.05f;
	}
	else if (worldTransformWeapon_.rotation_.num[0] <= 2.0f * (float)M_PI / 4) {
		worldTransformL_arm_.rotation_.num[0] += 0.1f;
		worldTransformR_arm_.rotation_.num[0] += 0.1f;

		worldTransformWeapon_.rotation_.num[0] += 0.1f;
	}
	else {
		behaviorRequest_ = Behavior::kRoot;
	}

	animationFrame_++;
}

void Player::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	worldTransformHead_.translation_ =
		globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransformL_arm_.translation_ =
		globalVariables->GetVector3Value(groupName, "L_Arm Translation");
	worldTransformR_arm_.translation_ =
		globalVariables->GetVector3Value(groupName, "R_Arm Translation");

	floatingCycle_ = globalVariables->GetIntValue(groupName, "floatingCycle");
	floatingAmplitude_ = globalVariables->GetFloatValue(groupName, "floatingAmplitude");
}

void Player::IsFallStart(){
	worldTransformBody_.translation_.num[1] -= 0.1f;
}

void Player::OnCollision(){
	gameOver = true;
}

void Player::Setparent(const WorldTransform* parent){
	worldTransformBody_.parent_ = parent;
}

void Player::IsCollision(const WorldTransform& worldtransform){
	if (!worldTransformBody_.parent_) {

		worldTransformBody_.translation_.num[1] = worldtransform.translation_.num[1] + worldtransform.scale_.num[1] + worldTransformBody_.scale_.num[1];
		Vector3 worldPos = worldTransformBody_.GetWorldPos();
		Vector3 objectWorldPos = { worldtransform.matWorld_.m[3][0],worldtransform.matWorld_.m[3][1],worldtransform.matWorld_.m[3][2] };
		Vector3 Position = worldPos - objectWorldPos;

		Matrix4x4 rotatematrix = MakeRotateXYZMatrix({ -worldtransform.rotation_.num[0] ,-worldtransform.rotation_.num[1] ,-worldtransform.rotation_.num[2]});
		Position = TransformNormal(Position, rotatematrix);

		worldTransformBody_.translation_ = Position;
		Setparent(&worldtransform);
		gameOver = false;
	}
}

void Player::DeleteParent(){
	if (worldTransformBody_.parent_) {
		worldTransformBody_.translation_ = worldTransformBody_.GetWorldPos();
		worldTransformBody_.parent_ = nullptr;
	}
}

void Player::SetWorldTransform(const Vector3 translation) {
	worldTransformBody_.translation_ = translation;
	gameOver = false;
}