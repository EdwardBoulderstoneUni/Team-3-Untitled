#pragma once
#include "Transform.h"
#include "..//../Gameplay/eGameObjectType.h"
#include "PhysicsXObject.h"
#include "RenderObject.h"
#include <vector>
#include <functional>

using std::vector;

namespace NCL
{
	namespace CSC8503
	{
		class NetworkObject;
		class GameObject
		{
			void ( *m_CollisionFunction)(GameObject*, Vector3) ;
		public:
			GameObject(string name = "");
			~GameObject();
			bool IsActive() const
			{
				return isActive;
			}

			Transform& GetTransform()
			{
				return transform;
			}

			RenderObject* GetRenderObject() const
			{
				return renderObject;
			}

			PhysicsXObject* GetPhysicsXObject() const
			{
				return physicsXObject;
			}

			NetworkObject* GetNetworkObject() const {
				return networkObject;
			}

			void SetRenderObject(RenderObject* newObject)
			{
				renderObject = newObject;
			}

			void SetPhysicsXObject(PhysicsXObject* newObject)
			{
				physicsXObject = newObject;
			}

			void SetNetworkObject(int id);

			void SetNetworkObject(NetworkObject* object) {
				networkObject = object;
			}

			const string& GetName() const
			{
				return name;
			}

			void SetCollisionFunction(void (*function)(GameObject*, Vector3)) { m_CollisionFunction = function; }

			virtual void OnCollisionBegin(GameObject* otherObject, Vector3 point = Vector3(0, 5, 0)) { if (m_CollisionFunction) m_CollisionFunction(otherObject, point); }

			virtual void OnCollisionEnd(GameObject* otherObject) {}
			
			bool GetBroadphaseAABB(Vector3& outsize) const;

			void UpdateBroadphaseAABB();

			void SetWorldID(int newID)
			{
				worldID = newID;
			}

			int GetWorldID() const
			{
				return worldID;
			}
			virtual void Update(float dt) {}
			GameObjectType type;

		protected:
			Transform transform;

			PhysicsXObject* physicsXObject;
			RenderObject* renderObject;
			NetworkObject* networkObject;

			bool isActive;
			int worldID;
			string name;

			Vector3 broadphaseAABB;
		};
	}
}
