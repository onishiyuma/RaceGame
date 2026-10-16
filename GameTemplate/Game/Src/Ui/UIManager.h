#pragma once
#include "UIAnimationSequence.h"
/**
 * @file UIManager.h。
 * @brief UIマネージャークラス。
 * @author Tanimoto。
 * @date 2026/10/13。
 */

/*UI全体を管理するクラス。*/
class UIManager : public Noncopyable
{
private:
	UIManager() {};/*コンストラクタ。*/

public:
	~UIManager() {};/*デストラクタ。*/

public:/*メンバ関数。*/
	void Update();/*UIの更新。*/
	void Render(RenderContext& rc);/*UIの描画。*/

public:/*メンバ関数。*/

	/**
	 * @brief 描画するUIの追加。
	 * @param ui 描画するUI。
	 * @param order 描画順序。
	 */
	inline void AddUI(SpriteRender* ui, int order = 0)
	{
		m_uiList.at(order).push_back(ui);
	}

	/**
	 * @brief 描画しているUIの削除。
	 * @param ui 描画しているUI。
	 */
	void DeleteUI(SpriteRender* ui)
	{
		for (auto& uiList : m_uiList) {
			auto it = std::find(uiList.begin(), uiList.end(), ui);
			if (it != uiList.end()) {
				g_renderingEngine->DeleteRenderObject(ui);
				uiList.erase(it);
				break; // 削除が完了したらループを抜ける
			}
		}
	}

	/**
	 * @brief UIの描画を有効化。
	 * @param ui 描画するUI。
	 */
	void EnableDrawUI(SpriteRender* ui)
	{
		for (auto& uiList : m_uiList) {
			auto it = std::find(uiList.begin(), uiList.end(), ui);
			if (it != uiList.end()) {
				ui->EnableDraw();
				break; // 削除が完了したらループを抜ける
			}
		}
	}

	/**
	 * @brief UIの描画を無効化。
	 * @param ui 描画しているUI。
	 */
	void DisableDrawUI(SpriteRender* ui)
	{
		for (auto& uiList : m_uiList) {
			auto it = std::find(uiList.begin(), uiList.end(), ui);
			if (it != uiList.end()) {
				ui->DisableDraw();
				break; // 削除が完了したらループを抜ける
			}
		}
	}

public:/*メンバ関数。*/

	/**
	 * @brief UIアニメーションシーケンス(並列)の追加。
	 * @param animSequence UIアニメーションシーケンス(並列)。
	 */
	void AddUIAnimationSequence(std::shared_ptr<UIAnimationSequenceParallel> animSequence)
	{
		m_uiAnimationSequenceParallels.push_back(animSequence);
	}

	/**
	 * @brief UIアニメーションシーケンス(直列)の追加。
	 * @param animSequence UIアニメーションシーケンス(直列)。
	 */
	void AddUIAnimationSequence(std::shared_ptr<UIAnimationSequenceSerial> animSequence)
	{
		m_uiAnimationSequenceSerials.push_back(animSequence);
	}

	/**
	 * @brief UIアニメーションシーケンス(並列)の削除。
	 * @param animSequence UIアニメーションシーケンス(並列)。
	 */
	void DeleteUIAnimationSequence(std::shared_ptr<UIAnimationSequenceParallel> animSequence)
	{
		auto it = std::find(m_uiAnimationSequenceParallels.begin(), m_uiAnimationSequenceParallels.end(), animSequence);
		if (it != m_uiAnimationSequenceParallels.end())
		{
			m_uiAnimationSequenceParallels.erase(it);
		}
	}

	/**
	 * @brief UIアニメーションシーケンス(直列)の削除。
	 * @param animSequence UIアニメーションシーケンス(直列)。
	 */
	void DeleteUIAnimationSequence(std::shared_ptr<UIAnimationSequenceSerial> animSequence)
	{
		auto it = std::find(m_uiAnimationSequenceSerials.begin(), m_uiAnimationSequenceSerials.end(), animSequence);
		if (it != m_uiAnimationSequenceSerials.end())
		{
			m_uiAnimationSequenceSerials.erase(it);
		}
	}

public:/*シングルトン用のメンバ関数。*/

	/**
	 * @brief シングルトンインスタンスの生成。
	 */
	inline static void CreateInstance()
	{
		if (m_instance != nullptr) { return; }
		m_instance = new UIManager();
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
	inline static UIManager* GetInstance()
	{
		return m_instance;
	}

private:/*メンバ変数。*/
	typedef std::list<SpriteRender*> UIList;/*UIの描画リスト。*/
	std::array<UIList, 10> m_uiList;/*UIの描画リスト。*/
	std::vector<std::shared_ptr<UIAnimationSequenceParallel>> m_uiAnimationSequenceParallels;/*UIアニメーションシーケンスリスト(並列)。*/
	std::vector<std::shared_ptr<UIAnimationSequenceSerial>> m_uiAnimationSequenceSerials;/*UIアニメーションシーケンスリスト(直列)。*/

private:/*シングルトン用の変数。*/
	static UIManager* m_instance;/*シングルトンインスタンス。*/
};

