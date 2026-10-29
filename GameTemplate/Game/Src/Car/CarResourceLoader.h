#pragma once

class AssetDatabase;
class CarDatabase;

/// <summary>
/// 車のモデルリソースをロードする。
/// CarDatabaseにある車種をAssetDatabaseを見ながらその車種に必要なものをロードする。
/// 同じモデルアセットIDはこの関数の呼び出し中に一度だけロードを要求する。
/// カーデータベースに書かれている車をすべてロードしている。（レースで使わない車でも）
/// TODO:今はレースで使う車種が固定だからデータベースのやつをすべてロードしてるけどもし変えるなら直さないといけない。
/// </summary>
/// <param name="carDatabase">車種IDと車種定義の対応を管理している。</param>
/// <param name="assetDatabase">アセットIDとアセット情報の対応をかんりしている。</param>
/// <returns></returns>
bool LoadCarResources(
	const CarDatabase& carDatabase,
	const AssetDatabase& assetDatabase
);