#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 100.0f, 100.0f, 100.0f };
	worldTransform_.UpdateMatrix();

	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{-0.2f,-1.5f,0.4f},1.0f };
}

void Skydome::Update() {}

void Skydome::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
}