#include "k2EnginePreCompile.h"
#include "SceneLight.h"

namespace nsK2Engine
{
	//初期化
	void SceneLight::Init()
	{
		//ディレクションライトの方向
		m_light.directionLight.direction.x = 0.0f;
		m_light.directionLight.direction.y = -1.0f;
		m_light.directionLight.direction.z = 1.0f;

		//正規化
		m_light.directionLight.direction.Normalize();

		//ディレクションライトのカラー
		m_light.directionLight.color.x = 0.5f;
		m_light.directionLight.color.y = 0.5f;
		m_light.directionLight.color.z = 0.5f;

		//環境光
		m_light.ambientLight.x = 0.55f;
		m_light.ambientLight.y = 0.55f;
		m_light.ambientLight.z = 0.55f;
		m_light.eyePos = g_camera3D->GetPosition();

		//半球ライトの地面色
		m_light.hemLight.groundColor = Vector3(0.1f, 0.1f, 0.1f);
		//半球ライトの天球色
		m_light.hemLight.skyColor = Vector3(0.05f, 0.1f, 0.2f);
		//半球ライトの地面の法線
		m_light.hemLight.groundNormal = Vector3(0.0f, 1.0f, 0.0f);

		//カメラの位置を設定
		m_lightCamera.SetPosition(GetLightCamera().GetTarget() + Vector3(0, 600, -300));
		//カメラの注視点を設定
		m_lightCamera.SetTarget(GetLightCamera().GetTarget());
		//上方向を設定
		m_lightCamera.SetUp(1, 0, 0);
		//ライトビュープロジェクション行列の計算
		m_lightCamera.Update();
		m_light.mLVP = m_lightCamera.GetViewProjectionMatrix();
		m_light.lightPos.Set(m_lightCamera.GetPosition());
	}

	//ライトカメラの更新処理
	void SceneLight::LightCameraUpdate()
	{
		//カメラの位置を設定
		Vector3 lightCamPos = g_camera3D->GetTarget();
		lightCamPos.x += m_lightCameraPosition.x;
		lightCamPos.y = m_lightCameraPosition.y;
		lightCamPos.z += m_lightCameraPosition.z;
		m_lightCamera.SetPosition(lightCamPos);
		//カメラの注視点を設定
		m_lightCamera.SetTarget(g_camera3D->GetTarget());
		m_light.mLVP = m_lightCamera.GetViewProjectionMatrix();
		m_light.lightPos.Set(m_lightCamera.GetPosition());
		//ライトビュープロジェクション行列の計算
		m_lightCamera.Update();
	}
}