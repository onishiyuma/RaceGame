#pragma once

#include "light/SceneLight.h"
#include "IRenderer.h"
#include "postEffect/Tone.h"
#include "postEffect/Bloom.h"
#include "postEffect/Shadow.h"

namespace nsK2Engine
{
	//スクリーンモード
	enum EnScreenMode
	{
		enScreenMode_One,
		enScreenMode_Two,
		enScreenMode_Three,
		enScreenMode_Four
	};

	/// <summary>
	/// レンダリングエンジン
	/// </summary>
	class RenderingEngine : public Noncopyable
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
		void Execute(RenderContext& rc);

		/// <summary>
		/// レンダリングオブジェクトの追加
		/// </summary>
		/// <param name="renderObject">レンダリングオブジェクト</param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		/// <summary>
		/// ディレクションライトを設定する
		/// </summary>
		/// <param name="direction">ライトの方向</param>
		/// <param name="color">ライトの色</param>
		void SetDirectionLight(const Vector3& direction, const Vector3& color)
		{
			m_sceneLight.SetDirectionLight(direction, color);
		}

		/// <summary>
		/// 環境光を設定する
		/// </summary>
		/// <param name="ambient">環境光</param>
		void SetAmbient(const Vector3& ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}

		/// <summary>
		/// ポイントライトを設定する
		/// </summary>
		/// <param name="num">ライト番号</param>
		/// <param name="position">ライトの位置</param>
		/// <param name="color">ライトのカラー</param>
		/// <param name="range">ライトの影響範囲</param>
		void SetPointLight(int num, const Vector3& position, const Vector3& color, float range)
		{
			m_sceneLight.SetPointLight(num, position, color, range);
		}

		/// <summary>
		/// スポットライトを設定する
		/// </summary>
		/// <param name="num">ライト番号</param>
		/// <param name="position">ライトの位置</param>
		/// <param name="color">ライトのカラー</param>
		/// <param name="range">ライトの影響範囲</param>
		/// <param name="direction">ライトの放射方向</param>
		/// <param name="angle">ライトの放射角度</param>
		void SetSpotLight(int num, const Vector3& position, const Vector3& color, float range, const Vector3& direction, float angle)
		{
			m_sceneLight.SetSpotLight(num, position, color, range, direction, angle);
		}

		/// <summary>
		/// 半球ライトを設定する
		/// </summary>
		/// <param name="groundColor">地面色</param>
		/// <param name="skyColor">天球色</param>
		/// <param name="groundNormal">地面の法線</param>
		void SetHemLight(const Vector3& groundColor, const Vector3& skyColor, const Vector3& groundNormal)
		{
			m_sceneLight.SetHemLight(groundColor, skyColor, groundNormal);
		}

		/// <summary>
		/// シャドウマップを取得
		/// </summary>
		/// <returns></returns>
		RenderTarget& GetShadow()
		{
			return m_shadow.GetRenderTarget();
		}

		/// <summary>
		/// ライトを取得
		/// </summary>
		/// <returns></returns>
		Light& GetLight()
		{
			return m_sceneLight.GetLight();
		}

		/// <summary>
		/// ライトカメラを取得
		/// </summary>
		Camera& GetLightCamera()
		{
			return m_sceneLight.GetLightCamera();
		}

		/// <summary>
		/// ぼかしたシャドウマップを取得
		/// </summary>
		/// <returns></returns>
		GaussianBlur& GetShadowBlur()
		{
			return m_shadow.GetShadowBlur();
		}

	public://ポストエフェクト処理を切り替えるメンバ関数

		/// <summary>
		/// ブルーム有効化。
		/// </summary>
		void EnableBloom()
		{
			m_isEnableBloom = true;
		}

		/// <summary>
		///	ブルーム無効化。
		/// </summary>
		void DisableBloom()
		{
			m_isEnableBloom = false;
		}

		/// <summary>
		/// ブルーム有効化？
		/// </summary>
		/// <returns></returns>
		bool IsEnableBloom() const
		{
			return m_isEnableBloom;
		}

	public:

		/// <summary>
		/// スクリーンモードの切り替え
		/// </summary>
		/// <param name="enScreenMode"></param>
		void ChangeScreenMode(EnScreenMode enScreenMode)
		{
			m_screenMode = enScreenMode;
			InitViewports();
		}

		/// <summary>
		/// スクリーンモードの取得。
		/// </summary>
		/// <returns></returns>
		EnScreenMode GetScreenMode() const
		{
			return m_screenMode;
		}

	private:

		/// <summary>
		/// メインレンダリングターゲットの初期化
		/// </summary>
		void InitMainRenderTarget();

		/// <summary>
		/// 深度値用レンダリングターゲットの初期化
		/// </summary>
		void InitDepthRenderTarget();

		/// <summary>
		/// 法線用レンダリングターゲットの初期化
		/// </summary>
		void InitNormalRenderTarget();

		/// <summary>
		/// 2D(フォントとスプライト)の初期化
		/// </summary>
		void Init2DSprite();

		/// <summary>
		/// ビューポートの初期化
		/// </summary>
		void InitViewports();

		/// <summary>
		/// 上画面用のビューポートの設定
		/// </summary>
		void SetUpViewPort();

		/// <summary>
		/// 下画面用のビューポートの設定
		/// </summary>
		void SetDownViewPort();

		/// <summary>
		/// 左画面用のビューポートの設定
		/// </summary>
		void SetLeftViewPort();

		/// <summary>
		/// 右画面用のビューポートの設定
		/// </summary>
		void SetRightViewPort();

		/// <summary>
		/// 左上画面用のビューポートの設定
		/// </summary>
		void SetLeftUpViewPort();

		/// <summary>
		/// 右上画面用のビューポートの設定
		/// </summary>
		void SetRightUpViewPort();

		/// <summary>
		/// 左下画面用のビューポートの設定
		/// </summary>
		void SetLeftDownViewPort();

		/// <summary>
		/// 右下画面用のビューポートの設定
		/// </summary>
		void SetRightDownViewPort();

		/// <summary>
		/// メインレンダリングターゲットのカラーバッファの内容を
		/// フレームバッファにコピーするためのスプライトを初期化する
		/// </summary>
		void InitCopyMainRenderTargetToFrameBuffer();

		/// <summary>
		/// モデルの描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void ModelDraw(RenderContext& rc);

		/// <summary>
		/// 2D(フォントとスプライト)の描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void SpriteFontDraw(RenderContext& rc);

		/// <summary>
		/// メインレンダリングターゲットの内容をフレームバッファにコピーする
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

		SceneLight m_sceneLight;						//シーンライト
		RenderTarget m_mainRenderTarget;				//メインレンダリングターゲット
		RenderTarget m_depthRenderTarget;				//深度値用レンダリングターゲット
		RenderTarget m_normalRenderTarget;				//法線用レンダリングターゲット
		RenderTarget m_2DRenderTarget;					//2D用レンダリングターゲット
		Sprite m_2DSprite;								//2D(フォントとスプライト)用スクリプト
		Sprite m_mainSprite;							//メイン(モデル)用スプライト
		Sprite m_copyToFrameBufferSprite;				//メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		Tone m_tone;									//トゥーン
		Bloom m_bloom;									//ブルーム
		Shadow m_shadow;								//シャドウマップ
		std::vector< IRenderer* > m_renderObjects;		//レンダリングオブジェクトの格納
		std::vector< D3D12_VIEWPORT > m_viewPorts;		//ビューポートの格納
		EnScreenMode m_screenMode = enScreenMode_One;	//スクリーンモード(デフォルトは1画面)
		bool m_isEnableBloom = false;					//ブルーム有効化しているか？
	};
}