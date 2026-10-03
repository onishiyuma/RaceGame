#include "TransformHelper.h"
#include <functional>
#include <Json/json.hpp>
#include<fstream>
#include"StageDefinition.h"



/// <summary>
/// 指定のジェイソンファイルパスを読み込みシーン定義を作成する。
/// </summary>
/// <param name="filepath"></param>
/// <param name="outSceneDefinition"></param>
/// <returns></returns>
bool LoadStageDefinition(const std::string& filepath, StageDefinition& outStageDefinition);
