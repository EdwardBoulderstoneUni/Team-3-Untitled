#include "PaintableSurface.h"

#include "PaintManager.h"

PaintableSurface::PaintableSurface(const GameObject* surface) : render_object_(
	dynamic_cast<PaintableRenderObject*>(surface->GetRenderObject()))
{
	type = Component::ComponentType::Paint;
}

void PaintableSurface::Paint(const NCL::Maths::Vector3& local_pos)
{
	PaintManager::paint(this, local_pos);
}

PaintableRenderObject* PaintableSurface::GetRenderObject() const
{
	return render_object_;
}
