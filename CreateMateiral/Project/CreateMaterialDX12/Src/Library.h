#pragma once

//����P�Ƃŉғ�����w�b�_�̏W��
#include<Windows.h>
#include<tchar.h>
#include <uchar.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<vector>
#include<map>
#include <unordered_map>
#include<d3dcompiler.h>
#include<DirectXTex.h>
#include<d3dx12.h>
#include<dxgidebug.h>
#include <codecvt>

#include <wrl.h>
#include <thread>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <string.h>
#include <pix.h>
#include <direct.h>
#include <atlstr.h>
#ifdef _DEBUG
#include<iostream>
#endif


//FBXSDK�֘A
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-mt.lib")
#pragma comment(lib, "zlib-mt.lib")
#pragma comment(lib, "libxml2-mt.lib")

//DX12&DXTK�֘A
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")


//�G�t�F�N�V�A�֘A
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#pragma comment(lib,"EffekseerRendererDX12.lib")
#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"LLGI.lib")

//ImGui�֘A
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_impl_dx12.h"

//�֗��@�\�܂Ƃ� �P�ƁA�������́��̃��C�u�����œ������̂����̋L�q
#include "Utility.h"
#include "input.h" 

