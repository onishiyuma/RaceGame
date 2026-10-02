#include "TransformHelper.h"
#include <string>
#include <functional>
#include <Json/json.hpp>
#include <iostream>
#include <fstream>
#include <functional>
#include"StageDefinition.h"



/// <summary>
/// 指定のジェイソンファイルパスを読み込みシーン定義を作成する。
/// </summary>
/// <param name="filepath"></param>
/// <param name="outSceneDefinition"></param>
/// <returns></returns>
bool LoadStage(const std::string& filepath, StageDefinition& outSceneDefinition);
