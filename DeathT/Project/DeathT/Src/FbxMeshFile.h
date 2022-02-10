#ifndef FBX_MESH_FILE_H_
#define FBX_MESH_FILE_H_
#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }

#include <fbxsdk.h>
#include <vector>
#include <map>
#include <d3d11.h>
#include "DirectGraphics.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "GraphicsUtility.h"

#include "GameMathLib.h"
class FbxMeshFile
{
public://ゲーム側で使うデータ
	Vector3 pos;
	Vector3 degree;
	Vector3 scale;
	Vector3 axis;	
	float axisAngle;
	Vector3 axisR;
	DirectX::XMFLOAT4 multiplyColor;
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
		
		SAFE_RELEASE(g_pRasterizerStateSV);
		SAFE_RELEASE(g_pDepthStencilStateSV);
		SAFE_RELEASE(g_pBlendStateShadow);
		SAFE_RELEASE(g_pDepthStencilStateShadow);

		//本体のモデル
		if (m_VertexShader != nullptr) {
			delete m_VertexShader;
			m_VertexShader = nullptr;
		}
		if (m_GeometryShader != nullptr) {
			delete m_GeometryShader;
			m_GeometryShader = nullptr;
		}
		if (m_PixelShader != nullptr) {
			delete m_PixelShader;
			m_PixelShader = nullptr;
		}
		//シャドウボリューム
		if (m_VertexSVShader != nullptr) {
			delete m_VertexSVShader;
			m_VertexSVShader = nullptr;
		}
		if (m_GeometrySVShader != nullptr) {
			delete m_GeometrySVShader;
			m_GeometrySVShader = nullptr;
		}
		//シャドウ
		if (m_VertexSShader != nullptr) {
			delete m_VertexSShader;
			m_VertexSShader = nullptr;
		}
		if (m_PixelSShader != nullptr) {
			delete m_PixelSShader;
			m_PixelSShader = nullptr;
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

	bool Load(const char* modelfile_name, const char* texfile_name, const char* VertexShaderFilePath, const char* GeometryShaderFilePath, const char* PixelShaderFilePath);

	void Render();

private:
	struct MeshData
	{
		ID3D11Buffer* m_VertexBuffer;			//!< @brief 頂点バッファ(Shader送信用)
		ID3D11Buffer* m_IndexBuffer;			//!< @brief インデックスバッファ(Shader送信用)
		std::vector<CustomVertex> m_Vertices;
		std::vector<UINT> m_Indices;
		std::string m_MaterialName;
		std::string m_UVSetName;



	};
public:
	void SetWorldMatrix();
	bool CreateWorldMatrixBuffer(int _instanceNum);
	void RenderSInstance(int _instanceNum);
	void RenderSMInstance(int _instanceNum);

	ID3D11Buffer* worldBuffer;
	std::vector<DirectX::XMFLOAT4X4> worldMatrix;
	ID3D11DeviceContext* con;
private:

	bool LoadFbxFile(const char* modelfile_name,const char* texfile_name);

	bool CreateVertexBuffer(ID3D11Device* device);

	bool CreateIndexBuffer(ID3D11Device* device);

	bool CreateInputLayout(ID3D11Device* device);

	void CreateMesh(FbxMesh* mesh);

public:
		//アニメーションで利用
		FbxTime* FrameTime;
		FbxTime* timeCount;
		FbxTime* start;
		FbxTime* stop;
		int nowAnimeNum = -1;
		FbxMesh* fMesh[50];
		FbxScene* fbxSceneAnime;
		int meshNum = 0;
		void Animation(int _num,float speed);
private:

	void CreateAnimation(FbxScene* fbxScene);

	void LoadIndices(MeshData& mesh_data, FbxMesh* mesh);

	void LoadVertices(MeshData& mesh_data, FbxMesh* mesh);

	void LoadNormals(MeshData& mesh_data, FbxMesh* mesh);

	void LoadColors(MeshData& mesh_data, FbxMesh* mesh);

	void LoadUV(MeshData& mesh_data, FbxMesh* mesh);

	void LoadMaterial(FbxSurfaceMaterial* material,const char* texfile_name);

	bool LoadTexture(FbxFileTexture* material, std::string& keyword, const char* texfile_name);

	void SetMaterialName(MeshData& mesh_data, FbxMesh* mesh);
	void SetMaterialColor(DirectGraphics* graphics, ObjMaterial& material);

	//シェーダーのロード
	void CreateShader(const char* VertexShaderFilePath=nullptr, const char* GeometryShaderFilePath=nullptr, const char* PixelShaderFilePath=nullptr);

public:
	//影のシェーダーロード
	void CreateSVShader(const char* VertexShaderFilePath, const char* GeometryShaderFilePath);
	void CreateSShader(const char* VertexShaderFilePath, const char* PixelShaderFilePath);

	//void RenderSV(DirectGraphics* graphics);
	//void RenderS(DirectGraphics* graphics);

	void RenderS();
	void RenderS_SM();

private:
	std::vector<MeshData> m_MeshList;
	std::map<std::string, ObjMaterial> m_Materials;
	ID3D11InputLayout* m_InputLayout;								//!< @brief 入力レイアウト
	std::map< std::string, ID3D11ShaderResourceView*> m_Textures;
	std::map<std::string, ID3D11ShaderResourceView*> m_MaterialLinks;

	VertexShader* m_VertexShader;	//!< @brief VertexShader保持用
	PixelShader* m_PixelShader;		//!< @brief PixelShader保持用
	GeometryShader* m_GeometryShader;		//!< @brief GeometryShader保持用

	//シャドウボリューム用シェーダ―
	VertexShader* m_VertexSVShader;	//!< @brief VertexShader保持用
	GeometryShader* m_GeometrySVShader;		//!< @brief GeometryShader保持用
	ID3D11BlendState*         g_pBlendStateSV = NULL;		// ブレンド・ステート・オブジェクト
	ID3D11RasterizerState*	  g_pRasterizerStateSV = NULL;	// ラスタライザ・ステート・オブジェクト
	ID3D11DepthStencilState*  g_pDepthStencilStateSV = NULL;	// 深度/ステンシル・ステート・オブジェクト

	//シャドウ用シェーダー
	VertexShader* m_VertexSShader;	//!< @brief VertexShader保持用
	PixelShader* m_PixelSShader;		//!< @brief PixelShader保持用
	ID3D11BlendState*         g_pBlendStateShadow = NULL;		// ブレンド・ステート・オブジェクト
	ID3D11DepthStencilState*  g_pDepthStencilStateShadow = NULL;	// 深度/ステンシル・ステート・オブジェクト


};
#endif
