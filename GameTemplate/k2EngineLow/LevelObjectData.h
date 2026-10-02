#pragma once
namespace nsK2EngineLow
{
	/// <summary>
	/// レベルに配置されたオブジェクトの情報
	/// </summary>
	struct LevelObjectData : public Noncopyable
	{
		bool EqualObjectName(const wchar_t* objName) const
		{
			return wcscmp(objName, name) == 0;
		}

		bool ForwardMatchName(const wchar_t* n) const
		{
			const auto len = wcslen(n);
			const auto nameLen = wcslen(name);

			if (len > nameLen) {
				return false;
			}

			return wcsncmp(n, name, len) == 0;
		}

		Vector3 position = Vector3::Zero;
		Quaternion rotation = Quaternion::Identity;
		Vector3 scale = Vector3::One;

		const wchar_t* name = nullptr;
		int number = 0;
	};
}

