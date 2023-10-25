#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) {
	models_ = models;
	worldTransform_.Initialize();

	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{-0.2f,-1.5f,0.4f},1.0f };
}

void BaseCharacter::Update() { worldTransform_.UpdateMatrix(); }

void BaseCharacter::Draw(const ViewProjection& viewProjection) {
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	}
}