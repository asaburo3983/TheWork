#pragma once
#include "LowApplication.h"
#include "Object.h"
#include "Light.h"

namespace frauEngine {

	struct DirectionalLightParam {
		Vector3 pos;
		Vector3 target;
		Vector3 up;
		float fov;
		Vector3 vector;
		Vector3 color = Vector3(1, 1, 1);
	};
	struct PointLightParam {
		Vector3 pos;
		float range;
		Vector3 color = Vector3(1, 1, 1);
	};
	struct SpotLightParam {
		Vector3 pos;
		float range;
		Vector3 target;
		float anglerRange;
		Vector3 color = Vector3(1, 1, 1);
	};

	class LightsObject : public frauEngine::Object {
	protected:

		float ambientPower = 0.5f;

		DirectionalLightParam directionalLightParam;
		PointLightParam pointLightParam[POINT_LIGHT];
		SpotLightParam spotLightParam[SPOT_LIGHT];

	public:

		void SetAmbientLight(float _power);

		void SetDirectionalLight(DirectionalLightParam _param);

		void SetPointLight(int _lightNum, PointLightParam _param);

		void SetSpotLight(int _lightNum, SpotLightParam _param);

		void DrawImGUI();

		void Update();
	};
}