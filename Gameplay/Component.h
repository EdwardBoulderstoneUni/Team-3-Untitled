#pragma once

class Component {
public:
	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
};
