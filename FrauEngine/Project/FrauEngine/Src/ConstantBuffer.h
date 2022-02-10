#pragma once
#include "LowApplication.h"
#include "Shader.h"

namespace frauEngine {

	enum ConstantBufferHeapSlot {
		HEAP_SLOT_MODEL = 7
	};
	enum LightSlot {
		POINT_LIGHT=4,
		SPOT_LIGHT=4,

	};


	//�萔�o�b�t�@�̒��g���`//////////////////
	struct WorldBuffer {
		DirectX::XMMATRIX world;
	};

	struct BoneBuffer {
		DirectX::XMMATRIX boneMatrix[512];
	};
	struct MaterialBuffer {
		DirectX::XMFLOAT4  ambient;
		DirectX::XMFLOAT4  diffuse;
		DirectX::XMFLOAT4  specular;
	};
	struct CameraBuffer {
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		DirectX::XMFLOAT4 pos;
	};
	struct LightBuffer {
		DirectX::XMFLOAT4 ambientLight;////�p�b�L���O�̉e����float4��x�����g�p

		DirectX::XMMATRIX directionalLightView;
		DirectX::XMMATRIX directionalLightProjection;
		DirectX::XMFLOAT4 directionalLightPos;
		DirectX::XMFLOAT4 directionalLightVector;
		DirectX::XMFLOAT4 directionalLightColor;

		DirectX::XMFLOAT4 pointLightPos[POINT_LIGHT];
		DirectX::XMFLOAT4 pointLightRange[POINT_LIGHT];////�p�b�L���O�̉e����float4��x�����g�p
		DirectX::XMFLOAT4 pointLightColor[POINT_LIGHT];

		DirectX::XMFLOAT4 spotLightPos[SPOT_LIGHT];
		DirectX::XMFLOAT4 spotLightRange[SPOT_LIGHT];
		DirectX::XMFLOAT4 spotLightTarget[SPOT_LIGHT];
		DirectX::XMFLOAT4 spotLightAngularRange[SPOT_LIGHT];
		DirectX::XMFLOAT4 spotLightColor[SPOT_LIGHT];

	};

	struct UtilityBuffer {
		int time = 0;
		int meshNum = -1;
	};
	struct PBRBuffer {

		float subsurface;

		float metallic;

		float specular;

		float specularTint;

		float roufhness;

		float anistropics;

		float sheen;

		float sheenTint;

		float clearColor;

		float clearcoatGloss;

		float outline;
		float outlineColorR;
		float outlineColorG;
		float outlineColorB;

		float workSpace;

		int anotherTexture[(int)frauEngine::ShaderTextureSlot::MODEL_ANOTHER];
	};
	/////////////////////////////////////////////
		//�|�X�g�G�t�F�N�g�p�̃o�b�t�@
	struct PostEffectBuffer {
		float forcusU;
		float forcusV;
		int effectNum;
		float blurPower = 1;
		Color colorPlus = { 0,0,0,0 };
		Color colorMulti = { 1,1,1,1 };
	};
	//�f�X�N���v�^�q�[�v��⊮���Ă���/////////////////////////
	class ConstantBufferHeap {
	public:
		ID3D12DescriptorHeap* descripterHeap = nullptr;
	protected:
		void CreateConstantBufferHeap(int dataSize, void** data);
	};

	class WorldBufferHeap : public ConstantBufferHeap {
	public:
		WorldBuffer* buffer = nullptr;
		void Create();
		void Destroy() {
			if (descripterHeap != nullptr) {
				descripterHeap->Release();
			}
			if (buffer != nullptr) {
				//delete buffer;
				buffer = nullptr;
			}
		}
	};
	class BoneBufferHeap : public ConstantBufferHeap {
	public:
		BoneBuffer* buffer=nullptr;
		void Create();
		void Destroy() {
			if (descripterHeap != nullptr) {
				descripterHeap->Release();
			}
			if (buffer != nullptr) {
				//delete buffer;
				buffer = nullptr;
			}
		}
	};	
	class MaterialBufferHeap : public ConstantBufferHeap {
	public:
		MaterialBuffer* buffer=nullptr;
		void Create();
		void Destroy() {
			if (descripterHeap != nullptr) {
				descripterHeap->Release();
			}
			if (buffer != nullptr) {
				//delete buffer;
				buffer = nullptr;
			}
		}
	};	
	class CameraBufferHeap : public ConstantBufferHeap {
	public:
		CameraBuffer* buffer=nullptr;
		void Create();
		void Destroy() {
			if (descripterHeap != nullptr) {
				descripterHeap->Release();
			}
			if (buffer != nullptr) {
				//delete buffer;
				buffer = nullptr;
			}
		}
	};	
	class LightBufferHeap : public ConstantBufferHeap {
	public:
		LightBuffer* buffer=nullptr;
		void Create();
		void Destroy() {
			if (descripterHeap != nullptr) {
				descripterHeap->Release();
			}
			if (buffer != nullptr) {
				//delete buffer;
				buffer = nullptr;
			}
		}
	};	
	class UtilityBufferHeap : public ConstantBufferHeap {
	public:
		UtilityBuffer* buffer=nullptr;
		void Create();
		void Destroy() {
			if (descripterHeap != nullptr) {
				descripterHeap->Release();
			}
			if (buffer != nullptr) {
				//delete buffer;
				buffer = nullptr;
			}
		}
	};
	class PBRBufferHeap : public ConstantBufferHeap {
	public:
		PBRBuffer* buffer=nullptr;
		void Create();
		void Destroy() {
			if (descripterHeap != nullptr) {
				descripterHeap->Release();
			}
			if (buffer != nullptr) {
				//delete buffer;
				buffer = nullptr;
			}
		}
	};
	class PostEffectBufferHeap : public ConstantBufferHeap {
	public:
		PostEffectBuffer* buffer=nullptr;
		void Create();
		void Destroy() {
			if (descripterHeap != nullptr) {
				descripterHeap->Release();
			}
			if (buffer != nullptr) {
				//delete buffer;
				buffer = nullptr;
			}
		}
	};
	//////////////////////////////////////////////////////
}