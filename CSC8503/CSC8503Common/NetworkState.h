#pragma once
#include "../../Common/Vector3.h"
#include "../../Common/Quaternion.h"

namespace NCL {
	using namespace Maths;
	namespace CSC8503 {
		class GameObject;
		class NetworkState {
		public:
			NetworkState();
			virtual ~NetworkState();

			Vector3		position;
			Quaternion	orientation;
			int			stateID;//为了给每个包排序用的id 每次服务器发送一个 这个值就会增加
			//如果客户端收到的包的值小于当前 应该被忽略 因为这是一个过去的无用包

		};
	}
}


