#include "Stage.h"

void Stage::Initialize() {


	Resource* resource = Resource::GetInstance();

	auto vertexShader = resource->Shader("VertexShader.hlsl");
	auto vertexShaderShadow = resource->Shader("VertexShader_Shadow.hlsl");

	auto vertexShaderAnim = resource->Shader("VertexShader_Anim.hlsl");
	auto vertexShaderAnimShadow = resource->Shader("VertexShader_AnimShadow.hlsl");

	auto pixelShader  = resource->Shader("PixelShader.hlsl");
	auto pixelShaderShadow = resource->Shader("PixelShader_Shadow.hlsl");
	auto pixelShaderToon = resource->Shader("PixelShader_Toon.hlsl");

	auto pixelShaderWater = resource->Shader("PixelShader_Water.hlsl");  
	auto pixelShaderTex = resource->Shader("PixelShader_Texture.hlsl");  

	//�w�i������///////////////////
	skyModel.Initialize(resource->LoadModel("Data/Model/Sky/Sky.fbx", "Data/Model/Sky/Tex/"), vertexShader, pixelShaderTex, nullptr, nullptr, nullptr, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
	waterModel.Initialize(resource->LoadModel("Data/Model/Water/Water.fbx", "Data/Model/Water/Tex/"), vertexShader, pixelShaderWater, nullptr, nullptr, nullptr, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);

	for (int i = 0; i < 3; i++) {
		planterModel[i].Initialize(resource->Model("MagicPlanter.fbx"), vertexShader, pixelShader);
		planterModel[i].SetAll(Vector3(-7.5, -3.5, 0 + i * 1), Vector3(-90, -60, 0), Vector3(0.3, 0.3, 0.3));
	}
}

void Stage::Load(StageNum _stageNum) {
	Resource* resource = Resource::GetInstance();
	auto vertexShader = resource->Shader("VertexShader.hlsl");
	auto vertexShaderShadow = resource->Shader("VertexShader_Shadow.hlsl");

	auto vertexShaderAnim = resource->Shader("VertexShader_Anim.hlsl");
	auto vertexShaderAnimShadow = resource->Shader("VertexShader_AnimShadow.hlsl");

	auto pixelShader = resource->Shader("PixelShader.hlsl");
	auto pixelShaderShadow = resource->Shader("PixelShader_Shadow.hlsl");
	auto pixelShaderToon = resource->Shader("PixelShader_Toon.hlsl");

	auto pixelShaderWater = resource->Shader("PixelShader_Water.hlsl");
	auto pixelShaderTex = resource->Shader("PixelShader_Texture.hlsl");

	//���[�h�̃^�C�~���O�̊֌W�ł������MoveStage�ɑg�ݍ��܂�Scene�ʂōs��
	switch (_stageNum) {
	case StageNum::FLOWER_SHOP:
		//�ԉ�
		model.Initialize(resource->Model("FlowerShop.fbx"), vertexShader, pixelShaderToon, nullptr, nullptr, nullptr, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
		model.InitializeSub(ModelType::SHADOW, vertexShaderShadow, pixelShaderShadow);
		LoadMaterialLinker("Data/Model/FlowerShop/MatLink/FlowerShop.matlink", "Data/Model/FlowerShop/MatLink/Material/", "Data/Model/FlowerShop/Tex/", &model);
		model.SetAll(Vector3(-8, 2, 1.8), Vector3(0, 90, 0), Vector3(2, 0.9, 1.5));
		
		directionalLightParam.pos = Vector3(12, 25, -21) / 2.0f;
		directionalLightParam.target = Vector3(0, 0, 0);
		directionalLightParam.up = Vector3(0, 1, 0);
		directionalLightParam.fov = 45;
		directionalLightParam.vector = Vector3(1, 1, 1);
		directionalLightParam.color = Vector3(1, 1, 1);
		break;
	case StageNum::HANDY_SHOP:
		//�Ȃ�ł���
		model.Initialize(resource->Model("HandyShop.fbx"), vertexShader, pixelShaderToon, nullptr, nullptr, nullptr, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
		model.InitializeSub(ModelType::SHADOW, vertexShaderShadow, pixelShaderShadow);
		LoadMaterialLinker("Data/Model/HandyShop/MatLink/HandyShop.matlink", "Data/Model/HandyShop/MatLink/Material/", "Data/Model/HandyShop/Tex/", &model);
		model.SetAll(Vector3(-8, 2, 1.8), Vector3(0, 90, 0), Vector3(2, 0.9, 1.5));
		////NPC������
		npc.Initialize(resource->Model("Handy.fbx"), vertexShaderAnim, pixelShader);
		//npc.InitializeSub(ModelType::SHADOW, vertexShaderAnimShadow, pixelShaderShadow);
		npc.SetAll(Vector3(0.2, -4.8, 4), Vector3(0, 205, 0), Vector3(0.03f, 0.03f, 0.03f));
		npc.SetAllAnimeState(true, 1, 0.3);
		//D���C�g�ݒ�
		directionalLightParam.pos = Vector3(12, 25, -21) / 2.0f;
		directionalLightParam.target = Vector3(0, 0, 0);
		directionalLightParam.up = Vector3(0, 1, 0);
		directionalLightParam.fov = 45;
		directionalLightParam.vector = Vector3(1, 1, 1);
		directionalLightParam.color = Vector3(1, 1, 1);
		break;
	case StageNum::MAGIC_SHOP:
		//�}�W�b�N�V���b�v
		model.Initialize(resource->Model("MagicShop.fbx"), vertexShader, pixelShaderToon, nullptr, nullptr, nullptr, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
		model.InitializeSub(ModelType::SHADOW, vertexShaderShadow, pixelShaderShadow);
		LoadMaterialLinker("Data/Model/MagicShop/MatLink/MagicShop.matlink", "Data/Model/MagicShop/MatLink/Material/", "Data/Model/MagicShop/Tex/", &model);
		model.SetAll(Vector3(0.1, 1.5, 4.8), Vector3(-90, 180, 0), Vector3(0.11, 0.1, 0.11));
		////NPC������
		npc.Initialize(resource->Model("Christa.fbx"), vertexShaderAnim, pixelShader);
		//npc.InitializeSub(ModelType::SHADOW, vertexShaderAnimShadow, pixelShaderShadow);
		npc.SetAll(Vector3(-6.5, -3.9, 0), Vector3(0, 90, 0), Vector3(0.027f, 0.027f, 0.027f));
		npc.SetAllAnimeState(true, 1, 0.1);

		directionalLightParam.pos = Vector3(12, 25, -21) / 2.0f;
		directionalLightParam.target = Vector3(0, 0, 0);
		directionalLightParam.up = Vector3(0, 1, 0);
		directionalLightParam.fov = 45;
		directionalLightParam.vector = Vector3(1, 1, 1);
		directionalLightParam.color = Vector3(1, 1, 1);
		break;
	case StageNum::MAP:
		//�}�b�v
		model.Initialize(resource->Model("Map.fbx"), vertexShader, pixelShaderToon, nullptr, nullptr, nullptr, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
		model.InitializeSub(ModelType::SHADOW, vertexShaderShadow, pixelShaderShadow);
		
		model.SetAll(Vector3(0, -0.45, 0), Vector3(0, 90, 0), Vector3(1, 0.7, 1));

		directionalLightParam.pos = Vector3(12, 25, -21);
		directionalLightParam.target = Vector3(0, 0, 0);
		directionalLightParam.up = Vector3(0, 1, 0);
		directionalLightParam.fov = 45;
		directionalLightParam.vector = Vector3(1, 1, 1);
		directionalLightParam.color = Vector3(1, 1, 1);

		break;
	}
}
void Stage::ResetCollision() {

	while (boxCollider.size() > 0) {
		boxCollider.pop_back();
	}
	while (circleCollider.size() > 0) {
		circleCollider.pop_back();
	}
	while (boxColliderEvent.size() > 0) {
		boxColliderEvent.pop_back();
	}
	while (slopeCollider.size() > 0) {
		slopeCollider.pop_back();
	}

}
void Stage::MoveStage(int _stageNum) {
	ResetCollision();
	stageNumOld = stageNum;
	stageNum = _stageNum;


	Player::GetInstance()->MoveStage();

	SetStageCollisions();

	SetLights();
}
void Stage::SetCollisionSize(int _box, int _circle, int _event, int _slope) {
	boxColliderMax = _box;
	circleColliderMax = _circle;
	boxEventColliderMax = _event;
	slopeColliderMax = _slope;

	boxCollider.resize(_box);
    circleCollider.resize(_circle);
	boxColliderEvent.resize(_event);
	slopeCollider.resize(_slope);
}
void Stage::SetStageCollisions() {

	switch (stageNum) {
	case (int)StageNum::FLOWER_SHOP:

		SetCollisionSize(9, 1, 3, 0);

		////�Ԃ��蔻��p�̃R���C�_�[	
		boxCollider[0].Setup(Vector2(7, 0), Vector2(1, 32));//�E��
		boxCollider[1].Setup(Vector2(-10, 6.8), Vector2(21, 5));//������
		boxCollider[8].Setup(Vector2(0, 7.2), Vector2(32, 2));//���E��

		boxCollider[2].Setup(Vector2(-7, 0), Vector2(1, 32));//����
		boxCollider[3].Setup(Vector2(0, -4.75), Vector2(32, 1));//��O��
		boxCollider[4].Setup(Vector2(-2.75, 0.9), Vector2(5.5, 3.6));
		boxCollider[5].Setup(Vector2(5.6, 3.5), Vector2(7.6, 3));
		boxCollider[6].Setup(Vector2(4.4, -4.8), Vector2(4.4, 4.8));
		boxCollider[7].Setup(Vector2(7.5, -4.5), Vector2(4, 10.3));

		circleCollider[0].Setup(Vector2(5.5, -2.5), 0.5);

		////�C�x���g�p�̂Ԃ��蔻��̂Ȃ��R���C�_�[
		boxColliderEvent[0].Setup(Vector2(-2.4, -4.6), Vector2(4.8, 4.6),(int)EventNum::EXIT);//�o��	
		boxColliderEvent[1].Setup(Vector2(-11, 1), Vector2(11, 2),(int)EventNum::PLANTER);//�Ԓd
		boxColliderEvent[2].Setup(Vector2(4.95, 1.75), Vector2(3, 0.5), (int)EventNum::OPEN_SHOP);//�J�X

		break; 
	case (int)StageNum::HANDY_SHOP:
		SetCollisionSize(6, 1, 2, 0);
		boxCollider[0].Setup(Vector2(7, 0), Vector2(1, 32));
		boxCollider[1].Setup(Vector2(-8, 0), Vector2(1, 32));
		boxCollider[2].Setup(Vector2(0, -4.75), Vector2(32, 1));
		boxCollider[3].Setup(Vector2(0, 1.1), Vector2(32, 1.1));

		boxCollider[4].Setup(Vector2(-7.7, 0), Vector2(3, 4.6));
		boxCollider[5].Setup(Vector2(-6.2, 0), Vector2(3, 4));

		circleCollider[0].Setup(Vector2(6.2, 0.75), 1.0);

		boxColliderEvent[0].Setup(Vector2(0, 0.5), Vector2(3, 2),(int)EventNum::BUY_HANDYSHOP);//��������
		boxColliderEvent[1].Setup(Vector2(0, -5), Vector2(3, 5), (int)EventNum::EXIT);//�o��	
		break;
	case (int)StageNum::MAGIC_SHOP:
		SetCollisionSize(4, 2, 2, 0);
		boxCollider[0].Setup(-32, 32, 4, 10);
		boxCollider[1].Setup(-32, 32, -3, -10);
		
		boxCollider[2].Setup(-3.4, -32, -32, 32);
		boxCollider[3].Setup(5.25, 32, -32, 32);

		circleCollider[0].Setup(Vector2(-3.8, -2.5), 1.0);
		circleCollider[1].Setup(Vector2(3.65, 3.4), 2.5);

		boxColliderEvent[0].Setup(0.45, 4, -1.6, -4, (int)EventNum::EXIT);
		boxColliderEvent[1].Setup(-4, -2, 2.8, -0.5, (int)EventNum::BUY_MAGICSHOP);

		break;	
	case (int)StageNum::MAP:
		SetCollisionSize(6, 0, 3, 10);

		boxCollider[0].Setup(18.5, 30, -50, 50);
		boxCollider[1].Setup(-50, 50, -50, -8.85);
		boxCollider[2].Setup(-50, -12.5, -50, 1.7);
		boxCollider[3].Setup(-8.3, 14.5, -7.0, 1.7);
		
		boxCollider[4].Setup(-20, -30, -50, 50);
		boxCollider[5].Setup(-50, 50, 3, 50);

	
		//�C�x���g
		boxColliderEvent[0].Setup(3.5, 6, -6, -7.5, (int)EventNum::JOIN_FLOWERSHOP);
		boxColliderEvent[1].Setup(-14.5, -11.5, 2.7, 50, (int)EventNum::JOIN_MAGICSHOP);
		boxColliderEvent[2].Setup(  5.6,  10.6, 2.7, 50, (int)EventNum::JOIN_HANDYSHOP);

		float bottom = -3.9f;
		float top = 1.47f;
		//�K�i
		slopeCollider[0].Setup(-14, -8.2, -6.4, 0.24, bottom, top, SimpleVector::UP);
		slopeCollider[1].Setup( 14.44, 18.6, -6.4, 0.24, bottom, top, SimpleVector::UP);
		
		float bridgeBottomDist = 2.05;
		float bridgeTopDist = 2.5;
		//��
		slopeCollider[2].Setup(-8.5, -6.0, -10, -5, bottom					 , bottom + bridgeBottomDist, SimpleVector::RIGHT);
		slopeCollider[3].Setup(-6.0, -3.5, -10, -5, bottom + bridgeBottomDist, bottom + bridgeTopDist   , SimpleVector::RIGHT);
		slopeCollider[4].Setup( 1.5, -1.0, -10, -5, bottom					 , bottom + bridgeBottomDist, SimpleVector::LEFT);
		slopeCollider[5].Setup(-1.0, -3.5, -10, -5, bottom + bridgeBottomDist, bottom + bridgeTopDist   , SimpleVector::LEFT);	

		slopeCollider[6].Setup(-8.5, -6.0,  1.0, 6, top						 , top    + bridgeBottomDist, SimpleVector::RIGHT);
		slopeCollider[7].Setup(-6.0, -3.5,  1.0, 6, top    + bridgeBottomDist, top    + bridgeTopDist	, SimpleVector::RIGHT);
		slopeCollider[8].Setup( 1.5, -1.0,  1.0, 6, top	   					 , top    + bridgeBottomDist, SimpleVector::LEFT);
		slopeCollider[9].Setup(-1.0, -3.5,  1.0, 6, top    + bridgeBottomDist, top    + bridgeTopDist	, SimpleVector::LEFT);

		break;
	}

}
void Stage::SetLights() {


	light.SetDirectionalLight(directionalLightParam);
	for (int i = 0; i < POINT_LIGHT; i++) {
		light.SetPointLight(i,pointLightParam[i]);
	}
	for (int i = 0; i < SPOT_LIGHT; i++) {
		light.SetSpotLight(i, spotLightParam[i]);
	}
	light.SetAmbientLight(0.75);
}
void Stage::Update() {
	auto player = Player::GetInstance();
	if (stageNum == (int)StageNum::MAP) {
		directionalLightParam.pos = Vector3(12, 25, -21) + player->GetPos();
		directionalLightParam.target = Vector3(0, 0, 0) + player->GetPos();
		directionalLightParam.up = Vector3(0, 1, 0);
		directionalLightParam.fov = 45;
		directionalLightParam.vector = Vector3(1, 1, 1);
		directionalLightParam.color = Vector3(1, 1, 1);
	}

	SetLights();

	light.Update();

}
void Stage::DrawShadow() {
	//�w�i�̉e
	model.Draw(ModelType::SHADOW);
}
void Stage::Draw() {
	auto item = Item::GetInstance();
	skyModel.SetAll(Vector3(0, 0, 0), Vector3(-90, 0, 0), Vector3(100, 100, 100));
	skyModel.Draw();

	//�w�i
	model.ModelObject::Draw();

	//�}�b�v���Ƃ̌ŗL�̃I�u�W�F�N�g
	switch (stageNum) {
	case (int)StageNum::FLOWER_SHOP:
		for (int i = 0; i < item->GetItemNum("�}�W�b�N�v�����^�["); i++) {
			planterModel[i].Draw();		
		}
		break;
	case (int)StageNum::HANDY_SHOP:
	case (int)StageNum::MAGIC_SHOP:
		if (npc.GetMeshNum() != 0) {
			npc.ModelObject::Draw();
		}
		break;
	case (int)StageNum::MAP:
		waterModel.SetAll(Vector3(0, -8, 0), Vector3(80 + 180, 0, 0), Vector3(50, 50, 50));
		waterModel.Draw();
		break;
	}

}
