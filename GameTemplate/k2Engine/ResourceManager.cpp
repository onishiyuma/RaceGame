#include "k2EnginePreCompile.h"
#include "ResourceManager.h"

namespace nsK2Engine
{

	bool ResourceManager::LoadTkm(const char* filePath)
	{
		// すでにロード済みなら何もしない
		if (GetTkm(filePath) != nullptr) {
			return true;
		}

		// ファイルをロード
		auto* tkmFile = new TkmFile;

		if (!tkmFile->Load(filePath, false))
		{
			delete tkmFile;
			return false;
		}

		// Bankへ登録
		m_tkmFileBank.Regist(filePath, tkmFile);

		return true;
	}

	TkmFile* ResourceManager::GetTkm(const char* filePath)
	{
		// 取得するだけ。
		// ここでは絶対にLoadしない。
		return m_tkmFileBank.Get(filePath);
	}

	bool ResourceManager::LoadTks(const char* filePath)
	{
		if (m_tksFileBank.Get(filePath) != nullptr) {
			return true;
		}

		auto* tksFile = new TksFile;
		if (!tksFile->Load(filePath))
		{
			delete tksFile;
			return false;
		}

		m_tksFileBank.Regist(filePath, tksFile);
		return true;
	}

	TksFile* ResourceManager::GetTks(const char* filePath)
	{
		return m_tksFileBank.Get(filePath);
	}

	bool ResourceManager::LoadTka(const char* filePath)
	{
		if (m_tkaFileBank.Get(filePath) != nullptr) {
			return true;
		}

		auto* tkaFile = new TkaFile;
		if (!tkaFile->Load(filePath))
		{
			delete tkaFile;
			return false;
		}

		m_tkaFileBank.Regist(filePath, tkaFile);
		return true;
	}

	TkaFile* ResourceManager::GetTka(const char* filePath)
	{
		return m_tkaFileBank.Get(filePath);
	}

}

