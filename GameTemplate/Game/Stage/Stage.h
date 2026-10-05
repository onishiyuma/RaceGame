#pragma once
#include"Stage/StageObject.h"

/// <summary>
/// ステージの実体
/// ステージのオブジェクトやスポーン地点などの
/// ステージ固有の情報を所有する。
/// </summary>
class Stage
{
public:
	void Update()
	{
		for (auto& object : m_objects)
		{
			object->Update();
		}
	}

	void Draw(RenderContext& rc)
	{
		for (auto& object : m_objects)
		{
			object->Draw(rc);
		}
	}

	void AddObject(std::unique_ptr<StageObject> object)
	{
		m_objects.push_back(std::move(object));
	}

	void Clear()
	{
		m_objects.clear();
	}

private:
	// Prop / BoostPad / ItemBox など
	std::vector<std::unique_ptr<StageObject>> m_objects;

};

