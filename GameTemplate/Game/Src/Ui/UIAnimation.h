#pragma once
/**
 * @file UIAnimation.h。
 * @brief UIアニメーションクラス。
 * @author Tanimoto。
 * @date 2026/10/05。
 */

/*UIアニメーションの基底クラス。*/
class UIAnimationBase
{
public:
	virtual ~UIAnimationBase() {};/*純粋仮想デストラクタ。*/

public:
	void Update();/*更新処理。*/

protected:/*列挙型。*/

	/*アニメーションステップ。*/
	enum EnAnimationStep
	{
		enAnimationStep_Min,/*下限。*/
		enAnimationStep_Max/*上限。*/
	};

protected:/*メンバ関数。*/

	/**
	* @brief 線形補間処理。
	* @param percent 補完率。
	*/
	virtual void LarpProcess(float percent) = 0;

public:/*メンバ関数。*/

	/**
	* @brief リセット処理。
	*/
	void Reset();

	/**
	* @brief アニメーションの再生終了したか？
	* @return trueなら再生終了している。
	*/
	inline bool IsEnd() const
	{
		return m_isEnd;
	}

	/**
	* @brief 補完関数を設定。
	* @param easingFunc 進行度(0.0f～1.0f)を受け取り、計算後の進行度を返す関数。
	*/
	inline void SetEasingFunction(std::function<float(float)> easingFunc)
	{
		m_easingFunc = easingFunc;
	}

	/**
	* @brief 逆再生を有効にする。
	*/
	inline void EnableReverse()
	{
		m_isReverse = true;
	}

	/**
	* @brief 逆再生を無効にする。
	*/
	inline void DisableReverse()
	{
		m_isReverse = false;
	}

	/**
	* @brief 逆再生中？
	* @return trueなら逆再生。
	*/
	inline bool IsReverse()
	{
		return m_isReverse;
	}

protected:/*メンバ変数。*/
	SpriteRender* m_render = nullptr;/*アニメーションさせるスプライト。*/
	float m_elapsedTime = 0.0f;/*経過した割合。*/
	float m_targetTime = 1.0f;/*ターゲットの割合。*/
	float m_playSpeed = 1.0f;/*アニメーションの再生速度。*/
	float m_startDelayTime = 0.0f;/*アニメーションを開始する前の遅延時間。*/
	float m_endDelayTime = 0.0f;/*アニメーションを終了した後の遅延時間。*/
	EnAnimationStep m_currentStep = enAnimationStep_Min;/*現在のステップ。*/
	bool m_isLoop = false;/*ループするか ?*/
	bool m_isEnd = false;/*アニメーションの再生終了したか？*/
	bool m_isCompleted = false;/*処理が完了したか ?*/
	bool m_isReverse = false;/*逆再生するか ?*/
	bool m_isCurrentReverseState = false;/*現在の逆再生の状態。*/
	std::function<float(float)> m_easingFunc = [](float t) {return t; };/*カスタム補完関数(デフォルトは線形保管)。*/
};

/*UIアニメーションのパラメーターをチェーンメソッド形式で設定するための基底クラス。*/
template <typename Derived>
class UIAnimationChainer : public UIAnimationBase
{
public:

	/**
	 * @brief アニメーションを再生させるスプライトを設定。
	 * @param render スプライト。
	 * @return 自身の参照。
	 */
	inline Derived& SetRender(SpriteRender* render)
	{
		m_render = render;
		return static_cast<Derived&>(*this);
	}

	/**
	 * @brief ターゲットの割合の設定。
	 * @param targetTime 割合。
	 * @return 自身の参照。
	 */
	inline Derived& SetTargetTime(const float targetTime)
	{
		m_targetTime = targetTime;
		return static_cast<Derived&>(*this);
	}

	/**
	 * @brief アニメーションの再生速度の設定。
	 * @param playSpeed 再生速度。
	 * @return 自身の参照。
	 */
	inline Derived& SetPlaySpeed(const float playSpeed)
	{
		m_playSpeed = playSpeed;
		return static_cast<Derived&>(*this);
	}

	/**
	 * @brief ループ再生するか？
	 * @param isLoop trueならループ再生。
	 * @return 自身の参照。
	 */
	inline Derived& SetLoop(const bool isLoop)
	{
		m_isLoop = isLoop;
		return static_cast<Derived&>(*this);
	}

	/**
	 * @brief アニメーションの再生開始後の遅延時間の設定。
	 * @param startDelayTime 遅延時間。
	 * @return 自身の参照。
	 */
	inline Derived& SetStartDelayTime(const float startDelayTime)
	{
		m_startDelayTime = startDelayTime;
		return static_cast<Derived&>(*this);
	}

	/**
	 * @brief アニメーションの再生終了後の遅延時間の設定。
	 * @param endDelayTime 遅延時間。
	 * @return 自身の参照。
	 */
	inline Derived& SetEndDelayTime(const float endDelayTime)
	{
		m_endDelayTime = endDelayTime;
		return static_cast<Derived&>(*this);;
	}
};

/*座標を変えるUIアニメーションクラス。*/
class PositionUIAnimation : public UIAnimationChainer<PositionUIAnimation>
{
public:
	PositionUIAnimation() {};/*コンストラクタ。*/
	~PositionUIAnimation() {};/*デストラクタ。*/

public:

	/**
	 * @brief 元の位置の設定。
	 * @param basePosition 元の位置。
	 * @return 自身の参照。
	 */
	inline PositionUIAnimation& SetBasePosition(const Vector3 basePosition)
	{
		m_basePosition = basePosition;
		return *this;
	}

	/**
	 * @brief ターゲットの位置の設定。
	 * @param targetPosition ターゲットの位置。
	 * @return 自身の参照。
	 */
	inline PositionUIAnimation& SetTargetPosition(const Vector3 targetPosition)
	{
		m_targetPosition = targetPosition;
		return *this;
	}

	/**
	 * @brief 相対的な位置の設定。
	 * @param x X方向。
	 * @param y Y方向。
	 * @param z Z方向。
	 * @return 自身の参照。
	 */
	inline PositionUIAnimation& SetRelativePosition(float x, float y, float z)
	{
		m_basePosition = m_render->GetPosition();
		m_targetPosition = Vector3{ m_basePosition.x + x,m_basePosition.y + y,m_basePosition.z + z };
		return *this;
	}

	/**
	 * @brief 相対的な位置(X方向)の設定。
	 * @param x X方向。
	 * @return 自身の参照。
	 */
	inline PositionUIAnimation& SetRelativePositionX(float x)
	{
		m_basePosition = m_render->GetPosition();
		m_targetPosition = Vector3{ m_basePosition.x + x,m_basePosition.y,m_basePosition.z };
		return *this;
	}

	/**
	 * @brief 相対的な位置(Y方向)の設定。
	 * @param y Y方向。
	 * @return 自身の参照。
	 */
	inline PositionUIAnimation& SetRelativePositionY(float y)
	{
		m_basePosition = m_render->GetPosition();
		m_targetPosition = Vector3{ m_basePosition.x,m_basePosition.y + y,m_basePosition.z };
		return *this;
	}

	/**
	 * @brief 相対的な位置(Z方向)の設定。
	 * @param z Z方向。
	 * @return 自身の参照。
	 */
	inline PositionUIAnimation& SetRelativePositionZ(float z)
	{
		m_basePosition = m_render->GetPosition();
		m_targetPosition = Vector3{ m_basePosition.x,m_basePosition.y,m_basePosition.z + z };
		return *this;
	}

private:

	/**
	* @brief 線形補間処理。
	* @param percent 補完率。
	*/
	void LarpProcess(float percent) override;

private:/*メンバ変数。*/
	Vector3 m_basePosition = Vector3::Zero;/*元の位置。*/
	Vector3 m_targetPosition = Vector3::Zero;/*ターゲットの位置。*/
};

/*回転を変えるUIアニメーションクラス。*/
class RotationUIAnimation : public UIAnimationChainer<RotationUIAnimation>
{
public:
	RotationUIAnimation(){}/*コンストラクタ。*/
	~RotationUIAnimation() {};/*デストラクタ。*/

public:

	/**
	 * @brief 元の回転の設定。
	 * @param baseRotation 元の回転。
	 * @return 自身の参照。
	 */
	inline RotationUIAnimation& SetBaseRotation(const Quaternion baseRotation)
	{
		m_baseRotation = baseRotation;
		return *this;
	}

	/**
	 * @brief ターゲットの回転の設定。
	 * @param targetRotation ターゲットの回転。
	 * @return 自身の参照。
	 */
	inline RotationUIAnimation& SetTargetRotation(const Quaternion targetRotation)
	{
		m_targetRotation = targetRotation;
		return *this;
	}

	/**
	 * @brief 相対的な回転(X方向)の設定。
	 * @param angle 角度(Deg)。
	 * @return 自身の参照。
	 */
	RotationUIAnimation& SetRelativeRotationX(float angle)
	{
		m_baseRotation = m_render->GetRotation();
		Quaternion rot;
		rot.SetRotationDegX(angle);
		m_targetRotation = m_baseRotation * rot;
		return *this;
	}

	/**
	 * @brief 相対的な回転(Y方向)の設定。
	 * @param angle 角度(Deg)。
	 * @return 自身の参照。
	 */
	RotationUIAnimation& SetRelativeRotationY(float angle)
	{
		m_baseRotation = m_render->GetRotation();
		Quaternion rot;
		rot.SetRotationDegY(angle);
		m_targetRotation = m_baseRotation * rot;
		return *this;
	}

	/**
	 * @brief 相対的な回転(Z方向)の設定。
	 * @param angle 角度(Deg)。
	 * @return 自身の参照。
	 */
	RotationUIAnimation& SetRelativeRotationZ(float angle)
	{
		m_baseRotation = m_render->GetRotation();
		Quaternion rot;
		rot.SetRotationDegZ(angle);
		m_targetRotation = m_baseRotation * rot;
		return *this;
	}

private:

	/**
	* @brief 線形補間処理。
	* @param percent 補完率。
	*/
	void LarpProcess(float percent) override;

private:
	Quaternion m_baseRotation = Quaternion::Identity;/*元の回転。*/
	Quaternion m_targetRotation = Quaternion::Identity;/*ターゲットの回転。*/
};

/*大きさを変えるアニメーションクラス。*/
class ScaleUIAnimation : public UIAnimationChainer<ScaleUIAnimation>
{
public:
	ScaleUIAnimation() {};/*コンストラクタ。*/
	~ScaleUIAnimation() {};/*デストラクタ。*/

public:

	/**
	 * @brief 元の大きさの設定。
	 * @param baseScale 元の大きさ。
	 * @return 自身の参照。
	 */
	inline ScaleUIAnimation& SetBaseScale(const Vector2 baseScale)
	{
		m_baseScale = baseScale;
		return *this;
	}

	/**
	 * @brief ターゲットの大きさの設定。
	 * @param targetScale ターゲットの大きさ。
	 * @return 自身の参照。
	 */
	inline ScaleUIAnimation& SetTargetScale(const Vector2 targetScale)
	{
		m_targetScale = targetScale;
		return *this;
	}

	/**
	 * @brief 相対的な大きさ(XY方向)の設定。
	 * @param xy XY方向。
	 * @return 自身の参照。
	 */
	inline ScaleUIAnimation& SetRelativeScale(float xy)
	{
		m_baseScale = Vector2{ m_render->GetScale().x,m_render->GetScale().y };
		m_targetScale = Vector2{ m_baseScale.x + xy ,m_baseScale.y + xy };
		return *this;
	}

	/**
	 * @brief 相対的な大きさの設定。
	 * @param x X方向。
	 * @param y Y方向。
	 * @return 自身の参照。
	 */
	inline ScaleUIAnimation& SetRelativeScale(float x, float y)
	{
		m_baseScale = Vector2{ m_render->GetScale().x,m_render->GetScale().y };
		m_targetScale = Vector2{ m_baseScale.x + x ,m_baseScale.y + y };
		return *this;
	}

	/**
	 * @brief 相対的な大きさ(X方向)の設定。
	 * @param x X方向。
	 * @return 自身の参照。
	 */
	inline ScaleUIAnimation& SetRelativeScaleX(float x)
	{
		m_baseScale = Vector2{ m_render->GetScale().x,m_render->GetScale().y };
		m_targetScale = Vector2{ m_baseScale.x + x,m_baseScale.y };
		return *this;
	}

	/**
	 * @brief 相対的な大きさ(Y方向)の設定。
	 * @param x X方向
	 * @return 自身の参照。
	 */
	inline ScaleUIAnimation& SetRelativeScaleY(float y)
	{
		m_baseScale = Vector2{ m_render->GetScale().x,m_render->GetScale().y };
		m_targetScale = Vector2{ m_baseScale.x,m_baseScale.y + y };
		return *this;
	}

private:

	/**
	* @brief 線形補間処理。
	* @param percent 補完率。
	*/
	void LarpProcess(float percent) override;

private:
	Vector2 m_baseScale = Vector2::Zero;/*元の大きさ。*/
	Vector2 m_targetScale = Vector2::Zero;/*ターゲットの大きさ。*/
};

/*色を変えるアニメーションクラス。*/
class ColorUIAnimation : public UIAnimationChainer<ColorUIAnimation>
{
public:
	ColorUIAnimation(){}/*コンストラクタ。*/
	~ColorUIAnimation() {};/*デストラクタ。*/

public:

	/**
	 * @brief 元の色の設定。
	 * @param baseColor 元の色。
	 * @return 自身の参照。
	 */
	inline ColorUIAnimation& SetBaseColor(const Vector4 baseColor)
	{
		m_baseColor = baseColor;
		return *this;
	}

	/**
	 * @brief ターゲットの色の設定。
	 * @param targetColor ターゲットの色。
	 * @return 自身の参照。
	 */
	inline ColorUIAnimation& SetTargetColor(const Vector4 targetColor)
	{
		m_targetColor = targetColor;
		return *this;
	}

	/**
	 * @brief 相対的な色(XYZ方向)の設定。
	 * @param xyz XYZ方向。
	 * @return 自身の参照。
	 */
	inline ColorUIAnimation& SetRelativeColor(float xyz)
	{
		m_baseColor = m_render->GetMulColor();
		m_targetColor = { m_render->GetMulColor().x + xyz,m_render->GetMulColor().y + xyz,m_render->GetMulColor().z + xyz,m_render->GetMulColor().a };
		return *this;
	}

	/**
	 * @brief 相対的な色の設定。
	 * @param x X方向。
	 * @param y Y方向。
	 * @param z Z方向。
	 * @return 自身の参照。
	 */
	inline ColorUIAnimation& SetRelativeColor(float x, float y, float z)
	{
		m_baseColor = m_render->GetMulColor();
		m_targetColor = { m_render->GetMulColor().x + x,m_render->GetMulColor().y + y,m_render->GetMulColor().z + z,m_render->GetMulColor().a };
		return *this;
	}

	/**
	 * @brief 相対的な色(X方向)の設定。
	 * @param x X方向。
	 * @return 自身の参照。
	 */
	inline ColorUIAnimation& SetRelativeColorX(float x)
	{
		m_baseColor = m_render->GetMulColor();
		m_targetColor = { m_render->GetMulColor().x + x,m_render->GetMulColor().y,m_render->GetMulColor().z,m_render->GetMulColor().a };
		return *this;
	}

	/**
	 * @brief 相対的な色(Y方向)の設定。
	 * @param x Y方向。
	 * @return 自身の参照。
	 */
	inline ColorUIAnimation& SetRelativeColoY(float y)
	{
		m_baseColor = m_render->GetMulColor();
		m_targetColor = { m_render->GetMulColor().x,m_render->GetMulColor().y + y,m_render->GetMulColor().z,m_render->GetMulColor().a };
		return *this;
	}

	/**
	 * @brief 相対的な色(Z方向)の設定。
	 * @param x Z方向。
	 * @return 自身の参照。
	 */
	inline ColorUIAnimation& SetRelativeColorZ(float z)
	{
		m_baseColor = m_render->GetMulColor();
		m_targetColor = { m_render->GetMulColor().x,m_render->GetMulColor().y,m_render->GetMulColor().z + z,m_render->GetMulColor().a };
		return *this;
	}

private:

	/**
	* @brief 線形補間処理。
	* @param percent 補完率。
	*/
	void LarpProcess(float percent) override;

private:
	Vector4 m_baseColor = Vector4::White;/*元の色。*/
	Vector4 m_targetColor = Vector4::White;/*ターゲットの色。*/
};

/*透明度を変えるアニメーションクラス。*/
class AlphaUIAnimation : public UIAnimationChainer<AlphaUIAnimation>
{
public:
	AlphaUIAnimation(){}/*コンストラクタ。*/
	~AlphaUIAnimation() {};/*デストラクタ。*/

public:

	/**
	 * @brief 元の透明度の設定。
	 * @param baseAlpha 元の透明度。
	 * @return 自身の参照。
	 */
	inline AlphaUIAnimation& SetBaseAlpha(const float baseAlpha)
	{
		m_baseAlpha = baseAlpha;
		return *this;
	}

	/**
	 * @brief ターゲットの透明度の設定。
	 * @param targetAlpha ターゲットの透明度。
	 * @return 自身の参照。
	 */
	inline AlphaUIAnimation& SetTargetAlpha(const float targetAlpha)
	{
		m_targetAlpha = targetAlpha;
		return *this;
	}

	/**
	 * @brief 相対的な透明度の設定。
	 * @param alpha 透明度。
	 * @return 自身の参照。
	 */
	inline AlphaUIAnimation& SetRelativeAlpha(float alpha)
	{
		m_baseAlpha = m_render->GetMulColor().a;
		m_targetAlpha = m_render->GetMulColor().a + alpha;
		return *this;
	}

private:

	/**
	* @brief 線形補間処理。
	* @param percent 補完率。
	*/
	void LarpProcess(float percent) override;

private:
	float m_baseAlpha = 0.0f;/*元の透明度。*/
	float m_targetAlpha = 0.0f;/*ターゲットの透明度。*/
};