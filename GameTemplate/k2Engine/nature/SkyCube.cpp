#include "k2EnginePreCompile.h"
#include "SkyCube.h"

namespace nsK2Engine
{
	//コンストラクタ
	SkyCube::SkyCube()
	{
		m_textureFilePaths[enSkyCubeType_Day] = L"Assets/modelData/preset/skyCubeMap.dds";							//昼間
		m_textureFilePaths[enSkyCubeType_Night] = L"Assets/modelData/preset/skyCubeMapNight.dds";					//夜間
		m_textureFilePaths[enSkyCubeType_Snow] = L"Assets/modelData/preset/skyCubeMapSnow.dds";						//雪山
		m_textureFilePaths[enSkyCubeType_Snow_2] = L"Assets/modelData/preset/skyCubeMapSnow_2.dds";					//雪山2
		m_textureFilePaths[enSkyCubeType_Wild] = L"Assets/modelData/preset/skyCubeMapWild.dds";						//荒野
		m_textureFilePaths[enSkyCubeType_Wild_2] = L"Assets/modelData/preset/skyCubeMapWild_2.dds";					//荒野2
		m_textureFilePaths[enSkyCubeType_Wild_Night] = L"Assets/modelData/preset/skyCubeMapWild_Night.dds";			//荒野(夜間)
		m_textureFilePaths[enSkyCubeType_Grass] = L"Assets/modelData/preset/skyCubeMapGrass.dds";					//芝生
		m_textureFilePaths[enSkyCubeType_Euro] = L"Assets/modelData/preset/skyCubeMapEuro.dds";						//欧州
		m_textureFilePaths[enSkyCubeType_DayToon] = L"Assets/modelData/preset/skyCubeMapDay_Toon.dds";				//昼間(トゥーン調)
		m_textureFilePaths[enSkyCubeType_DayToon_2] = L"Assets/modelData/preset/skyCubeMapDay_Toon_02.dds";			//昼間(トゥーン調)2
		m_textureFilePaths[enSkyCubeType_DayToon_3] = L"Assets/modelData/preset/skyCubeMapDay_Toon_03.dds";			//昼間(トゥーン調)3
		m_textureFilePaths[enSkyCubeType_DayToon_4] = L"Assets/modelData/preset/skyCubeMapDay_Toon_04.dds";			//昼間(トゥーン調)4
		m_textureFilePaths[enSkyCubeType_NightToon] = L"Assets/modelData/preset/skyCubeMapNight_Toon.dds";			//夜間(トゥーン調)
		m_textureFilePaths[enSkyCubeType_NightToon_2] = L"Assets/modelData/preset/skyCubeMapNight_Toon_02.dds";		//夜間(トゥーン調)2
		m_textureFilePaths[enSkyCubeType_SunriseToon] = L"Assets/modelData/preset/skyCubeMapSunrise_Toon.dds";		//明け方(トゥーン調)
		m_textureFilePaths[enSkyCubeType_SpaceToon] = L"Assets/modelData/preset/skyCubeMapSpace_Toon_2.dds";		//大気圏(トゥーン調)
	}

	//デストラクタ
	SkyCube::~SkyCube()
	{

	}

	//開始処理
	bool SkyCube::Start()
	{
		//モデルの初期化
		ModelInitData initData;
		// ロード済みのTKMリソースを取得
		initData.m_tkmFile =
			g_resourceManager->GetTkm(
				"Assets/modelData/preset/sky.tkm"
			);

		K2_ASSERT(
			initData.m_tkmFile != nullptr,
			"SkyCube TKM is not loaded.",
			0
		);

		//シェーダーファイルのファイルパスを指定
		initData.m_fxFilePath = "Assets/shader/SkyCubeMap.fx";
		initData.m_vsEntryPointFunc = "VSMain";
		initData.m_psEntryPointFunc = "PSMain";

		for (int i = 0; i < enSkyCubeType_Num; i++)
		{
			//テクスチャの初期化
			m_texture[i].InitFromDDSFile(m_textureFilePaths[i]);
		}

		//テクスチャの設定
		initData.m_expandShaderResoruceView[0] = &m_texture[m_type];
		//定数バッファの設定
		initData.m_expandConstantBuffer = &m_luminance;
		initData.m_expandConstantBufferSize = sizeof(m_luminance);

		//スカイキューブの初期化
		m_modelRender.InitSkyCube(initData);
		//座標と回転と大きさの設定
		m_modelRender.SetTRS(m_position, m_rotation, m_scale);
		//スカイキューブの更新
		m_modelRender.Update();

		return true;
	}

	//更新処理
	void SkyCube::Update()
	{
		//データの変更がされたか？
		if (m_isDirty)
		{
			//座標・回転・大きさをまとめて設定する
			m_modelRender.SetTRS(m_position, m_rotation, m_scale);
			//モデルの更新
			m_modelRender.Update();
			m_isDirty = false;
		}
	}

	//描画処理
	void SkyCube::Render(RenderContext& rc)
	{
		//スカイキューブの描画
		m_modelRender.Draw(rc);
	}
}
