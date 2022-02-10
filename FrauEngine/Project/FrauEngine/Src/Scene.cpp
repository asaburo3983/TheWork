#include "Scene.h"

using namespace frauEngine;


void Scene::Init() {
	key = KeyInput::GetInstance();
	mouse = MouseInput::GetInstance();
	resource = Resource::GetInstance();
}
void Scene::StartUpdate() {

}
void Scene::EndUpdate() {

}

void Scene::SetObjectList(Object* _object, const char* _name) {
	_object->SetName(_name);
	objectList.push_back(_object);
}
void Scene::DrawObjectList() {

	ImGui::Begin("ObjectList");

	ImGui::Text("Enable ImGui");
	for (int i = 0; i < objectList.size(); i++) {
		ImGui::Checkbox(objectList[i]->GetName(), objectList[i]->GetImGuiEnable());//ƒu[ƒ‹‚ğØ‚è‘Ö‚¦‚ê‚é
	}

	ImGui::End();
}
void Scene::SetStaticLoadOFF() {
	staticLoad = false;
}

void Scene::LoadScene(std::string _str) {
	loadScene = true;
	nextScene = _str;
	
}