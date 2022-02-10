#pragma once

#include "LowApplication.h"

#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl/client.h>
#include <wrl/event.h>

#include <d3d12.h>
#include <dxgi1_6.h>

#include <DirectXMath.h>
#include <DirectXColors.h>

#include "d3dx12.h"

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <stdio.h>

// To use graphics and CPU markup events with the latest version of PIX, change this to include <pix3.h>
// then add the NuGet package WinPixEventRuntime to the project.
#include <pix.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include "Audio.h"
#include "CommonStates.h"
#include "DirectXHelpers.h"
#include "WICTextureLoader.h"
#include "DescriptorHeap.h"
#include "GraphicsMemory.h"
#include "ResourceUploadBatch.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h" 

#include <mfplay.h>
#pragma comment(lib,"mfplay.lib")

#pragma comment(lib, "DirectXTK12.lib")

namespace dxtk {

	//定義
	class ManagerDXTK {
	public:
		static ManagerDXTK* GetInstance()
		{
			static ManagerDXTK def;
			return &def;
		}
		void Init();//初期化処理
		void UnInit();//終了処理
		void DrawFinish();
		void SetImageEX(float ex = 1.0f);
		float GetImageEX();
	public:
		std::unique_ptr<DirectX::SpriteBatch> sprites;
		std::unique_ptr<DirectX::CommonStates> commonStates;
		std::unique_ptr<DirectX::GraphicsMemory> graphicsMemory;

	private:
		float imageExtend = 1;

	};

	class Image2D {
	private:
		
		UINT width;
		UINT height;
		Microsoft::WRL::ComPtr<ID3D12Resource> texture;
		std::unique_ptr<DirectX::DescriptorHeap> resourceDescriptors;
	public:
		void Load(const char* filepath);
		void Draw(int _x=0, int _y=0, float _scaleX=1, float _scaleY=1, float _angle=0,float _alpha=1.0f, bool _turnLR=false, bool _turnUD=false, bool _center=true);
		UINT GetWidth();
		UINT GetHeight();
		void SetResource(Microsoft::WRL::ComPtr<ID3D12Resource> resource);
	};

	class Sound_MP3 {
	private:
		Microsoft::WRL::ComPtr<IMFPMediaPlayer> mediaPlayer = nullptr;//音楽用の変数
		MFP_MEDIAPLAYER_STATE mediaPlayerState = MFP_MEDIAPLAYER_STATE_EMPTY;

	public:
		Sound_MP3() {};
		Sound_MP3(const wchar_t* filePath);
		void Load(const wchar_t* filePath); //こっちのほうがchar型のロードより処理がちょっと早い
		void Load(const char* filePath);
		void Play(bool loop = true);
		void Stop();
		void Volume(float vol = 1.0f);

	};
}

