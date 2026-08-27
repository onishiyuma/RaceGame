#pragma once

namespace nsK2Engine
{
	/// <summary>
	/// アイレンダラー
	/// </summary>
	class IRenderer : public Noncopyable
	{
	public:
		/// <summary>
		/// モデルの描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		virtual void OnRenderModel(RenderContext& rc)
		{

		}

		/// <summary>
		/// シャドウマップの描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="ca">カメラ</param>
		virtual void OnRenderShadowMap(RenderContext& rc, Camera& ca)
		{

		}

		/// <summary>
		/// 2D(フォントとスプライト)の描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
	};
}

