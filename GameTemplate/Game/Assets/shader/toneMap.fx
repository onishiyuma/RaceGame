//スプライトの定数バッファ
cbuffer cb : register(b0)
{
    float4x4 mvp; //MVP行列
    float4 mulColor; //乗算カラー
};

//頂点シェーダーの入力
struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

//ピクセルシェーダーの入力
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D<float4> g_sceneTexture : register(t0);
Texture2D<float4> g_depthTexture : register(t1);
Texture2D<float4> g_normalTexture : register(t2);

sampler g_sampler : register(s0);

//定数
static const float CameraNear = 0.1f; //カメラの近平面
static const float CameraFar = 1000000000.0f; //カメラの遠平面
static const float ToonSteps = 5.0f;//トゥーン調にする際の色の階調数
static const float2 TexelSize = { 1.0 / 1600.0, 1.0 / 900.0 };//1ピクセル当たりのUVサイズ
static const float DepthThreshold = 0.00009;//深度エッジを判定する閾値
static const float NormalThreshold = 5.0f;//法線エッジを判定する閾値

//関数
float CalculateEdge(float2 uv);
float LinearizeDepth(float rawDepth);

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
    //元のシーンカラーを取得
    float4 baseColor = g_sceneTexture.Sample(g_sampler, In.uv);
    
    //深度値
    float depth = g_depthTexture.Sample(g_sampler, In.uv).r;
    
    //深度値が0.9999以下の場合はトゥーン調にせず元の色を返す（遠くのオブジェクトはトゥーン調にしない）
    if (depth <= 0.9999f)
    {
        return baseColor;
    }
    
    //トゥーン調にするために色を階調化
    float3 toonColor = floor(baseColor.rgb * ToonSteps) / ToonSteps;
    
    //エッジの計算
    float edgeLine = CalculateEdge(In.uv);
    
    //最終合成
    return float4(toonColor * edgeLine, baseColor.a);
}

//深度値の線形化
//深度バッファに保存されている非線形な深度値（Z値）を、計算しやすいように実際の距離（線形）に変換する
float LinearizeDepth(float rawDepth)
{
    return (CameraNear * CameraFar) / (CameraFar - rawDepth * (CameraFar - CameraNear));
}

//エッジ検出計算
float CalculateEdge(float2 uv)
{
    // 3x3ピクセルのUVオフセット
    float2 offsets[9] =
    {
        float2(-1, -1), float2(0, -1), float2(1, -1),
        float2(-1, 0), float2(0, 0), float2(1, 0),
        float2(-1, 1), float2(0, 1), float2(1, 1)
    };

    // 周囲9ピクセルの深度と法線を取得
    float depth[9];
    float3 normal[9];
    for (int i = 0; i < 9; i++)
    {
        //1ピクセル分ずらしたUV座標を計算
        float2 sampleUV = uv + (offsets[i] * TexelSize);
        
        //深度をサンプリングして線形化
        float rawDepth = g_depthTexture.Sample(g_sampler, sampleUV).r;
        depth[i] = LinearizeDepth(rawDepth);
        
        //法線をサンプリング
        normal[i] = g_normalTexture.Sample(g_sampler, sampleUV).rgb;
    }

    //Sobelフィルタのカーネル（重み）
    //X方向（左右の差）
    float kernelX[9] =
    {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    //Y方向（上下の差）
    float kernelY[9] =
    {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1
    };

    //差分の合計を計算
    float depthDeltaX = 0, depthDeltaY = 0;
    float3 normalDeltaX = 0, normalDeltaY = 0;

    for (int j = 0; j < 9; j++)
    {
        depthDeltaX += depth[j] * kernelX[j];
        depthDeltaY += depth[j] * kernelY[j];
        
        normalDeltaX += normal[j] * kernelX[j];
        normalDeltaY += normal[j] * kernelY[j];
    }

    //X方向とY方向の差分の大きさを合成（ピタゴラスの定理）
    float edgeDepth = sqrt(depthDeltaX * depthDeltaX + depthDeltaY * depthDeltaY);
    float edgeNormal = sqrt(dot(normalDeltaX, normalDeltaX) + dot(normalDeltaY, normalDeltaY));

    //閾値を超えたら「エッジ（線）」とみなす
    //step(a, x) は x >= a のとき 1.0、そうでないとき 0.0 を返す
    float isDepthEdge = step(DepthThreshold, edgeDepth);
    float isNormalEdge = step(NormalThreshold, edgeNormal);

    //深度か法線のどちらかがエッジであれば 1.0 になる
    float isEdge = saturate(isDepthEdge + isNormalEdge);

    //線を暗く描画するため、エッジ部分を 0.0、平らな部分を 1.0 に反転して返す
    return 1.0 - isEdge;
}