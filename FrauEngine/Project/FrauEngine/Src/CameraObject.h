#pragma once
#include "LowApplication.h"
#include "Object.h"

#include "Camera.h"
namespace frauEngine {

	class CameraObject : public frauEngine::Object {
	protected:
	
		Vector3 pos;
		Vector3 target;
		Vector3 up;

		Vector3 angle;
		float fov;

		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	public:
		void SetPos(Vector3 _pos);
		void SetTarget(Vector3 _target);
		void SetAngle(Vector3 _angle);
		void SetUp(Vector3 _up);
		void SetFov(float _fov);
		void SetAll(Vector3 _pos, Vector3 _angle);


		Vector3 GetPos() { return pos; };
		Vector3 GetTarget() { return target; };
		Vector3 GetAngle() { return angle; };
		Vector3 GetUp() { return up; };
		float GetFov() { return fov; };

		Vector3 GetScreenPos(Vector3 _pos);
		void DrawImGUI();
		void Update();

	};
}