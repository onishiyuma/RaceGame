using System;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

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
                GetAssetId(stageObject.gameObject);

            definition.position =
                new Vector3Data(transform.position);

            definition.rotation =
                new QuaternionData(transform.rotation);

            definition.scale =
                new Vector3Data(transform.lossyScale);

            stageData.objects.Add(definition);
        }

        string json =
            JsonUtility.ToJson(stageData, true);

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

    private static string GetAssetId(GameObject obj)
    {
        MeshFilter meshFilter =
            obj.GetComponent<MeshFilter>();

        if (meshFilter == null)
        {
            return "";
        }

        Mesh mesh =
            meshFilter.sharedMesh;

        if (mesh == null)
        {
            return "";
        }

        string assetPath =
            AssetDatabase.GetAssetPath(mesh);

        if (string.IsNullOrEmpty(assetPath))
        {
            return "";
        }

        string fileName =
            Path.GetFileNameWithoutExtension(assetPath);

        return fileName;
    }
}