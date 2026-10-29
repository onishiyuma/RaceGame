using System;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
using Newtonsoft.Json;

[Serializable]
public class StageData
{
    public List<ObjectDefinition> objects =
        new List<ObjectDefinition>();
}

[Serializable]
public class ObjectDefinition
{
    public string type;
    public string assetId;

    public Vector3Data position;
    public QuaternionData rotation;
    public Vector3Data scale;
    public Dictionary<string, object> properties = new Dictionary<string, object>();
}

[Serializable]
public class Vector3Data
{
    public float x;
    public float y;
    public float z;

    public Vector3Data(Vector3 value)
    {
        x = value.x;
        y = value.y;
        z = value.z;
    }
}

[Serializable]
public class QuaternionData
{
    public float x;
    public float y;
    public float z;
    public float w;

    public QuaternionData(Quaternion value)
    {
        x = value.x;
        y = value.y;
        z = value.z;
        w = value.w;
    }
}

public static class StageExporter
{
    [MenuItem("Tools/Stage/Export Stage JSON")]
    public static void Export()
    {
        StageData stageData = new StageData();

        StageObjectInfo[] stageObjects =
            UnityEngine.Object.FindObjectsByType<StageObjectInfo>(
                FindObjectsSortMode.None
            );

        foreach (StageObjectInfo stageObject in stageObjects)
        {
            ObjectDefinition definition =
                new ObjectDefinition();

            Transform transform =
                stageObject.transform;

            definition.type =
                stageObject.type.ToString();

            definition.assetId =
                stageObject.GetExportAssetId();

            definition.position =
                new Vector3Data(transform.position);

            definition.rotation =
                new QuaternionData(transform.rotation);

            definition.scale =
                new Vector3Data(transform.lossyScale);

            foreach (StageProperty property in stageObject.properties)
            {
                if (string.IsNullOrWhiteSpace(property.key))
                {
                    Debug.LogError(
                        $"{stageObject.name}: プロパティ名が空です。"
                    );
                    return;
                }

                if (!definition.properties.TryAdd(
                    property.key,
                    property.GetValue()))
                {
                    Debug.LogError(
                        $"{stageObject.name}: プロパティ名が重複しています: {property.key}"
                    );
                    return;
                }
            }

            // スポーン地点のIDが未設定ならオブジェクト名を使用する。
            if (stageObject.type == StageObjectType.SpawnPoint)
            {
                if (!definition.properties.ContainsKey("spawnPointId"))
                {
                    definition.properties.Add(
                        "spawnPointId",
                        stageObject.gameObject.name
                    );
                }
            }

            stageData.objects.Add(definition);
        }

        string json =
            JsonConvert.SerializeObject(
                stageData,
                Formatting.Indented
            );

        string path =
            EditorUtility.SaveFilePanel(
                "Save Stage JSON",
                "",
                "stage.json",
                "json"
            );

        if (string.IsNullOrEmpty(path))
        {
            return;
        }

        File.WriteAllText(path, json);

        Debug.Log(
            "Stage JSON exported: " + path
        );
    }

}
