#include"stdafx.h"
#include "SceneManager.h"
#include "IScene.h"

SceneManager::~SceneManager()
{
	//所有してるシーンをすべて終わらせて破棄する。
	while (!m_sceneStack.empty())
	{
		auto& scene = m_sceneStack.back();

		scene->Deactivate();
		scene->Unload();

		m_sceneStack.pop_back();
	}
	//繊維待ちのシーンがの取ってた場合
	if (m_nextScene)
	{
		m_nextScene->Unload();
		m_nextScene.reset();
	}
}

bool SceneManager::Init(std::unique_ptr<IScene> firstScene)
{
	if (!firstScene)return false;

	if (!m_sceneStack.empty() || m_nextScene)return false;

	if (IsTransitioning())return false;

	m_nextScene = std::move(firstScene);
	m_requestType = RequestType::Change;

	// 最初のシーンは切り替え元がないのでフェードアウトしない。
	m_transitionState = TransitionState::Loading;

	return true;
}

void SceneManager::Update()
{
	// 通常時だけ更新する。
	if (m_transitionState == TransitionState::Idle)
	{
		if (IScene* currentScene = GetCurrentScene())
		{
			currentScene->Update();
		}
	}

	// シーン遷移中なら状態を更新する
	UpdateTransition();
}

void SceneManager::Render(RenderContext& renderContext)
{
	for (auto& scene : m_sceneStack)
	{
		scene->Render(renderContext);
	}
}

void SceneManager::ChangeScene(std::unique_ptr<IScene> nextScene)
{
	if (IsTransitioning())return;

	if (!nextScene)return;

	//SceneManagerを設定する。
	nextScene->SetSceneManager(this);

	//まだ実際にシーンは切り換えない。
	//ロードなどの処理が終わったら実際に切り換える。

	m_nextScene = std::move(nextScene);
	m_requestType = RequestType::Change;
	m_transitionState = TransitionState::FadingOut;
}

void SceneManager::PushScene(std::unique_ptr<IScene> scene)
{
	if (IsTransitioning())return;

	if (!scene)return;

	// まだ実際にシーンは切り換えない。
	// フェードアウト後に現在のシーンを中断し、新しいシーンを重ねる。

	m_nextScene = std::move(scene);
	m_requestType = RequestType::Push;
	m_transitionState = TransitionState::FadingOut;
}

void SceneManager::PopScene()
{
	if (IsTransitioning())return;

	if (m_sceneStack.size() <= 1)return;

	// まだ実際にシーンは切り換えない。
	// フェードアウト後に現在のシーンを終了し、前のシーンを再開する。

	m_requestType = RequestType::Pop;
	m_transitionState = TransitionState::FadingOut;
}

IScene* SceneManager::GetCurrentScene() const
{
	if (m_sceneStack.empty()) {
		return nullptr;
	}

	return m_sceneStack.back().get();
}

void SceneManager::UpdateTransition()
{
	switch (m_transitionState)
	{
	case TransitionState::Idle:
		break;

	case TransitionState::FadingOut:
		UpdateFadingOut();
		break;

	case TransitionState::Loading:
		UpdateLoading();
		break;

	case TransitionState::Switching:
		UpdateSwitching();
		break;

	case TransitionState::FadingIn:
		UpdateFadingIn();
		break;
	}
}

void SceneManager::UpdateFadingOut()
{
	// TODO: フェードアウト処理をあとでかく
	m_transitionState = TransitionState::Loading;
}

void SceneManager::UpdateLoading()
{
	if (m_requestType == RequestType::Change ||
		m_requestType == RequestType::Push)
	{
		//次のシーンがないなら遷移をキャンセルする。
		if (!m_nextScene)
		{
			CancelTransition();
			return;
		}

		//次のシーンをロードする。
		if (!m_nextScene->Load())
		{
			//ロードに失敗したら遷移をキャンセルする。
			//Loadの中のOnLoad側でロード途中のリソースを解放する前提。
			m_nextScene.reset();
			CancelTransition();
			return;
		}

		//次のシーンを初期化する。
		if (!m_nextScene->Init())
		{
			//初期化に失敗したら遷移をキャンセルする。
			m_nextScene->Unload();
			m_nextScene.reset();

			CancelTransition();
			return;
		}
	}

	m_transitionState = TransitionState::Switching;
}

void SceneManager::UpdateSwitching()
{
	switch (m_requestType)
	{
	case RequestType::Change:
		ExecuteChangeScene();
		break;

	case RequestType::Push:
		ExecutePushScene();
		break;

	case RequestType::Pop:
		ExecutePopScene();
		break;

	case RequestType::None:
		break;
	}

	m_requestType = RequestType::None;
	m_transitionState = TransitionState::FadingIn;
}

void SceneManager::UpdateFadingIn()
{
	// TODO: フェードイン処理をあとで書く
	m_transitionState = TransitionState::Idle;
}

void SceneManager::ExecuteChangeScene()
{
	//スタックに入っているシーンをすべて終了する。
	while (!m_sceneStack.empty())
	{
		auto& scene = m_sceneStack.back();

		scene->Deactivate();
		scene->Unload();

		m_sceneStack.pop_back();
	}

	// 次のシーンをアクティブにしてスタックに積む。
	if (m_nextScene)
	{
		m_nextScene->Activate();

		m_sceneStack.emplace_back(
			std::move(m_nextScene)
		);
	}
}

void SceneManager::ExecutePushScene()
{
	//現在のシーンを中断する。
	if (IScene* currentScene = GetCurrentScene())
	{
		currentScene->Suspend();
	}

	// 次のシーンをスタックの一番上に積む。
	if (m_nextScene)
	{
		m_nextScene->Activate();

		m_sceneStack.emplace_back(
			std::move(m_nextScene)
		);
	}
}

void SceneManager::ExecutePopScene()
{
	if (m_sceneStack.empty())return;
	//現在のシーンを終了する。
	auto& currentScene = m_sceneStack.back();
	currentScene->Deactivate();
	currentScene->Unload();
	m_sceneStack.pop_back();

	//前のシーンを再開する。
	if (IScene* previousScene = GetCurrentScene())
	{
		previousScene->Resume();
	}
}

void SceneManager::CancelTransition()
{
	m_nextScene.reset();
	m_requestType = RequestType::None;
	// 失敗した場合も元のシーンへフェードインする。
	m_transitionState = TransitionState::FadingIn;
}