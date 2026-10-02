/*!
 *@brief	レベル。
 */

#pragma once
#include "level3D/TklFile.h"

namespace nsK2EngineLow {
	class MapChip;
	/// <summary>
	/// レベルオブジェクト
	/// </summary>
	struct LevelObjectData : public Noncopyable
	{
		/// <summary>
		/// 引数で渡したオブジェクト名のオブジェクトか調べる
		/// </summary>
		/// <param name="objName">調べる名前</param>
		/// <returns>名前が同じ場合trueを返す</returns>
		const bool EqualObjectName(const wchar_t* objName)
		{
			// 変更前のコード
			return wcscmp(objName, name) == 0;
		}

		/// <summary>
		/// 名前が前方一致か調べる
		/// </summary>
		/// <param name="n">調べる名前</param>
		/// <returns>一致していたらtrueを返す</returns>
		const bool ForwardMatchName(const wchar_t* n)
		{
			auto len = wcslen(n);
			auto namelen = wcslen(name);
			if (len > namelen) {
				//名前が長い。不一致
				return false;
			}
			return wcsncmp(n, name, len) == 0;
		}

		Vector3 position = Vector3::Zero;				//座標
		Quaternion rotation = Quaternion::Identity;		//回転
		Vector3 scale = Vector3::One;					//拡大率


		const wchar_t* name;							//名前
		int number = 0;
	};
}

namespace nsK2Engine
{
	/// <summary>
	/// レベル3Dレンダー
	/// </summary>
	class Level3DRender : public Noncopyable
	{
	private:
		using MapChipPtr = std::shared_ptr<MapChip>;
	public:
		Level3DRender(); //コンストラクタ
		~Level3DRender(); //デストラクタ

		/// <summary>
		/// レベルを初期化
		/// </summary>
		/// <param name="filePath">tklファイルのファイルパス</param>
		/// <param name="hookFunc"></param>
		void Init(const char* filePath, std::function<bool(nsK2EngineLow::LevelObjectData& objData) > hookFunc);

		/// <summary>
		/// モデルを描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Draw(RenderContext& rc);

	private:
		/// <summary>
		/// マップチップを作成
		/// </summary>
		/// <param name="objData">レベルオブジェクトデータ</param>
		/// <param name="filePath">ファイルパス</param>
		void CreateMapChip(const nsK2EngineLow::LevelObjectData& objData, const char* filePath);

		/// <summary>
		/// tklファイルの行列を変換する
		/// </summary>
		void MatrixTklToLevel();

	private:
		using BonePtr = std::unique_ptr<Bone>;				//ボーンPtr。
		std::vector<BonePtr> m_bonelist;					//ボーンのリスト。
		std::unique_ptr<Matrix[]> m_matrixlist;				//行列のリスト。
		std::vector<MapChipPtr> m_mapChipPtrs;				//マップチップの可変長配列。
		TklFile m_tklFile;									//Tklファイル。
	};
}