#pragma once
#include "components/input/Input.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"

class FollowCamera {
public:
	void Initialize();

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }
	const ViewProjection& GetViewProjection() { return viewprojection_; }

private:
	ViewProjection viewprojection_;
	const WorldTransform* target_ = nullptr;
	Input* input_ = nullptr;

};
