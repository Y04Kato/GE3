#pragma once
#include "components/manager/Iscene.h"

class GameClearScene :public Iscene {
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
};