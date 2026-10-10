using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;

[CustomEditor(typeof(StageObjectInfo))]
[CanEditMultipleObjects]
public class StageObjectInfoEditor : Editor
{
    public override void OnInspectorGUI()
    {
        DrawDefaultInspector();

        StageObjectInfo source = Selection.activeGameObject != null
            ? Selection.activeGameObject.GetComponent<StageObjectInfo>() : null;
        if (source == null || Array.IndexOf(targets, source) < 0)
        {
            source = (StageObjectInfo)target;
        }

        using (new EditorGUI.DisabledScope(true))
        {
            EditorGUILayout.ObjectField("コピー元", source, typeof(StageObjectInfo), true);
            EditorGUILayout.ObjectField("Mesh", source.GetAssetMesh(), typeof(Mesh), false);
        }

        EditorGUILayout.HelpBox(
            "Meshは同じGameObject上のみ参照します。各ボタンは値を一度コピーする操作です。" +
            "同じMeshへの適用はコピー元のシーン内（非アクティブを含む）が対象です。" +
            "StageObjectInfoがない対象には追加します。適用後はシーンを保存してください。",
            MessageType.Info);

        using (new EditorGUI.DisabledScope(!IsSceneObject(source.gameObject) || EditorApplication.isPlaying))
        {
            if (GUILayout.Button("Mesh名からIDを設定"))
            {
                var objects = new List<GameObject>();
                foreach (UnityEngine.Object item in targets)
                    objects.Add(((StageObjectInfo)item).gameObject);
                Apply(objects, obj =>
                {
                    MeshFilter filter = obj.GetComponent<MeshFilter>();
                    return filter != null && filter.sharedMesh != null ? filter.sharedMesh.name : null;
                }, "Mesh名からAsset IDを設定");
            }

            using (new EditorGUI.DisabledScope(string.IsNullOrEmpty(source.assetId)))
            {
                if (GUILayout.Button("選択中のオブジェクトに適用"))
                {
                    string id = source.assetId;
                    Apply(Selection.gameObjects, obj => id, "選択中のAsset IDを設定");
                }

                using (new EditorGUI.DisabledScope(source.GetAssetMesh() == null))
                {
                    if (GUILayout.Button("同じMeshのオブジェクトすべてに適用"))
                    {
                        Mesh mesh = source.GetAssetMesh();
                        string id = source.assetId;
                        var objects = new List<GameObject>();
                        foreach (GameObject root in source.gameObject.scene.GetRootGameObjects())
                        {
                            foreach (MeshFilter filter in root.GetComponentsInChildren<MeshFilter>(true))
                            {
                                if (filter.sharedMesh == mesh)
                                    objects.Add(filter.gameObject);
                            }
                        }
                        Apply(objects, obj => id, "同じMeshのAsset IDを設定");
                    }
                }
            }
        }
    }

    private static bool IsSceneObject(GameObject obj)
    {
        return obj != null && !EditorUtility.IsPersistent(obj)
            && obj.scene.IsValid() && obj.scene.isLoaded
            && !EditorSceneManager.IsPreviewScene(obj.scene)
            && PrefabStageUtility.GetPrefabStage(obj) == null;
    }

    private static void Apply(IEnumerable<GameObject> objects, Func<GameObject, string> getId, string undoName)
    {
        Undo.IncrementCurrentGroup();
        int group = Undo.GetCurrentGroup();
        Undo.SetCurrentGroupName(undoName);
        int count = 0;
        try
        {
            foreach (GameObject obj in objects)
            {
                if (!IsSceneObject(obj)) continue;
                string id = getId(obj);
                if (id == null) continue;

                StageObjectInfo info = obj.GetComponent<StageObjectInfo>();
                if (info == null) info = Undo.AddComponent<StageObjectInfo>(obj);
                if (info == null || info.assetId == id) continue;

                Undo.RecordObject(info, undoName);
                info.assetId = id;
                if (PrefabUtility.IsPartOfPrefabInstance(info))
                    PrefabUtility.RecordPrefabInstancePropertyModifications(info);
                EditorUtility.SetDirty(info);
                EditorSceneManager.MarkSceneDirty(obj.scene);
                count++;
            }
        }
        finally
        {
            Undo.CollapseUndoOperations(group);
        }
        Debug.Log($"{undoName}: {count}個を変更しました。");
    }
}
