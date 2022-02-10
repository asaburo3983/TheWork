#pragma once
#include "LowApplication.h"

#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "ShaderResourceBuffer.h"

using namespace std;
using namespace DirectX;

namespace frauEngine {
	//���_�f�[�^
	struct CustomVertex
	{
		Vector3 Posision;		// ���W(x, y, z)
		Vector3 Normal;			// �@��
		Color Color;			// ���_�J���[
		Vector2 TexturePos;		// �e�N�X�`�����W(u, v)
		int BoneIndex[4] = { 0,0,0,0 };
		float BoneWeight[4] = { 0,0,0,0 };

	};

	//FBX�f�[�^���o�p�̍\����
	struct MeshData
	{
		D3D12_VERTEX_BUFFER_VIEW m_VertexBuffer;			//!< @brief ���_�o�b�t�@(Shader���M�p)
		D3D12_INDEX_BUFFER_VIEW m_IndexBuffer;			//!< @brief �C���f�b�N�X�o�b�t�@(Shader���M�p)
		std::vector<CustomVertex> m_Vertices;
		std::vector<UINT> m_Indices;
		std::string m_MaterialName;
		std::string m_UVSetName;
	};

	class FBX {
	public:
		FBX();
		~FBX();
	private:
		using LoadTextureLambda = std::function<HRESULT(const std::wstring path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
		map<std::string, LoadTextureLambda> loadTextureLambda;

		bool isTrianglate = false;

		std::string modelPath;

		FbxManager*  fbxManager;
		FbxImporter* fbxImporter;
		FbxScene*    fbxScene;

		//�}�e���A���֌W
		//�K���ɏ����Ă�
		FbxFileTexture* texture;
		FbxSurfaceMaterial* material;
		vector<ID3D12Resource*> vertBuff;
		vector<ID3D12Resource*> idxBuff;

		
		FbxLayerElementMaterial* materialElements;
		FbxSurfaceMaterial* surface_material;

		std::map<std::string, frauEngine::MaterialBuffer> materials;
		map<string, ID3D12Resource*> textures;
		map<string, ID3D12Resource*> materialLinks;

		//�A�j���[�V�����ŗ��p
		int animeNumMax = 0;
		FbxTime* FrameTime=nullptr;
		FbxTime* timeCount = nullptr;
		FbxTime* start = nullptr;
		FbxTime* stop = nullptr;
		int nowAnimeNum = -1;
		std::vector<FbxMesh*> fMesh;

		int meshNum = 0;
		//�A�j���[�V�����̃}�g���N�X�@�A�j���ԍ��A���ԁA�ő�512�̃}�g���N�X�ō\������Ă���
		std::vector<std::vector<std::vector<DirectX::XMMATRIX>>> animeMat;//�N���X�ɂԂ�����ł���
		std::vector<int> timeMax;//�e�A�j���[�V�����̍ő�t���[����ۑ�


		std::vector<MeshData> meshList;

		D3D12_INPUT_ELEMENT_DESC* inputLayout=nullptr;

		//�}�e���A���̃|�C���^�ʒu��ۑ����Ă���
		map<std::string, int> materialPointerName;
		map<std::string, int> materialIndexName;

		//���f���{�̂����萔�o�b�t�@
		std::unordered_map<std::string, frauEngine::MaterialBufferHeap> materialBufferHeap;//�}�e���A��
		std::unordered_map<std::string, frauEngine::ShaderResourceBufferHeap> textureBufferHeap;//�x�[�X�e�N�X�`��


	public:
		bool Load(std::string _modelPath, std::string _texturePath);
		void Draw(
			frauEngine::WorldBufferHeap* worldBufferHeap,
			frauEngine::BoneBufferHeap* boneBufferHeap,
			frauEngine::UtilityBufferHeap* utilityBufferHeap,
			frauEngine::PBRBufferHeap* pbrBufferHeap,
			frauEngine::ShaderResourceBufferHeap** anotherTexBufferHeap,
			ID3D12PipelineState* pipelineState,
			ID3D12RootSignature* rootSignature
		);



		void GetBoneMatrix(frauEngine::BoneBuffer* boneBuffer, int _animeNum, int _time) {
			for (int i = 0; i < 512; i++) {
				boneBuffer->boneMatrix[i] = animeMat[_animeNum][_time][i];
			}
		}
		int GetAnimeTimeMax(int _animeNum) { return timeMax[_animeNum]; };
		int GetAnimeNumMax() { return animeNumMax; };
		int GetMeshMax() { return meshList.size(); };
	private:
		bool InitFbxScene();

		void LoadMaterials(std::string _textureFolderPath);
		void LoadMaterial(FbxSurfaceMaterial* material, string _texturePath);
		bool LoadTexture(FbxFileTexture* texture, std::string& keyword, string _texturePath);

		void LoadMeshs();
		void LoadMesh(FbxMesh* mesh);
		void LoadIndices(MeshData& mesh_data, FbxMesh* mesh);
		void LoadVertices(MeshData& mesh_data, FbxMesh* mesh);
		void LoadNormals(MeshData& mesh_data, FbxMesh* mesh);
		void LoadColors(MeshData& mesh_data, FbxMesh* mesh);
		void LoadUV(MeshData& mesh_data, FbxMesh* mesh);
		void SetMaterialName(MeshData& mesh_data, FbxMesh* mesh);

		void LoadAnimations();
		void LoadWeight();
		void LoadAnimation();

		bool CreateVertexBuffer();

		bool CreateIndexBuffer();

		void CreateInputLayout();
	};

}