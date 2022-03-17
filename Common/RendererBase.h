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

namespace NCL
{
	namespace Rendering
	{
		enum class VerticalSyncState
		{
			VSync_ON,
			VSync_OFF,
			VSync_ADAPTIVE
		};

		class RendererBase
		{
		public:
			friend class Window;

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

			
			template<class ShaderArgs>
			void bind_shader_property(const std::string& shader_property_name, const ShaderArgs& data) {
				throw std::logic_error("Class cannot be passed to shader");
			}
		protected:
			virtual void bind_float_to_shader(const std::string& shader_property_name, const float& data) = 0;
			virtual void bind_vector2_to_shader(const std::string& shader_property_name, const float* data) = 0;
			virtual void bind_vector3_to_shader(const std::string& shader_property_name, const float* data) = 0;
			virtual void bind_vector4_to_shader(const std::string& shader_property_name, const float* data) = 0;
			virtual void bind_matrix4_to_shader(const std::string& shader_property_name, const float* data) = 0;
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

		template <>
		inline void RendererBase::bind_shader_property<float>(const std::string& shader_property_name, const float& data) {
			bind_float_to_shader(shader_property_name, data);
		}
		template <>
		inline void RendererBase::bind_shader_property<Vector2>(const std::string& shader_property_name, const Vector2& data) {
			bind_vector2_to_shader(shader_property_name, data.as_float_array());
		}
		template <>
		inline void RendererBase::bind_shader_property<Vector3>(const std::string& shader_property_name, const Vector3& data) {
			bind_vector3_to_shader(shader_property_name, data.as_float_array());
		}
		template <>
		inline void RendererBase::bind_shader_property<Vector4>(const std::string& shader_property_name, const Vector4& data) {
			bind_vector4_to_shader(shader_property_name, data.as_float_array());
		}
		template <>
		inline void RendererBase::bind_shader_property<Matrix4>(const std::string& shader_property_name, const Matrix4& data) {
			bind_matrix4_to_shader(shader_property_name, data.as_float_array());
		}
	}
}
