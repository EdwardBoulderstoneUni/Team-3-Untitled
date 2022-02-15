// ReSharper disable CppClangTidyConcurrencyMtUnsafe
#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame() : world_(new GameWorld()), use_gravity_(false), in_selection_mode_(false), force_magnitude_(10.0f)
{
	renderer_ = new GameTechRenderer(*world_);
	physics_ = new PhysicsSystem(*world_);

	Debug::SetRenderer(renderer_);
	initialise_assets();
}

void TutorialGame::initialise_assets()
{
	auto load_func = [](const string& name, OGLMesh** into)
	{
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(Triangles);
		(*into)->UploadToGPU();
	};

	load_func("cube.msh", &cube_mesh_);
	load_func("sphere.msh", &sphere_mesh_);
	load_func("Male1.msh", &char_mesh_a_);
	load_func("courier.msh", &char_mesh_b_);
	load_func("security.msh", &enemy_mesh_);
	load_func("coin.msh", &bonus_mesh_);
	load_func("capsule.msh", &capsule_mesh_);

	basic_tex_ = dynamic_cast<OGLTexture*>(TextureLoader::LoadAPITexture("checkerboard.png"));
	basic_shader_ = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	init_camera();
	init_world();
}

TutorialGame::~TutorialGame()
{
	delete cube_mesh_;
	delete sphere_mesh_;
	delete char_mesh_a_;
	delete char_mesh_b_;
	delete enemy_mesh_;
	delete bonus_mesh_;

	delete basic_tex_;
	delete basic_shader_;

	delete physics_;
	delete renderer_;
	delete world_;
}

void TutorialGame::update_game(const float dt)
{
	if (!in_selection_mode_)
	{
		world_->GetMainCamera()->UpdateCamera(dt);
	}

	update_keys();

	if (use_gravity_)
	{
		Debug::Print("(G)ravity on", Vector2(5, 95));
	}
	else
	{
		Debug::Print("(G)ravity off", Vector2(5, 95));
	}

	select_object();
	physics_->Update(dt);

	if (locked_object_ != nullptr)
	{
		const Vector3 obj_pos = locked_object_->GetTransform().GetPosition();
		const Vector3 cam_pos = obj_pos + locked_offset_;

		const Matrix4 temp = Matrix4::BuildViewMatrix(cam_pos, obj_pos, Vector3(0, 1, 0));

		const Matrix4 model_mat = temp.Inverse();

		const Quaternion q(model_mat);
		const Vector3 angles = q.ToEuler(); //nearly there now!

		world_->GetMainCamera()->SetPosition(cam_pos);
		world_->GetMainCamera()->SetPitch(angles.x);
		world_->GetMainCamera()->SetYaw(angles.y);

		//Debug::DrawAxisLines(lockedObject->GetTransform().GetMatrix(), 2.0f);
	}

	world_->UpdateWorld(dt);
	renderer_->Update(dt);

	Debug::FlushRenderables(dt);
	renderer_->Render();
}

void TutorialGame::update_keys()
{
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1))
	{
		init_world();
		selection_object_ = nullptr;
		locked_object_ = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2))
	{
		init_camera();
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G))
	{
		use_gravity_ = !use_gravity_;
		physics_->UseGravity(use_gravity_);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9))
	{
		world_->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10))
	{
		world_->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7))
	{
		world_->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8))
	{
		world_->ShuffleObjects(false);
	}

	if (locked_object_)
	{
		locked_object_movement();
	}
	else
	{
		debug_object_movement();
	}
}

void TutorialGame::locked_object_movement() const
{
	const Matrix4 view = world_->GetMainCamera()->BuildViewMatrix();
	const Matrix4 cam_world = view.Inverse();

	const auto right_axis = Vector3(cam_world.GetColumn(0));

	Vector3 fwd_axis = Vector3::Cross(Vector3(0, 1, 0), right_axis);
	fwd_axis.y = 0.0f;
	fwd_axis.Normalise();

	constexpr float force = 100.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT))
	{
		locked_object_->GetPhysicsObject()->AddForce(-right_axis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT))
	{
		locked_object_->GetPhysicsObject()->AddForce(right_axis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP))
	{
		locked_object_->GetPhysicsObject()->AddForce(fwd_axis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN))
	{
		locked_object_->GetPhysicsObject()->AddForce(-fwd_axis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NEXT))
	{
		locked_object_->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
	}
}

void TutorialGame::debug_object_movement() const
{
	if (in_selection_mode_ && selection_object_)
	{
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT))
		{
			selection_object_->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT))
		{
			selection_object_->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM7))
		{
			selection_object_->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8))
		{
			selection_object_->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT))
		{
			selection_object_->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP))
		{
			selection_object_->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN))
		{
			selection_object_->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5))
		{
			selection_object_->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}
}

void TutorialGame::init_camera()
{
	world_->GetMainCamera()->SetNearPlane(0.1f);
	world_->GetMainCamera()->SetFarPlane(500.0f);
	world_->GetMainCamera()->SetPitch(-15.0f);
	world_->GetMainCamera()->SetYaw(315.0f);
	world_->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	locked_object_ = nullptr;
}

void TutorialGame::init_world() const
{
	world_->ClearAndErase();
	physics_->Clear();

	init_mixed_grid_world(5, 5, 3.5f, 3.5f);
	init_game_examples();
	init_default_floor();
}

GameObject* TutorialGame::add_floor_to_world(const Vector3& position) const
{
	const auto floor = new GameObject();

	constexpr auto floor_size = Vector3(100, 2, 100);
	const auto volume = new AABBVolume(floor_size);
	floor->SetBoundingVolume(reinterpret_cast<CollisionVolume*>(volume));
	floor->GetTransform()
	     .SetScale(floor_size * 2)
	     .SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cube_mesh_, basic_tex_, basic_shader_));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world_->AddGameObject(floor);

	return floor;
}


GameObject* TutorialGame::add_sphere_to_world(const Vector3& position, const float radius, const float inverse_mass) const
{
	const auto sphere = new GameObject();

	const auto sphere_size = Vector3(radius, radius, radius);
	const auto volume = new SphereVolume(radius);
	sphere->SetBoundingVolume(reinterpret_cast<CollisionVolume*>(volume));

	sphere->GetTransform()
	      .SetScale(sphere_size)
	      .SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphere_mesh_, basic_tex_, basic_shader_));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverse_mass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world_->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::add_capsule_to_world(const Vector3& position, const float half_height, const float radius, const float inverse_mass) const
{
	const auto capsule = new GameObject();

	const auto volume = new CapsuleVolume(half_height, radius);
	capsule->SetBoundingVolume(volume);

	capsule->GetTransform()
	       .SetScale(Vector3(radius * 2, half_height, radius * 2))
	       .SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsule_mesh_, basic_tex_, basic_shader_));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverse_mass);
	capsule->GetPhysicsObject()->InitCubeInertia();

	world_->AddGameObject(capsule);

	return capsule;
}

GameObject* TutorialGame::add_cube_to_world(const Vector3& position, const Vector3& dimensions, const float inverse_mass) const
{
	const auto cube = new GameObject();

	const auto volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume(reinterpret_cast<CollisionVolume*>(volume));

	cube->GetTransform()
	    .SetPosition(position)
	    .SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cube_mesh_, basic_tex_, basic_shader_));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverse_mass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world_->AddGameObject(cube);

	return cube;
}

void TutorialGame::init_sphere_grid_world(const int num_rows, const int num_cols, const float row_spacing, const float col_spacing, const float radius) const
{
	for (int x = 0; x < num_cols; ++x)
	{
		for (int z = 0; z < num_rows; ++z)
		{
			auto position = Vector3(static_cast<float>(x) * col_spacing, 10.0f, static_cast<float>(z) * row_spacing);
			const auto _ = add_sphere_to_world(position, radius, 1.0f);
		}
	}
	add_floor_to_world(Vector3(0, -2, 0));
}

void TutorialGame::init_mixed_grid_world(const int num_rows, const int num_cols, const float row_spacing, const float col_spacing) const
{
	constexpr auto cube_dims = Vector3(1, 1, 1);

	for (int x = 0; x < num_cols; ++x)
	{
		for (int z = 0; z < num_rows; ++z)
		{
			auto position = Vector3(static_cast<float>(x) * col_spacing * col_spacing, 10.0f, static_cast<float>(z) * row_spacing);

			if (x*z < num_cols * num_rows/2)
			{
				const auto _ = add_cube_to_world(position, cube_dims);
			}
			else
			{
				const auto _ = add_sphere_to_world(position, 1.0f);
			}
		}
	}
}

void TutorialGame::init_cube_grid_world(const int num_rows, const int num_cols, const float row_spacing, const float col_spacing,
                                        const Vector3& cube_dims) const
{
	for (int x = 1; x < num_cols + 1; ++x)
	{
		for (int z = 1; z < num_rows + 1; ++z)
		{
			auto position = Vector3(static_cast<float>(x) * col_spacing, 10.0f, static_cast<float>(z) * row_spacing);
			const auto _ = add_cube_to_world(position, cube_dims, 1.0f);
		}
	}
}

void TutorialGame::init_default_floor() const
{
	add_floor_to_world(Vector3(0, -2, 0));
}

void TutorialGame::init_game_examples() const
{
	add_player_to_world(Vector3(0, 5, 0));
	add_enemy_to_world(Vector3(5, 5, 0));
	add_bonus_to_world(Vector3(10, 5, 0));
}

GameObject* TutorialGame::add_player_to_world(const Vector3& position) const
{
	constexpr float mesh_size = 3.0f;
	constexpr float inverse_mass = 0.5f;

	const auto character = new GameObject();

	const auto volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * mesh_size);

	character->SetBoundingVolume(reinterpret_cast<CollisionVolume*>(volume));

	character->GetTransform()
	         .SetScale(Vector3(mesh_size, mesh_size, mesh_size))
	         .SetPosition(position);

	if (rand() % 2)
	{
		character->SetRenderObject(new RenderObject(&character->GetTransform(), char_mesh_a_, nullptr, basic_shader_));
	}
	else
	{
		character->SetRenderObject(new RenderObject(&character->GetTransform(), char_mesh_b_, nullptr, basic_shader_));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverse_mass);
	character->GetPhysicsObject()->InitSphereInertia();

	world_->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::add_enemy_to_world(const Vector3& position) const
{
	constexpr float mesh_size = 3.0f;
	constexpr float inverse_mass = 0.5f;

	const auto character = new GameObject();

	const auto volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * mesh_size);
	character->SetBoundingVolume(reinterpret_cast<CollisionVolume*>(volume));

	character->GetTransform()
	         .SetScale(Vector3(mesh_size, mesh_size, mesh_size))
	         .SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemy_mesh_, nullptr, basic_shader_));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverse_mass);
	character->GetPhysicsObject()->InitSphereInertia();

	world_->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::add_bonus_to_world(const Vector3& position) const
{
	const auto apple = new GameObject();

	const auto volume = new SphereVolume(0.25f);
	apple->SetBoundingVolume(reinterpret_cast<CollisionVolume*>(volume));
	apple->GetTransform()
	     .SetScale(Vector3(0.25, 0.25, 0.25))
	     .SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonus_mesh_, nullptr, basic_shader_));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world_->AddGameObject(apple);

	return apple;
}

bool TutorialGame::select_object()
{
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q))
	{
		in_selection_mode_ = !in_selection_mode_;
		if (in_selection_mode_)
		{
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else
		{
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (in_selection_mode_)
	{
		renderer_->DrawString("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(MouseButtons::LEFT))
		{
			if (selection_object_)
			{
				selection_object_->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selection_object_ = nullptr;
				locked_object_ = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world_->GetMainCamera());

			RayCollision closestCollision;
			if (world_->Raycast(ray, closestCollision, true))
			{
				selection_object_ = static_cast<GameObject*>(closestCollision.node);
				selection_object_->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				return true;
			}
			return false;
		}
	}
	else
	{
		renderer_->DrawString("Press Q to change to select mode!", Vector2(5, 85));
	}

	if (locked_object_)
	{
		renderer_->DrawString("Press L to unlock object!", Vector2(5, 80));
	}

	else if (selection_object_)
	{
		renderer_->DrawString("Press L to lock selected object object!", Vector2(5, 80));
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::L))
	{
		if (selection_object_)
		{
			if (locked_object_ == selection_object_)
			{
				locked_object_ = nullptr;
			}
			else
			{
				locked_object_ = selection_object_;
			}
		}
	}

	return false;
}