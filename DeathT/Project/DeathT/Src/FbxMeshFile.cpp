#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <codecvt> 
#include <cstdio>
#include <sstream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include "DirectGraphics.h"
#include "Utility.h"
#include "FbxMeshFile.h"
#include <iostream>

#include <string>
#include <map>
#include <vector>
#include <windows.h>
#include <mmsystem.h>
#include <crtdbg.h>
#include <d3d11.h>
#include <DirectXmath.h>//xnamath.hの代替品
#include <d3dcompiler.h>
#include <fbxsdk.h> 
#include < locale.h >
#include "Window.h"
// 必要なライブラリをリンクする
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "legacy_stdio_definitions.lib" )//バージョンが新しいのでこれを書かないとdxerr.libはばぐる
#pragma comment(lib, "d3dcompiler.lib")
#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>
#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#ifdef _DEBUG
#pragma comment(lib, "DirectXTK_d.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#endif
#include <SpriteBatch.h>
#include <DDSTextureLoader.h>
#include <SimpleMath.h>
#include <Audio.h>
#include <SpriteFont.h>

ID3D11Device* dev;



bool FbxMeshFile::Load(const char* modelfile_name, const char* texfile_name, const char* VertexShaderFilePath, const char* GeometryShaderFilePath, const char* PixelShaderFilePath)
{
	
	dev = DirectGraphics::GetInstance()->GetDevice();
	
	//シェーダのコンパイル
	CreateShader(VertexShaderFilePath, GeometryShaderFilePath,PixelShaderFilePath);

	if (LoadFbxFile(modelfile_name,texfile_name) == false)//文字列リスト、フォルダ場所、全ファイルパスを送る　多分フォルダ場所はいらん
	{
		return false;
	}

	if (CreateVertexBuffer(dev) == false)
	{
		return false;
	}

	if (CreateIndexBuffer(dev) == false)
	{
		return false;
	}

	if (CreateInputLayout(dev) == false)
	{
		return false;
	}

	return true;
}

void FbxMeshFile::Render()
{
	DirectGraphics* dg = DirectGraphics::GetInstance();

	dg->m_ConstantBufferData.multiplyColor = multiplyColor;
	dg->SetUpContext(pos,scale,degree, axis,axisAngle,axisR,m_VertexShader,m_GeometryShader,m_PixelShader);


	dg->GetContext()->IASetInputLayout(m_InputLayout);
	UINT strides = sizeof(Vector3);
	strides = sizeof(CustomVertex);
	UINT offsets = 0;
	for (auto& mesh : m_MeshList){

		// IAに設定する頂点バッファの指定
		dg->GetContext()->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer, &strides, &offsets);

		dg->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		SetMaterialColor(dg, m_Materials[mesh.m_MaterialName]);

		// テクスチャ設定
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
			dg->SetTexture(m_MaterialLinks[mesh.m_MaterialName]);
		else
			dg->SetTexture(nullptr);
		
		// 描画
		dg->GetContext()->DrawIndexed(mesh.m_Indices.size(),0,0);
	}
	//DirectGraphics::GetInstance()->SetUpContext(nullptr, nullptr, nullptr);

}

void FbxMeshFile::RenderSInstance(int _instanceNum) {
	UINT strides[2] = { sizeof(CustomVertex) ,sizeof(DirectX::XMFLOAT4X4) };
	UINT offsets[2] = { 0 , 0 };
	DirectGraphics* graphics = DirectGraphics::GetInstance();
	graphics->m_ConstantBufferData.multiplyColor = multiplyColor;
	graphics->SetUpContextSM2(pos, scale, degree, axis, axisAngle, axisR, m_VertexShader, m_GeometryShader, m_PixelShader);

	graphics->GetContext()->IASetInputLayout(m_InputLayout);
	for (auto& mesh : m_MeshList)
	{
		ID3D11Buffer* pBuf[2] = { mesh.m_VertexBuffer,  worldBuffer };
		// IAに設定する頂点バッファの指定
		graphics->GetContext()->IASetVertexBuffers(0, 2, pBuf, strides, offsets);

		graphics->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		SetMaterialColor(graphics, m_Materials[mesh.m_MaterialName]);

		// テクスチャ設定
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
			graphics->SetTextureSM(m_MaterialLinks[mesh.m_MaterialName]);
		else
			graphics->SetTextureSM(nullptr);

		graphics->GetContext()->DrawIndexedInstanced(mesh.m_Indices.size(), _instanceNum, 0, 0, 0);
	}
}
void FbxMeshFile::RenderSMInstance(int _instanceNum) {
	UINT strides[2] = { sizeof(CustomVertex) ,sizeof(DirectX::XMFLOAT4X4) };
	UINT offsets[2] = { 0 , 0 };
	DirectGraphics* graphics = DirectGraphics::GetInstance();


	graphics->SetUpContextSM(pos, scale, degree, axis, axisAngle, axisR, m_VertexShader);

	graphics->GetContext()->IASetInputLayout(m_InputLayout);

	//シャドウマップ書き込み
	for (auto& mesh : m_MeshList)
	{
		ID3D11Buffer* pBuf[2] = { mesh.m_VertexBuffer,  worldBuffer };
		// IAに設定する頂点バッファの指定
		graphics->GetContext()->IASetVertexBuffers(0, 2, pBuf, strides, offsets);

		graphics->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		SetMaterialColor(graphics, m_Materials[mesh.m_MaterialName]);

		// テクスチャ設定
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
			graphics->SetTextureSM(m_MaterialLinks[mesh.m_MaterialName]);
		else
			graphics->SetTextureSM(nullptr);

		// 描画
		graphics->GetContext()->DrawIndexedInstanced(mesh.m_Indices.size(), _instanceNum, 0, 0, 0);

	}
}

void FbxMeshFile::RenderS() {
	UINT strides = sizeof(Vector3);
	strides = sizeof(CustomVertex);
	UINT offsets = 0;
	DirectGraphics* graphics = DirectGraphics::GetInstance();
	graphics->m_ConstantBufferData.multiplyColor = multiplyColor;
	graphics->SetUpContextSM2(pos, scale, degree, axis, axisAngle, axisR, m_VertexShader, m_GeometryShader, m_PixelShader);

	graphics->GetContext()->IASetInputLayout(m_InputLayout);
	for (auto& mesh : m_MeshList)
	{

		// IAに設定する頂点バッファの指定
		graphics->GetContext()->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer, &strides, &offsets);

		graphics->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		SetMaterialColor(graphics, m_Materials[mesh.m_MaterialName]);

		// テクスチャ設定
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
			graphics->SetTextureSM(m_MaterialLinks[mesh.m_MaterialName]);
		else
			graphics->SetTextureSM(nullptr);

		// 描画
		graphics->GetContext()->DrawIndexed(mesh.m_Indices.size(), 0, 0);
	}
}
void FbxMeshFile::RenderS_SM() {
	UINT strides = sizeof(Vector3);
	strides = sizeof(CustomVertex);
	UINT offsets = 0;
	DirectGraphics* graphics = DirectGraphics::GetInstance();

	
	graphics->SetUpContextSM(pos, scale, degree,axis,axisAngle,axisR, m_VertexShader);

	graphics->GetContext()->IASetInputLayout(m_InputLayout);

	//シャドウマップ書き込み
	for (auto& mesh : m_MeshList)
	{
		// IAに設定する頂点バッファの指定
		graphics->GetContext()->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer, &strides, &offsets);

		graphics->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer,DXGI_FORMAT_R32_UINT,0);

		SetMaterialColor(graphics, m_Materials[mesh.m_MaterialName]);

		// テクスチャ設定
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
			graphics->SetTextureSM(m_MaterialLinks[mesh.m_MaterialName]);
		else
			graphics->SetTextureSM(nullptr);

		// 描画
		graphics->GetContext()->DrawIndexed(mesh.m_Indices.size(),0,0);			

	}

	


}

void FbxMeshFile::LoadVertices(MeshData& mesh_data, FbxMesh* mesh)
{


	// 頂点バッファの取得
	FbxVector4* vertices = mesh->GetControlPoints();
	// インデックスバッファの取得
	int* indices = mesh->GetPolygonVertices();
	// 頂点座標の数の取得
	int polygon_vertex_count = mesh->GetPolygonVertexCount();
	
	for (int i = 0; i < polygon_vertex_count; i++)
	{
		CustomVertex vertex;
		// インデックスバッファから頂点番号を取得
		int index = indices[i];

		// 頂点座標リストから座標を取得する
		vertex.Posision.X = (float)-vertices[index][0];
		vertex.Posision.Y = (float)vertices[index][1];
		vertex.Posision.Z = (float)vertices[index][2];

		// 追加
		mesh_data.m_Vertices.push_back(vertex);
		
	}
}

void FbxMeshFile::LoadIndices(MeshData& mesh_data, FbxMesh* mesh)
{
	// ポリゴン数の取得
	int polygon_count = mesh->GetPolygonCount();

	// ポリゴンの数だけ連番として保存する
	for (int i = 0; i < polygon_count; i++)
	{
		mesh_data.m_Indices.push_back(i * 3 + 2);
		mesh_data.m_Indices.push_back(i * 3 + 1);
		mesh_data.m_Indices.push_back(i * 3);
	}
}

void FbxMeshFile::LoadNormals(MeshData& mesh_data, FbxMesh* mesh)
{
	FbxArray<FbxVector4> normals;
	// 法線リストの取得
	mesh->GetPolygonVertexNormals(normals);

	// 法線設定
	for (int i = 0; i < normals.Size(); i++)
	{
		mesh_data.m_Vertices[i].Normal.X = (float)-normals[i][0];
		mesh_data.m_Vertices[i].Normal.Y = (float)normals[i][1];
		mesh_data.m_Vertices[i].Normal.Z = (float)normals[i][2];
	}
}

void FbxMeshFile::LoadColors(MeshData& mesh_data, FbxMesh* mesh)
{
	// 頂点カラーデータの数を確認
	int color_count = mesh->GetElementVertexColorCount();
	if (color_count == 0)
	{
		return;
	}
	
	// 頂点カラーデータの取得
	FbxGeometryElementVertexColor* vertex_colors = mesh->GetElementVertexColor(0);

	if (vertex_colors == nullptr)
	{
		return;
	}

	FbxLayerElement::EMappingMode mapping_mode = vertex_colors->GetMappingMode();
	FbxLayerElement::EReferenceMode reference_mode = vertex_colors->GetReferenceMode();

	if (mapping_mode == FbxLayerElement::eByPolygonVertex)
	{
		if (reference_mode == FbxLayerElement::eIndexToDirect)
		{
			// 頂点カラーバッファ取得
			FbxLayerElementArrayTemplate<FbxColor>& colors = vertex_colors->GetDirectArray();
			// 頂点カラーインデックスバッファ取得
			FbxLayerElementArrayTemplate<int>& indeces = vertex_colors->GetIndexArray();
			for (int i = 0; i < indeces.GetCount(); i++)
			{
				int id = indeces.GetAt(i);
				FbxColor color = colors.GetAt(id);
				mesh_data.m_Vertices[i].Color.Alpha =  (float)color.mAlpha;
				mesh_data.m_Vertices[i].Color.Red = (float)color.mRed;
				mesh_data.m_Vertices[i].Color.Green = (float)color.mGreen;
				mesh_data.m_Vertices[i].Color.Blue = (float)color.mBlue;
			}
		}
	}
}

void FbxMeshFile::LoadUV(MeshData& mesh_data, FbxMesh* mesh)
{
	FbxStringList uvset_names;
	// UVSetの名前リストを取得
	mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// UVSetの名前からUVSetを取得する
	// 今回はマルチテクスチャには対応しないので最初の名前を使う
	mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), uv_buffer);

	for (int i = 0; i < uv_buffer.Size(); i++)
	{
		FbxVector2& uv = uv_buffer[i];

		// そのまま使用して問題ない
		mesh_data.m_Vertices[i].TexturePos.X = (float)uv[0];
		mesh_data.m_Vertices[i].TexturePos.Y = (float)(1.0 - uv[1]);
	}
}

void FbxMeshFile::SetMaterialName(MeshData& mesh_data, FbxMesh* mesh)
{
	// マテリアルが無ければ終わり
	if (mesh->GetElementMaterialCount() == 0)
	{
		mesh_data.m_MaterialName = "";
		return;
	}

	// Mesh側のマテリアル情報を取得
	FbxLayerElementMaterial* material = mesh->GetElementMaterial(0);
	int index = material->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* surface_material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);
	if (surface_material != nullptr)
	{
		mesh_data.m_MaterialName = surface_material->GetName();
	}
	else
	{
		mesh_data.m_MaterialName = "";
	}
}

bool FbxMeshFile::LoadTexture(FbxFileTexture* texture, std::string& keyword, const char* texfile_name)
{
	if (texture == nullptr)
	{
		return false;
	}

	// ファイル名を取得
	std::string file_path = texture->GetRelativeFileName();
	// ファイル分解
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, file_path.c_str(), sizeof(char) * 256);

	// 記号統一
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// 「/」で分解
	Split('/', buffer, split_list);

	std::string root_path = texfile_name;

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);

	// 文字化け対策
	char* file_name;
	size_t size = 0;
	FbxUTF8ToAnsi(split_list[split_list.size() - 1].c_str(), file_name, &size);

	int eye = m_Textures.count(file_name);
	m_Textures[file_name];

	if (m_Textures.count(file_name) > 0 &&
		m_Textures[file_name] != nullptr)
	{
	//	FbxFree(file_name);
	//	return true;
	}

	if (FAILED(DirectX::CreateWICTextureFromFile(
		DirectGraphics::GetInstance()->GetDevice(),			// ID3D11Device
		wstr_file_name.c_str(),									// ファイル名(wchar_t型なので注意)
		nullptr,											// 通常のテクスチャデータ
		&m_Textures[file_name])))							// シェーダで使用することができるテクスチャデータ
	{
		FbxFree(file_name);
		return false;
	}

	keyword = file_name;
	FbxFree(file_name);
	return true;
}

void FbxMeshFile::LoadMaterial(FbxSurfaceMaterial* material,const char* texfile_name)
{
	ObjMaterial entry_material;
	enum class MaterialOrder
	{
		Ambient,
		Diffuse,
		Specular,
		MaxOrder,
	};

	FbxDouble3 colors[(int)MaterialOrder::MaxOrder];
	FbxDouble factors[(int)MaterialOrder::MaxOrder];
	FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sAmbient);
	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		const char* element_check_list[] =
		{
			FbxSurfaceMaterial::sAmbient,
			FbxSurfaceMaterial::sDiffuse,
		};

		const char* factor_check_list[] =
		{
			FbxSurfaceMaterial::sAmbientFactor,
			FbxSurfaceMaterial::sDiffuseFactor,
		};

		for (int i = 0; i < 2; i++)
		{
			prop = material->FindProperty(element_check_list[i]);
			if (prop.IsValid())//有効か調べているらしい
			{
				colors[i] = prop.Get<FbxDouble3>();

			}
			else
			{
				colors[i] = FbxDouble3(1.0, 0.0, 1.0);
			}
			
			prop = material->FindProperty(factor_check_list[i]);
			if (prop.IsValid())
			{
				factors[i] = prop.Get<FbxDouble>();
			}
			else
			{
				factors[i] = 1.0;
			}
		}
	}

	FbxDouble3 color = colors[(int)MaterialOrder::Ambient];
	FbxDouble factor = factors[(int)MaterialOrder::Ambient];
	entry_material.SetAmbient((float)color[0], (float)color[1], (float)color[2], (float)factor);

	color = colors[(int)MaterialOrder::Diffuse];
	factor = factors[(int)MaterialOrder::Diffuse];
	entry_material.SetDiffuse((float)color[0], (float)color[1], (float)color[2], (float)factor);

	m_Materials[material->GetName()] = entry_material;//全部のマテリアル情報をぶち込む

	// テクスチャ読み込み(シングル対応)　マテリアルとテクスチャの関係をつけたり
	// Diffuseプロパティを取得
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxFileTexture* texture = nullptr;
	std::string keyword;
	int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0)
	{
		// propからFbxFileTextureを取得	
		texture = prop.GetSrcObject<FbxFileTexture>(0);
	}
	else
	{
		// FbxLayeredTextureからFbxFileTextureを取得	
		int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layer_num > 0)
		{
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
	}

	if (texture != nullptr &&
		LoadTexture(texture, keyword,texfile_name) == true)
	{
		// 読み込んだテクスチャとマテリアルの関係を覚えておく
		m_MaterialLinks[material->GetName()] = m_Textures[keyword];
	}
}

void FbxMeshFile::CreateMesh(FbxMesh* mesh)
{
	MeshData mesh_data;
	LoadIndices(mesh_data, mesh);
	LoadVertices(mesh_data, mesh);
	LoadNormals(mesh_data, mesh);
	LoadUV(mesh_data, mesh);
	LoadColors(mesh_data, mesh);
	SetMaterialName(mesh_data, mesh);

	m_MeshList.push_back(mesh_data);
}

void FbxMeshFile::Animation(int _num, float speed) {
	//ここがアニメーションを設定している
	if (nowAnimeNum != _num) {
		nowAnimeNum = _num;
		FbxArray<FbxString*> AnimStackNameArray;//文字列リスト
		fbxSceneAnime->FillAnimStackNameArray(AnimStackNameArray);//アニメーションの初期設定
		FbxAnimStack* AnimationStack = fbxSceneAnime->FindMember<FbxAnimStack>(AnimStackNameArray[_num]->Buffer());//sceneからアニメーションを取り出している
		fbxSceneAnime->SetCurrentAnimationStack(AnimationStack);//アニメーションを設定

		timeCount[_num] = start[_num];
	}


	for (int count = 0; count < meshNum; count++) {
		// 各頂点に掛けるための最終的な行列の配列
		FbxMatrix* clusterDeformation = new FbxMatrix[fMesh[count]->GetControlPointsCount()];
		memset(clusterDeformation, 0, sizeof(FbxMatrix) * fMesh[count]->GetControlPointsCount());

		FbxSkin* skinDeformer = (FbxSkin*)fMesh[count]->GetDeformer(0, FbxDeformer::eSkin);
		int clusterCount = skinDeformer->GetClusterCount();
		// 各クラスタから各頂点に影響を与えるための行列作成
		FbxCluster* cluster;
		FbxMatrix vertexTransformMatrix;
		FbxAMatrix referenceGlobalInitPosition;
		FbxAMatrix clusterGlobalInitPosition;
		FbxMatrix clusterGlobalCurrentPosition;
		FbxMatrix clusterRelativeInitPosition;

		//ここの計算は１度に計算して数値を持っておくことで対応できる ロード時間が長くなりメモリも食うがFPSは落ちない
		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
			// クラスタ(ボーン)の取り出し
			cluster = skinDeformer->GetCluster(clusterIndex);//ボーンを取り出す

			cluster->GetTransformMatrix(referenceGlobalInitPosition);
			cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

			clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(timeCount[_num]);//10m
			clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;//3m

			vertexTransformMatrix = clusterGlobalCurrentPosition * clusterRelativeInitPosition;
			//行列の順番が悪いのでボーンがおかしな挙動をする
			DirectX::XMMATRIX cb;

			cb = DirectX::XMMATRIX(////x             y                           z                               w
				vertexTransformMatrix[0][0], vertexTransformMatrix[0][1], vertexTransformMatrix[0][2], vertexTransformMatrix[0][3],

				vertexTransformMatrix[1][0], vertexTransformMatrix[1][1], vertexTransformMatrix[1][2], vertexTransformMatrix[1][3],

				vertexTransformMatrix[2][0], vertexTransformMatrix[2][1], vertexTransformMatrix[2][2], vertexTransformMatrix[2][3],

				vertexTransformMatrix[3][0], vertexTransformMatrix[3][1], vertexTransformMatrix[3][2], vertexTransformMatrix[3][3]
			);

			//普通の演算ではない
			//   wが１に正規化された頂点                =  クラスタ行列　＊　　頂点座標
			//verticesCluster[i] = clusterDeformation[i].MultNormalize(fMesh->GetControlPointAt(i));

			//頂点シェーダに渡す変数に入れる
			DirectGraphics::GetInstance()->GetConstantBufferData()->mBoneMatrix[clusterIndex] = cb; //XMMatrixTranspose(cb);

		}
		delete[] clusterDeformation;
	}
	// ----- Animation -----

	timeCount[_num] += FrameTime[_num] * speed;

	if (timeCount[_num] > stop[_num])
		timeCount[_num] = start[_num];

	

}

//アニメーション読み込み LoadFbxFileの子
void FbxMeshFile::CreateAnimation(FbxScene* fbxScene) {

	FbxArray<FbxString*> AnimStackNameArray;//文字列リスト
	fbxScene->FillAnimStackNameArray(AnimStackNameArray);//アニメーションの初期設定
	if (AnimStackNameArray.Size() <= 0) {
		return;
	}

	start = new FbxTime[AnimStackNameArray.Size()];
	stop = new FbxTime[AnimStackNameArray.Size()];
	FrameTime = new FbxTime[AnimStackNameArray.Size()];
	timeCount = new FbxTime[AnimStackNameArray.Size()];

	for (int i = 0; i < AnimStackNameArray.Size(); i++) {


			FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(*(AnimStackNameArray[i]));//アニメーションの情報を取り出す
			start[i] = takeInfo->mLocalTimeSpan.GetStart();
			stop[i] = takeInfo->mLocalTimeSpan.GetStop();
			FrameTime[i].SetTime(0, 0, 0, 1, 0, fbxScene->GetGlobalSettings().GetTimeMode());//進める時間を設定
			timeCount[i] = start[i];
		}


	/////////////////////////////////////////////////////////////////////////////////

	int count = 0;
	for (int count = 0; count < meshNum; count++) {
		//メッシュ数分処理をするべき？
		FbxSkin* skinDeformer = (FbxSkin*)fMesh[count]->GetDeformer(0, FbxDeformer::eSkin);
		int clusterCount = skinDeformer->GetClusterCount();
		
		FbxCluster* cluster;
		//配列生成
		double** weightOll = new double* [fMesh[count]->GetControlPointsCount()];
		int** indexOll = new int* [fMesh[count]->GetControlPointsCount()];
		for (int i = 0; i < fMesh[count]->GetControlPointsCount(); i++) {
			weightOll[i] = new double[4];
			indexOll[i] = new int[4];
			for (int h = 0; h < 4; h++) {
				weightOll[i][h] = 0;
				indexOll[i][h] = 39;
			}
		}
		//クラスタとインデックス、ウェイト取り出し
		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
			// クラスタ(ボーン)の取り出し
			cluster = skinDeformer->GetCluster(clusterIndex);//ボーンを取り出す
			for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++) {
				int index = cluster->GetControlPointIndices()[i];//頂点のインデックス
				//clusterIndexが対応する頂点番号＝index
				double weight = cluster->GetControlPointWeights()[i];//頂点のウェイト
				for (int h = 0; h < 4; h++) {
					if (weightOll[index][h] <= 0) {
						weightOll[index][h] = weight;
						break;
					}
				}
				//ここが悪い、１つのクラスタにつき４回決めてる
				for (int h = 0; h < 4; h++) {
					if (indexOll[index][h] == 39) {
						indexOll[index][h] = clusterIndex;
						break;
					}
				}
			}
		}


		int* indices = fMesh[count]->GetPolygonVertices();//三角の数をゲット
		int polygon_vertex_count = fMesh[count]->GetPolygonVertexCount();//頂点の数をゲット

			//ウェイトを代入
		for (int i = 0; i < polygon_vertex_count; i++)
		{
			// インデックスバッファから頂点番号を取得
			int index = indices[i];
			for (int h = 0; h < 4; h++) {

				m_MeshList[count].m_Vertices[i].BoneWeight[h] = weightOll[index][h];
				m_MeshList[count].m_Vertices[i].BoneIndex[h] = indexOll[index][h]; //0~24のボーン番号  
			}
		}

		for (int i = 0; i < fMesh[count]->GetControlPointsCount(); i++) {
			delete weightOll[i];
			delete indexOll[i];
		}
		delete weightOll;
		delete indexOll;
	}
}
bool FbxMeshFile::LoadFbxFile(const char* modelfile_name,const char* texfile_name)
{
	// FbxManager作成
	FbxManager* fbx_manager = fbxsdk::FbxManager::Create();
	if (fbx_manager == nullptr)
	{
		return false;
	}
	
	// FbxImporter作成
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
	if (fbx_importer == nullptr)
	{
		fbx_manager->Destroy();
		return false;
	}
	
	// FbxScene作成
	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "");
	if (fbx_scene == nullptr)
	{
		fbx_importer->Destroy();
		fbx_manager->Destroy();
		return false;
	}

	// Fileを初期化
	fbx_importer->Initialize(modelfile_name);
	// sceneにインポート
	fbx_importer->Import(fbx_scene);

	FbxGeometryConverter converter(fbx_manager);
	// アニメーションが読み込めない原因？メッシュに使われているマテリアル単位でメッシュを分割する
	converter.SplitMeshesPerMaterial(fbx_scene, true);
	// ポリゴンを三角形にする
	converter.Triangulate(fbx_scene, true);

	int material_num = fbx_scene->GetSrcObjectCount<FbxSurfaceMaterial>();//マテリアルの個数を調べる

	//マテリアル読み込み
	for (int i = 0; i < material_num; i++)
	{
		LoadMaterial(fbx_scene->GetSrcObject<FbxSurfaceMaterial>(i),texfile_name);
	}

	// FbxMeshの数を取得
	int mesh_num = fbx_scene->GetSrcObjectCount<FbxMesh>();
	//メッシュ読み込み
	for (int i = 0; i < mesh_num; i++)
	{
		// Mesh作成
		CreateMesh(fbx_scene->GetSrcObject<FbxMesh>(i));
	}
	//テクスチャの読み込み
	int texture_num = fbx_scene->GetSrcObjectCount<FbxFileTexture>();
	for (int i = 0; i < texture_num; i++)
	{
		FbxFileTexture* texture = fbx_scene->GetSrcObject<FbxFileTexture>(i);
		if (texture)
		{
			const char* file_name01 = texture->GetFileName();
			const char* file_name02 = texture->GetRelativeFileName();
			int tex = texture->GetSrcObjectCount< FbxSurfaceMaterial>();
		}
	}
	
	//アニメーションの読み込み
	for (int i = 0; i < mesh_num; i++)
	{
		fMesh[i] = fbx_scene->GetSrcObject<FbxMesh>(i);
	}
	meshNum = mesh_num;

	CreateAnimation(fbx_scene);
	fbxSceneAnime = fbx_scene;

	//fbx_importer->Destroy();
	//fbx_scene->Destroy();
	//fbx_manager->Destroy();

	return true;
}

bool FbxMeshFile::CreateVertexBuffer(ID3D11Device* device)
{
	for (auto& mesh : m_MeshList)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(CustomVertex) * mesh.m_Vertices.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;			// 使用方法
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;						// リソースへのCPUのアクセス権限についての設定
		buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
		buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.m_Vertices[0];	// バッファの中身の設定
		sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
		sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

		// バッファ作成
		if (FAILED(device->CreateBuffer(
			&buffer_desc,								// バッファ情報
			&sub_resource,								// リソース情報
			&mesh.m_VertexBuffer)))	// 作成されたバッファの格納先
		{
			return false;
		}
	}

	return true;
}

bool FbxMeshFile::CreateIndexBuffer(ID3D11Device* device)
{
	for (auto& mesh : m_MeshList)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = (UINT)sizeof(UINT) *  mesh.m_Indices.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// 使用方法
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND設定
		buffer_desc.CPUAccessFlags = 0;										// リソースへのCPUのアクセス権限についての設定
		buffer_desc.MiscFlags = 0;											// リソースオプションのフラグ
		buffer_desc.StructureByteStride = 0;								// 構造体のサイズ

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.m_Indices[0];							// バッファの中身の設定
		sub_resource.SysMemPitch = 0;										// textureデータを使用する際に使用するメンバ
		sub_resource.SysMemSlicePitch = 0;									// textureデータを使用する際に使用するメンバ

		// バッファ作成
		if (FAILED(device->CreateBuffer(
			&buffer_desc,						// バッファ情報
			&sub_resource,						// リソース情報
			&mesh.m_IndexBuffer)))		// 作成されたバッファの格納先
		{
			return false;
		}
	}

	return true;
}

bool FbxMeshFile::CreateInputLayout(ID3D11Device* device)
{


	D3D11_INPUT_ELEMENT_DESC vertex_desc[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// インスタンス毎データ(スロット1)
		{"MATRIX",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,                             D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"MATRIX",   1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, sizeof(DirectX::XMFLOAT4),     D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"MATRIX",   2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, sizeof(DirectX::XMFLOAT4) * 2, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"MATRIX",   3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, sizeof(DirectX::XMFLOAT4) * 3, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	//頂点レイアウト作成
	if (FAILED(dev->CreateInputLayout(
		vertex_desc,				// レイアウト設定
		ARRAYSIZE(vertex_desc),		// 配列サイズ
		m_VertexShader->GetData(),	// レイアウトと関連付ける頂点シェーダのデータ
		m_VertexShader->GetSize(),	// レイアウトと関連付ける頂点シェーダのサイズ
		&m_InputLayout)))			// 作成された頂点レイアウトの格納先
	{
		return false;
	}

	return true;
}

void FbxMeshFile::SetMaterialColor(DirectGraphics* graphics, ObjMaterial& material)
{
	ConstantBuffer* constant = graphics->GetConstantBufferData();
	constant->MaterialDiffuse = DirectX::XMFLOAT4(material.Diffuse[0], material.Diffuse[1], material.Diffuse[2], material.Diffuse[3]);
	constant->MaterialAmbient = DirectX::XMFLOAT4(material.Ambient[0], material.Ambient[1], material.Ambient[2], material.Ambient[3]);
}

//ピクセルシェーダとジオメトリシェーダと頂点シェーダのみ
void FbxMeshFile::CreateShader(const char* VertexShaderFilePath, const char* GeometryShaderFilePath,const char* PixelShaderFilePath) {

	m_VertexShader = new VertexShader();
	m_VertexShader->Create(dev, VertexShaderFilePath);

	m_GeometryShader = new GeometryShader();
	m_GeometryShader->Create(dev, GeometryShaderFilePath);

	m_PixelShader = new PixelShader();
	m_PixelShader->Create(dev, PixelShaderFilePath);

}


//インスタンスのワールド座標を追加を決める
void  FbxMeshFile::SetWorldMatrix() {

	//１回のインスタンス描画で複数回呼ぶ（同じインスタンスのもの）

	// ワールドマトリクス設定
	DirectX::XMMATRIX world_matrix;
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(pos.X, pos.Y, pos.Z);
	DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(degree.X));
	DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(degree.Y));
	DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(degree.Z));
	DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);


	DirectX::XMMATRIX axisRLen = DirectX::XMMatrixTranslation(axisR.X, axisR.Y, axisR.Z);

	DirectX::XMVECTOR vaxis;
	DirectX::XMMATRIX quaternion;
	// 軸を同時座標に変換
	DirectX::XMFLOAT4 axis4 = { axis.X,axis.Y,axis.Z,0 };
	vaxis = XMLoadFloat4(&axis4);
	// 度数法をラジアンに変換
	axisAngle = (axisAngle * 3.1415) / 180.0f;
	// 指定軸を中心に回転する行列を作成する
	quaternion = DirectX::XMMatrixRotationAxis(vaxis, axisAngle);
	//半径　　　回転率　　　回転中心
	world_matrix = scale_mat * rotate_z * rotate_x * rotate_y * axisRLen * quaternion * translate;

	DirectX::XMFLOAT4X4 wm;
	XMStoreFloat4x4(&wm, XMMatrixTranspose(world_matrix));

	worldMatrix.push_back(wm);

}

//インスタンス用のワールドバッファの作成
bool FbxMeshFile::CreateWorldMatrixBuffer(int _instanceNum) {

	//１回のインスタンス描画の最初に呼ぶ
	if (worldBuffer != nullptr) {
		worldBuffer->Release();
	}

	//頂点バッファ作成
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * _instanceNum;	// バッファのサイズ
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;			// 使用方法
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;						// リソースへのCPUのアクセス権限についての設定
	buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
	buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &worldMatrix[0];	// バッファの中身の設定
	sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
	sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

	// バッファ作成
	if (FAILED(DirectGraphics::GetInstance()->GetDevice()->CreateBuffer(
		&buffer_desc,								// バッファ情報
		&sub_resource,								// リソース情報
		&worldBuffer)))	// 作成されたバッファの格納先
	{
		return false;
	}

	while (worldMatrix.size() != 0) {
		worldMatrix.pop_back();
	}
	return true;
}
