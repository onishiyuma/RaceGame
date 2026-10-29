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

	void Render(RenderContext& rc)
	{
		for (auto& object : m_objects)
		{
			object->Render(rc);
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

	/// <summary>
	/// 指定の型のステージオブジェクトを取得する。
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	std::vector<const T*> GetObjectsOfType() const
	{
		std::vector<const T*> result;

		for (const auto& object : m_objects)
		{
			if (const auto* target =
				dynamic_cast<const T*>(object.get()))
			{
				result.push_back(target);
			}
		}

		return result;
	}

private:
	// Prop / BoostPad / ItemBox など
	std::vector<std::unique_ptr<StageObject>> m_objects;

};

