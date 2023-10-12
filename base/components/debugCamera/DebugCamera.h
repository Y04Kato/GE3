#pragma once
#include "components/3d/ViewProjection.h"
#include "components/input/Input.h"

class DebugCamera{
public:
	static DebugCamera* GetInstance();

	void initialize();

	void Update();

	ViewProjection* GetViewProjection() { return &viewProjection_; }

private:
	ViewProjection viewProjection_;
};
