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
#include <DirectXmath.h>//xnamath.h�̑�֕i
#include <d3dcompiler.h>
#include <fbxsdk.h> 
#include < locale.h >
#include "Window.h"
// �K�v�ȃ��C�u�����������N����
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "legacy_stdio_definitions.lib" )//�o�[�W�������V�����̂ł���������Ȃ���dxerr.lib�͂΂���
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
	
	//�V�F�[�_�̃R���p�C��
	CreateShader(VertexShaderFilePath, GeometryShaderFilePath,PixelShaderFilePath);

	if (LoadFbxFile(modelfile_name,texfile_name) == false)//�����񃊃X�g�A�t�H���_�ꏊ�A�S�t�@�C���p�X�𑗂�@�����t�H���_�ꏊ�͂����
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

		// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
		dg->GetContext()->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer, &strides, &offsets);

		dg->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		SetMaterialColor(dg, m_Materials[mesh.m_MaterialName]);

		// �e�N�X�`���ݒ�
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
			dg->SetTexture(m_MaterialLinks[mesh.m_MaterialName]);
		else
			dg->SetTexture(nullptr);
		
		// �`��
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
		// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
		graphics->GetContext()->IASetVertexBuffers(0, 2, pBuf, strides, offsets);

		graphics->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		SetMaterialColor(graphics, m_Materials[mesh.m_MaterialName]);

		// �e�N�X�`���ݒ�
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

	//�V���h�E�}�b�v��������
	for (auto& mesh : m_MeshList)
	{
		ID3D11Buffer* pBuf[2] = { mesh.m_VertexBuffer,  worldBuffer };
		// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
		graphics->GetContext()->IASetVertexBuffers(0, 2, pBuf, strides, offsets);

		graphics->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		SetMaterialColor(graphics, m_Materials[mesh.m_MaterialName]);

		// �e�N�X�`���ݒ�
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
			graphics->SetTextureSM(m_MaterialLinks[mesh.m_MaterialName]);
		else
			graphics->SetTextureSM(nullptr);

		// �`��
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

		// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
		graphics->GetContext()->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer, &strides, &offsets);

		graphics->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		SetMaterialColor(graphics, m_Materials[mesh.m_MaterialName]);

		// �e�N�X�`���ݒ�
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
			graphics->SetTextureSM(m_MaterialLinks[mesh.m_MaterialName]);
		else
			graphics->SetTextureSM(nullptr);

		// �`��
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

	//�V���h�E�}�b�v��������
	for (auto& mesh : m_MeshList)
	{
		// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
		graphics->GetContext()->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer, &strides, &offsets);

		graphics->GetContext()->IASetIndexBuffer(mesh.m_IndexBuffer,DXGI_FORMAT_R32_UINT,0);

		SetMaterialColor(graphics, m_Materials[mesh.m_MaterialName]);

		// �e�N�X�`���ݒ�
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
			graphics->SetTextureSM(m_MaterialLinks[mesh.m_MaterialName]);
		else
			graphics->SetTextureSM(nullptr);

		// �`��
		graphics->GetContext()->DrawIndexed(mesh.m_Indices.size(),0,0);			

	}

	


}

void FbxMeshFile::LoadVertices(MeshData& mesh_data, FbxMesh* mesh)
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
		// �C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
		int index = indices[i];

		// ���_���W���X�g������W���擾����
		vertex.Posision.X = (float)-vertices[index][0];
		vertex.Posision.Y = (float)vertices[index][1];
		vertex.Posision.Z = (float)vertices[index][2];

		// �ǉ�
		mesh_data.m_Vertices.push_back(vertex);
		
	}
}

void FbxMeshFile::LoadIndices(MeshData& mesh_data, FbxMesh* mesh)
{
	// �|���S�����̎擾
	int polygon_count = mesh->GetPolygonCount();

	// �|���S���̐������A�ԂƂ��ĕۑ�����
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
	// �@�����X�g�̎擾
	mesh->GetPolygonVertexNormals(normals);

	// �@���ݒ�
	for (int i = 0; i < normals.Size(); i++)
	{
		mesh_data.m_Vertices[i].Normal.X = (float)-normals[i][0];
		mesh_data.m_Vertices[i].Normal.Y = (float)normals[i][1];
		mesh_data.m_Vertices[i].Normal.Z = (float)normals[i][2];
	}
}

void FbxMeshFile::LoadColors(MeshData& mesh_data, FbxMesh* mesh)
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
	// UVSet�̖��O���X�g���擾
	mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// UVSet�̖��O����UVSet���擾����
	// ����̓}���`�e�N�X�`���ɂ͑Ή����Ȃ��̂ōŏ��̖��O���g��
	mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), uv_buffer);

	for (int i = 0; i < uv_buffer.Size(); i++)
	{
		FbxVector2& uv = uv_buffer[i];

		// ���̂܂܎g�p���Ė��Ȃ�
		mesh_data.m_Vertices[i].TexturePos.X = (float)uv[0];
		mesh_data.m_Vertices[i].TexturePos.Y = (float)(1.0 - uv[1]);
	}
}

void FbxMeshFile::SetMaterialName(MeshData& mesh_data, FbxMesh* mesh)
{
	// �}�e���A����������ΏI���
	if (mesh->GetElementMaterialCount() == 0)
	{
		mesh_data.m_MaterialName = "";
		return;
	}

	// Mesh���̃}�e���A�������擾
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

	// �t�@�C�������擾
	std::string file_path = texture->GetRelativeFileName();
	// �t�@�C������
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, file_path.c_str(), sizeof(char) * 256);

	// �L������
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// �u/�v�ŕ���
	Split('/', buffer, split_list);

	std::string root_path = texfile_name;

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);

	// ���������΍�
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
		wstr_file_name.c_str(),									// �t�@�C����(wchar_t�^�Ȃ̂Œ���)
		nullptr,											// �ʏ�̃e�N�X�`���f�[�^
		&m_Textures[file_name])))							// �V�F�[�_�Ŏg�p���邱�Ƃ��ł���e�N�X�`���f�[�^
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
	entry_material.SetAmbient((float)color[0], (float)color[1], (float)color[2], (float)factor);

	color = colors[(int)MaterialOrder::Diffuse];
	factor = factors[(int)MaterialOrder::Diffuse];
	entry_material.SetDiffuse((float)color[0], (float)color[1], (float)color[2], (float)factor);

	m_Materials[material->GetName()] = entry_material;//�S���̃}�e���A�������Ԃ�����

	// �e�N�X�`���ǂݍ���(�V���O���Ή�)�@�}�e���A���ƃe�N�X�`���̊֌W��������
	// Diffuse�v���p�e�B���擾
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxFileTexture* texture = nullptr;
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
		LoadTexture(texture, keyword,texfile_name) == true)
	{
		// �ǂݍ��񂾃e�N�X�`���ƃ}�e���A���̊֌W���o���Ă���
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
	//�������A�j���[�V������ݒ肵�Ă���
	if (nowAnimeNum != _num) {
		nowAnimeNum = _num;
		FbxArray<FbxString*> AnimStackNameArray;//�����񃊃X�g
		fbxSceneAnime->FillAnimStackNameArray(AnimStackNameArray);//�A�j���[�V�����̏����ݒ�
		FbxAnimStack* AnimationStack = fbxSceneAnime->FindMember<FbxAnimStack>(AnimStackNameArray[_num]->Buffer());//scene����A�j���[�V���������o���Ă���
		fbxSceneAnime->SetCurrentAnimationStack(AnimationStack);//�A�j���[�V������ݒ�

		timeCount[_num] = start[_num];
	}


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

		//�����̌v�Z�͂P�x�Ɍv�Z���Đ��l�������Ă������ƂőΉ��ł��� ���[�h���Ԃ������Ȃ胁�������H����FPS�͗����Ȃ�
		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
			// �N���X�^(�{�[��)�̎��o��
			cluster = skinDeformer->GetCluster(clusterIndex);//�{�[�������o��

			cluster->GetTransformMatrix(referenceGlobalInitPosition);
			cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

			clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(timeCount[_num]);//10m
			clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;//3m

			vertexTransformMatrix = clusterGlobalCurrentPosition * clusterRelativeInitPosition;
			//�s��̏��Ԃ������̂Ń{�[�����������ȋ���������
			DirectX::XMMATRIX cb;

			cb = DirectX::XMMATRIX(////x             y                           z                               w
				vertexTransformMatrix[0][0], vertexTransformMatrix[0][1], vertexTransformMatrix[0][2], vertexTransformMatrix[0][3],

				vertexTransformMatrix[1][0], vertexTransformMatrix[1][1], vertexTransformMatrix[1][2], vertexTransformMatrix[1][3],

				vertexTransformMatrix[2][0], vertexTransformMatrix[2][1], vertexTransformMatrix[2][2], vertexTransformMatrix[2][3],

				vertexTransformMatrix[3][0], vertexTransformMatrix[3][1], vertexTransformMatrix[3][2], vertexTransformMatrix[3][3]
			);

			//���ʂ̉��Z�ł͂Ȃ�
			//   w���P�ɐ��K�����ꂽ���_                =  �N���X�^�s��@���@�@���_���W
			//verticesCluster[i] = clusterDeformation[i].MultNormalize(fMesh->GetControlPointAt(i));

			//���_�V�F�[�_�ɓn���ϐ��ɓ����
			DirectGraphics::GetInstance()->GetConstantBufferData()->mBoneMatrix[clusterIndex] = cb; //XMMatrixTranspose(cb);

		}
		delete[] clusterDeformation;
	}
	// ----- Animation -----

	timeCount[_num] += FrameTime[_num] * speed;

	if (timeCount[_num] > stop[_num])
		timeCount[_num] = start[_num];

	

}

//�A�j���[�V�����ǂݍ��� LoadFbxFile�̎q
void FbxMeshFile::CreateAnimation(FbxScene* fbxScene) {

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


	/////////////////////////////////////////////////////////////////////////////////

	int count = 0;
	for (int count = 0; count < meshNum; count++) {
		//���b�V����������������ׂ��H
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
				indexOll[i][h] = 39;
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
				//�����������A�P�̃N���X�^�ɂ��S�񌈂߂Ă�
				for (int h = 0; h < 4; h++) {
					if (indexOll[index][h] == 39) {
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

				m_MeshList[count].m_Vertices[i].BoneWeight[h] = weightOll[index][h];
				m_MeshList[count].m_Vertices[i].BoneIndex[h] = indexOll[index][h]; //0~24�̃{�[���ԍ�  
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
	// FbxManager�쐬
	FbxManager* fbx_manager = fbxsdk::FbxManager::Create();
	if (fbx_manager == nullptr)
	{
		return false;
	}
	
	// FbxImporter�쐬
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
	if (fbx_importer == nullptr)
	{
		fbx_manager->Destroy();
		return false;
	}
	
	// FbxScene�쐬
	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "");
	if (fbx_scene == nullptr)
	{
		fbx_importer->Destroy();
		fbx_manager->Destroy();
		return false;
	}

	// File��������
	fbx_importer->Initialize(modelfile_name);
	// scene�ɃC���|�[�g
	fbx_importer->Import(fbx_scene);

	FbxGeometryConverter converter(fbx_manager);
	// �A�j���[�V�������ǂݍ��߂Ȃ������H���b�V���Ɏg���Ă���}�e���A���P�ʂŃ��b�V���𕪊�����
	converter.SplitMeshesPerMaterial(fbx_scene, true);
	// �|���S�����O�p�`�ɂ���
	converter.Triangulate(fbx_scene, true);

	int material_num = fbx_scene->GetSrcObjectCount<FbxSurfaceMaterial>();//�}�e���A���̌��𒲂ׂ�

	//�}�e���A���ǂݍ���
	for (int i = 0; i < material_num; i++)
	{
		LoadMaterial(fbx_scene->GetSrcObject<FbxSurfaceMaterial>(i),texfile_name);
	}

	// FbxMesh�̐����擾
	int mesh_num = fbx_scene->GetSrcObjectCount<FbxMesh>();
	//���b�V���ǂݍ���
	for (int i = 0; i < mesh_num; i++)
	{
		// Mesh�쐬
		CreateMesh(fbx_scene->GetSrcObject<FbxMesh>(i));
	}
	//�e�N�X�`���̓ǂݍ���
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
	
	//�A�j���[�V�����̓ǂݍ���
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
		//���_�o�b�t�@�쐬
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(CustomVertex) * mesh.m_Vertices.size();	// �o�b�t�@�̃T�C�Y
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;			// �g�p���@
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND�ݒ�
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;						// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
		buffer_desc.MiscFlags = 0;							// ���\�[�X�I�v�V�����̃t���O
		buffer_desc.StructureByteStride = 0;				// �\���̂̃T�C�Y

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.m_Vertices[0];	// �o�b�t�@�̒��g�̐ݒ�
		sub_resource.SysMemPitch = 0;			// texture�f�[�^���g�p����ۂɎg�p���郁���o
		sub_resource.SysMemSlicePitch = 0;		// texture�f�[�^���g�p����ۂɎg�p���郁���o

		// �o�b�t�@�쐬
		if (FAILED(device->CreateBuffer(
			&buffer_desc,								// �o�b�t�@���
			&sub_resource,								// ���\�[�X���
			&mesh.m_VertexBuffer)))	// �쐬���ꂽ�o�b�t�@�̊i�[��
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
		//���_�o�b�t�@�쐬
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = (UINT)sizeof(UINT) *  mesh.m_Indices.size();	// �o�b�t�@�̃T�C�Y
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// �g�p���@
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND�ݒ�
		buffer_desc.CPUAccessFlags = 0;										// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
		buffer_desc.MiscFlags = 0;											// ���\�[�X�I�v�V�����̃t���O
		buffer_desc.StructureByteStride = 0;								// �\���̂̃T�C�Y

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.m_Indices[0];							// �o�b�t�@�̒��g�̐ݒ�
		sub_resource.SysMemPitch = 0;										// texture�f�[�^���g�p����ۂɎg�p���郁���o
		sub_resource.SysMemSlicePitch = 0;									// texture�f�[�^���g�p����ۂɎg�p���郁���o

		// �o�b�t�@�쐬
		if (FAILED(device->CreateBuffer(
			&buffer_desc,						// �o�b�t�@���
			&sub_resource,						// ���\�[�X���
			&mesh.m_IndexBuffer)))		// �쐬���ꂽ�o�b�t�@�̊i�[��
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
		// �C���X�^���X���f�[�^(�X���b�g1)
		{"MATRIX",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,                             D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"MATRIX",   1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, sizeof(DirectX::XMFLOAT4),     D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"MATRIX",   2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, sizeof(DirectX::XMFLOAT4) * 2, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"MATRIX",   3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, sizeof(DirectX::XMFLOAT4) * 3, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	//���_���C�A�E�g�쐬
	if (FAILED(dev->CreateInputLayout(
		vertex_desc,				// ���C�A�E�g�ݒ�
		ARRAYSIZE(vertex_desc),		// �z��T�C�Y
		m_VertexShader->GetData(),	// ���C�A�E�g�Ɗ֘A�t���钸�_�V�F�[�_�̃f�[�^
		m_VertexShader->GetSize(),	// ���C�A�E�g�Ɗ֘A�t���钸�_�V�F�[�_�̃T�C�Y
		&m_InputLayout)))			// �쐬���ꂽ���_���C�A�E�g�̊i�[��
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

//�s�N�Z���V�F�[�_�ƃW�I���g���V�F�[�_�ƒ��_�V�F�[�_�̂�
void FbxMeshFile::CreateShader(const char* VertexShaderFilePath, const char* GeometryShaderFilePath,const char* PixelShaderFilePath) {

	m_VertexShader = new VertexShader();
	m_VertexShader->Create(dev, VertexShaderFilePath);

	m_GeometryShader = new GeometryShader();
	m_GeometryShader->Create(dev, GeometryShaderFilePath);

	m_PixelShader = new PixelShader();
	m_PixelShader->Create(dev, PixelShaderFilePath);

}


//�C���X�^���X�̃��[���h���W��ǉ������߂�
void  FbxMeshFile::SetWorldMatrix() {

	//�P��̃C���X�^���X�`��ŕ�����Ăԁi�����C���X�^���X�̂��́j

	// ���[���h�}�g���N�X�ݒ�
	DirectX::XMMATRIX world_matrix;
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(pos.X, pos.Y, pos.Z);
	DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(degree.X));
	DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(degree.Y));
	DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(degree.Z));
	DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);


	DirectX::XMMATRIX axisRLen = DirectX::XMMatrixTranslation(axisR.X, axisR.Y, axisR.Z);

	DirectX::XMVECTOR vaxis;
	DirectX::XMMATRIX quaternion;
	// ���𓯎����W�ɕϊ�
	DirectX::XMFLOAT4 axis4 = { axis.X,axis.Y,axis.Z,0 };
	vaxis = XMLoadFloat4(&axis4);
	// �x���@�����W�A���ɕϊ�
	axisAngle = (axisAngle * 3.1415) / 180.0f;
	// �w�莲�𒆐S�ɉ�]����s����쐬����
	quaternion = DirectX::XMMatrixRotationAxis(vaxis, axisAngle);
	//���a�@�@�@��]���@�@�@��]���S
	world_matrix = scale_mat * rotate_z * rotate_x * rotate_y * axisRLen * quaternion * translate;

	DirectX::XMFLOAT4X4 wm;
	XMStoreFloat4x4(&wm, XMMatrixTranspose(world_matrix));

	worldMatrix.push_back(wm);

}

//�C���X�^���X�p�̃��[���h�o�b�t�@�̍쐬
bool FbxMeshFile::CreateWorldMatrixBuffer(int _instanceNum) {

	//�P��̃C���X�^���X�`��̍ŏ��ɌĂ�
	if (worldBuffer != nullptr) {
		worldBuffer->Release();
	}

	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * _instanceNum;	// �o�b�t�@�̃T�C�Y
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;			// �g�p���@
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND�ݒ�
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;						// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
	buffer_desc.MiscFlags = 0;							// ���\�[�X�I�v�V�����̃t���O
	buffer_desc.StructureByteStride = 0;				// �\���̂̃T�C�Y

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &worldMatrix[0];	// �o�b�t�@�̒��g�̐ݒ�
	sub_resource.SysMemPitch = 0;			// texture�f�[�^���g�p����ۂɎg�p���郁���o
	sub_resource.SysMemSlicePitch = 0;		// texture�f�[�^���g�p����ۂɎg�p���郁���o

	// �o�b�t�@�쐬
	if (FAILED(DirectGraphics::GetInstance()->GetDevice()->CreateBuffer(
		&buffer_desc,								// �o�b�t�@���
		&sub_resource,								// ���\�[�X���
		&worldBuffer)))	// �쐬���ꂽ�o�b�t�@�̊i�[��
	{
		return false;
	}

	while (worldMatrix.size() != 0) {
		worldMatrix.pop_back();
	}
	return true;
}