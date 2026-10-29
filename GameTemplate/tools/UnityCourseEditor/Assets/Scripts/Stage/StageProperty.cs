using System;
using UnityEngine;

public enum StagePropertyType
{
    String,
    Int,
    Float,
    Bool
}

[Serializable]
public class StageProperty
{
    public string key;
    public StagePropertyType type;

    public string stringValue;
    public int intValue;
    public float floatValue;
    public bool boolValue;

    public object GetValue()
    {
        switch (type)
        {
            case StagePropertyType.String:
                return stringValue;

            case StagePropertyType.Int:
                return intValue;

            case StagePropertyType.Float:
                return floatValue;

            case StagePropertyType.Bool:
                return boolValue;

            default:
                return null;
        }
    }
}