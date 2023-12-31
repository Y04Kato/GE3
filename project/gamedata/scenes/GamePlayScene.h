#pragma once
#include "components/manager/Iscene.h"
#include "components/audio/Audio.h"
#include "components/input/Input.h"
#include "components/math/Vector.h"
#include "components/2d/CreateTriangle.h"
#include "components/2d/CreateSprite.h"
#include "components/3d/CreateSphere.h"
#include "components/3d/Model.h"
#include "components/debugcamera/DebugCamera.h"
#include "components/utilities/collisionManager/CollisionManager.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

class GamePlayScene :public Iscene {
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void ApplyGlobalVariables();

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;

	CreateTriangle* triangle_[2];
	TriangleData triangleData_[2];
	Transform triangleTransform_[2];

	CreateSprite* sprite_[2];
	SpriteData spriteData_;
	Transform spriteTransform_;
	Transform SpriteuvTransform_;

	CreateSphere* sphere_;
	Transform sphereTransform_;
	Vector4 sphereMaterial_;

	Model* model_;
	Transform modelTransform_;
	Vector4 modelMaterial_;

	DirectionalLight directionalLight_;

	uint32_t uvResourceNum_;
	uint32_t monsterBallResourceNum_;
	uint32_t modelResourceNum_;

	Audio* audio_;
	SoundData soundData1_;

	Input* input_;

	DebugCamera* debugCamera_;

	CollisionManager* collisionManager_;

	int texture_;

	bool isTriangleDraw1_;
	bool isTriangleDraw2_;
	int isSphereDraw_;
	int isSpriteDraw_;
	int isModelDraw_;
};
