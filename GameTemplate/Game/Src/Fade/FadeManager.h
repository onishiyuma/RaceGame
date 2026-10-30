#pragma once
/**
 * @file TitleScene.h。
 * @brief フェードマネージャークラス。
 * @author Tanimoto。
 * @date 2026/10/19。
 */

/*フェードを管理するクラス。*/
class FadeManager : public Noncopyable
{
private:
	FadeManager() {};/*コンストラクタ。*/

public:
	~FadeManager() {};/*デストラクタ。*/

public:
	void Update();/*更新処理。*/

public:/*列挙型。*/

	/*フェードの状態。*/
	enum EnFade
	{
		enFade_In,/*フェードイン。*/
		enFade_Out,/*フェードアウト。*/
		enFade_None/*なし。*/
	};

private:/*メンバ関数。*/
	void ChangeTargetFadesAlpha();/*フェードさせるスプライトの透明度を切り替える処理。*/

	void UpdateFade();/*フェードの更新処理。*/

	void DecisionEndFade();/*フェードの終了するかの判断処理。*/

public:/*メンバ関数。*/

	/**
	 * @brief フェードさせるスプライトの設定。
	 * @param targetFade フェードさせるスプライト。
	 */
	inline void SetTargetFede(SpriteRender* targetFade)
	{
		m_targetFades.push_back(targetFade);
	}

	/**
	 * @brief フェードさせるスプライトの削除。
	 * @param targetFade フェードさせるスプライト。
	 */
	void DeleteTargetFade(SpriteRender* targetFade)
	{
		auto it = std::find(m_targetFades.begin(), m_targetFades.end(), targetFade);
		if (it != m_targetFades.end()) {
			m_targetFades.erase(it);
		}
	}

	/**
	 * @brief フェードの状態の切り替え。
	 * @param fade フェードの状態。
	 */
	inline void ChangeFade(EnFade fade)
	{
		m_currentFade = fade;
	}

	/**
	 * @brief 遅延時間有りのフェードの状態の切り替え。
	 * @param fade フェードの状態。
	 * @param delay 遅延時間。
	 */
	inline void ChangeFade(EnFade fade, float delay)
	{
		m_currentFade = fade;
		m_delay = delay;
	}

	/**
	 * @brief フェード処理が終わったか？
	 * @param fade フェードの状態 (enFade_Noneを指定しても意味ない)。
	 * @return trueなら終わっている。
	 */
	inline bool IsEnd(EnFade fade)
	{
		return m_previousFade == fade && m_currentFade == enFade_None;
	}

public:/*シングルトン用のメンバ関数。*/

	/**
	 * @brief シングルトンインスタンスの生成。
	 */
	inline static void CreateInstance()
	{
		if (m_instance != nullptr) { return; }
		m_instance = new FadeManager();
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
	inline static FadeManager* GetInstance()
	{
		return m_instance;
	}

private:/*メンバ変数。*/
	std::vector<SpriteRender*> m_targetFades;/*フェードさせるスプライト。*/
	float m_fadeAlpha = 0.0f;/*フェードの透明度。*/
	float m_delay = 0.0f;/*遅延時間。*/
	EnFade m_currentFade = enFade_None;/*現在のフェードの状態。*/
	EnFade m_previousFade = enFade_None;/*前のフェードの状態。*/

private:/*シングルトンインスタンス。*/
	static FadeManager* m_instance;/*シングルトンインスタンス。*/
};

