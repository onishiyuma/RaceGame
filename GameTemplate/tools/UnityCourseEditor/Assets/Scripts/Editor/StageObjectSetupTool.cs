using UnityEditor;
using UnityEngine;

public static class StageObjectSetupTool
{
    [MenuItem("Tools/Stage/Add StageObjectInfo To Children")]
    public static void AddStageObjectInfoToChildren()
    {
        foreach (GameObject selectedObject in Selection.gameObjects)
        {
            Transform[] children =
                selectedObject.GetComponentsInChildren<Transform>(true);

            foreach (Transform child in children)
            {
                // 選択した親自身には付けない
                if (child == selectedObject.transform)
                {
                    continue;
                }

                // MeshFilterを持っていないものには付けない
                MeshFilter meshFilter =
                    child.GetComponent<MeshFilter>();

                if (meshFilter == null)
                {
                    continue;
                }

                // すでに付いているなら何もしない
                StageObjectInfo info =
                    child.GetComponent<StageObjectInfo>();

                if (info == null)
                {
                    info =
                        Undo.AddComponent<StageObjectInfo>(
                            child.gameObject
                        );

                    info.type = StageObjectType.Prop;
                }
            }
        }

        Debug.Log(
            "MeshFilterを持つ子オブジェクトにStageObjectInfoを追加しました。"
        );
    }
}