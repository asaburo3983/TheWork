#include "Camera.h"

using namespace frauEngine;

Camera::Camera() {
	cameraBufferHeap.Create();
}
Camera::Camera(Vector3 _pos, Vector3 _target, Vector3 _up, float _fov) {
	SetCamera(_pos, _target, _up, _fov);
	cameraBufferHeap.Create();
}
void Camera::SetCamera(Vector3 _pos, Vector3 _target, Vector3 _up,float _fov) {

	SetPos(_pos);
	SetTarget(_target);
	SetUp(_up);
	SetFov(_fov);

	//セット時には必ず１度アップデートをはさむ
	Update();
}
void Camera::SetPos(Vector3 _pos) {
	pos = GetXMFLOAT3FromVector3(_pos);
}
void Camera::SetTarget(Vector3 _target) {
	target = GetXMFLOAT3FromVector3(_target);
}
void Camera::SetUp(Vector3 _up) {
	up = GetXMFLOAT3FromVector3(_up);
}
void Camera::SetFov(float _fov) {
	fov = DirectX::XMConvertToRadians(_fov);
}

Vector3 Camera::GetPos() {
	return GetVector3FromXMFLOAT3(pos);
}
Vector3 Camera::GetTarget() {
	return GetVector3FromXMFLOAT3(target);
}
Vector3 Camera::GetUp() {
	return GetVector3FromXMFLOAT3(up);
}
float Camera::GetFov() {
	return XMConvertToDegrees(fov);
}
Vector3 Camera::GetScreenPos(Vector3 _pos) {

	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(_pos.X, _pos.Y, _pos.Z);
	DirectX::XMMATRIX ret = translate * cameraBufferHeap.buffer->view * cameraBufferHeap.buffer->projection;
	
	return Vector3(0, 0, 0);
}
DirectX::XMMATRIX Camera::GetView() {
	return cameraBufferHeap.buffer->view;
}
DirectX::XMMATRIX Camera::GetProjection() {
	return cameraBufferHeap.buffer->projection;
}
DirectX::XMFLOAT3 Camera::GetCameraPos() {
	return pos;
}
CameraBufferHeap* Camera::GetConstantBuffer() {
	return &cameraBufferHeap;
}
void Camera::Update() {
	
	//定数バッファのデータ更新
	cameraBufferHeap.buffer->pos.x = pos.x;
	cameraBufferHeap.buffer->pos.y = pos.y;
	cameraBufferHeap.buffer->pos.z = pos.z;


	cameraBufferHeap.buffer->view = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
	cameraBufferHeap.buffer->projection = XMMatrixPerspectiveFovLH(fov,
		static_cast<float>(LowApplication::GetInstance()->GetWindowSizeX()) / static_cast<float>(LowApplication::GetInstance()->GetWindowSizeY()),//アス比
		0.1f,//近い方
		1000.0f//遠い方
	);
}