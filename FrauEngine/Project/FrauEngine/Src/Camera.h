#pragma once
#include "Library.h"
#include "ConstantBuffer.h"

using namespace DirectX;
namespace frauEngine {
	class Camera {
	public:
		static Camera* GetInstance() {
			static Camera mc;
			return &mc;
		}
	private:
		XMFLOAT3 pos;
		XMFLOAT3 target;
		XMFLOAT3 up;

		float fov;

		frauEngine::CameraBufferHeap cameraBufferHeap;

	public:
		Camera();
		Camera(Vector3 _pos, Vector3 _target, Vector3 _up, float _fov);

		void SetCamera(Vector3 _pos, Vector3 _target, Vector3 _up, float _fov);
		void SetPos(Vector3 _pos);
		void SetTarget(Vector3 _target);
		void SetUp(Vector3 _up);
		void SetFov(float _fov);

		Vector3 GetPos();
		Vector3 GetTarget();
		Vector3 GetUp();
		float GetFov();

		Vector3 GetScreenPos(Vector3 _pos);
		DirectX::XMMATRIX GetView();
		DirectX::XMMATRIX GetProjection();
		DirectX::XMFLOAT3 GetCameraPos();

		frauEngine::CameraBufferHeap* GetConstantBuffer();
		void Update();
	};

}