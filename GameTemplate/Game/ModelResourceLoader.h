#pragma once

struct ModelAssetDefinition;

/// <summary>
/// モデルアセット定義に含まれる
/// TKM・TKS・TKAのリソースをロードする。
/// </summary>
bool LoadModelResources(const ModelAssetDefinition& asset);