#pragma once
#include "components/manager/Iscene.h"
#include "CJEngine.h"
#include "components/input/Input.h"

class GameTitleScene :public Iscene{
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	CitrusJunosEngine* CJEngine_;

	Input* input_ = nullptr;
};
