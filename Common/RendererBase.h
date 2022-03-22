/******************************************************************************
Class:RendererBase
Implements:
Author:Rich Davison
Description:TODO

-_-_-_-_-_-_-_,------,
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""

*/ /////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Matrix4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Window.h"

namespace NCL {
	class MeshGeometry;
}

namespace NCL
{
	namespace Rendering
	{
		class TextureBase;
		class ShaderBase;

		enum class VerticalSyncState
		{
			VSync_ON,
			VSync_OFF,
			VSync_ADAPTIVE
		};

		class RendererBase
		{
		public:
			friend class ::NCL::Window;

			explicit RendererBase(Window& w) : hostWindow(w), currentWidth(0), currentHeight(0) {}
			virtual ~RendererBase() = default;

			virtual bool HasInitialised() const { return true; }

			virtual void Update(float dt)
			{
			}

			void Render()
			{
				BeginFrame();
				RenderFrame();
				EndFrame();
				SwapBuffers();
			}

			virtual bool SetVerticalSync(VerticalSyncState s)
			{
				return false;
			}

			virtual void bind_shader(ShaderBase* shader) = 0;
			virtual void bind_shader_defaults() {}
			virtual void reset_shader_for_next_object() {}
			virtual void reset_state_for_next_shader() {}
			virtual void reset_state_for_next_frame();
			virtual void free_reserved_textures() = 0;
			virtual unsigned reserve_texture(const TextureBase& data) = 0;
			virtual void bind_reserved_texture(const std::string& shader_property_name, unsigned texture_address) = 0;
			template<class ShaderArgs>
			void bind_shader_property(const std::string& shader_property_name, const ShaderArgs& data) {
				throw std::logic_error("Class cannot be passed to shader");
			}
			virtual void bind_mesh(MeshGeometry* m) = 0;
			virtual void draw_bound_mesh(unsigned sub_layer = 0, unsigned num_instances = 1) const = 0;
			virtual ShaderBase* load_default_shader() const = 0;
			virtual TextureBase* init_blank_texture(unsigned width, unsigned height) const = 0;
			virtual void render_to(TextureBase* texture_base) const = 0;
			virtual void blit(TextureBase* source, TextureBase* dest) = 0;
		protected:
			virtual void bind_int_to_shader(const std::string& shader_property_name, const int& data) = 0;
			virtual void bind_float_to_shader(const std::string& shader_property_name, const float& data) = 0;
			virtual void bind_vector_to_shader(const std::string& shader_property_name, unsigned size, const float* data) = 0;
			virtual void bind_matrix4_to_shader(const std::string& shader_property_name, const float* data) = 0;
			virtual void bind_texture_to_shader(const std::string& shader_property_name, const TextureBase& data) = 0;
			virtual void OnWindowResize(int w, int h) = 0;

			virtual void OnWindowDetach() {}

			virtual void BeginFrame() = 0;
			virtual void RenderFrame() = 0;
			virtual void EndFrame() = 0;
			virtual void SwapBuffers() = 0;
			Window& hostWindow;

			int currentWidth;
			int currentHeight;
		};

		inline void RendererBase::reset_state_for_next_frame()
		{
			free_reserved_textures();
		}
		template <>
		inline void RendererBase::bind_shader_property<int>(const std::string& shader_property_name, const int& data) {
			bind_int_to_shader(shader_property_name, data);
		}
		template <>
		inline void RendererBase::bind_shader_property<bool>(const std::string& shader_property_name, const bool& data) {
			bind_int_to_shader(shader_property_name, data);
		}
		template <>
		inline void RendererBase::bind_shader_property<float>(const std::string& shader_property_name, const float& data) {
			bind_float_to_shader(shader_property_name, data);
		}
		template <>
		inline void RendererBase::bind_shader_property<Vector2>(const std::string& shader_property_name, const Vector2& data) {
			bind_vector_to_shader(shader_property_name, 2, data.as_float_array());
		}
		template <>
		inline void RendererBase::bind_shader_property<Vector3>(const std::string& shader_property_name, const Vector3& data) {
			bind_vector_to_shader(shader_property_name, 3, data.as_float_array());
		}
		template <>
		inline void RendererBase::bind_shader_property<Vector4>(const std::string& shader_property_name, const Vector4& data) {
			bind_vector_to_shader(shader_property_name, 4, data.as_float_array());
		}
		template <>
		inline void RendererBase::bind_shader_property<Matrix4>(const std::string& shader_property_name, const Matrix4& data) {
			bind_matrix4_to_shader(shader_property_name, data.as_float_array());
		}
		template <>
		inline void RendererBase::bind_shader_property<TextureBase>(const std::string& shader_property_name, const TextureBase& data) {
			bind_texture_to_shader(shader_property_name, data);
		}
	}
}
