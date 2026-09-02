using UnityEngine;
using UnityEditor;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;

public class SceneJsonExporter : EditorWindow
{
    private string exportPath = "SceneExport.json";

    [MenuItem("Tools/Scene JSON Exporter")]
    public static void ShowWindow()
    {
        GetWindow<SceneJsonExporter>("Scene JSON Exporter");
    }

    private void OnGUI()
    {
        GUILayout.Label("Export Scene as JSON", EditorStyles.boldLabel);

        if (GUILayout.Button("Select Export Location"))
        {
            string path = EditorUtility.SaveFilePanel(
                "Save Scene JSON",
                "",
                "SceneExport.json",
                "json"
            );

            if (!string.IsNullOrEmpty(path))
            {
                exportPath = path;
            }
        }

        EditorGUILayout.TextField("Export Path", exportPath);

        if (GUILayout.Button("Export Scene"))
        {
            ExportScene();
        }
    }

    private void ExportScene()
    {
        var allObjects = GameObject.FindObjectsOfType<GameObject>();
        var exportList = new List<Dictionary<string, object>>();

        foreach (var go in allObjects)
        {
            if (!go.scene.IsValid()) continue;
            if (HasDoNotExportInHierarchy(go)) continue;

            var objData = new Dictionary<string, object>
            {
                // ★ここがポイント：Prefab由来ならPrefabの元名を使う
                ["name"] = GetStablePrefabName(go),

                ["Transform"] = new Dictionary<string, object>
                {
                    ["position"] = RoundVector3(go.transform.localPosition),
                    ["rotation"] = RoundQuaternion(go.transform.localRotation),
                    ["scale"] = RoundVector3(go.transform.localScale)
                }
            };

            foreach (var comp in go.GetComponents<MonoBehaviour>())
            {
                if (comp is IJsonExportable exportable)
                {
                    string key = exportable.ExportKey();
                    var data = exportable.ExportData();
                    objData[key] = data;
                }
            }

            exportList.Add(objData);
        }

        var json = JsonConvert.SerializeObject(new { objects = exportList }, Formatting.Indented);
        File.WriteAllText(exportPath, json);

        EditorUtility.DisplayDialog("Export Complete", $"Scene exported to:\n{exportPath}", "OK");
    }

    // ===== ここから追加 =====

    /// <summary>
    /// シーン上の名前(例: "Plant_6 Variant (2)")ではなく、
    /// Prefab由来なら元Prefab名(例: "Plant_6")を返す。
    /// Prefabじゃない場合は、"(2)" 等だけ削って返す。
    /// </summary>
    private static string GetStablePrefabName(GameObject go)
    {
        // Prefab / Prefab Variant の元アセット（source）を取得
        var source = PrefabUtility.GetCorrespondingObjectFromSource(go);
        if (source != null)
        {
            return source.name; // ← "Plant_6"
        }

        // Prefabじゃないオブジェクト向け： "xxx (2)" だけ削る（保険）
        return StripUnitySuffix(go.name);
    }

    /// <summary>
    /// Unityが自動で付ける " (2)" や " (Clone)" を削る
    /// ※ 文字列に " (" が含まれる場合はそこから前を採用
    /// </summary>
    private static string StripUnitySuffix(string name)
    {
        int idx = name.IndexOf(" (");
        if (idx >= 0)
            return name.Substring(0, idx);
        return name;
    }

    // ===== 追加ここまで =====

    private static bool HasDoNotExportInHierarchy(GameObject go)
    {
        while (go != null)
        {
            if (go.GetComponent<DoNotExport>() != null)
                return true;
            go = go.transform.parent ? go.transform.parent.gameObject : null;
        }
        return false;
    }

    private static Dictionary<string, float> RoundVector3(Vector3 v)
    {
        return new Dictionary<string, float>
        {
            ["x"] = RoundFloat(v.x),
            ["y"] = RoundFloat(v.y),
            ["z"] = RoundFloat(v.z)
        };
    }

    private static Dictionary<string, float> RoundQuaternion(Quaternion q)
    {
        return new Dictionary<string, float>
        {
            ["x"] = RoundFloat(q.x),
            ["y"] = RoundFloat(q.y),
            ["z"] = RoundFloat(q.z),
            ["w"] = RoundFloat(q.w)
        };
    }

    private static float RoundFloat(float value)
    {
        return Mathf.Round(value * 10000f) / 10000f;
    }
}
