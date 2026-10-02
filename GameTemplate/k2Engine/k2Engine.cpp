#include "k2EnginePreCompile.h"
#include "k2Engine.h"

namespace nsK2Engine {
	K2Engine* K2Engine::m_instance = nullptr;
	SceneLight* g_sceneLight = nullptr;
	RenderingEngine* g_renderingEngine = nullptr;
	CollisionObjectManager* g_collisionObjectManager = nullptr;
	K2Engine* g_k2Engine = nullptr;
	ResourceManager* g_resourceManager = nullptr;

	void K2Engine::Init(const InitData& initData)
	{
		g_k2Engine = this;
		g_engine = &m_k2EngineLow;
		g_collisionObjectManager = &m_collisionObjectManager;
		g_renderingEngine = &m_renderingEngine;
		g_resourceManager = &m_resourceManager; // 追加（高橋）

		m_k2EngineLow.Init(
			initData.hwnd,
			initData.frameBufferWidth,
			initData.frameBufferHeight
		);
		m_renderingEngine.Init();
		g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
		g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	}
	K2Engine::~K2Engine()
	{
		g_renderingEngine = nullptr;
		g_collisionObjectManager = nullptr;
		g_engine = nullptr;
	}

	void K2Engine::Execute()
	{
		auto& renderContext = g_graphicsEngine->GetRenderContext();

		g_engine->BeginFrame();

		g_engine->ExecuteUpdate();

		g_engine->ExecuteRender();

		//レンダリングエンジンを実行。		
		m_renderingEngine.Execute(renderContext);

		//当たり判定描画。
		g_engine->DebubDrawWorld();

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
}