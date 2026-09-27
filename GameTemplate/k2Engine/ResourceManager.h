#pragma once
namespace nsK2Engine
{
	/// <summary>
	/// リソースマネージャー。
	/// </summary>
	class ResourceManager : public Noncopyable
	{
	public:
		/// <summary>
		/// ロードする。すでにロード済みなら再ロードしない。
		/// </summary>
		/// <param name="filePath"></param>
		void LoadTkm(const char* filePath);

		/// <summary>
		/// ロード済みのものを取得する。
		/// ここでは絶対にロードしない。
		/// </summary>
		/// <param name="filePath"></param>
		/// <returns></returns>
		TkmFile* GetTkm(const char* filePath);

		/// <summary>
		/// Tksファイルをロードする。すでにロード済みなら再ロードしない。
		/// </summary>
		/// <param name="filePath"></param>
		void LoadTks(const char* filePath);

		/// <summary>
		/// ロード済みのTksファイルを取得する。
		/// ここでは絶対にロードしない。
		/// </summary>
		/// <param name="filePath"></param>
		/// <returns></returns>
		TksFile* GetTks(const char* filePath);
	private:
		TResourceBank<TkmFile> m_tkmFileBank;
		TResourceBank<TksFile> m_tksFileBank;
	};
}

