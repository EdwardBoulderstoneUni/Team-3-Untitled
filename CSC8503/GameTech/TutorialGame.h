#pragma once
#include "GameTechRenderer.h"
#include "OGLTexture.h"
#include "../CSC8503Common/PhysicsSystem.h"

namespace NCL
{
	namespace CSC8503
	{
		class TutorialGame
		{
		public:
			TutorialGame();
			virtual ~TutorialGame();

			virtual void update_game(float dt);

		protected:
			void initialise_assets();

			void init_camera();
			void update_keys();

			void init_world() const;

			void init_game_examples() const;

			void init_sphere_grid_world(int num_rows, int num_cols, float row_spacing, float col_spacing, float radius) const;
			void init_mixed_grid_world(int num_rows, int num_cols, float row_spacing, float col_spacing) const;
			void init_cube_grid_world(int num_rows, int num_cols, float row_spacing, float col_spacing,
			                       const Vector3& cube_dims) const;
			void init_default_floor() const;

			bool select_object();
			void debug_object_movement() const;
			void locked_object_movement() const;

			GameObject* add_floor_to_world(const Vector3& position) const;
			GameObject* add_sphere_to_world(const Vector3& position, float radius, float inverse_mass = 10.0f) const;
			GameObject* add_cube_to_world(const Vector3& position, const Vector3& dimensions, float inverse_mass = 10.0f) const;

			GameObject* add_capsule_to_world(const Vector3& position, float half_height, float radius,
			                              float inverse_mass = 10.0f) const;

			GameObject* add_player_to_world(const Vector3& position) const;
			GameObject* add_enemy_to_world(const Vector3& position) const;
			GameObject* add_bonus_to_world(const Vector3& position) const;

			GameTechRenderer* renderer_;
			PhysicsSystem* physics_;
			GameWorld* world_;

			bool use_gravity_;
			bool in_selection_mode_;

			float force_magnitude_;

			GameObject* selection_object_ = nullptr;

			OGLMesh* capsule_mesh_ = nullptr;
			OGLMesh* cube_mesh_ = nullptr;
			OGLMesh* sphere_mesh_ = nullptr;
			OGLTexture* basic_tex_ = nullptr;
			OGLShader* basic_shader_ = nullptr;
			
			OGLMesh* char_mesh_a_ = nullptr;
			OGLMesh* char_mesh_b_ = nullptr;
			OGLMesh* enemy_mesh_ = nullptr;
			OGLMesh* bonus_mesh_ = nullptr;
			
			GameObject* locked_object_ = nullptr;
			Vector3 locked_offset_ = Vector3(0, 14, 20);

			void lock_camera_to_object(GameObject* o)
			{
				locked_object_ = o;
			}
		};
	}
}
