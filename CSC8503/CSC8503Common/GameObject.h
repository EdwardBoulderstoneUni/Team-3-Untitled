#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "PhysicsObject.h"
#include "../GameTech/PhysicsXObject.h"
#include "RenderObject.h"

#include <vector>
#include "../GameTech/PhysicsXSystem.h"

using std::vector;

namespace NCL
{
	namespace CSC8503
	{
		class NetworkObject;
		class GameObject
		{
		public:
			explicit GameObject(string name = "");
			GameObject(PhysicsXSystem* physics_system, VolumeType volume_type, const Vector3& mesh_shape, const Vector3& shape_dimensions,
			           const Vector3& position, float inverse_mass,
			           const string& name = "");
			virtual ~GameObject();

			void SetBoundingVolume(CollisionVolume* vol)
			{
				bounding_volume_ = vol;
			}

			const CollisionVolume* GetBoundingVolume() const
			{
				return bounding_volume_;
			}

			bool IsActive() const
			{
				return is_active_;
			}

			Transform& GetTransform()
			{
				return transform_;
			}

			RenderObject* GetRenderObject() const
			{
				return render_object_;
			}

			PhysicsObject* GetPhysicsObject() const
			{
				return physics_object_;
			}

			PhysicsXObject* GetPhysicsXObject() const
			{
				return physics_x_object_;
			}

			NetworkObject* GetNetworkObject() const {
				return network_object_;
			}

			void SetRenderObject(RenderObject* new_object)
			{
				render_object_ = new_object;
			}

			void SetPhysicsObject(PhysicsObject* new_object)
			{
				physics_object_ = new_object;
			}

			void SetPhysicsXObject(PhysicsXObject* new_object)
			{
				physics_x_object_ = new_object;
			}

			void SetNetworkObject(int id);

			void SetNetworkObject(NetworkObject* object) {
				network_object_ = object;
			}

			const string& GetName() const
			{
				return name_;
			}

			virtual void OnCollisionBegin(GameObject* other_object) {}

			virtual void OnCollisionEnd(GameObject* other_object) {}

			bool GetBroadphaseAABB(Vector3& outsize) const;

			void UpdateBroadphaseAABB();

			void SetWorldID(const int new_id)
			{
				worldID = new_id;
			}

			int GetWorldID() const
			{
				return worldID;
			}

		protected:
			Transform transform_;

			CollisionVolume* bounding_volume_;
			PhysicsObject* physics_object_;
			PhysicsXObject* physics_x_object_;
			RenderObject* render_object_;
			NetworkObject* network_object_;

			bool is_active_;
			int worldID;
			string name_;

			Vector3 broadphaseAABB;
		};
	}
}