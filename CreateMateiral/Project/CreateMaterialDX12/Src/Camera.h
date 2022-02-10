#pragma once
#include "LowApplication.h"
#include "ConstantBuffer.h"

using namespace DirectX;
namespace camera3D {
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

		cBuffer::CameraBufferHeap cameraBufferHeap;

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

		DirectX::XMMATRIX GetView();
		DirectX::XMMATRIX GetProjection();
		DirectX::XMFLOAT3 GetCameraPos();

		cBuffer::CameraBufferHeap* GetConstantBuffer();
		void Updata();
	};

}