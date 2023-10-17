#pragma once
#include "components/3d/ViewProjection.h"
#include "components/input/Input.h"

class DebugCamera{
public:
	static DebugCamera* GetInstance();

	void initialize();

	void Update();

	void ShakeCamera(int shakePower,int dividePower);

	void SetCamera(Vector3 translation, Vector3 rotation);

	ViewProjection* GetViewProjection() { return &viewProjection_; }

private:
	ViewProjection viewProjection_;

	Input* input_;
};
