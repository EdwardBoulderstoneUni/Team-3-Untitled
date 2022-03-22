#include "TextureBase.h"
using namespace NCL;
using namespace Rendering;

void TextureBase::reserve(const unsigned address)
{
	reserved_ = true;
	reserved_address_ = address;
}


TextureBase::TextureBase() : width_(0), height_(0), reserved_(false), reserved_address_(0)
{ }
