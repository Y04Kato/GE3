#pragma once

class Iscene{
public:
	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void Finalize() = 0;

	static int sceneNo;

	virtual ~Iscene() {};
};
