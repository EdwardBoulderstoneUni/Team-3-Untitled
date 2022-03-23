#pragma once
#include "PaintableRenderObject.h"
#include "../Common/Vector3.h"
#include "../CSC8503/CSC8503Common/GameObject.h"
#include "../Gameplay/Component.h"

class PaintableSurface : Component
{

public:
	explicit PaintableSurface(const GameObject* surface);
	void Paint(const NCL::Maths::Vector3& local_pos);
	Vector3 GetColour(const NCL::Maths::Vector3& local_pos) const;
	PaintableRenderObject* GetRenderObject() const;
protected:
	PaintableRenderObject* render_object_;
};

