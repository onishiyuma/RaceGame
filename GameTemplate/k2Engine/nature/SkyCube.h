#pragma once

namespace nsK2Engine
{
	//スカイキューブの背景一覧
	enum EnSkyCubeType {
		enSkyCubeType_Day,			//昼間
		enSkyCubeType_Night,		//夜間
		enSkyCubeType_Snow,			//雪山
		enSkyCubeType_Snow_2,		//雪山2
		enSkyCubeType_Wild,			//荒野
		enSkyCubeType_Wild_2,		//荒野2
		enSkyCubeType_Wild_Night,	//荒野(夜間)
		enSkyCubeType_Grass,		//芝生
		enSkyCubeType_Euro,			//欧州
		enSkyCubeType_DayToon,		//昼間(トゥーン調)
		enSkyCubeType_DayToon_2,	//昼間(トゥーン調)2
		enSkyCubeType_DayToon_3,	//昼間(トゥーン調)3
		enSkyCubeType_DayToon_4,	//昼間(トゥーン調)4
		enSkyCubeType_NightToon,	//夜間(トゥーン調)
		enSkyCubeType_NightToon_2,	//夜間(トゥーン調)2
		enSkyCubeType_SunriseToon,	//明け方(トゥーン調)
		enSkyCubeType_SpaceToon,	//大気圏(トゥーン調)
		enSkyCubeType_Num,
	};

	/// <summary>
	/// スカイキューブ
	/// </summary>
	class SkyCube : public IGameObject
	{
	public:
		SkyCube(); //コンストラクタ
		~SkyCube(); //デストラクタ
		bool Start()override final;	//開始処理
		void Update()override final; //更新処理
		void Render(RenderContext& rc)override final; //描画処理

	public:
		/// <summary>
		/// 座標を設定
		/// </summary>
		/// <param name="pos">座標</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
			m_isDirty = true;
		}

		/// <summary>
		/// 大きさを設定
		/// </summary>
		/// <param name="scale">大きさ</param>
		void SetScale(const float scale)
		{
			m_scale = g_vec3One;
			m_scale.Scale(scale);
			m_isDirty = true;
		}
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
			m_isDirty = true;
		}

		/// <summary>
		/// スカイキューブのタイプを設定
		/// </summary>
		/// <param name="type">スカイキューブのタイプ</param>
		void SetType(EnSkyCubeType type)
		{
			m_type = type;
		}

		/// <summary>
		/// 明るさを設定
		/// </summary>
		/// <param name="lum">明るさ</param>
		void SetLuminance(const float lum)
		{
			m_luminance = lum;
		}

		/// <summary>
		/// スカイキューブのテクスチャを取得
		/// </summary>
		/// <returns>テクスチャ</returns>
		const wchar_t* GetTextureFilePath()
		{
			return m_textureFilePaths[m_type];
		}

	private:
		ModelRender m_modelRender;								//モデル
		Texture m_texture[enSkyCubeType_Num];					//スカイキューブ用のテクスチャ
		const wchar_t* m_textureFilePaths[enSkyCubeType_Num];	//スカイキューブ用のテクスチャのファイルパス
		Vector3 m_position = g_vec3Zero;						//座標
		Vector3 m_scale = g_vec3One * 1000.0f;					//大きさ
		float m_luminance = 1.0f;								//明るさ
		bool m_isDirty = false;									//データの変更がされたか?
		EnSkyCubeType m_type = enSkyCubeType_Day;				//スカイキューブのタイプ
		Quaternion m_rotation = { 0.0f,0.0f,0.0f,1.0f };		//回転
	};
}

