#include "GameTechRenderer.h"

#include "OGLTexture.h"
#include "../CSC8503Common/GameObject.h"
#include "../../Common/Camera.h"
#include "../../Common/Vector3.h"
#include "../../Common/TextureLoader.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

constexpr unsigned shadow_size = 4096;

Matrix4 bias_matrix = Matrix4::Translation(Vector3(0.5, 0.5, 0.5)) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));

GameTechRenderer::GameTechRenderer(GameWorld& world) : OGLRenderer(*Window::GetWindow()), game_world_(world),
                                                       skybox_mesh_(new OGLMesh()), skybox_tex_(0), shadow_tex_(0), shadow_fbo_(0),
                                                       light_radius_(1000.0f)
{
	glEnable(GL_DEPTH_TEST);

	shadow_shader_ = new OGLShader("GameTechShadowVert.glsl", "GameTechShadowFrag.glsl");
	glGenTextures(1, &shadow_tex_);
	glBindTexture(GL_TEXTURE_2D, shadow_tex_);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	             shadow_size, shadow_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadow_fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, shadow_tex_, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 1, 1, 1);
	
	light_colour_ = Vector4(0.8f, 0.8f, 0.5f, 1.0f);
	light_position_ = Vector3(-200.0f, 60.0f, -200.0f);
	
	skybox_shader_ = new OGLShader("skyboxVertex.glsl", "skyboxFragment.glsl");
	skybox_mesh_->SetVertexPositions({Vector3(-1, 1, -1), Vector3(-1, -1, -1), Vector3(1, -1, -1), Vector3(1, 1, -1)});
	skybox_mesh_->SetVertexIndices({0, 1, 2, 2, 3, 0});
	skybox_mesh_->UploadToGPU();

	load_skybox();


}

GameTechRenderer::~GameTechRenderer()
{
	glDeleteTextures(1, &shadow_tex_);
	glDeleteFramebuffers(1, &shadow_fbo_);
}

void GameTechRenderer::load_skybox()
{
	constexpr const char *file_names[] = {
		"/Cubemap/skyrender0004.png",
		"/Cubemap/skyrender0001.png",
		"/Cubemap/skyrender0003.png",
		"/Cubemap/skyrender0006.png",
		"/Cubemap/skyrender0002.png",
		"/Cubemap/skyrender0005.png"
	};

	int width[6] = {0};
	int height[6] = {0};
	int channels[6] = {0};
	int flags[6] = {0};

	vector<char*> tex_data(6, nullptr);

	for (int i = 0; i < 6; ++i)
	{
		TextureLoader::LoadTexture(file_names[i], tex_data[i], width[i], height[i], channels[i], flags[i]);
		if (i > 0 && (width[i] != width[0] || height[i] != height[0]))
		{
			std::cout << __FUNCTION__ << " cubemap input textures don't match in size?\n";
			return;
		}
	}
	glGenTextures(1, &skybox_tex_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex_);

	const GLenum type = channels[0] == 4 ? GL_RGBA : GL_RGB;

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width[i], height[i], 0, type, GL_UNSIGNED_BYTE,
		             tex_data[i]);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GameTechRenderer::RenderFrame()
{
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	build_object_list();
	render_shadow_map();
	render_skybox();
	render_camera();
	glDisable(GL_CULL_FACE);
	if (gameUI)
	{
		gameUI->DrawUI();
	}
}

void GameTechRenderer::build_object_list()
{
	active_objects_.clear();

	game_world_.OperateOnContents(
		[&](const GameObject* o)
		{
			if (o->IsActive())
			{
				const RenderObject* g = o->GetRenderObject();
				if (g)
				{
					active_objects_.emplace_back(g);
				}
			}
		}
	);
}

void GameTechRenderer::render_shadow_map()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo_);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, shadow_size, shadow_size);

	glCullFace(GL_FRONT);

	bind_shader(shadow_shader_);
	const int mvp_location = glGetUniformLocation(shadow_shader_->GetProgramID(), "mvpMatrix");

	const Matrix4 shadow_view_matrix = Matrix4::BuildViewMatrix(light_position_, Vector3(0, 0, 0), Vector3(0, 1, 0));
	const Matrix4 shadow_proj_matrix = Matrix4::Perspective(100.0f, 500.0f, 1, 45.0f);

	const Matrix4 mv_matrix = shadow_proj_matrix * shadow_view_matrix;

	shadow_matrix_ = bias_matrix * mv_matrix;

	for (const auto& object : active_objects_)
	{
		Matrix4 model_matrix = (*object).GetTransform()->GetMatrix();
		Matrix4 mvp_matrix = mv_matrix * model_matrix;
		glUniformMatrix4fv(mvp_location, 1, false, reinterpret_cast<float*>(&mvp_matrix));
		BindMesh((*object).GetMesh());
		const unsigned layer_count = (*object).GetMesh()->GetSubMeshCount();
		for (unsigned layer_index = 0; layer_index < layer_count; ++layer_index)
		{
			DrawBoundMesh(layer_index);
		}
	}

	glViewport(0, 0, currentWidth, currentHeight);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void GameTechRenderer::render_skybox()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	const float screen_aspect = static_cast<float>(currentWidth) / static_cast<float>(currentHeight);
	Matrix4 view_matrix = game_world_.GetMainCamera()->BuildViewMatrix();
	Matrix4 proj_matrix = game_world_.GetMainCamera()->BuildProjectionMatrix(screen_aspect);

	bind_shader(skybox_shader_);

	const int proj_location = glGetUniformLocation(skybox_shader_->GetProgramID(), "projMatrix");
	const int view_location = glGetUniformLocation(skybox_shader_->GetProgramID(), "viewMatrix");
	const int tex_location = glGetUniformLocation(skybox_shader_->GetProgramID(), "cubeTex");

	glUniformMatrix4fv(proj_location, 1, false, reinterpret_cast<float*>(&proj_matrix));
	glUniformMatrix4fv(view_location, 1, false, reinterpret_cast<float*>(&view_matrix));

	glUniform1i(tex_location, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex_);

	BindMesh(skybox_mesh_);
	DrawBoundMesh();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void GameTechRenderer::render_camera()
{
	const float screen_aspect = static_cast<float>(currentWidth) / static_cast<float>(currentHeight);
	Matrix4 view_matrix = game_world_.GetMainCamera()->BuildViewMatrix();
	Matrix4 proj_matrix = game_world_.GetMainCamera()->BuildProjectionMatrix(screen_aspect);

	OGLShader* active_shader = nullptr;
	int model_location = 0;
	int colour_location = 0;
	int has_v_col_location = 0;
	int has_tex_location = 0;
	int shadow_location = 0;

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, shadow_tex_);

	for (const auto& object : active_objects_)
	{
		const auto shader = dynamic_cast<OGLShader*>((*object).GetShader());
		bind_shader(shader);
		const bool use_material = object->GetMaterial() != nullptr;


		if (active_shader != shader)
		{
			
			has_tex_location = glGetUniformLocation(shader->GetProgramID(), "hasTexture");
			
			bind_shader_property("cameraPos", game_world_.GetMainCamera()->GetPosition());
			bind_shader_property("projMatrix", proj_matrix);
			bind_shader_property("viewMatrix", view_matrix);
			bind_shader_property("lightPos", light_position_);
			bind_shader_property("lightColour", light_colour_);
			bind_shader_property("lightRadius", light_radius_);
			// bind_shader_property("shadowTex", 1);
			const int shadow_tex_location = glGetUniformLocation(shader->GetProgramID(), "shadowTex");
			glUniform1i(shadow_tex_location, 1);

			active_shader = shader;
		}

		Matrix4 model_matrix = object->GetTransform()->GetMatrix();
		model_matrix = model_matrix * object->GetMesh()->GetLocalTransform();
		bind_shader_property("modelMatrix", model_matrix);

		Matrix4 full_shadow_mat = shadow_matrix_ * model_matrix;
		bind_shader_property("shadowMatrix", full_shadow_mat);

		bind_shader_property("objectColour", object->colour_);
		bind_shader_property("hasVertexColours", !(*object).GetMesh()->GetColourData().empty());
		if (!use_material)
		{
			bind_shader_property("mainTex", *object->texture_);
			bind_shader_property("hasTexture", (OGLTexture*)object->texture_ ? 1 : 0);
		}

		BindMesh(object->GetMesh());
		const unsigned layer_count = object->GetMesh()->GetSubMeshCount();
		for (unsigned count = 0; count < layer_count; ++count) {

			if (use_material)
			{
				TextureBase* texture = object->GetMaterial()->GetMaterialForLayer(count)->GetEntry("Diffuse");
				bind_shader_property("mainTex", *texture);
				bind_shader_property("hasTexture", (OGLTexture*)texture ? 1 : 0);
			}
			DrawBoundMesh(count);
			reset_shader_for_next_object();
		}
	}
}

Matrix4 GameTechRenderer::SetupDebugLineMatrix() const
{
	const float screen_aspect = static_cast<float>(currentWidth) / static_cast<float>(currentHeight);
	const Matrix4 view_matrix = game_world_.GetMainCamera()->BuildViewMatrix();
	const Matrix4 proj_matrix = game_world_.GetMainCamera()->BuildProjectionMatrix(screen_aspect);

	return proj_matrix * view_matrix;
}

Matrix4 GameTechRenderer::SetupDebugStringMatrix() const
{
	return Matrix4::Orthographic(-1, 1.0f, 100, 0, 0, 100);
}

void GameTechRenderer::BeginFrame() {
	gameUI->UpdateUI();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	bind_shader(nullptr);
	BindMesh(nullptr);
}