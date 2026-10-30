#pragma once
/**
 * @file LoadingUI.h。
 * @brief ローディングUIクラス。
 * @author Tanimoto。
 * @date 2026/10/20。
 */

/*ローディングUIクラス。*/
class LoadingUI : public Noncopyable
{
private:
	LoadingUI() {};/*コンストラクタ。*/

public:
	~LoadingUI() {};/*デストラクタ。*/

public:
	bool Start();/*開始処理。*/
	void Update();/*更新処理。*/
	void Render(RenderContext& rc);/*描画処理。*/

public:/*メンバ関数。*/

	inline SpriteRender* GetBackGround()
	{
		return &m_backGround;
	}

	inline SpriteRender* GetLoadingTire()
	{
		return &m_loadingTire;
	}

	/**
	 * @brief ローディング中？
	 * @return trueならローディング中。
	 */
	inline bool IsLoading() const
	{
		return m_loadingTire.GetMulColor().a > 0.0f;
	}

public:/*シングルトン用のメンバ関数。*/

	/**
	 * @brief シングルトンインスタンスの生成。
	 */
	inline static void CreateInstance()
	{
		if (m_instance != nullptr) { return; }
		m_instance = new LoadingUI();
		m_instance->Start();
	}

	/**
	 * @brief シングルトンインスタンスの削除。
	 */
	inline static void DeleteInstance()
	{
		if (m_instance == nullptr) { return; }
		delete m_instance;
		m_instance = nullptr;
	}

	/**
	 * @brief シングルトンインスタンスの取得。
	 * @return シングルトンインスタンス。
	 */
	inline static LoadingUI* GetInstance()
	{
		return m_instance;
	}

private:/*メンバ変数。*/
	SpriteRender m_backGround;/*背景。*/
	SpriteRender m_loadingTire;/*ローディング用のタイヤ。*/
	Quaternion m_tireRotation = Quaternion::Identity;/*タイヤの回転。*/
	std::unique_ptr<PositionUIAnimation> m_vibrationAnimation;/*振動アニメーション。*/

private:/*シングルトンインスタンス。*/
	static LoadingUI* m_instance;/*シングルトンインスタンス。*/
};

