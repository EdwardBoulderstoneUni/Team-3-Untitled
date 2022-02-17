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
			int			stateID;//Ϊ�˸�ÿ���������õ�id ÿ�η���������һ�� ���ֵ�ͻ�����
			//����ͻ����յ��İ���ֵС�ڵ�ǰ Ӧ�ñ����� ��Ϊ����һ����ȥ�����ð�

		};
	}
}


