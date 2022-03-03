#pragma once
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "OGLShader.h"
#include "../CSC8503Common/GameWorld.h"
#include "../GameTech/GameUI.h"

namespace NCL
{
	namespace CSC8503
	{
		class RenderObject;

		class GameTechRenderer : public OGLRenderer
		{
		public:
			explicit GameTechRenderer(GameWorld& world);
			~GameTechRenderer() override;
			void SetUI(const GameUI* ui) { gameUI = ui; };
		protected:
			void RenderFrame() override;
			void BeginFrame() override;

			Matrix4 SetupDebugLineMatrix() const override;
			Matrix4 SetupDebugStringMatrix() const override;

			GameWorld& game_world_;
	

			void build_object_list();
			void render_shadow_map();
			void render_camera();
			void render_skybox();

			void load_skybox();

			vector<const RenderObject*> active_objects_;

			OGLShader* skybox_shader_;
			OGLMesh* skybox_mesh_;
			GLuint skybox_tex_;

			//shadow mapping things
			OGLShader* shadow_shader_;
			GLuint shadow_tex_;
			GLuint shadow_fbo_;
			Matrix4 shadow_matrix_;

			Vector4 light_colour_;
			float light_radius_;
			Vector3 light_position_;

			const GameUI* gameUI = nullptr;
		};
	}
}
