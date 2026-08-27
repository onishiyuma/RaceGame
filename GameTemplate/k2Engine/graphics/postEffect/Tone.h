#pragma once

namespace nsK2Engine
{
	/// <summary>
	/// トゥーン
	/// </summary>
	class Tone : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init(RenderTarget& mainRt, RenderTarget& depthRt, RenderTarget& normalRt);

		/// <summary>
		/// 描画処理を実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="rt">レンダーターゲット</param>
		void Execute(RenderContext& rc, RenderTarget& rt);

	public:

		/// <summary>
		/// トゥーンマップを取得
		/// </summary>
		/// <returns></returns>
		RenderTarget& GetToneRenderTarget()
		{
			return m_toneRenderTarget;
		}

	private:
		Texture m_toneTexture;				//トゥーン用テクスチャ
		RenderTarget m_toneRenderTarget;	//トゥーン用レンダリングターゲット
		Sprite m_toneMap;					//トゥーン用スプライト
		Sprite m_finalSprite;				//最終合成用スプライト
	};
}

