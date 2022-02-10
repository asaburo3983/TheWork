#include "ImGuiDX12.h"

void frauEngine::ManagerImGui::CreateSRVHeap() {
    auto lowApp = LowApplication::GetInstance();

    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = 1;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//デプスステンシルビューとして使う
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

   auto result= lowApp->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvDescHeap));
}
void frauEngine::ManagerImGui::Init() {

    CreateSRVHeap();
   auto lowApp= LowApplication::GetInstance();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("Data/Font/BIZ-UDGothicR.ttc", 15.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

    ImGui::StyleColorsDark();

    bool result =ImGui_ImplWin32_Init(*lowApp->GetHWND());
    result =  ImGui_ImplDX12_Init(lowApp->GetDevice(), NUM_FRAMES_IN_FLIGHT,
        DXGI_FORMAT_R8G8B8A8_UNORM, 
        srvDescHeap,
        srvDescHeap->GetCPUDescriptorHandleForHeapStart(),
        srvDescHeap->GetGPUDescriptorHandleForHeapStart());

}
void frauEngine::ManagerImGui::UnInit() {
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
void frauEngine::ManagerImGui::DrawStart() {
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}
void frauEngine::ManagerImGui::DrawFinish() {
    auto lowApp = LowApplication::GetInstance();
    ImGui::Render();
    lowApp->GetCommandList()->SetDescriptorHeaps(1, &srvDescHeap);

    //ビューポートの読み込みがおかしい
    //リファクタリング行ったせいである
    //
    ImDrawData* a = ImGui::GetDrawData();

    ImGui_ImplDX12_RenderDrawData(a, lowApp->GetCommandList());

    //ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), lowApp->GetCommandList());
}

void frauEngine::ManagerImGui::Test() {

    auto lowApp = LowApplication::GetInstance();

   



    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::SetNextWindowPos(ImVec2(100,100), 1);
        ImGui::SetNextWindowSize(ImVec2(1,1), 1);

        ImGui::Begin("Hello, world!");                          //ウィンドウ名になる

        ImGui::Text("This is some useful text.");               //テキストが描画される

        if (ImGui::Button("Button"))                            // 押されたとき処理
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

    }


}