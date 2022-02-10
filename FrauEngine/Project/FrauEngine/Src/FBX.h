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
	//頂点データ
	struct CustomVertex
	{
		Vector3 Posision;		// 座標(x, y, z)
		Vector3 Normal;			// 法線
		Color Color;			// 頂点カラー
		Vector2 TexturePos;		// テクスチャ座標(u, v)
		int BoneIndex[4] = { 0,0,0,0 };
		float BoneWeight[4] = { 0,0,0,0 };

	};

	//FBXデータ抽出用の構造体
	struct MeshData
	{
		D3D12_VERTEX_BUFFER_VIEW m_VertexBuffer;			//!< @brief 頂点バッファ(Shader送信用)
		D3D12_INDEX_BUFFER_VIEW m_IndexBuffer;			//!< @brief インデックスバッファ(Shader送信用)
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

		//マテリアル関係
		//適当に書いてる
		FbxFileTexture* texture;
		FbxSurfaceMaterial* material;
		vector<ID3D12Resource*> vertBuff;
		vector<ID3D12Resource*> idxBuff;

		
		FbxLayerElementMaterial* materialElements;
		FbxSurfaceMaterial* surface_material;

		std::map<std::string, frauEngine::MaterialBuffer> materials;
		map<string, ID3D12Resource*> textures;
		map<string, ID3D12Resource*> materialLinks;

		//アニメーションで利用
		int animeNumMax = 0;
		FbxTime* FrameTime=nullptr;
		FbxTime* timeCount = nullptr;
		FbxTime* start = nullptr;
		FbxTime* stop = nullptr;
		int nowAnimeNum = -1;
		std::vector<FbxMesh*> fMesh;

		int meshNum = 0;
		//アニメーションのマトリクス　アニメ番号、時間、最大512個のマトリクスで構成されている
		std::vector<std::vector<std::vector<DirectX::XMMATRIX>>> animeMat;//クラスにぶち込んでおく
		std::vector<int> timeMax;//各アニメーションの最大フレームを保存


		std::vector<MeshData> meshList;

		D3D12_INPUT_ELEMENT_DESC* inputLayout=nullptr;

		//マテリアルのポインタ位置を保存しておく
		map<std::string, int> materialPointerName;
		map<std::string, int> materialIndexName;

		//モデル本体が持つ定数バッファ
		std::unordered_map<std::string, frauEngine::MaterialBufferHeap> materialBufferHeap;//マテリアル
		std::unordered_map<std::string, frauEngine::ShaderResourceBufferHeap> textureBufferHeap;//ベーステクスチャ


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