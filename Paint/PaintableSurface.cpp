#include "PaintableSurface.h"

#include "PaintManager.h"
constexpr float kMinIntensity = 0.7f;
PaintableSurface::PaintableSurface(const GameObject* parent) : render_object_(dynamic_cast<PaintableRenderObject*>(parent->GetRenderObject()))
{

	type = ComponentType::Paint;
}

void PaintableSurface::Paint(const NCL::Maths::Vector3& world_pos) const
{
	PaintManager::paint(this, world_pos);
}

Vector3 PaintableSurface::GetColour(const NCL::Maths::Vector3& world_pos) const
{
	auto colour = render_object_->get_colour_at(world_pos);
	if (colour.x > kMinIntensity)
		colour.x = 1;
	else
		colour.x = 0;
	if (colour.y > kMinIntensity)
		colour.y = 1;
	else
		colour.y = 0;
	if (colour.z > kMinIntensity)
		colour.z = 1;
	else
		colour.z = 0;
	return colour;
}

PaintableRenderObject* PaintableSurface::GetRenderObject() const
{
	return render_object_;
}
