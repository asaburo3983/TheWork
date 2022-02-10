#pragma once
#include "LowApplication.h"
#include "ConstantBuffer.h"
using namespace DirectX;
namespace light {

	class Lights {
	public:
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 up;
		DirectX::XMFLOAT3 target;

		cBuffer::LightBufferHeap lightBufferHeap;

		ID3D12DescriptorHeap* depthHeap;//�[�x�f�[�^�̃q�[�v
		ID3D12DescriptorHeap* depthSRVHeap;//�[�x�e�N�N�`���̃q�[�v

		int shadowTexSize = 1024;

	public:
		static Lights* GetInstance() {
			static Lights Dlight;
			return &Dlight;
		}
		Lights();

		void SetAmbientLight(float _power);

		void SetDirectionalLight(Vector3 _pos, Vector3 _target, Vector3 _up, float _fov, Vector3 _vector, Vector3 _color);

		void SetPointLight(int _lightNum, Vector3 _pos, float _range, Vector3 _color = Vector3(1, 1, 1));
		void SetSpotLight(int _lightNum, Vector3 _pos, float _range, Vector3 _target, float _anglerRange, Vector3 _color = Vector3(1, 1, 1));

		cBuffer::LightBufferHeap* GetConstantBuffer() {return  &lightBufferHeap;};
	};

}