#include "FBX.h"
using namespace frauEngine;
using namespace std;

FBX::FBX() {
	//拡張子ごとのテクスチャのロード形式を変更
	loadTextureLambda["sph"] = loadTextureLambda["spa"] = loadTextureLambda["bmp"] = loadTextureLambda["png"] = loadTextureLambda["jpg"] = [](const wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, meta, img);
	};
	loadTextureLambda["tga"] = [](const wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromTGAFile(path.c_str(), meta, img);
	};
	loadTextureLambda["dds"] = [](const wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, meta, img);
	};
}

bool FBX::Load(string _modelPath, string _texturePath) {

	modelPath = _modelPath;


	if (!InitFbxScene()) {
		MessageBox(NULL, TEXT("FbxSceneの作成失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		return false;
	}

	LoadMaterials(_texturePath);

	LoadMeshs();

	
	LoadAnimations();

	if (CreateVertexBuffer() == false)
	{
		DebugOutputFormatString("頂点バッファ生成に失敗しました\n");
		return false;
	}

	if (CreateIndexBuffer() == false)
	{
		DebugOutputFormatString("インデクスバッファ生成に失敗しました\n");
		return false;
	}

	CreateInputLayout();


}

bool FBX::InitFbxScene() {
	fbxManager = fbxsdk::FbxManager::Create();
	if (fbxManager == nullptr)
	{
		MessageBox(NULL, TEXT("FbxManagerの作成に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
	}

	fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr)
	{
		MessageBox(NULL, TEXT("FbxImporterの作成に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxManager->Destroy();
		return false;
	}

	fbxScene = FbxScene::Create(fbxManager, "");
	if (fbxScene == nullptr)
	{
		MessageBox(NULL, TEXT("FbxSceneの作成に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	if (!fbxImporter->Initialize(modelPath.c_str())) {
		MessageBox(NULL, TEXT("FbxImporterの初期化に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	if (!fbxImporter->Import(fbxScene)) {
		MessageBox(NULL, TEXT("FbxSceneへのインポートに失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}
	FbxGeometryConverter converter(fbxManager);

	converter.SplitMeshesPerMaterial(fbxScene, true);
	if (isTrianglate) {
		converter.Triangulate(fbxScene, true);
	}
	meshNum = fbxScene->GetSrcObjectCount<FbxMesh>();//メッシュ数をキャッシュしておく

	return true;
}

void FBX::LoadMaterials(string _textureFolderPath) {
	//マテリアルの定数バッファを設定 TODO ランバートは古い＆無駄な処理が書かれているので修正する予定あり
	int material_num = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < material_num; i++)
	{
		material = fbxScene->GetSrcObject<FbxSurfaceMaterial>(i);
		LoadMaterial(material, _textureFolderPath);//TODO 中身の修理
		materialBufferHeap[material->GetName()].Create();//ヒープの作成
		materialBufferHeap[material->GetName()].buffer = &materials[material->GetName()];
		//テクスチャのヒープを作成
		textureBufferHeap[material->GetName()].CreateShaderResourceBufferHeap(materialLinks[material->GetName()]);
		
	}
}
void FBX::LoadMaterial(FbxSurfaceMaterial* material, string _texturePath)
{
	frauEngine::MaterialBuffer entry_material;
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
			FbxSurfaceMaterial::sSpecular,
		};

		const char* factor_check_list[] =
		{
			FbxSurfaceMaterial::sAmbientFactor,
			FbxSurfaceMaterial::sDiffuseFactor,
			FbxSurfaceMaterial::sSpecularFactor,
		};

		for (int i = 0; i < (int)MaterialOrder::MaxOrder; i++)
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

		entry_material.ambient.x = (float)color[0];
		entry_material.ambient.y = (float)color[1];
		entry_material.ambient.z = (float)color[2];
	
	entry_material.ambient.w = (float)factor;


	color = colors[(int)MaterialOrder::Diffuse];
	factor = factors[(int)MaterialOrder::Diffuse];

	entry_material.diffuse.x = (float)color[0];
	entry_material.diffuse.y = (float)color[1];
	entry_material.diffuse.z = (float)color[2];
	
	entry_material.diffuse.w = (float)factor;


	color = colors[(int)MaterialOrder::Specular];
	factor = factors[(int)MaterialOrder::Specular];

		entry_material.specular.x = (float)color[0];
		entry_material.specular.y = (float)color[1];
		entry_material.specular.z = (float)color[2];

		entry_material.ambient.w = (float)factor;

	materials[material->GetName()] = entry_material;

	// テクスチャ読み込み
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	texture = nullptr;
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
		LoadTexture(texture, keyword, _texturePath) == true)
	{
		// 読み込んだテクスチャとマテリアルの関係を覚えておく
		materialLinks[material->GetName()] = textures[keyword];
	}

}
bool FBX::LoadTexture(FbxFileTexture* texture, std::string& keyword, string _texturePath)
{
	if (texture == nullptr)
	{
		return false;
	}
	// ファイル名を取得
	std::string file_path = texture->GetRelativeFileName();

	// ファイル分解
	char buffer[256];
	sprintf_s(buffer, file_path.c_str());
	// 記号統一
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// 「/」で分解
	Split('/', buffer, split_list);

	std::string root_path = _texturePath;


	// 文字化け対策
	char* file_name;
	size_t size = 0;
	FbxUTF8ToAnsi(split_list[split_list.size() - 1].c_str(), file_name, &size);

	DirectX::TexMetadata meta{};
	DirectX::ScratchImage img;

	string ext = GetExtension(split_list[split_list.size() - 1]);
	
	string filePath = _texturePath + file_name;
	wstring filePathW = GetWideStringFromString(filePath);
	//テクスチャ読み込み処理
	auto result = loadTextureLambda[ext](filePathW, &meta, img);

	if (FAILED(result)) {
		MessageBox(NULL, TEXT("テクスチャの読み込みに失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxsdk::FbxFree(file_name);
		return false;
	}

	//WriteToSubresourceで転送する用のヒープ設定
	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;//特殊な設定なのでdefaultでもuploadでもなく
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//ライトバックで
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//転送がL0つまりCPU側から直で
	texHeapProp.CreationNodeMask = 0;//単一アダプタのため0
	texHeapProp.VisibleNodeMask = 0;//単一アダプタのため0
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = meta.format;
	resDesc.Width = static_cast<UINT>(meta.width);//幅
	resDesc.Height = static_cast<UINT>(meta.height);//高さ
	resDesc.DepthOrArraySize = static_cast<UINT16>(meta.arraySize);
	resDesc.SampleDesc.Count = 1;//通常テクスチャなのでアンチェリしない
	resDesc.SampleDesc.Quality = 0;//
	resDesc.MipLevels = static_cast<UINT16>(meta.mipLevels);//ミップマップしないのでミップ数は１つ
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(meta.dimension);
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//レイアウトについては決定しない
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//とくにフラグなし

	ID3D12Resource* texbuff = nullptr;
	result = LowApplication::GetInstance()->GetDevice()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,//特に指定なし
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);
	if (FAILED(result)) {
		MessageBox(NULL, TEXT("テクスチャのリソースの作成に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxsdk::FbxFree(file_name);
		return false;
	}

	auto rawImg = img.GetImage(0, 0, 0);//生データ抽出

	result = texbuff->WriteToSubresource(0,
		nullptr,//全領域へコピー
		rawImg->pixels,//元データアドレス
		static_cast<UINT>(rawImg->rowPitch),//1ラインサイズ
		static_cast<UINT>(rawImg->slicePitch)//全サイズ
	);
	if (FAILED(result)) {
		MessageBox(NULL, TEXT("テクスチャヒープ作成に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxsdk::FbxFree(file_name);
		return false;
	}

	textures[file_name] = texbuff;

	keyword = file_name;
	fbxsdk::FbxFree(file_name);

	return true;
}

void FBX::LoadMeshs(){
	for (int i = 0; i < meshNum; i++)
	{
		LoadMesh(fbxScene->GetSrcObject<FbxMesh>(i));
		fMesh.push_back(fbxScene->GetSrcObject<FbxMesh>(i));
	}
}
void FBX::LoadMesh(FbxMesh* mesh)
{
	MeshData mesh_data;
	LoadIndices(mesh_data, mesh);
	LoadVertices(mesh_data, mesh);
	LoadNormals(mesh_data, mesh);
	LoadUV(mesh_data, mesh);
	LoadColors(mesh_data, mesh);

	SetMaterialName(mesh_data, mesh);

	meshList.push_back(mesh_data);
}
void FBX::LoadIndices(MeshData& mesh_data, FbxMesh* mesh)
{
	int polygon_count = mesh->GetPolygonCount();

	// ポリゴンの数だけ連番として保存する
	for (int i = 0; i < polygon_count; i++)
	{
		mesh_data.m_Indices.push_back(i * 3 + 2);
		mesh_data.m_Indices.push_back(i * 3 + 1);
		mesh_data.m_Indices.push_back(i * 3);
	}
}
void FBX::LoadVertices(MeshData& mesh_data, FbxMesh* mesh)
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
		// インデックスバッファから頂&点番号を取得
		int index = indices[i];

		// 頂点座標リストから座標を取得する
		vertex.Posision.X = (float)-vertices[index][0];
		vertex.Posision.Y = (float)vertices[index][1];
		vertex.Posision.Z = (float)vertices[index][2];

		mesh_data.m_Vertices.push_back(vertex);
	}



}
void FBX::LoadNormals(MeshData& mesh_data, FbxMesh* mesh)
{
	FbxArray<FbxVector4> normals;

	mesh->GetPolygonVertexNormals(normals);

	for (int i = 0; i < normals.Size(); i++)
	{
		mesh_data.m_Vertices[i].Normal.X = (float)-normals[i][0];
		mesh_data.m_Vertices[i].Normal.Y = (float)normals[i][1];
		mesh_data.m_Vertices[i].Normal.Z = (float)normals[i][2];
	}
}
void FBX::LoadColors(MeshData& mesh_data, FbxMesh* mesh)
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
		vertex_colors->Destroy();
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
				mesh_data.m_Vertices[i].Color.A = (float)color.mAlpha;
				mesh_data.m_Vertices[i].Color.R = (float)color.mRed;
				mesh_data.m_Vertices[i].Color.G = (float)color.mGreen;
				mesh_data.m_Vertices[i].Color.B = (float)color.mBlue;
			}
		}
	}
	vertex_colors->Destroy();
}
void FBX::LoadUV(MeshData& mesh_data, FbxMesh* mesh)
{
	FbxStringList uvset_names;
	// UVSetの名前リストを取得
	mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// UVSetの名前からUVSetを取得する
	mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), uv_buffer);

	for (int i = 0; i < uv_buffer.Size(); i++)
	{
		FbxVector2& uv = uv_buffer[i];

		mesh_data.m_Vertices[i].TexturePos.X = (float)uv[0];
		mesh_data.m_Vertices[i].TexturePos.Y = (float)(1.0 - uv[1]);
	}
}
void FBX::SetMaterialName(MeshData& mesh_data, FbxMesh* mesh)
{
	// マテリアルが無ければ終わり
	if (mesh->GetElementMaterialCount() == 0)
	{
		mesh_data.m_MaterialName = "";
		return;
	}

	// Mesh側のマテリアル情報を取得
	materialElements = mesh->GetElementMaterial(0);
	int index = materialElements->GetIndexArray().GetAt(0);
	surface_material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);
	if (surface_material != nullptr)
	{
		mesh_data.m_MaterialName = surface_material->GetName();
	}
	else
	{
		mesh_data.m_MaterialName = "";
	}


}

void FBX::LoadAnimations() {
	LoadWeight();

	LoadAnimation();
}
void FBX::LoadAnimation() {

	FbxArray<FbxString*> AnimStackNameArray;//文字列リスト
	fbxScene->FillAnimStackNameArray(AnimStackNameArray);//アニメーションの初期設定

	//アニメの個数を取得
	animeNumMax=AnimStackNameArray.Size();
	animeMat.resize(animeNumMax);//アニメの個数を設定 １重目のサイズ決定

	for (int i = 0; i < AnimStackNameArray.Size(); i++) {
		int tmp = 0;
		timeCount[i] = start[i];
		while (timeCount[i] < stop[i]) {
			timeCount[i] += FrameTime[i];
			tmp++;
		}
		timeMax.push_back(tmp);
		//配列サイズを決定
		animeMat[i].resize(tmp);//アニメの終了時間で設定 この時点で２重めのサイズ決定
		//512個のマトリクスを取れるように設定
		for (int h = 0; h < tmp; h++) {
			animeMat[i][h].resize(512);
		}
	}

	for (int i = 0; i < AnimStackNameArray.Size(); i++) {
		timeCount[i] = start[i];

		FbxAnimStack* AnimationStack = fbxScene->FindMember<FbxAnimStack>(AnimStackNameArray[i]->Buffer());//sceneからアニメーションを取り出している
		fbxScene->SetCurrentAnimationStack(AnimationStack);//アニメーションを設定

		for (int h = 0; h < timeMax[i]; h++) {
			timeCount[i] += FrameTime[i];

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

				//アニメはキャッシュする ロード時間が長くなりメモリも食うがFPSは落ちない
				for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
					// クラスタ(ボーン)の取り出し
					cluster = skinDeformer->GetCluster(clusterIndex);//ボーンを取り出す

					cluster->GetTransformMatrix(referenceGlobalInitPosition);
					cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

					clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(timeCount[i]);//10m
					clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;//3m

					vertexTransformMatrix = clusterGlobalCurrentPosition * clusterRelativeInitPosition;

					animeMat[i][h][clusterIndex] = DirectX::XMMATRIX(////x             y                           z                               w
						vertexTransformMatrix[0][0], vertexTransformMatrix[0][1], vertexTransformMatrix[0][2], vertexTransformMatrix[0][3],
						vertexTransformMatrix[1][0], vertexTransformMatrix[1][1], vertexTransformMatrix[1][2], vertexTransformMatrix[1][3],
						vertexTransformMatrix[2][0], vertexTransformMatrix[2][1], vertexTransformMatrix[2][2], vertexTransformMatrix[2][3],
						vertexTransformMatrix[3][0], vertexTransformMatrix[3][1], vertexTransformMatrix[3][2], vertexTransformMatrix[3][3]
					);
				}
				delete[] clusterDeformation;
			}
		}
	}

}
void FBX::LoadWeight() {

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

	int count = 0;
	for (int count = 0; count < meshNum; count++) {
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
				indexOll[i][h] = 512;
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
				for (int h = 0; h < 4; h++) {
					if (indexOll[index][h] == 512) {
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

				meshList[count].m_Vertices[i].BoneWeight[h] = weightOll[index][h];
				meshList[count].m_Vertices[i].BoneIndex[h] = indexOll[index][h]; //0~24のボーン番号  
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

bool FBX::CreateVertexBuffer()
{

	auto device = LowApplication::GetInstance()->GetDevice();
	int _meshNum = 0;
	vertBuff.resize(meshList.size());
	for (auto& mesh : meshList)
	{
		vertBuff[_meshNum] = nullptr;

		D3D12_HEAP_PROPERTIES tmpProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		D3D12_RESOURCE_DESC tmpDesc = CD3DX12_RESOURCE_DESC::Buffer(mesh.m_Vertices.size() * sizeof(CustomVertex));
		//頂点のヒープを作成する。。。メモリを食っているから削除したい
		auto result = device->CreateCommittedResource(
			&tmpProp,
			D3D12_HEAP_FLAG_NONE,
			&tmpDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff[_meshNum]));
		
		if (FAILED(result)) {
			MessageBox(NULL, TEXT("頂点バッファの生成に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
			return false;
		}


		////頂点情報をコピー
		CustomVertex* vertMap = nullptr;
		result = vertBuff[_meshNum]->Map(0, nullptr, (void**)&vertMap);
		std::copy(std::begin(mesh.m_Vertices), std::end(mesh.m_Vertices), vertMap);
		vertBuff[_meshNum]->Unmap(0, nullptr);
		if (FAILED(result)) {
			MessageBox(NULL, TEXT("頂点バッファの生成に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
			return false;
		}
		mesh.m_VertexBuffer.BufferLocation = vertBuff[_meshNum]->GetGPUVirtualAddress();//バッファの仮想アドレス
		mesh.m_VertexBuffer.SizeInBytes = static_cast<UINT>(mesh.m_Vertices.size() * sizeof(CustomVertex));//全バイト数
		mesh.m_VertexBuffer.StrideInBytes = sizeof(CustomVertex);//1頂点あたりのバイト数

		meshNum++;
		

	}

	return true;
}

bool FBX::CreateIndexBuffer()
{
	auto device = LowApplication::GetInstance()->GetDevice();
	
	idxBuff.resize(meshList.size());

	int _meshNum = 0;
	for (auto& mesh : meshList)
	{
		idxBuff[_meshNum] = nullptr;

		D3D12_HEAP_PROPERTIES tmpProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC   tmpDesc = CD3DX12_RESOURCE_DESC::Buffer(mesh.m_Indices.size() * sizeof(UINT));

		auto result=device->CreateCommittedResource(
			&tmpProp,
			D3D12_HEAP_FLAG_NONE,
			&tmpDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&idxBuff[_meshNum]));
		if (FAILED(result)) {
			MessageBox(NULL, TEXT("インデクスバッファの生成に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
			return false;
		}
		////作ったバッファにインデックスデータをコピー
		UINT* mappedIdx = nullptr;
		result=idxBuff[_meshNum]->Map(0, nullptr, (void**)&mappedIdx);
		std::copy(std::begin(mesh.m_Indices), std::end(mesh.m_Indices), mappedIdx);

		idxBuff[_meshNum]->Unmap(0, nullptr);
		if (FAILED(result)) {
			MessageBox(NULL, TEXT("インデクスバッファの生成に失敗しました"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
			return false;
		}
		//インデックスバッファビューを作成
		mesh.m_IndexBuffer.BufferLocation = idxBuff[_meshNum]->GetGPUVirtualAddress();
		mesh.m_IndexBuffer.Format = DXGI_FORMAT_R32_UINT;
		mesh.m_IndexBuffer.SizeInBytes = static_cast<UINT>(mesh.m_Indices.size() * sizeof(UINT));

		meshNum++;
	}

	return true;
}

void FBX::CreateInputLayout()
{

	D3D12_INPUT_ELEMENT_DESC _inputLayout[] = {
	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	inputLayout = _inputLayout;
}

//TODO 他との兼ね合いで引数等が変わるので放置
void FBX::Draw(
	frauEngine::WorldBufferHeap* worldBufferHeap, 
	frauEngine::BoneBufferHeap* boneBufferHeap,
	frauEngine::UtilityBufferHeap* utilityBufferHeap,
	frauEngine::PBRBufferHeap* pbrBufferHeap,
	
	frauEngine::ShaderResourceBufferHeap** anotherTexBufferHeap,

	ID3D12PipelineState* pipelineState,
	ID3D12RootSignature* rootSignature
	){
	auto lowApp = LowApplication::GetInstance();
	auto cmdList = lowApp->GetCommandList();

	//パイプラインを渡します
	cmdList->SetPipelineState(pipelineState);

	//シェーダーに渡すデータの設定
	cmdList->SetGraphicsRootSignature(rootSignature);



	//ワールド情報を渡す
	cmdList->SetDescriptorHeaps(1, &worldBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(0, worldBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());//0番に設定
	//ボーン情報を渡す
	cmdList->SetDescriptorHeaps(1, &boneBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(1, boneBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());//1番に設定
	//変化できる情報を渡す（時間とか影かとかの数値系）
	cmdList->SetDescriptorHeaps(1, &utilityBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(5, utilityBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());

	//カメラ情報を渡す
	frauEngine::CameraBufferHeap* cameraBufferHeap = frauEngine::Camera::GetInstance()->GetConstantBuffer();
	cmdList->SetDescriptorHeaps(1, &cameraBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(3, cameraBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());//3番に設定
	//ディレクショナルライト情報を渡す
	frauEngine::LightBufferHeap* directionalLightBufferHeap= frauEngine::Lights::GetInstance()->GetConstantBuffer();
	cmdList->SetDescriptorHeaps(1, &directionalLightBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(4, directionalLightBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());//4番に設定


	//深度テクスチャ情報を渡す
	frauEngine::Lights* dLight = frauEngine::Lights::GetInstance();
	cmdList->SetDescriptorHeaps(1, &dLight->depthSRVHeap);
	cmdList->SetGraphicsRootDescriptorTable(8, dLight->depthSRVHeap->GetGPUDescriptorHandleForHeapStart());//6番に設定

	int i = 0;
	for (auto& mesh : meshList)
	{

		//ランバートマテリアル情報を渡す
		cmdList->SetDescriptorHeaps(1, &materialBufferHeap[mesh.m_MaterialName].descripterHeap);
		cmdList->SetGraphicsRootDescriptorTable(2, materialBufferHeap[mesh.m_MaterialName].descripterHeap->GetGPUDescriptorHandleForHeapStart());
		//PBRマテリアル
		cmdList->SetDescriptorHeaps(1, &pbrBufferHeap[i].descripterHeap);
		cmdList->SetGraphicsRootDescriptorTable(6, pbrBufferHeap[i].descripterHeap->GetGPUDescriptorHandleForHeapStart());

		//アルベドテクスチャ情報を渡す
		cmdList->SetDescriptorHeaps(1, &textureBufferHeap[mesh.m_MaterialName].descripterHeap);
		cmdList->SetGraphicsRootDescriptorTable(7, textureBufferHeap[mesh.m_MaterialName].descripterHeap->GetGPUDescriptorHandleForHeapStart());
		//アルベド以外のテクスチャ情報を渡す
		for (int h = 0; h < (int)frauEngine::ShaderTextureSlot::MODEL_ANOTHER; h++) {
			if (pbrBufferHeap[i].buffer->anotherTexture[h]) {
				cmdList->SetDescriptorHeaps(1, &anotherTexBufferHeap[h][i].descripterHeap);
				cmdList->SetGraphicsRootDescriptorTable(9 + h, anotherTexBufferHeap[h][i].descripterHeap->GetGPUDescriptorHandleForHeapStart());
			}
		}


		//インプットアセンブラの設定
		cmdList->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer);
		cmdList->IASetIndexBuffer(&mesh.m_IndexBuffer);

		cmdList->DrawIndexedInstanced(mesh.m_Indices.size(), 1, 0, 0, 0);

		i++;
	}
}







FBX::~FBX() {


	loadTextureLambda.clear();
	materials.clear();

	animeMat.clear();
	timeMax.clear();
	materialPointerName.clear();
	materialIndexName.clear();

	while (meshList.size()) {
		meshList[meshList.size() - 1].m_VertexBuffer.BufferLocation=NULL;
		meshList[meshList.size() - 1].m_VertexBuffer.SizeInBytes=NULL;
		meshList[meshList.size() - 1].m_VertexBuffer.StrideInBytes=NULL;
		
		meshList[meshList.size() - 1].m_IndexBuffer.BufferLocation=NULL;
		meshList[meshList.size() - 1].m_IndexBuffer.SizeInBytes=NULL;

		while (meshList[meshList.size() - 1].m_Vertices.size()) {
			meshList[meshList.size() - 1].m_Vertices.pop_back();
		}
		while (meshList[meshList.size() - 1].m_Indices.size()) {
			meshList[meshList.size() - 1].m_Indices.pop_back();
		}
		meshList.pop_back();
	}


	while (textures.size()) {
		auto itr = textures.begin();
		itr->second->Release();
		textures.erase(itr);
	}
	while (materialLinks.size()) {
		auto itr = materialLinks.begin();
		itr->second->Release();
		materialLinks.erase(itr);
	}
	if (FrameTime != nullptr) {
		delete FrameTime;
		delete timeCount;
		delete start;
		delete stop;
	}
	while (fMesh.size()) {
		fMesh[fMesh.size() - 1]->Destroy();
		fMesh.pop_back();
	}
	//ちょっと危険かも
	if (inputLayout != nullptr) {
		inputLayout=nullptr;
	}
	while (materialBufferHeap.size()) {
		auto itr = materialBufferHeap.begin();
		itr->second.Destroy();
		materialBufferHeap.erase(itr);
	}
	while (textureBufferHeap.size()) {
		auto itr = textureBufferHeap.begin();
		itr->second.Destroy();
		textureBufferHeap.erase(itr);
	}

	auto cmdList=LowApplication::GetInstance()->GetCommandList();

	while (vertBuff.size()) {
		if (vertBuff[vertBuff.size() - 1] != nullptr) {

			vertBuff[vertBuff.size() - 1]->Release();
			
		}
		vertBuff.pop_back();
	}
	while (idxBuff.size()) {
		if (idxBuff[idxBuff.size() - 1] != nullptr) {

			idxBuff[idxBuff.size() - 1]->Release();
		}
		idxBuff.pop_back();
	}

	//適当に削除処理を書いてあります
	material->Destroy();
	texture->Destroy();



	//delete vertices;
	//delete indices;
	//materialElements->Destroy();
	//surface_material->Destroy();
	//FBXImpoter系は最後にこの順番でないと削除できません
	fbxScene->Destroy();
	fbxImporter->Destroy();
	fbxManager->Destroy();


}