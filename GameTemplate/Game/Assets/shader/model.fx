////////////////////////////////
/// モデルシェーダー
////////////////////////////////

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーの入力
struct SVSIn{
	float4 pos : POSITION;		//位置
    float3 normal : NORMAL;		//法線
	float2 uv 	: TEXCOORD0;	//UV
    float3 tangent : TANGENT;   //接ベクトル
    float3 biNormal : BINORMAL; //従ベクトル
	SSkinVSIn skinVert;			//スキン用のデータ
};
//ピクセルシェーダーの入力
struct SPSIn{
	float4 pos : SV_POSITION;	        //位置
    float3 normal : NORMAL;		        //法線
	float2 uv : TEXCOORD0;	            //UV
    float3 tangent : TANGENT;           //接ベクトル
    float3 biNormal : BINORMAL;         //従ベクトル
    float3 worldPos : TEXCOORD1;        //ワールド空間座標
    float3 normalInView : TEXCOORD2;    //カメラ空間の法線
    float4 posInLVP : TEXCOORD3;        //ライトビュースクリーン空間でのピクセルの座標
    float3 depthInView : TEXCOORD4;     //カメラ空間でのピクセルの深度
};

struct SPSOut{
    float4 color : SV_Target0;
    float depth : SV_Target1;
    float3 normal : SV_Target2;
};

//ディレクションライト
struct DirectionLight
{
    float3 direction; //ライトの方向
    float3 color;     //ライトのカラー
};

//ポイントライト
struct PointLight
{
    float3 position; //ライトの地
    float3 color;    //ライトのカラー
    float range;     //ライトの影響範囲
};

//スポットライト
struct SpotLight
{
    float3 position;    //ライトの位置
    float3 color;       //ライトのカラー
    float range;        //ライトの影響範囲
    float3 direction;   //ライトの放射方向
    float angle;        //ライトの放射角度
};

//半球ライト
struct HemLight
{
    float3 groundColor;     //地面色
    float3 skyColor;        //天球色
    float3 groundNormal;    //地面の法線
};

//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;        //ワールド行列
    float4x4 mView;         //ビュー行列
    float4x4 mProj;         //プロジェクション行列
};

//ライトの定数バッファ
cbuffer LightCb : register(b1)
{
    DirectionLight directionLight;  //ディレクションライト
    float3 eyePos;                  //視点の位置
    float3 ambientLig;              //環境光
    PointLight pointLight[10];      //ポイントライト
    SpotLight spotLight[10];        //スポットライト
    HemLight hemLight;              //半球ライト
    float4x4 mLVP;                  //ライトビュースクリーン
    float3 lightPos;                //ライトの位置
};

////////////////////////////////////////////////
// 繧ｰ繝ｭ繝ｼ繝舌Ν螟画焚縲・
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap :register(t1);            //法線マップ
Texture2D<float4> g_specularMap : register(t2);         //スペキュラマップ
//Texture2D<float4> g_aoMap : register(t10);            //AOマップ
Texture2D<float4> g_shadowMap : register(t10);          //シャドウマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列
sampler g_sampler : register(s0);	                    //サンプラー

////////////////////////////////////////////////
// 髢｢謨ｰ螳夂ｾｩ縲・
////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal,float specular);
float3 CalcLigFromDirectionLight(SPSIn psIn, float3 normal, float specular);
float3 CalcLigFromPointLight(SPSIn psIn, int num, float3 normal, float specular);
float3 CalcLigFromSpotLight(SPSIn psIn, int num, float3 normal, float specular);
float3 CalcLimPower(SPSIn psIn);
float3 CalcLigFromHemLight(SPSIn psIn);
float3 CalcNormalMap(SPSIn psIn);
float CalcSpecularMap(SPSIn psIn);
//float3 CalcAoMap(SPSIn psIn);
float CalcShadowMap(SPSIn psIn);

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    float4 worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
    psIn.depthInView = psIn.pos.z;
	psIn.pos = mul(mProj, psIn.pos);

    psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	psIn.uv = vsIn.uv;
    
    psIn.normalInView = mul(mView, psIn.normal);
    
    psIn.posInLVP = mul(mLVP, worldPos);
    psIn.posInLVP.z = length(worldPos.xyz - lightPos) / 1000.0f;
    
	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
SPSOut PSMain(SPSIn psIn, int isShadowReceiver) : SV_Target0
{
    SPSOut psOut;
    
    //法線マップの計算
    float3 normalMap = CalcNormalMap(psIn);
    
    //スペキュラマップの計算
    float specularMap = CalcSpecularMap(psIn);
   
    //ディレクションライトの計算
    float3 directionLig = CalcLigFromDirectionLight(psIn, normalMap, specularMap);
    
    //複数個のライティング計算
    float3 pointLig[10];
    float3 spotLig[10];
    for (int i = 0; i < 10; i++)
    {
        //ポイントライトの計算
        pointLig[i] = CalcLigFromPointLight(psIn, i, normalMap, specularMap);
        //スポットライトの計算
        spotLig[i] = CalcLigFromSpotLight(psIn, i, normalMap, specularMap);
    }
    
    //リムライトの計算
    float3 limLig = CalcLimPower(psIn);
    
    //半球ライトの計算
    float3 hemLig = CalcLigFromHemLight(psIn);
     
    ////AOマップの計算
    //float3 aoMap = CalcAoMap(psIn);
    
	//最終的な光を求める
    //ディレクションライト + 環境光 + 半球ライト
    float3 lig = directionLig + ambientLig + hemLig;
    
    for (int j = 0; j < 10; j++)
    {
        lig += pointLig[j];
        lig += spotLig[j];
    }
    
    lig += limLig;
    
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
    
    psOut.depth = psIn.depthInView;
    psOut.normal = normalize(mul(mView, normalMap));
	
    ///最終出力カラーに光を乗算する
    albedoColor.xyz *= lig;
    
    float shadow = 1.0f;
    //影を受けるなら
    if(isShadowReceiver==1)
    {
        //シャドウマップ
        shadow = CalcShadowMap(psIn);
    }
    
    //最終出力カラーに影を乗算する
    albedoColor.xyz *= shadow;
    psOut.color = albedoColor;
	
	return psOut;
}

SPSOut PSShadowReceiverMain(SPSIn psIn) : SV_Target0
{
    return PSMain(psIn, 1);
}

SPSOut PSNormalMain(SPSIn psIn) : SV_Target0
{
    return PSMain(psIn, 0);
}

//拡散反射光
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{    
    //拡散反射光
    return max(0.0f, dot(normal, -lightDirection)) * lightColor;
}

//鏡面反射光
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal,float specular)
{
    //反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);
	
    //光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);
	
    //鏡面反射の強さを求める
    float t = saturate(dot(refVec, toEye));
    //鏡面反射の強さを0以上の数値にする
    if (t < 0.0f)
    {
        t = 0.0f;
    }
	
    //鏡面反射の強さを絞る
    t = pow(t, 5.0f);
    
    float3 specLig = lightColor * t;
    specLig *= specular * 10.0f;
	
    //鏡面反射光
    return specLig;
}

//ディレクションライト
float3 CalcLigFromDirectionLight(SPSIn psIn, float3 normal, float specular)
{
    //拡散反射光を求める
    float3 diffuseLig = CalcLambertDiffuse(directionLight.direction, directionLight.color, normal);
    
    //スポットライトを求める
    float3 specularLig = CalcPhongSpecular(directionLight.direction, directionLight.color, psIn.worldPos, normal, specular);
 
    //最終的な光
    return diffuseLig + specularLig;
}

//ポイントライト
float3 CalcLigFromPointLight(SPSIn psIn, int num, float3 normal, float specular)
{
    //このサーフェイスに入射しているポイントライトの光の向きを計算する
    float3 ligDir = psIn.worldPos - pointLight[num].position;
    //正規化
    ligDir = normalize(ligDir);
    
    //減衰なしの拡散反射光を計算する
    float3 diffusePoint = CalcLambertDiffuse(ligDir, pointLight[num].color, normal);
    
    //減衰なしの鏡面反射光を計算する
    float3 specularPoint = CalcPhongSpecular(ligDir, pointLight[num].color, psIn.worldPos, normal, specular);
    
    //ポイントライトとの距離を計算する
    float distance = length(psIn.worldPos - pointLight[num].position);
    
    //影響率は距離に比例して小さくなっていく
    float affect = 1.0f - 1.0f / pointLight[num].range * distance;
    
    //影響力がマイナスにならないように補正をかける
    if(affect<0.0f)
    {
        affect = 0.0f;
    }
    
    //影響を指数関数的にする
    affect = pow(affect, 3.0f);
    
    //拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
    diffusePoint *= affect;
    specularPoint *= affect;
    
    //最終的な光
    return diffusePoint + specularPoint;
}

//スポットライト
float3 CalcLigFromSpotLight(SPSIn psIn, int num, float3 normal, float specular)
{
    //サーフェイスに入射するスポットライトの光の向きを計算する
    float3 ligDir = psIn.worldPos - spotLight[num].position;
    //正規化
    ligDir = normalize(ligDir);
    
    //減衰なしの拡散反射光を計算する
    float3 diffuseSpot = CalcLambertDiffuse(ligDir, spotLight[num].color, normal);
    
    //減衰なしの鏡面反射光を計算する
    float3 specularSpot = CalcPhongSpecular(ligDir, spotLight[num].color, psIn.worldPos, normal, specular);
    
    //スポットライトとの距離を計算する
    float distance = length(psIn.worldPos - spotLight[num].position);
    
    //影響率は距離に比例して小さくなっていく
    float affect = 1.0f - 1.0f / spotLight[num].range * distance;
    
    //影響力がマイナスにならないように補正をかける
    if(affect<0.0f)
    {
        affect = 0.0f;
    }
    
    //影響を指数関数的にする
    affect = pow(affect, 3.0f);
    
    //拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
    diffuseSpot *= affect;
    specularSpot *= affect;
    
    //入射光と射出方向の角度を求める
    float angle = dot(ligDir, spotLight[num].direction);
    //dot()で求めた値をacos()に渡して角度を求める
    angle = abs(acos(angle));
    
    //角度に比例して小さくなっていく影響率を計算する
    affect = 1.0f - 1.0f / spotLight[num].angle * angle;
    
    //影響力がマイナスにならないように補正をかける
    if(affect<0.0f)
    {
        affect = 0.0f;
    }
    
    //影響を指数関数的にする
    affect = pow(affect, 0.5f);
    
    //角度による影響率を反射光に乗算して、影響を弱める
    diffuseSpot *= affect;
    specularSpot *= affect;
    
    //最終的な光
    return diffuseSpot + specularSpot;
}

//リムライト
float3 CalcLimPower(SPSIn psIn)
{
    //サーフェイスの法線と光の入射方向に依存するリムの強さを求める
    float power1 = 1.0f - max(0.0f, dot(directionLight.direction, psIn.normal));
    
    //サーフェイスの法線と視線の方向に依存するリムの強さを求める
    float power2 = 1.0f - max(0.0f, psIn.normalInView.z * -1.0f);
    
    //最終的なリムの強さを求める
    float limPower = power1 * power2;
    
    //pow()を使用し強さの変化を指数関数的にする
    limPower = pow(limPower, 1.3f);
    
    //最終的な光
    return limPower * directionLight.color;
}

//半球ライト
float3 CalcLigFromHemLight(SPSIn psIn)
{
    //サーフェイスの法線と地面の法線との内積を計算する
    float t = dot(psIn.normal, hemLight.groundNormal);
    
    //内積の結果を0～1の範囲に変換
    t = (t + 1.0f) / 2.0f;
    
    //最終的な光
    return lerp(hemLight.groundColor, hemLight.skyColor, t);
}

//法線マップ
float3 CalcNormalMap(SPSIn psIn)
{
    float3 normal = psIn.normal;   
    //法線マップからタンジェントスペースの法線をサンプリングする
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    //タンジェントスペースの法線を0～1の範囲から-1～1の範囲に復元する
    localNormal = (localNormal - 0.5f) * 2.0f;
    
    //タンジェントスペースの法線をワールドスペースに変換する
    normal = psIn.tangent * localNormal.x
           + psIn.biNormal * localNormal.y
           + normal * localNormal.z;
    
    //法線マップ
    return normal;
}

//スペキュラマップ
float CalcSpecularMap(SPSIn psIn)
{    
    //スペキュラマップからスペキュラ反射の強さをサンプリング
    float specPower = g_specularMap.Sample(g_sampler, psIn.uv).r;
    
    //スペキュラマップ
    return specPower;
}

////AOマップの計算
//float3 CalcAoMap(SPSIn psIn)
//{
//    //環境光を計算
//    float3 ambient = ambientLig;
    
//    //AOマップから環境光の強さをサンプリング
//    float ambientPower = g_aoMap.Sample(g_sampler, psIn.uv);
    
//    //環境光の強さを環境光に乗算する
//    ambient *= ambientPower;
    
//    //AOマップ
//    return ambient;
//}

//シャドウマップ
float CalcShadowMap(SPSIn psIn)
{
    // ライトビュースクリーン空間からUV空間に座標変換
    float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    float4 shadowMap = 1.0f;
    //ライトビュースクリーン空間でのZ値を計算する
    float zInLVP = psIn.posInLVP.z;
    if(shadowMapUV.x>0.0f&&shadowMapUV.x<1.0f
        && shadowMapUV.y>0.0f&&shadowMapUV.y<1.0f)
    {
        //シャドウマップから値をサンプリング
        float2 shadowValue = g_shadowMap.Sample(g_sampler, shadowMapUV).xy;
        
        //まずこのピクセルが遮蔽されているか調べる。これは通常のデプスシャドウと同じ
        if (zInLVP > shadowValue.r && zInLVP <= 1.0f)
        {
            //遮蔽されているなら、チェビシェフの不等式を利用して光が当たる確率を求める
            float depth_sq = shadowValue.x * shadowValue.x;

            //このグループの分散具合を求める
            //分散が大きいほど、varianceの数値は大きくなる
            float variance = min(max(shadowValue.y - depth_sq, 0.0001f), 1.0f);

            //このピクセルのライトから見た深度値とシャドウマップの平均の深度値の差を求める
            float md = zInLVP - shadowValue.x;

            //光が届く確率を求める
            float lit_factor = variance / (variance + md * md);

            //シャドウカラーを求める
            float3 shadowColor = shadowMap.xyz * 0.5f;

            //光が当たる確率を使って通常カラーとシャドウカラーを線形補完
            shadowMap.xyz = lerp(shadowColor, shadowMap.xyz, lit_factor);
        }
    }
    
    //シャドウマップ
    return shadowMap;
}