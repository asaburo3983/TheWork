#include "ConstantBuffer.hlsli"

float4 main(VS_OUT input) : SV_TARGET{

   //カラーをCPU側で決めれるようにする
   return float4(0,0,0,1.0f);
}