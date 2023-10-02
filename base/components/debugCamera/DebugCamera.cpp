#include "DebugCamera.h"

DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;
	return &instance;
}

void DebugCamera::initialize() {
	//x,y,z軸周りのローカル回転角
	rotation_ = { 0,0,0 };
	//ローカル座標
	translation_ = { 0,0,-10 };
	matRot_ = MakeIdentity4x4();
}

void DebugCamera::Update() {
	if (Input::GetInstance()->PressKey(DIK_SPACE)) {
		const float speed = 0.1f;

		Vector3 move = { 0,0,speed };

		//移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}

	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
		const float speed = -0.1f;

		Vector3 move = { speed,0,0 };

		//移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}

	if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
		const float speed = 0.1f;

		Vector3 move = { speed,0,0 };

		//移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}

	if (Input::GetInstance()->PressKey(DIK_UP)) {
		const float speed = 0.1f;

		Vector3 move = { 0,speed,0 };

		//移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}

	if (Input::GetInstance()->PressKey(DIK_DOWN)) {
		const float speed = -0.1f;

		Vector3 move = { 0,speed,0 };

		//移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, worldTransform_);

		translation_ = Add(translation_, move);
	}

	//回転
	Matrix4x4 matRotDelta = MakeIdentity4x4();
	if (Input::GetInstance()->PressKey(DIK_W)) {
		const float speed = -0.05f;
		matRotDelta = Multiply(matRotDelta, MakeRotateXMatrix(speed));
	}
	else if (Input::GetInstance()->PressKey(DIK_S)) {
		const float speed = 0.05f;
		matRotDelta = Multiply(matRotDelta, MakeRotateXMatrix(speed));
	}
	if (Input::GetInstance()->PressKey(DIK_A)) {
		const float speed = -0.05f;
		matRotDelta = Multiply(matRotDelta, MakeRotateYMatrix(speed));
	}
	else if (Input::GetInstance()->PressKey(DIK_D)) {
		const float speed = 0.05f;
		matRotDelta = Multiply(matRotDelta, MakeRotateYMatrix(speed));
	}
	matRot_ = Multiply(matRotDelta, matRot_);

	worldTransform_ = MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 });
	Matrix4x4 cameraMatrix = MakeIdentity4x4();
	cameraMatrix = Multiply(cameraMatrix, matRot_);
	viewMatrix_ = Inverse(cameraMatrix);

	Matrix4x4 Move = MakeTranslateMatrix(translation_);
	cameraMatrix = Multiply(cameraMatrix, Move);
	viewMatrix_ = Inverse(cameraMatrix);
}