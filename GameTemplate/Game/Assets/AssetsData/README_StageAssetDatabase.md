# StageAssetDatabase JSON の書き方

このフォルダの JSON ファイルでは、ステージで使用する **アセットID** と、
モデル・スケルトン・アニメーションなどのアセット情報の対応を定義します。

`StageAssetDatabase` がこの JSON を読み込み、
アセットIDから必要なアセット情報を取得します。

---

## 基本形式

```json
{
    "Palm": {
        "modelPath": "Assets/model/stage/Palm.tkm"
    },

    "Character": {
        "modelPath": "Assets/model/character/Character.tkm",
        "skeletonPath": "Assets/model/character/Character.tks",
        "animations": [
            {
                "name": "Idle",
                "path": "Assets/model/character/Idle.tka",
                "isLoop": true
            },
            {
                "name": "Run",
                "path": "Assets/model/character/Run.tka",
                "isLoop": true
            }
        ]
    }
}
```

---

## アセットID

```json
"Palm"
```

JSON の一番外側のキーがアセットIDです。

ゲーム側では、このIDを使って対応するアセット情報を取得します。

アセットIDは重複しないようにしてください。

---

## modelPath

```json
"modelPath": "Assets/model/stage/Palm.tkm"
```

モデルファイル（`.tkm`）のパスです。

**必須項目です。**

すべてのアセット定義に設定してください。

---

## skeletonPath

```json
"skeletonPath": "Assets/model/character/Character.tks"
```

スケルトンファイル（`.tks`）のパスです。

スケルトンを使用するモデルにのみ設定します。

**使用しない場合は省略できます。**

例：

```json
{
    "Palm": {
        "modelPath": "Assets/model/stage/Palm.tkm"
    }
}
```

---

## animations

```json
"animations": [
    {
        "name": "Idle",
        "path": "Assets/model/character/Idle.tka",
        "isLoop": true
    }
]
```

モデルで使用するアニメーションを定義します。

アニメーションを使用しない場合は省略できます。

### name

```json
"name": "Idle"
```

アニメーションを識別するための名前です。

同じアセット内では重複しない名前を設定してください。

### path

```json
"path": "Assets/model/character/Idle.tka"
```

アニメーションファイル（`.tka`）のパスです。

### isLoop

```json
"isLoop": true
```

アニメーションをループ再生するかどうかを指定します。

- `true` : ループする
- `false` : ループしない

---

## モデルだけを登録する例

```json
{
    "Tree": {
        "modelPath": "Assets/model/stage/Tree.tkm"
    },

    "Rock": {
        "modelPath": "Assets/model/stage/Rock.tkm"
    }
}
```

---

## スケルトンとアニメーションを持つモデルの例

```json
{
    "Character": {
        "modelPath": "Assets/model/character/Character.tkm",
        "skeletonPath": "Assets/model/character/Character.tks",
        "animations": [
            {
                "name": "Idle",
                "path": "Assets/model/character/Idle.tka",
                "isLoop": true
            },
            {
                "name": "Run",
                "path": "Assets/model/character/Run.tka",
                "isLoop": true
            },
            {
                "name": "Goal",
                "path": "Assets/model/character/Goal.tka",
                "isLoop": false
            }
        ]
    }
}
```

---

## 注意事項

- `modelPath` は必須です。
- `skeletonPath` は必要な場合のみ設定します。
- `animations` は必要な場合のみ設定します。
- アセットIDは重複させないでください。
- 同じアセット内でアニメーションの `name` を重複させないでください。
- パスはプロジェクト内で使用している形式に合わせて記述してください。
- JSON の末尾に余分なカンマを付けないでください。

### 間違った例

```json
{
    "Palm": {
        "modelPath": "Assets/model/stage/Palm.tkm",
    }
}
```

`modelPath` の行の末尾に余分なカンマがあるため、正しい JSON ではありません。

### 正しい例

```json
{
    "Palm": {
        "modelPath": "Assets/model/stage/Palm.tkm"
    }
}
```
