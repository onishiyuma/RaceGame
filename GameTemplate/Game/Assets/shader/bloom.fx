//スプライトの定数バッファ
cbuffer cb : register(b0)
{
    float4x4 mvp;       //MVP行列
    float4 mulColor;    //乗算カラー
};

//頂点シェーダーの入力
struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD;
};

//ピクセルシェーダーの入力
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
};

Texture2D<float4> g_sceneTexture : register(t0);
Texture2D<float4> g_bokeTexture_0 : register(t1);
Texture2D<float4> g_bokeTexture_1 : register(t2);
Texture2D<float4> g_bokeTexture_2 : register(t3);
Texture2D<float4> g_bokeTexture_3 : register(t4);
sampler g_sampler : register(s0);

//頂点シェーダー
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

//ピクセルシェーダー
float4 PSMain(PSInput In) : SV_Target0
{
    float4 color = g_sceneTexture.Sample(g_sampler, In.uv);
    
    float y = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
    color.r = y;
    color.g = y;
    color.b = y;
    
    return color;
}

//輝度抽出
float4 PSLuminance(PSInput In) : SV_Target0
{
    float4 color = g_sceneTexture.Sample(g_sampler, In.uv);
    
    float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    
    clip(t - 1.0f);
    return color;
}

//ボケ画像の合成
float4 PSBloomFinal(PSInput In) : SV_Target0
{
    float4 combineColor = g_bokeTexture_0.Sample(g_sampler, In.uv);
    combineColor += g_bokeTexture_1.Sample(g_sampler, In.uv);
    combineColor += g_bokeTexture_2.Sample(g_sampler, In.uv);
    combineColor += g_bokeTexture_3.Sample(g_sampler, In.uv);
    combineColor /= 4.0f;
    combineColor.a = 1.0f;
    return combineColor;
}