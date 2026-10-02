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
}