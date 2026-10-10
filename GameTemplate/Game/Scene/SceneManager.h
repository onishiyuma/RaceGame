#pragma once

class IScene;
class SceneManager : public IGameObject
{
public:

	/// <summary>
	/// シーン遷移の状態。
	/// </summary>
	enum class TransitionState
	{
		Idle,           // 通常状態
		FadingOut,      // フェードアウト中
		Loading,        // 次シーンのロード・初期化
		Switching,      // シーン切り替え
		FadingIn        // フェードイン中
	};

public:

	SceneManager() = default;
	~SceneManager();

	/// <summary>
	/// SceneManagerを初期化し、最初のシーンを設定する。
	/// </summary>
	bool Init(std::unique_ptr<IScene> firstScene);

	/// <summary>
	/// SceneManagerを更新する。
	/// </summary>
	void Update() override;

	/// <summary>
	/// シーンを描画する。
	/// </summary>
	void Render(RenderContext& renderContext) override;

	/// <summary>
	/// シーンを変更する。
	/// ゲームシーンの上にメニューシーンを出すみたいな場合はPushSceneを使ってください。
	/// スタック全体を新しいシーンで書き換えるので注意して。
	/// </summary>
	void ChangeScene(std::unique_ptr<IScene> nextScene);

	/// <summary>
	/// 現在のシーンを中断して、
	/// その上に新しいシーンを追加する。
	/// </summary>
	void PushScene(std::unique_ptr<IScene> scene);

	/// <summary>
	/// 一番上のシーンを終了し、
	/// 下のシーンへ戻る。
	/// </summary>
	void PopScene();

	/// <summary>
	/// 現在操作対象になっているシーンを取得する。
	/// </summary>
	IScene* GetCurrentScene() const;

	/// <summary>
	/// シーン遷移中か。
	/// </summary>
	bool IsTransitioning() const
	{
		return m_transitionState != TransitionState::Idle;
	}

	TransitionState GetTransitionState() const
	{
		return m_transitionState;
	}


private:

	/// <summary>
	/// ChangeSceneの実際の切り替え処理。
	/// </summary>
	void ExecuteChangeScene();

	/// <summary>
	/// PushSceneの実際の処理。
	/// </summary>
	void ExecutePushScene();

	/// <summary>
	/// PopSceneの実際の処理。
	/// </summary>
	void ExecutePopScene();

	/// <summary>
	/// シーンの状態を更新する。
	/// </summary>
	void UpdateTransition();

	void UpdateFadingOut();
	void UpdateLoading();
	void UpdateSwitching();
	void UpdateFadingIn();

	void CancelTransition();

private:

	/// <summary>
	/// 現在存在しているシーン。
	/// 一番後ろが現在のシーン。
	/// </summary>
	std::vector<std::unique_ptr<IScene>> m_sceneStack;

	/// <summary>
	/// 次に使用するシーン。
	/// </summary>
	std::unique_ptr<IScene> m_nextScene;

	TransitionState m_transitionState = TransitionState::Idle;


	enum class RequestType
	{
		None,
		Change,
		Push,
		Pop
	};

	RequestType m_requestType = RequestType::None;
};