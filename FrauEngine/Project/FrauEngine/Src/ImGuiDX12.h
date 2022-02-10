#pragma once
#include "LowApplication.h"

namespace frauEngine {
	struct FrameContext
	{
		ID3D12CommandAllocator* CommandAllocator;
		UINT64                  FenceValue;
	};
	//’è‹`
	class ManagerImGui {
	private:

		const int NUM_FRAMES_IN_FLIGHT = 3;
		FrameContext                 frameContext[3];
		UINT                         frameIndex = 0;
		ID3D12DescriptorHeap* srvDescHeap = NULL;

		void CreateSRVHeap();
	public:
		static ManagerImGui* GetInstance()
		{
			static ManagerImGui def;
			return &def;
		}
		void Init();
		void UnInit();

		void DrawStart();
		void DrawFinish();

		void Test();
	};
}