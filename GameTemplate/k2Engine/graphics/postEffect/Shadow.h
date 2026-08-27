#pragma once

namespace nsK2Engine
{
	/// <summary>
	/// シャドウ
	/// </summary>
	class Shadow : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();

		/// <summary>
		/// 描画処理を実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="roj">レンダリングオブジェクト</param>
		void Execute(RenderContext& rc, std::vector<IRenderer*>& ro);

		/// <summary>
		/// シャドウマップを取得
		/// </summary>
		/// <returns></returns>
		RenderTarget& GetRenderTarget()
		{
			return m_shadowMap;
		}

		/// <summary>
		/// ぼかしたシャドウマップを取得
		/// </summary>
		/// <returns></returns>
		GaussianBlur& GetShadowBlur()
		{
			return m_shadowBlur;
		}

	private:
		RenderTarget m_shadowMap; //シャドウマップ用のレンダリングターゲット
		GaussianBlur m_shadowBlur; //ガウシアンブラー
	};
}

