#pragma once
#include "LowApplication.h"

#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "ShaderResourceBuffer.h"

using namespace std;
using namespace DirectX;

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


namespace fbx {
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
	private:
		using LoadTextureLambda = std::function<HRESULT(const std::wstring path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
		map<std::string, LoadTextureLambda> loadTextureLambda;

		bool isTrianglate = false;

		std::string modelPath;

		FbxManager* fbxManager;
		FbxImporter* fbxImporter;
		FbxScene* fbxScene;

		//�}�e���A���֌W
		std::map<std::string, cBuffer::MaterialBuffer> materials;
		map<string, ID3D12Resource*> textures;
		map<string, ID3D12Resource*> materialLinks;

		//�A�j���[�V�����ŗ��p
		int animeNumMax = 0;
		FbxTime* FrameTime;
		FbxTime* timeCount;
		FbxTime* start;
		FbxTime* stop;
		int nowAnimeNum = -1;
		std::vector<FbxMesh*> fMesh;

		int meshNum = 0;
		//�A�j���[�V�����̃}�g���N�X�@�A�j���ԍ��A���ԁA�ő�512�̃}�g���N�X�ō\������Ă���
		std::vector<std::vector<std::vector<DirectX::XMMATRIX>>> animeMat;//�N���X�ɂԂ�����ł���
		std::vector<int> timeMax;//�e�A�j���[�V�����̍ő�t���[����ۑ�


		std::vector<MeshData> meshList;

		D3D12_INPUT_ELEMENT_DESC* inputLayout;

		//�}�e���A���̃|�C���^�ʒu��ۑ����Ă���
		map<std::string, int> materialPointerName;
		map<std::string, int> materialIndexName;

		//���f���{�̂����萔�o�b�t�@
		std::unordered_map<std::string, cBuffer::MaterialBufferHeap> materialBufferHeap;//�}�e���A��
		std::unordered_map<std::string, srBuffer::ShaderResourceBufferHeap> textureBufferHeap;//�x�[�X�e�N�X�`��

	
	public:
		bool Load(std::string _modelPath, std::string _texturePath);
		void Draw(
			cBuffer::WorldBufferHeap* worldBufferHeap, 
			cBuffer::BoneBufferHeap* boneBufferHeap,
			cBuffer::UtilityBufferHeap* utilityBufferHeap,
			cBuffer::PBRBufferHeap* pbrBufferHeap,
			srBuffer::ShaderResourceBufferHeap** anotherTexBufferHeap,
			ID3D12PipelineState* pipelineState,
			ID3D12RootSignature* rootSignature
			);


		
		void GetBoneMatrix(cBuffer::BoneBuffer* boneBuffer, int _animeNum, int _time) {
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