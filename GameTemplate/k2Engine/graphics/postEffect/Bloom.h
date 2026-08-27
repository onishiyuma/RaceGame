#pragma once

namespace nsK2Engine
{
	/// <summary>
	/// ブルーム
	/// </summary>
	class Bloom : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init(RenderTarget& rt);

		/// <summary>
		/// 描画処理を実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="rt">レンダリングターゲット</param>
		void Execute(RenderContext& rc, RenderTarget& rt);

	private:
		/// <summary>
		/// 輝度抽出用レンダリングターゲットの初期化
		/// </summary>
		/// <param name="rc">レンダリングターゲット</param>
		void InitLuminnceRenderTarget(RenderTarget& rt);
		
		/// <summary>
		/// 輝度抽出用スプライトの設定
		/// </summary>
		/// <param name="rt">レンダリングターゲット</param>
		void SetLuminanceSprite(RenderTarget& rt);

		/// <summary>
		/// ガウシアンブラーを初期化
		/// </summary>
		void InitGaussianBlur();

		/// <summary>
		/// 最終合成用スプライトの設定
		/// </summary>
		/// <param name="rt">レンダリングターゲット</param>
		void SetFinalSprite(RenderTarget& rt);

		RenderTarget m_luminnceRenderTarget;	//輝度抽出用レンダリングターゲット
		Sprite m_luminanceSprite;				//輝度抽出用スプライト
		GaussianBlur m_gaussianBlur[4];			//ガウシアンブラー
		Sprite m_finalSprite;					//最終合成用スプライト
	};
}

