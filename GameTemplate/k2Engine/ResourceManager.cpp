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

	void ResourceManager::LoadTks(const char* filePath)
	{
		if (m_tksFileBank.Get(filePath) != nullptr) {
			return;
		}

		auto* tksFile = new TksFile;
		tksFile->Load(filePath);

		m_tksFileBank.Regist(filePath, tksFile);
	}

	TksFile* ResourceManager::GetTks(const char* filePath)
	{
		return m_tksFileBank.Get(filePath);
	}

	void ResourceManager::LoadTka(const char* filePath)
	{
		if (m_tkaFileBank.Get(filePath) != nullptr) {
			return;
		}

		auto* tkaFile = new TkaFile;
		tkaFile->Load(filePath);

		m_tkaFileBank.Regist(filePath, tkaFile);
	}

	TkaFile* ResourceManager::GetTka(const char* filePath)
	{
		return m_tkaFileBank.Get(filePath);
	}

}

