
#include "OrbisAllocator.h"
#include <.\graphics\api_gnm\toolkit\allocators.h>
#include <.\graphics\api_gnm\toolkit\stack_allocator.h>
using namespace NCL::PS4;

sce::Gnmx::Toolkit::IAllocator* OrbisAllocator::onionAllocator;
sce::Gnmx::Toolkit::IAllocator* OrbisAllocator::garlicAllocator;
sce::Gnm::OwnerHandle				OrbisAllocator::ownerHandle;
