#pragma once
class SceneManager;
/// <summary>
/// シーンの基底クラス。
/// シーンのライフサイクルと状態管理を行う。
/// シーンマネージャを依存性注入で受け取るけど、
/// シーン推移の要求以外でシーンマネージャは絶対に使わないで。
/// ただ今は疎結合にするためのインターフェースを追加してもそこまでメリットがないから、
/// シーンマネージャーを直接注入する設計にしている。
/// </summary>
class IScene
{
public:

	/// <summary>
	/// シーンの状態。
	/// </summary>
	enum class State
	{
		Unloaded,      // 未ロード
		Loaded,        // ロード済み
		Initialized,   // 初期化済み
		Active,        // 動作中
		Suspended      // 中断中
	};

public:

	virtual ~IScene() = default;

	bool Load()
	{
		// すでにロード済みなら何もしない
		if (m_state != State::Unloaded) {
			return true;
		}

		if (!OnLoad()) {
			return false;
		}

		m_state = State::Loaded;

		return true;
	}

	bool Init()
	{
		// Loadされていない
		if (m_state == State::Unloaded) {
			return false;
		}

		// すでに初期化済み
		if (m_state == State::Initialized ||
			m_state == State::Active ||
			m_state == State::Suspended) {
			return true;
		}

		if (!OnInit()) {
			return false;
		}

		m_state = State::Initialized;

		return true;
	}

	/// <summary>
	/// シーンをアクティブにする。
	/// </summary>
	void Activate()
	{
		if (m_state != State::Initialized) {
			return;
		}

		OnActivate();

		m_state = State::Active;
	}

	/// <summary>
	/// シーンを一時的に中断する。
	/// シーン自体は有効なまま保持する。
	/// </summary>
	void Suspend()
	{
		if (m_state != State::Active) {
			return;
		}

		OnSuspend();

		m_state = State::Suspended;
	}

	/// <summary>
	/// 中断中のシーンを再開する。
	/// </summary>
	void Resume()
	{
		if (m_state != State::Suspended) {
			return;
		}

		OnResume();

		m_state = State::Active;
	}

	/// <summary>
	/// シーンを更新する。
	/// 中断中は更新しない。
	/// </summary>
	void Update()
	{
		if (m_state != State::Active) {
			return;
		}

		OnUpdate();
	}

	/// <summary>
	/// シーンを描画する。
	/// 中断中でも描画する。
	/// </summary>
	void Render(RenderContext& renderContext)
	{
		if (m_state != State::Active &&
			m_state != State::Suspended) {
			return;
		}

		OnRender(renderContext);
	}

	/// <summary>
	/// シーンを無効化する。
	/// </summary>
	void Deactivate()
	{
		if (m_state != State::Active &&
			m_state != State::Suspended) {
			return;
		}

		OnDeactivate();

		m_state = State::Initialized;
	}

	void Unload()
	{
		if (m_state == State::Unloaded) {
			return;
		}

		// 動作中または中断中なら先に無効化する
		if (m_state == State::Active ||
			m_state == State::Suspended) {
			Deactivate();
		}

		OnUnload();

		m_state = State::Unloaded;
	}


	State GetState() const
	{
		return m_state;
	}

	bool IsLoaded() const
	{
		return m_state != State::Unloaded;
	}
	/// <summary>
	///	初期化してるか？
	/// </summary>
	/// <returns></returns>
	bool IsInitialized() const
	{
		return
			m_state == State::Initialized ||
			m_state == State::Active ||
			m_state == State::Suspended;
	}
	/// <summary>
	/// シーンが有効化されているか判定する。
	/// 中断中も有効として扱う。
	/// </summary>
	bool IsActivated() const
	{
		return
			m_state == State::Active ||
			m_state == State::Suspended;
	}

	bool IsRunning() const
	{
		return m_state == State::Active;
	}

	bool IsSuspended() const
	{
		return m_state == State::Suspended;
	}

	void SetSceneManager(SceneManager* manager)
	{
		m_sceneManager = manager;
	}


protected:

	/// <summary>
	/// シーンをロードする。
	/// 必ず失敗したらロード中のリソースを解放すること。
	/// </summary>
	/// <returns></returns>
	virtual bool OnLoad() = 0;

	virtual bool OnInit() = 0;

	/// <summary>
	/// シーンがアクティブになったときの処理。
	/// </summary>
	virtual void OnActivate() {}

	/// <summary>
	/// シーンが中断されたとき。
	/// </summary>
	virtual void OnSuspend() {}

	/// <summary>
	/// 中断から復帰したとき。
	/// </summary>
	virtual void OnResume() {}

	virtual void OnUpdate() = 0;

	virtual void OnRender(RenderContext& renderContext) = 0;

	virtual void OnDeactivate() {}
	virtual void OnUnload() = 0;

	SceneManager* GetSceneManager() const
	{
		return m_sceneManager;
	}


private:

	State m_state = State::Unloaded;
	SceneManager* m_sceneManager = nullptr;		// シーンマネージャーへの参照。シーンが管理されている場合のみ有効。
};