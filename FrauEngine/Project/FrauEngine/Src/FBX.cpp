#include "FBX.h"
using namespace frauEngine;
using namespace std;

FBX::FBX() {
	//�g���q���Ƃ̃e�N�X�`���̃��[�h�`����ύX
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
		MessageBox(NULL, TEXT("FbxScene�̍쐬���s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		return false;
	}

	LoadMaterials(_texturePath);

	LoadMeshs();

	
	LoadAnimations();

	if (CreateVertexBuffer() == false)
	{
		DebugOutputFormatString("���_�o�b�t�@�����Ɏ��s���܂���\n");
		return false;
	}

	if (CreateIndexBuffer() == false)
	{
		DebugOutputFormatString("�C���f�N�X�o�b�t�@�����Ɏ��s���܂���\n");
		return false;
	}

	CreateInputLayout();


}

bool FBX::InitFbxScene() {
	fbxManager = fbxsdk::FbxManager::Create();
	if (fbxManager == nullptr)
	{
		MessageBox(NULL, TEXT("FbxManager�̍쐬�Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
	}

	fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr)
	{
		MessageBox(NULL, TEXT("FbxImporter�̍쐬�Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxManager->Destroy();
		return false;
	}

	fbxScene = FbxScene::Create(fbxManager, "");
	if (fbxScene == nullptr)
	{
		MessageBox(NULL, TEXT("FbxScene�̍쐬�Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	if (!fbxImporter->Initialize(modelPath.c_str())) {
		MessageBox(NULL, TEXT("FbxImporter�̏������Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	if (!fbxImporter->Import(fbxScene)) {
		MessageBox(NULL, TEXT("FbxScene�ւ̃C���|�[�g�Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}
	FbxGeometryConverter converter(fbxManager);

	converter.SplitMeshesPerMaterial(fbxScene, true);
	if (isTrianglate) {
		converter.Triangulate(fbxScene, true);
	}
	meshNum = fbxScene->GetSrcObjectCount<FbxMesh>();//���b�V�������L���b�V�����Ă���

	return true;
}

void FBX::LoadMaterials(string _textureFolderPath) {
	//�}�e���A���̒萔�o�b�t�@��ݒ� TODO �����o�[�g�͌Â������ʂȏ�����������Ă���̂ŏC������\�肠��
	int material_num = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < material_num; i++)
	{
		material = fbxScene->GetSrcObject<FbxSurfaceMaterial>(i);
		LoadMaterial(material, _textureFolderPath);//TODO ���g�̏C��
		materialBufferHeap[material->GetName()].Create();//�q�[�v�̍쐬
		materialBufferHeap[material->GetName()].buffer = &materials[material->GetName()];
		//�e�N�X�`���̃q�[�v���쐬
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
			if (prop.IsValid())//�L�������ׂĂ���炵��
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

	// �e�N�X�`���ǂݍ���
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	texture = nullptr;
	std::string keyword;
	int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0)
	{
		// prop����FbxFileTexture���擾	
		texture = prop.GetSrcObject<FbxFileTexture>(0);
	}
	else
	{
		// FbxLayeredTexture����FbxFileTexture���擾	
		int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layer_num > 0)
		{
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
	}

	if (texture != nullptr &&
		LoadTexture(texture, keyword, _texturePath) == true)
	{
		// �ǂݍ��񂾃e�N�X�`���ƃ}�e���A���̊֌W���o���Ă���
		materialLinks[material->GetName()] = textures[keyword];
	}

}
bool FBX::LoadTexture(FbxFileTexture* texture, std::string& keyword, string _texturePath)
{
	if (texture == nullptr)
	{
		return false;
	}
	// �t�@�C�������擾
	std::string file_path = texture->GetRelativeFileName();

	// �t�@�C������
	char buffer[256];
	sprintf_s(buffer, file_path.c_str());
	// �L������
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// �u/�v�ŕ���
	Split('/', buffer, split_list);

	std::string root_path = _texturePath;


	// ���������΍�
	char* file_name;
	size_t size = 0;
	FbxUTF8ToAnsi(split_list[split_list.size() - 1].c_str(), file_name, &size);

	DirectX::TexMetadata meta{};
	DirectX::ScratchImage img;

	string ext = GetExtension(split_list[split_list.size() - 1]);
	
	string filePath = _texturePath + file_name;
	wstring filePathW = GetWideStringFromString(filePath);
	//�e�N�X�`���ǂݍ��ݏ���
	auto result = loadTextureLambda[ext](filePathW, &meta, img);

	if (FAILED(result)) {
		MessageBox(NULL, TEXT("�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxsdk::FbxFree(file_name);
		return false;
	}

	//WriteToSubresource�œ]������p�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;//����Ȑݒ�Ȃ̂�default�ł�upload�ł��Ȃ�
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//���C�g�o�b�N��
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//�]����L0�܂�CPU�����璼��
	texHeapProp.CreationNodeMask = 0;//�P��A�_�v�^�̂���0
	texHeapProp.VisibleNodeMask = 0;//�P��A�_�v�^�̂���0
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = meta.format;
	resDesc.Width = static_cast<UINT>(meta.width);//��
	resDesc.Height = static_cast<UINT>(meta.height);//����
	resDesc.DepthOrArraySize = static_cast<UINT16>(meta.arraySize);
	resDesc.SampleDesc.Count = 1;//�ʏ�e�N�X�`���Ȃ̂ŃA���`�F�����Ȃ�
	resDesc.SampleDesc.Quality = 0;//
	resDesc.MipLevels = static_cast<UINT16>(meta.mipLevels);//�~�b�v�}�b�v���Ȃ��̂Ń~�b�v���͂P��
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(meta.dimension);
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//���C�A�E�g�ɂ��Ă͌��肵�Ȃ�
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//�Ƃ��Ƀt���O�Ȃ�

	ID3D12Resource* texbuff = nullptr;
	result = LowApplication::GetInstance()->GetDevice()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,//���Ɏw��Ȃ�
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);
	if (FAILED(result)) {
		MessageBox(NULL, TEXT("�e�N�X�`���̃��\�[�X�̍쐬�Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
		fbxsdk::FbxFree(file_name);
		return false;
	}

	auto rawImg = img.GetImage(0, 0, 0);//���f�[�^���o

	result = texbuff->WriteToSubresource(0,
		nullptr,//�S�̈�փR�s�[
		rawImg->pixels,//���f�[�^�A�h���X
		static_cast<UINT>(rawImg->rowPitch),//1���C���T�C�Y
		static_cast<UINT>(rawImg->slicePitch)//�S�T�C�Y
	);
	if (FAILED(result)) {
		MessageBox(NULL, TEXT("�e�N�X�`���q�[�v�쐬�Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
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

	// �|���S���̐������A�ԂƂ��ĕۑ�����
	for (int i = 0; i < polygon_count; i++)
	{
		mesh_data.m_Indices.push_back(i * 3 + 2);
		mesh_data.m_Indices.push_back(i * 3 + 1);
		mesh_data.m_Indices.push_back(i * 3);
	}
}
void FBX::LoadVertices(MeshData& mesh_data, FbxMesh* mesh)
{

	// ���_�o�b�t�@�̎擾
	FbxVector4* vertices = mesh->GetControlPoints();
	// �C���f�b�N�X�o�b�t�@�̎擾
	int* indices = mesh->GetPolygonVertices();
	// ���_���W�̐��̎擾
	int polygon_vertex_count = mesh->GetPolygonVertexCount();

	for (int i = 0; i < polygon_vertex_count; i++)
	{
		CustomVertex vertex;
		// �C���f�b�N�X�o�b�t�@���璸&�_�ԍ����擾
		int index = indices[i];

		// ���_���W���X�g������W���擾����
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
	// ���_�J���[�f�[�^�̐����m�F
	int color_count = mesh->GetElementVertexColorCount();
	if (color_count == 0)
	{
		return;
	}
	// ���_�J���[�f�[�^�̎擾
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
			// ���_�J���[�o�b�t�@�擾
			FbxLayerElementArrayTemplate<FbxColor>& colors = vertex_colors->GetDirectArray();
			// ���_�J���[�C���f�b�N�X�o�b�t�@�擾
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
	// UVSet�̖��O���X�g���擾
	mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// UVSet�̖��O����UVSet���擾����
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
	// �}�e���A����������ΏI���
	if (mesh->GetElementMaterialCount() == 0)
	{
		mesh_data.m_MaterialName = "";
		return;
	}

	// Mesh���̃}�e���A�������擾
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

	FbxArray<FbxString*> AnimStackNameArray;//�����񃊃X�g
	fbxScene->FillAnimStackNameArray(AnimStackNameArray);//�A�j���[�V�����̏����ݒ�

	//�A�j���̌����擾
	animeNumMax=AnimStackNameArray.Size();
	animeMat.resize(animeNumMax);//�A�j���̌���ݒ� �P�d�ڂ̃T�C�Y����

	for (int i = 0; i < AnimStackNameArray.Size(); i++) {
		int tmp = 0;
		timeCount[i] = start[i];
		while (timeCount[i] < stop[i]) {
			timeCount[i] += FrameTime[i];
			tmp++;
		}
		timeMax.push_back(tmp);
		//�z��T�C�Y������
		animeMat[i].resize(tmp);//�A�j���̏I�����ԂŐݒ� ���̎��_�łQ�d�߂̃T�C�Y����
		//512�̃}�g���N�X������悤�ɐݒ�
		for (int h = 0; h < tmp; h++) {
			animeMat[i][h].resize(512);
		}
	}

	for (int i = 0; i < AnimStackNameArray.Size(); i++) {
		timeCount[i] = start[i];

		FbxAnimStack* AnimationStack = fbxScene->FindMember<FbxAnimStack>(AnimStackNameArray[i]->Buffer());//scene����A�j���[�V���������o���Ă���
		fbxScene->SetCurrentAnimationStack(AnimationStack);//�A�j���[�V������ݒ�

		for (int h = 0; h < timeMax[i]; h++) {
			timeCount[i] += FrameTime[i];

			for (int count = 0; count < meshNum; count++) {
				// �e���_�Ɋ|���邽�߂̍ŏI�I�ȍs��̔z��
				FbxMatrix* clusterDeformation = new FbxMatrix[fMesh[count]->GetControlPointsCount()];
				memset(clusterDeformation, 0, sizeof(FbxMatrix) * fMesh[count]->GetControlPointsCount());

				FbxSkin* skinDeformer = (FbxSkin*)fMesh[count]->GetDeformer(0, FbxDeformer::eSkin);
				int clusterCount = skinDeformer->GetClusterCount();
				// �e�N���X�^����e���_�ɉe����^���邽�߂̍s��쐬
				FbxCluster* cluster;
				FbxMatrix vertexTransformMatrix;
				FbxAMatrix referenceGlobalInitPosition;
				FbxAMatrix clusterGlobalInitPosition;
				FbxMatrix clusterGlobalCurrentPosition;
				FbxMatrix clusterRelativeInitPosition;

				//�A�j���̓L���b�V������ ���[�h���Ԃ������Ȃ胁�������H����FPS�͗����Ȃ�
				for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
					// �N���X�^(�{�[��)�̎��o��
					cluster = skinDeformer->GetCluster(clusterIndex);//�{�[�������o��

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

	FbxArray<FbxString*> AnimStackNameArray;//�����񃊃X�g
	fbxScene->FillAnimStackNameArray(AnimStackNameArray);//�A�j���[�V�����̏����ݒ�
	if (AnimStackNameArray.Size() <= 0) {
		return;
	}

	start = new FbxTime[AnimStackNameArray.Size()];
	stop = new FbxTime[AnimStackNameArray.Size()];
	FrameTime = new FbxTime[AnimStackNameArray.Size()];
	timeCount = new FbxTime[AnimStackNameArray.Size()];

	for (int i = 0; i < AnimStackNameArray.Size(); i++) {

		FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(*(AnimStackNameArray[i]));//�A�j���[�V�����̏������o��
		start[i] = takeInfo->mLocalTimeSpan.GetStart();
		stop[i] = takeInfo->mLocalTimeSpan.GetStop();
		FrameTime[i].SetTime(0, 0, 0, 1, 0, fbxScene->GetGlobalSettings().GetTimeMode());//�i�߂鎞�Ԃ�ݒ�
		timeCount[i] = start[i];
	}

	int count = 0;
	for (int count = 0; count < meshNum; count++) {
		FbxSkin* skinDeformer = (FbxSkin*)fMesh[count]->GetDeformer(0, FbxDeformer::eSkin);
		int clusterCount = skinDeformer->GetClusterCount();

		FbxCluster* cluster;
		//�z�񐶐�
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
		//�N���X�^�ƃC���f�b�N�X�A�E�F�C�g���o��
		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
			// �N���X�^(�{�[��)�̎��o��
			cluster = skinDeformer->GetCluster(clusterIndex);//�{�[�������o��
			for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++) {
				int index = cluster->GetControlPointIndices()[i];//���_�̃C���f�b�N�X
				//clusterIndex���Ή����钸�_�ԍ���index
				double weight = cluster->GetControlPointWeights()[i];//���_�̃E�F�C�g
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

		int* indices = fMesh[count]->GetPolygonVertices();//�O�p�̐����Q�b�g
		int polygon_vertex_count = fMesh[count]->GetPolygonVertexCount();//���_�̐����Q�b�g

		//�E�F�C�g����
		for (int i = 0; i < polygon_vertex_count; i++)
		{
			// �C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
			int index = indices[i];
			for (int h = 0; h < 4; h++) {

				meshList[count].m_Vertices[i].BoneWeight[h] = weightOll[index][h];
				meshList[count].m_Vertices[i].BoneIndex[h] = indexOll[index][h]; //0~24�̃{�[���ԍ�  
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
		//���_�̃q�[�v���쐬����B�B�B��������H���Ă��邩��폜������
		auto result = device->CreateCommittedResource(
			&tmpProp,
			D3D12_HEAP_FLAG_NONE,
			&tmpDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff[_meshNum]));
		
		if (FAILED(result)) {
			MessageBox(NULL, TEXT("���_�o�b�t�@�̐����Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
			return false;
		}


		////���_�����R�s�[
		CustomVertex* vertMap = nullptr;
		result = vertBuff[_meshNum]->Map(0, nullptr, (void**)&vertMap);
		std::copy(std::begin(mesh.m_Vertices), std::end(mesh.m_Vertices), vertMap);
		vertBuff[_meshNum]->Unmap(0, nullptr);
		if (FAILED(result)) {
			MessageBox(NULL, TEXT("���_�o�b�t�@�̐����Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
			return false;
		}
		mesh.m_VertexBuffer.BufferLocation = vertBuff[_meshNum]->GetGPUVirtualAddress();//�o�b�t�@�̉��z�A�h���X
		mesh.m_VertexBuffer.SizeInBytes = static_cast<UINT>(mesh.m_Vertices.size() * sizeof(CustomVertex));//�S�o�C�g��
		mesh.m_VertexBuffer.StrideInBytes = sizeof(CustomVertex);//1���_������̃o�C�g��

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
			MessageBox(NULL, TEXT("�C���f�N�X�o�b�t�@�̐����Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
			return false;
		}
		////������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
		UINT* mappedIdx = nullptr;
		result=idxBuff[_meshNum]->Map(0, nullptr, (void**)&mappedIdx);
		std::copy(std::begin(mesh.m_Indices), std::end(mesh.m_Indices), mappedIdx);

		idxBuff[_meshNum]->Unmap(0, nullptr);
		if (FAILED(result)) {
			MessageBox(NULL, TEXT("�C���f�N�X�o�b�t�@�̐����Ɏ��s���܂���"), TEXT("FbxError"), MB_OK | MB_ICONERROR);
			return false;
		}
		//�C���f�b�N�X�o�b�t�@�r���[���쐬
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

//TODO ���Ƃ̌��ˍ����ň��������ς��̂ŕ��u
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

	//�p�C�v���C����n���܂�
	cmdList->SetPipelineState(pipelineState);

	//�V�F�[�_�[�ɓn���f�[�^�̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature);



	//���[���h����n��
	cmdList->SetDescriptorHeaps(1, &worldBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(0, worldBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());//0�Ԃɐݒ�
	//�{�[������n��
	cmdList->SetDescriptorHeaps(1, &boneBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(1, boneBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());//1�Ԃɐݒ�
	//�ω��ł������n���i���ԂƂ��e���Ƃ��̐��l�n�j
	cmdList->SetDescriptorHeaps(1, &utilityBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(5, utilityBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());

	//�J��������n��
	frauEngine::CameraBufferHeap* cameraBufferHeap = frauEngine::Camera::GetInstance()->GetConstantBuffer();
	cmdList->SetDescriptorHeaps(1, &cameraBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(3, cameraBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());//3�Ԃɐݒ�
	//�f�B���N�V���i�����C�g����n��
	frauEngine::LightBufferHeap* directionalLightBufferHeap= frauEngine::Lights::GetInstance()->GetConstantBuffer();
	cmdList->SetDescriptorHeaps(1, &directionalLightBufferHeap->descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(4, directionalLightBufferHeap->descripterHeap->GetGPUDescriptorHandleForHeapStart());//4�Ԃɐݒ�


	//�[�x�e�N�X�`������n��
	frauEngine::Lights* dLight = frauEngine::Lights::GetInstance();
	cmdList->SetDescriptorHeaps(1, &dLight->depthSRVHeap);
	cmdList->SetGraphicsRootDescriptorTable(8, dLight->depthSRVHeap->GetGPUDescriptorHandleForHeapStart());//6�Ԃɐݒ�

	int i = 0;
	for (auto& mesh : meshList)
	{

		//�����o�[�g�}�e���A������n��
		cmdList->SetDescriptorHeaps(1, &materialBufferHeap[mesh.m_MaterialName].descripterHeap);
		cmdList->SetGraphicsRootDescriptorTable(2, materialBufferHeap[mesh.m_MaterialName].descripterHeap->GetGPUDescriptorHandleForHeapStart());
		//PBR�}�e���A��
		cmdList->SetDescriptorHeaps(1, &pbrBufferHeap[i].descripterHeap);
		cmdList->SetGraphicsRootDescriptorTable(6, pbrBufferHeap[i].descripterHeap->GetGPUDescriptorHandleForHeapStart());

		//�A���x�h�e�N�X�`������n��
		cmdList->SetDescriptorHeaps(1, &textureBufferHeap[mesh.m_MaterialName].descripterHeap);
		cmdList->SetGraphicsRootDescriptorTable(7, textureBufferHeap[mesh.m_MaterialName].descripterHeap->GetGPUDescriptorHandleForHeapStart());
		//�A���x�h�ȊO�̃e�N�X�`������n��
		for (int h = 0; h < (int)frauEngine::ShaderTextureSlot::MODEL_ANOTHER; h++) {
			if (pbrBufferHeap[i].buffer->anotherTexture[h]) {
				cmdList->SetDescriptorHeaps(1, &anotherTexBufferHeap[h][i].descripterHeap);
				cmdList->SetGraphicsRootDescriptorTable(9 + h, anotherTexBufferHeap[h][i].descripterHeap->GetGPUDescriptorHandleForHeapStart());
			}
		}


		//�C���v�b�g�A�Z���u���̐ݒ�
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
	//������Ɗ댯����
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

	//�K���ɍ폜�����������Ă���܂�
	material->Destroy();
	texture->Destroy();



	//delete vertices;
	//delete indices;
	//materialElements->Destroy();
	//surface_material->Destroy();
	//FBXImpoter�n�͍Ō�ɂ��̏��ԂłȂ��ƍ폜�ł��܂���
	fbxScene->Destroy();
	fbxImporter->Destroy();
	fbxManager->Destroy();


}