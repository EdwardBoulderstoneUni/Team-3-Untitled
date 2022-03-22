#pragma once


#include "OrbisAllocator.h"

#include <gnm.h>
#include <gnmx\fetchshaderhelper.h>

#include <..\samples\sample_code\graphics\api_gnm\toolkit\allocators.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\stack_allocator.h>
//#include <..\samples\sample_code\graphics\api_gnm\toolkit\toolkit.h>
#include "../Common/RendererBase.h"
#include "PS4Frame.h"
#include "PS4Texture.h"

using namespace sce;
using namespace Gnmx;
using namespace Gnmx::Toolkit;

namespace NCL {
	namespace Maths
	{
		class Matrix4;
	}

	namespace PS4 {
		enum MemoryLocation {
			MEMORY_GARLIC,
			MEMORY_ONION,
			MEMORY_MAX
		};

		struct PS4ScreenBuffer {
			sce::Gnm::RenderTarget		colourTarget;
			sce::Gnm::DepthRenderTarget depthTarget;
		};

		class PS4Window;
		class PS4Shader;
		class PS4Mesh;

		class RenderObject;

		class PS4RendererBase :
			public RendererBase, public OrbisAllocator
		{
		public:
			PS4RendererBase(PS4Window* window);
			~PS4RendererBase();

		protected:
			void	OnWindowResize(int w, int h) override;
			void	BeginFrame()    override;
			void	EndFrame()		override;
			void	SwapBuffers()	override;

			void	SwapScreenBuffer();
			void	SwapCommandBuffer();
			void	SetRenderBuffer(PS4ScreenBuffer* buffer, bool clearColour, bool clearDepth, bool clearStencil);
			void	ClearBuffer(bool colour, bool depth, bool stencil);
			PS4ScreenBuffer* GenerateScreenBuffer(uint width, uint height, bool colour = true, bool depth = true, bool stencil = false);

			virtual void free_reserved_textures() {}
			virtual unsigned reserve_texture(const TextureBase& data)  {}
			virtual void bind_reserved_texture(const std::string& shader_property_name, unsigned texture_address)  {}
			template<class ShaderArgs>
			void bind_shader_property(const std::string& shader_property_name, const ShaderArgs& data) {
				throw std::logic_error("Class cannot be passed to shader");
			}
			virtual void bind_mesh(MeshGeometry* m)  {}
			virtual void draw_bound_mesh(unsigned sub_layer = 0, unsigned num_instances = 1) const  {}
			virtual ShaderBase* load_default_shader() const  {}
			virtual TextureBase* init_blank_texture(unsigned width, unsigned height) const  {}
		protected:
			virtual void bind_int_to_shader(const std::string& shader_property_name, const int& data)  {}
			virtual void bind_float_to_shader(const std::string& shader_property_name, const float& data)  {}
			virtual void bind_vector_to_shader(const std::string& shader_property_name, unsigned size, const float* data)  {}
			virtual void bind_matrix4_to_shader(const std::string& shader_property_name, const float* data)  {}
			virtual void bind_texture_to_shader(const std::string& shader_property_name, const TextureBase& data)  {}
			
			virtual void bind_shader(ShaderBase* shader) {}
		private:
			void	InitialiseMemoryAllocators();
			void	InitialiseVideoSystem();
			void	InitialiseGCMRendering();

			void	DestroyMemoryAllocators();
			void	DestroyVideoSystem();
			void	DestroyGCMRendering();

		protected:
			int currentGPUBuffer;

			const int _MaxCMDBufferCount;

			//VIDEO SYSTEM VARIABLES
			int videoHandle;		//Handle to video system

		//SCREEN BUFFER VARIABLES
			const int			_bufferCount;	//How many screen buffers should we have
			int					currentScreenBuffer;
			int					prevScreenBuffer;
			PS4ScreenBuffer** screenBuffers;	//Pointer to our screen buffers
		//Memory Allocation
			const int _GarlicMemory;
			const int _OnionMemory;

			sce::Gnmx::Toolkit::StackAllocator	stackAllocators[MEMORY_MAX];

			//Individual Frames
			PS4Frame* frames;

			int framesSubmitted;

			//Per frame pointers...
			PS4ScreenBuffer* currentPS4Buffer;  //Pointer to whichever buffer we're currently using...
			Gnmx::GnmxGfxContext* currentGFXContext;
			PS4Frame* currentFrame;
		};
	}
}
