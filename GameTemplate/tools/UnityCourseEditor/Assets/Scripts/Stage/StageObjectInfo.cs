using UnityEngine;

public enum StageObjectType
{
    Prop,
    Coin,
    ItemBox,
    BoostPad,
    CheckPoint,
    SpawnPoint,
    Goal,
    CameraTrack,
    finishLine,
}

public class StageObjectInfo : MonoBehaviour
{
    public StageObjectType type = StageObjectType.Prop;

    [Tooltip("JSONへ出力するID。空の場合は同じGameObjectのMesh名を使用します。")]
    public string assetId = "";

    // Keep the original exporter rule: only the MeshFilter on this object.
    public Mesh GetAssetMesh()
    {
        MeshFilter meshFilter = GetComponent<MeshFilter>();
        return meshFilter != null ? meshFilter.sharedMesh : null;
    }

    public string GetExportAssetId()
    {
        if (!string.IsNullOrEmpty(assetId))
        {
            return assetId;
        }

        Mesh mesh = GetAssetMesh();
        return mesh != null ? mesh.name : "";
    }
}
