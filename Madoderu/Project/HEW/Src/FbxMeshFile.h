#ifndef FBX_MESH_FILE_H_
#define FBX_MESH_FILE_H_

#include <fbxsdk.h>
#include <vector>
#include <map>
#include <d3d11.h>
#include "DirectGraphics.h"
#include "VertexShader.h"
#include "GraphicsUtility.h"

#include "GameMathLib.h"
class FbxMeshFile
{
public:
	/** Constructor */
	FbxMeshFile() :
		m_InputLayout(nullptr)
	{
		m_MeshList.clear();
	}

	/** Destructor */
	~FbxMeshFile()
	{

		if (m_VertexShader != nullptr) {
			delete m_VertexShader;
			m_VertexShader = nullptr;
		}
		if (m_PixelShader != nullptr) {
			delete m_PixelShader;
			m_PixelShader = nullptr;
		}
		

		for (auto texture : m_Textures)
		{
			if (texture.second != nullptr)
			{
				texture.second->Release();
			}
		}
		m_Textures.clear();
		m_MaterialLinks.clear();

		for (auto mesh : m_MeshList)
		{
			if (mesh.m_VertexBuffer != nullptr)
			{
				mesh.m_VertexBuffer->Release();
			}

			if (mesh.m_IndexBuffer != nullptr)
			{
				mesh.m_IndexBuffer->Release();
			}
			mesh.m_Indices.clear();
			mesh.m_Vertices.clear();
		}

		if (m_InputLayout != nullptr)
		{
			m_InputLayout->Release();
			m_InputLayout = nullptr;
		}


		if (FrameTime != nullptr) {
			delete FrameTime;
			FrameTime = nullptr;
		}
		if (timeCount != nullptr) {
			delete timeCount;
			timeCount = nullptr;
		}
		if (start != nullptr) {
			delete start;
			start = nullptr;
		}
		if (stop != nullptr) {
			delete stop;
			stop = nullptr;
		}
	}

	/**
	* @brief ���f���ǂݍ���@n
	* ���f���f�[�^��ǂݍ���
	* @return �쐬�̐��� ����(true)
	* @param[in] device �f�o�C�X
	* @param[in] vertex_shader ���f���Ɗ֘A�t����VertexShader
	*/
	bool Load(const char* file_name, const char* VertexShaderFilePath, const char* PixelShaderFilePath);

	/**
	* @brief ���f���`��֐�@n
	* �w�肳�ꂽ���Ń��f����`�悷��
	* @return �쐬�̐��� ����(true)
	* @param[in] graphics �`��Ǘ��N���X
	* @param[in] pos �`����W
	* @param[in] scale �g�k��
	* @param[in] degree �p�x
	*/
	void Render(DirectGraphics* graphics, Vector3 pos, Vector3 scale, Vector3 degree);

private:
	struct MeshData
	{
		ID3D11Buffer* m_VertexBuffer;			//!< @brief ���_�o�b�t�@(Shader���M�p)
		ID3D11Buffer* m_IndexBuffer;			//!< @brief �C���f�b�N�X�o�b�t�@(Shader���M�p)
		std::vector<CustomVertex> m_Vertices;
		std::vector<UINT> m_Indices;
		std::string m_MaterialName;
		std::string m_UVSetName;



	};
public:
	ID3D11DeviceContext* con;
private:

	/**
	* @brief ���b�V������@n
	* �t�@�C�����烁�b�V���𐶐�����
	* @return �쐬�̐��� ����(true)
	* @param[in] file_name �t�@�C����
	*/
	bool LoadFbxFile(const char* file_name);

	/**
	* @brief VertexBuffer�쐬@n
	* VertexBuffer���쐬����
	* @return �쐬�̐��� ����(true)
	* @param[in] device DirectX11�O���t�B�N�X�f�o�C�X
	*/
	bool CreateVertexBuffer(ID3D11Device* device);

	/**
	* @brief IndexBuffer�쐬@n
	* IndexBuffer���쐬����
	* @return �쐬�̐��� ����(true)
	* @param[in] device DirectX11�O���t�B�N�X�f�o�C�X
	*/
	bool CreateIndexBuffer(ID3D11Device* device);

	/**
	* @brief InputLayout�쐬@n
	* InputLayout���쐬����
	* @return �쐬�̐��� ����(true)
	* @param[in] device DirectX11�O���t�B�N�X�f�o�C�X
	* @param[in] vertex_shader InputLayout�Ɗ֘A�t���钸�_�V�F�[�_
	*/
	bool CreateInputLayout(ID3D11Device* device);

	/**
	* @brief ���b�V���f�[�^���쐬����
	* @param[in] node_name �m�[�h�̖��O
	* @param[in] mesh Mesh����邽�߂�Node��Mesh�f�[�^
	*/
	void CreateMesh(FbxMesh* mesh);

	/**
	* @brief ���b�V���f�[�^���쐬����
	* @param[in] node_name �m�[�h�̖��O
	* @param[in] mesh Mesh����邽�߂�Node��Mesh�f�[�^
	*/
	public:
		//�A�j���[�V�����ŗ��p
		FbxTime* FrameTime;
		FbxTime* timeCount;
		FbxTime* start;
		FbxTime* stop;
		int nowAnimeNum = -1;
		FbxMesh* fMesh;
		FbxScene* fbxSceneAnime;

		void Animation(int _num,float speed);
	private:
	void CreateAnimation(FbxScene* fbxScene);
	/**
	* @brief ���_�C���f�b�N�X�f�[�^��ǂݍ���
	* @param[out] mesh_data �o�b�t�@�ۑ��p
	* @param[in] mesh �o�b�t�@�擾������Mesh�f�[�^
	*/
	void LoadIndices(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief ���_�f�[�^��ǂݍ���
	* @param[out] mesh_data �o�b�t�@�ۑ��p
	* @param[in] mesh �o�b�t�@�擾������Mesh�f�[�^
	*/
	void LoadVertices(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief �@���f�[�^��ǂݍ���
	* @param[out] mesh_data �o�b�t�@�ۑ��p
	* @param[in] mesh �o�b�t�@�擾������Mesh�f�[�^
	*/
	void LoadNormals(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief ���_�J���[�f�[�^��ǂݍ���
	* @param[out] mesh_data �o�b�t�@�ۑ��p
	* @param[in] mesh �o�b�t�@�擾������Mesh�f�[�^
	*/
	void LoadColors(MeshData& mesh_data, FbxMesh* mesh);

	void LoadUV(MeshData& mesh_data, FbxMesh* mesh);

	/**
	* @brief �}�e���A���f�[�^��ǂݍ���
	* @param[out] mesh_data �o�b�t�@�ۑ��p
	* @param[in] mesh �o�b�t�@�擾������Mesh�f�[�^
	*/
	void LoadMaterial(FbxSurfaceMaterial* material);

	bool LoadTexture(FbxFileTexture* material, std::string& keyword);

	void SetMaterialName(MeshData& mesh_data, FbxMesh* mesh);
	void SetMaterialColor(DirectGraphics* graphics, ObjMaterial& material);

	//�V�F�[�_�[�̃��[�h
	void CreateShader(const char* VertexShaderFilePath=nullptr, const char* PixelShaderFilePath=nullptr);

private:
	std::vector<MeshData> m_MeshList;
	std::map<std::string, ObjMaterial> m_Materials;
	ID3D11InputLayout* m_InputLayout;								//!< @brief ���̓��C�A�E�g
	std::map< std::string, ID3D11ShaderResourceView*> m_Textures;
	std::map<std::string, ID3D11ShaderResourceView*> m_MaterialLinks;

	VertexShader* m_VertexShader;	//!< @brief VertexShader�ێ��p
	PixelShader* m_PixelShader;		//!< @brief PixelShader�ێ��p

};
#endif
