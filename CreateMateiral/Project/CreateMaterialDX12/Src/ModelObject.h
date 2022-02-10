#pragma once
#include "LowApplication.h"
#include "Object.h"
#include "FBX.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "ShaderResourceBuffer.h"

namespace obj {
	
	class ModelObject :public obj::Object {
	//TODO Publicは危険なので早めに修正
	public:
		int meshMax = 0;

		//GPUに転送するデータ Objectが持っているほうがいい
		D3D12_SHADER_BYTECODE shaderVS;//シェーダーを保持しておく
		D3D12_SHADER_BYTECODE shaderPS;
		D3D12_SHADER_BYTECODE shaderDS;
		D3D12_SHADER_BYTECODE shaderHS;
		D3D12_SHADER_BYTECODE shaderGS;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc = {};
		ID3D12PipelineState* pipelineState = nullptr;
		ID3D12RootSignature* rootSignature = nullptr;
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;

		fbx::FBX* resource;
		D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_NONE;

	

		cBuffer::WorldBufferHeap worldBufferHeap;
		cBuffer::BoneBufferHeap boneBufferHeap;
		cBuffer::UtilityBufferHeap utilityBufferHeap;
		cBuffer::PBRBufferHeap* pbrBufferHeap;

		srBuffer::ShaderResourceBufferHeap* anotherResourceBufferHeap[(int)shader::TextureSlot::MODEL_ANOTHER];



	public:
		bool enable = true;
		Vector3 pos;
		Vector3 angle;
		Vector3 scale;

		bool anime = false;
		int animeNum = 0;
		int animeNumOld = 0;

		float animeSpeed = 1;

		float animeCount = 0;

	public:
		~ModelObject();
		void SetResource(fbx::FBX* _model);
		void SetShader(shader::Shader* _shaderV = nullptr, shader::Shader* _shaderP = nullptr, shader::Shader* _shaderG = nullptr, shader::Shader* _shaderH = nullptr, shader::Shader* _shaderD = nullptr);

		void CreatePipeline();

		void Initialize(fbx::FBX* _model,
			shader::Shader* _shaderV = nullptr,
			shader::Shader* _shaderP = nullptr,
			shader::Shader* _shaderG = nullptr,
			shader::Shader* _shaderH = nullptr,
			shader::Shader* _shaderD = nullptr, 
			D3D12_CULL_MODE _cullMode = D3D12_CULL_MODE_NONE
			);

		void SetAnotherTexture(int _meshNum, std::string _filePath, int _num);

		void SetPBR(
			int _meshNum,

			float subsurface,

			float metallic,

			float specular,

			float specularTint,

			float roufhness,

			float anistropics,

			float sheen,

			float sheenTint,

			float clearColor,

			float clearcoatGloss,

			float outline,
			Vector3 outlineColor,

			float workSpace);


		void SetPipelineState(ID3D12PipelineState* _pipelinestate) { pipelineState = _pipelinestate; };
		void SetRootSignature(ID3D12RootSignature* _rootsignature) { rootSignature = rootSignature; };

		//パブリックな変数をあえて関数にする理由はVisualStudioの機能で検索しやすくするため
		void SetPos(Vector3 _pos);
		void SetAngle(Vector3 _angle);
		void SetScale(Vector3 _scale);

		void SetAll(Vector3 _pos, Vector3 _angle, Vector3 _scale);

		void SetAnime(bool _anime);
		void SetAnimeNum(int _animeNum);
		void SetAnimeSpeed(float _animeSpeed);

		void SetAllAnimeState(bool _anime, int _animeNum, float _animeSpeed);

		ID3D12PipelineState* GetPipelineState() { return pipelineState; };
		ID3D12RootSignature* GetRootSignature() { return rootSignature; };
		Vector3 GetPos() { return pos; };
		Vector3 GetScale() { return scale; };
		Vector3 GetAngle() { return angle; };
		bool GetAnime() { return anime; };
		int GetAnimeNum() { return animeNum; };
		float GetAnimeSpeed() { return animeSpeed; }

		int GetMeshNum() { return meshMax; }

		void DrawImGUI();
		void Updata();
		void Draw();
	};
	
}