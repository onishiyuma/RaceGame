#include "k2EnginePreCompile.h"
#include "ResourceManager.h"

namespace nsK2Engine
{

	void ResourceManager::LoadTkm(const char* filePath)
	{
		// すでにロード済みなら何もしない
		if (GetTkm(filePath) != nullptr) {
			return;
		}

		// ファイルをロード
		auto* tkmFile = new TkmFile;
		tkmFile->Load(filePath, false);

		// Bankへ登録
		m_tkmFileBank.Regist(filePath, tkmFile);
	}

	TkmFile* ResourceManager::GetTkm(const char* filePath)
	{
		// 取得するだけ。
		// ここでは絶対にLoadしない。
		return m_tkmFileBank.Get(filePath);
	}

}

