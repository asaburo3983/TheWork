#pragma once
#include "LowApplication.h"
#include "DXTK.h"
#include "Object.h"

namespace obj {
	class ImageObject : public obj::Object {

	protected:
		dxtk::Image2D* resource;

		bool enable = true;
		Vector2 pos;
		Vector2 scale;
		float angle = 0.0f;
		float alpha = 1.0f;
		bool turnLR = false;
		bool turnUD = false;
		bool center = true;

	public:
		void SetResource(dxtk::Image2D* _image);

		void SetPos(Vector2 _pos);
		void SetScale(Vector2 _scale);
		void SetAngle(float _angle);
		void SetAlpha(float _alpha);
		void SetTurn(bool _turnLR, bool _turnUD);
		void SetCenter(bool _center);

		bool GetEnable() { return enable; };
		Vector2 GetPos() { return pos; };
		Vector2 GetScale() { return scale; };
		float GetAngle() { return angle; };
		float GetAlpha() { return alpha; };
		bool GetTurnLR() { return turnLR; };
		bool GetTurnUD() { return turnUD; };
		bool GetCenter() { return center; }

		void SetAll(Vector2 _pos, Vector2 _scale, float _angle, float _alpha);
	public:
		void DrawImGUI();
		void Updata();
		void Draw();
	};

}