#include "ModelObject.h"

//�C���[�W
using namespace obj;
using namespace lowApp;

//���f��
ModelObject::~ModelObject() {
}
void ModelObject::SetResource(fbx::FBX* _model) {
	resource = _model;

	//�萔�o�b�t�@�̍쐬
	worldBufferHeap.Create();
	boneBufferHeap.Create();
	utilityBufferHeap.Create();

	//���b�V�������̃q�[�v���쐬
	meshMax = resource->GetMeshMax();
	pbrBufferHeap = new cBuffer::PBRBufferHeap[meshMax];
	for (int i = 0; i < (int)shader::TextureSlot::MODEL_ANOTHER; i++) {
		anotherResourceBufferHeap[i] = new srBuffer::ShaderResourceBufferHeap[meshMax];
	}

	for (int i = 0; i < meshMax; i++) {
		pbrBufferHeap[i].Create();
		
	}
}

void ModelObject::SetShader(
	shader::Shader* _shaderV,
	shader::Shader* _shaderP,
	shader::Shader* _shaderG,
	shader::Shader* _shaderH,
	shader::Shader* _shaderD) {

	shader::Shader* shaders[5]{ _shaderV,_shaderP,_shaderG,_shaderH,_shaderD };

	for (int i = 0; i < 5; i++) {
		if (shaders[i] == nullptr) {
			break;
		}
		switch (shaders[i]->GetShaderType()) {
		case shader::NONE:
			return;
			break;
		case shader::VS:
			shaderVS = shaders[i]->GetShaderByteCode();
			break;
		case shader::HS:
			shaderHS = shaders[i]->GetShaderByteCode();
			break;
		case shader::DS:
			shaderDS = shaders[i]->GetShaderByteCode();
			break;
		case shader::GS:
			shaderGS = shaders[i]->GetShaderByteCode();
			break;
		case shader::PS:
			shaderPS = shaders[i]->GetShaderByteCode();
			break;
		}
	}
}

void ModelObject::CreatePipeline() {


	auto lowApp = LowApplication::GetInstance();

	gpipelineDesc = lowApp->GetPipelineStateDesc(PipelineType::DEFAULT);//�ЂȌ`�����Ă���

	//�|���S���̖ʕ`��̐ݒ�
	gpipelineDesc.RasterizerState.CullMode = cullMode;

	gpipelineDesc.pRootSignature = nullptr;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_INPUT_ELEMENT_DESC _inputLayout[] = {
	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	gpipelineDesc.InputLayout.pInputElementDescs = _inputLayout;//���C�A�E�g�擪�A�h���X
	gpipelineDesc.InputLayout.NumElements = _countof(_inputLayout);//���C�A�E�g�z��
	//�V�F�[�_�[��ݒ�
	gpipelineDesc.VS = shaderVS;
	gpipelineDesc.HS = shaderHS;
	gpipelineDesc.DS = shaderDS;
	gpipelineDesc.GS = shaderGS;
	gpipelineDesc.PS = shaderPS;

	const int constantNum = cBuffer::HEAP_SLOT_MODEL;
	const int textureNum = (int)shader::TextureSlot::MODEL_DEFAULT + (int)shader::TextureSlot::MODEL_ANOTHER;
	D3D12_DESCRIPTOR_RANGE descTblRange[constantNum + textureNum] = {};
	for (int i = 0; i < constantNum; i++) {
		descTblRange[i].NumDescriptors = 1;
		descTblRange[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//��ʂ͒萔
		descTblRange[i].BaseShaderRegister = i;
		descTblRange[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}
	for (int i = constantNum; i < constantNum + textureNum; i++) {
		descTblRange[i].NumDescriptors = 1;
		descTblRange[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//��ʂ̓e�N�X�`��
		descTblRange[i].BaseShaderRegister = i;
		descTblRange[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	//���[�g�p�����[�^�[�̐ݒ�
	D3D12_ROOT_PARAMETER rootparam[constantNum + textureNum] = {};
	for (int i = 0; i < constantNum + textureNum; i++) {
		rootparam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootparam[i].DescriptorTable.pDescriptorRanges = &descTblRange[i];//�f�X�N���v�^�����W�̃A�h���X
		rootparam[i].DescriptorTable.NumDescriptorRanges = 1;//�f�X�N���v�^�����W��
		rootparam[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩���� (�킩�肸�炳��������邽�߂ɂ��ׂ�)
	}

	rootSignatureDesc.pParameters = rootparam;//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = constantNum + textureNum;//���[�g�p�����[�^��

		//�T���v���[�̍쐬
	D3D12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ�
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ�
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;//��Ԃ��Ȃ�(�j�A���X�g�l�C�o�[)
	samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
	samplerDesc[0].MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//�I�[�o�[�T���v�����O�̍ۃ��T���v�����O���Ȃ��H
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�s�N�Z���V�F�[�_����̂݉�
	samplerDesc[0].ShaderRegister = 0;
	//�V���h�E�p�̃T���v��
	samplerDesc[1] = samplerDesc[0];//�ύX�_�ȊO���R�s�[
	samplerDesc[1].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;//�{�[�_�[�̎��͍�
	samplerDesc[1].ShaderRegister = 1;
	samplerDesc[1].MaxAnisotropy = 2;
	samplerDesc[1].MipLODBias = 0;
	samplerDesc[1].ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 2;

	//���[�g�V�O�l�`���̐ݒ�
	ID3DBlob* rootSigBlob = nullptr;
	auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, NULL);
	lowApp->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	rootSigBlob->Release();

	//�p�C�v���C���ɐݒ�
	gpipelineDesc.pRootSignature = rootSignature;

	lowApp->GetDevice()->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelineState));
	resource;
}

void ModelObject::Initialize(fbx::FBX* _model,
	shader::Shader* _shaderV,
	shader::Shader* _shaderP,
	shader::Shader* _shaderG,
	shader::Shader* _shaderH,
	shader::Shader* _shaderD,
	D3D12_CULL_MODE _cullMode
) {
	
	
	SetResource(_model);
	resource;
	SetShader(
		_shaderV,
		_shaderP,
		_shaderG,
		_shaderH,
		_shaderD
	);

	cullMode = _cullMode;

	CreatePipeline();
}

void ModelObject::SetAnotherTexture(int _meshNum, std::string _filePath, int _num) {
	anotherResourceBufferHeap[_num][_meshNum].CreateShaderResourceBufferHeap(_filePath);


	pbrBufferHeap[_meshNum].buffer->anotherTexture[_num] = true;
}


void ModelObject::SetPBR(
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

	float workSpace
) {

	
	pbrBufferHeap[_meshNum].buffer->subsurface = subsurface;
	pbrBufferHeap[_meshNum].buffer->metallic = metallic;
	pbrBufferHeap[_meshNum].buffer->specular = specular;
	pbrBufferHeap[_meshNum].buffer->specularTint = specularTint;
	pbrBufferHeap[_meshNum].buffer->roufhness = roufhness;
	pbrBufferHeap[_meshNum].buffer->anistropics = anistropics;
	pbrBufferHeap[_meshNum].buffer->sheen = sheen;
	pbrBufferHeap[_meshNum].buffer->sheenTint = sheenTint;
	pbrBufferHeap[_meshNum].buffer->clearColor = clearColor;
	pbrBufferHeap[_meshNum].buffer->clearcoatGloss = clearcoatGloss;

	pbrBufferHeap[_meshNum].buffer->outline = outline;
	pbrBufferHeap[_meshNum].buffer->outlineColorR= outlineColor.X;
	pbrBufferHeap[_meshNum].buffer->outlineColorG= outlineColor.Y;
	pbrBufferHeap[_meshNum].buffer->outlineColorB= outlineColor.Z;

	pbrBufferHeap[_meshNum].buffer->workSpace = workSpace;

}


void ModelObject::SetPos(Vector3 _pos) {
	if (imgui == true) {
		return;
	}
	pos = _pos;
}

void ModelObject::SetAngle(Vector3 _angle) {
	if (imgui == true) {
		return;
	}
	angle = _angle;
}

void ModelObject::SetScale(Vector3 _scale) {
	if (imgui == true) {
		return;
	}
	scale = _scale;
}

void ModelObject::SetAll(Vector3 _pos, Vector3 _angle, Vector3 _scale) {
	if (imgui == true) {
		return;
	}
	SetPos(_pos);
	SetAngle(_angle);
	SetScale(_scale);

}

void ModelObject::SetAnime(bool _anime) {
	if (imgui == true) {
		return;
	}
	anime = _anime;
}

void ModelObject::SetAnimeNum(int _animeNum) {
	if (imgui == true) {
		return;
	}
	animeNum = _animeNum;
}

void ModelObject::SetAnimeSpeed(float _animeSpeed) {
	if (imgui == true) {
		return;
	}
	animeSpeed = _animeSpeed;
}

void ModelObject::SetAllAnimeState(bool _anime, int _animeNum, float _animeSpeed) {
	if (imgui == true) {
		return;
	}
	SetAnime(_anime);
	SetAnimeNum(_animeNum);
	SetAnimeSpeed(_animeSpeed);

}

void ModelObject::DrawImGUI() {

	if (imgui == false) {
		return;
	}

	ImGui::Begin(name);//�E�B���h�E���ɂȂ�

	float v3[3];
	ImGui::Checkbox("Enable", &enable);

	v3[0] = pos.X; v3[1] = pos.Y; v3[2] = pos.Z;
	ImGui::InputFloat3("Position", v3);
	pos.X = v3[0]; pos.Y = v3[1]; pos.Z = v3[2];

	v3[0] = angle.X; v3[1] = angle.Y; v3[2] = angle.Z;
	ImGui::SliderFloat3("Angle", v3, 0.0f, 360.0f);
	//ImGui::InputFloat3("Angle", v3);
	angle.X = v3[0]; angle.Y = v3[1]; angle.Z = v3[2];

	v3[0] = scale.X; v3[1] = scale.Y; v3[2] = scale.Z;
	ImGui::InputFloat3("Scale", v3);
	scale.X = v3[0]; scale.Y = v3[1]; scale.Z = v3[2];

	ImGui::Checkbox("Anime", &anime);
	ImGui::InputInt("AnimeNum", &animeNum);
	ImGui::InputFloat("AnimeSpeed", &animeSpeed);

	ImGui::End();
}
void ModelObject::Updata() {
	if (enable == false) {
		return;
	}
}

void ModelObject::Draw() {
	DrawImGUI();
	if (enable == false) {
		return;
	}

	//���[���h���W�v�Z
	DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(pos.X, pos.Y, pos.Z);
	DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angle.X));
	DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angle.Y));
	DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angle.Z));
	worldBufferHeap.buffer->world = scale_mat * rotate_z * rotate_x * rotate_y * translate;

	if (anime&& resource->GetAnimeNumMax()) {

		//�������A�j���[�V������ݒ肵�Ă���
		if (animeNum != animeNumOld) {
			animeNumOld = animeNum;//�Â��A�j���ԍ���ۑ�
			animeCount = 0;
		}
		//FBX���\�[�X���Ń{�[���}�g���N�X�̑�����s��
		resource->GetBoneMatrix(boneBufferHeap.buffer, animeNum, (int)animeCount);
		//���Ԍo��
		animeCount += animeSpeed;
		if (animeCount >= resource->GetAnimeTimeMax(animeNum)) {
			animeCount = 0;
		}
	}
	//GPU�ɕK�v�ȃf�[�^�𑗂��ĕ`�悷��
	resource->Draw(
		&worldBufferHeap, &boneBufferHeap, &utilityBufferHeap, pbrBufferHeap,//�萔�o�b�t�@
		anotherResourceBufferHeap,	//���\�[�X�o�b�t�@
		pipelineState, rootSignature);//�p�C�v���C���n��

}
