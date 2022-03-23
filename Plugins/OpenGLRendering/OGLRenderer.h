/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include "../../Common/RendererBase.h"

#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"

#ifdef _WIN32
#include "Windows.h"
#endif

#ifdef _DEBUG
#define OPENGL_DEBUGGING
#endif

#include <string>
#include <vector>
#include "glad/glad.h"

namespace NCL
{
	class MeshGeometry;

	namespace Maths
	{
		class Matrix4;
	}

	namespace Rendering
	{
		class ShaderBase;
		class TextureBase;

		class OGLMesh;
		class OGLShader;

		class SimpleFont;

		class OGLRenderer : public RendererBase
		{
		public:
			friend class OGLRenderer;
			explicit OGLRenderer(Window& w);
			~OGLRenderer() override;

			void OnWindowResize(int w, int h) override;

			bool HasInitialised() const override
			{
				return init_state_;
			}

			void ForceValidDebugState(const bool new_state)
			{
				force_valid_debug_state_ = new_state;
			}

			bool SetVerticalSync(VerticalSyncState s) override;

			void DrawString(const std::string& text, const Vector2& pos,
			                const Vector4& colour = Vector4(0.75f, 0.75f, 0.75f, 1), float size = 20.0f);
			void DrawLine(const Vector3& start, const Vector3& end, const Vector4& colour);

			virtual Matrix4 SetupDebugLineMatrix() const;
			virtual Matrix4 SetupDebugStringMatrix() const;

			void bind_shader(ShaderBase* shader) override;
			void bind_mesh(MeshGeometry* m) override;
			void draw_bound_mesh(unsigned sub_layer = 0, unsigned num_instances = 1) const override;
			ShaderBase* load_default_shader() const override;
		protected:
			void BeginFrame() override;
			void RenderFrame() override {}
			void EndFrame() override;
			void SwapBuffers() override;

			void DrawDebugData();
			void DrawDebugStrings();
			void DrawDebugLines();

			int get_shader_property_location(const std::string& shader_property_name) const;

			void bind_int_to_shader(const std::string& shader_property_name, const int& data) override;
			void bind_float_to_shader(const std::string& shader_property_name, const float& data) override;
			void bind_vector_to_shader(const std::string& shader_property_name, unsigned size, const float* data) override;
			void bind_matrix4_to_shader(const std::string& shader_property_name, const float* data) override;
			void bind_texture_to_shader(const std::string& shader_property_name, const TextureBase& data) override;
			
			void reset_shader_for_next_object() override;
			void reset_state_for_next_frame() override;
			void free_reserved_textures() override;
			unsigned reserve_texture(const TextureBase& data) override;
			void bind_reserved_texture(const std::string& shader_property_name, unsigned texture_address) override;

			TextureBase* init_blank_texture(unsigned width, unsigned height) const override;

#ifdef _WIN32
			void InitWithWin32(Window& w);
			void DestroyWithWin32() const;
			
			HDC device_context_{}; //...Device context?
			HGLRC render_context_{}; //Permanent Rendering Context		
#endif
		private:
			struct DebugString
			{
				Vector4 colour;
				Vector2 pos;
				float size{};
				std::string text;
			};

			struct DebugLine
			{
				Vector3 start;
				Vector3 end;
				Vector4 colour;
			};

			OGLMesh* debug_lines_mesh_;
			OGLMesh* debug_text_mesh_;

			OGLMesh* bound_mesh_;
			OGLShader* bound_shader_;

			OGLShader* debug_shader_;
			SimpleFont* font_;
			std::vector<DebugString> debug_strings_;
			std::vector<DebugLine> debug_lines_;

			bool init_state_;
			bool force_valid_debug_state_;
			unsigned current_tex_unit_;

			bool reserved_texture_slot_[GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS] = { false };
		};
	}
}
