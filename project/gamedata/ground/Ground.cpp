#include "ground.h"

void Ground::Initialize(Model* model, Vector3 translation, Vector3 scale){
	worldTransform_.Initialize();
	worldTransform_.translation_ = translation;
	worldTransform_.scale_ = scale;

	groundModel_ = model;

	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{-0.2f,-1.5f,0.4f},1.0f };
}

void Ground::Update(){
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(const ViewProjection& viewProjection) {
	groundModel_->Draw(worldTransform_,viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
}
