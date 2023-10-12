#include "DebugCamera.h"

DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;
	return &instance;
}

void DebugCamera::initialize() {
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0,0,-30 };
	viewProjection_.rotation_ = { 0,0,0 };
}

void DebugCamera::Update() {
	ImGui::Begin("DebugCamera");
	ImGui::SliderFloat3("rotation", viewProjection_.rotation_.num, -20, 20);
	ImGui::SliderFloat3("translation", viewProjection_.translation_.num, -20, 20);
	ImGui::End();
	viewProjection_.UpdateMatrix();
}