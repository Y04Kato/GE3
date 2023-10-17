#include "DebugCamera.h"

DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;
	return &instance;
}

void DebugCamera::initialize() {
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0,0,-30 };
	viewProjection_.rotation_ = { 0,0,0 };

	input_ = Input::GetInstance();
}

void DebugCamera::Update() {
	if (input_->PressKey(DIK_UPARROW)) {
		viewProjection_.rotation_.num[0] -= 0.05f;
	}
	if (input_->PressKey(DIK_DOWNARROW)) {
		viewProjection_.rotation_.num[0] += 0.05f;
	}
	if (input_->PressKey(DIK_RIGHTARROW)) {
		viewProjection_.rotation_.num[1] += 0.05f;
	}
	if (input_->PressKey(DIK_LEFTARROW)) {
		viewProjection_.rotation_.num[1] -= 0.05f;
	}

	ImGui::Begin("DebugCamera");
	ImGui::SliderFloat3("rotation", viewProjection_.rotation_.num, -20, 20);
	ImGui::SliderFloat3("translation", viewProjection_.translation_.num, -20, 20);
	ImGui::End();
	viewProjection_.UpdateMatrix();
}