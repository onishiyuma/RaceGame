#include "k2EnginePreCompile.h"
#include "CollisionObject.h"

namespace nsK2Engine
{
	//コンストラクタ
	CollisionObject::CollisionObject()
	{

	}

	//デストラクタ
	CollisionObject::~CollisionObject()
	{
		if (g_collisionObjectManager) {
			g_collisionObjectManager->RemoveCollisionObject(this);
		}
	}

	//開始処理
	bool CollisionObject::Start()
	{
		g_collisionObjectManager->AddCollisionObject(this);
		return true;
	}

	//更新処理
	void CollisionObject::Update()
	{
		if (m_isEnableAutoDelete == false)
		{
			return;
		}


		if (m_timer > m_timeLimit + 0000.1f)
		{
			DeleteGO(this);
		}
		m_timer += g_gameTime->GetFrameDeltaTime();
	}

	//コンストラクタ
	CollisionObjectManager::CollisionObjectManager()
	{

	}

	//デストラクタ
	CollisionObjectManager::~CollisionObjectManager()
	{
		m_collisionObjectVector.clear();
	}
}