#pragma once
#include "LowApplication.h"
#include "DXTK.h"
#include "Object.h"

namespace frauEngine {
	class ImageObject : public frauEngine::Object {

	protected:
		frauEngine::Image2D* resource;

		bool enable = true;
		Vector2 pos;
		Vector2 scale;
		float angle = 0.0f;
		float alpha = 1.0f;
		bool turnLR = false;
		bool turnUD = false;
		bool center = true;

	public:
		void SetResource(frauEngine::Image2D* _image);

		void SetPos(Vector2 _pos);
		void SetScale(Vector2 _scale);
		void SetAngle(float _angle);
		void SetAlpha(float _alpha);
		void SetTurn(bool _turnLR, bool _turnUD);
		void SetCenter(bool _center);

		bool GetEnable() { return enable; };
		Vector2 GetPos() { return pos; };
		Vector2 GetPosLerp(Vector2 _lerp) {
			Vector2 ret;
			float minX = pos.X - (float)GetWidth() / 2.0 * scale.X;
			float maxX = pos.X + (float)GetWidth() / 2.0 * scale.X;

			float minY = pos.Y - (float)GetHeight() / 2.0 * scale.Y;		
			float maxY = pos.Y + (float)GetHeight() / 2.0 * scale.Y;

			if (center == false) {
				minX = pos.X;
				minY = pos.Y;
				maxX = pos.X + GetWidth() * scale.X;
				maxY = pos.Y + GetHeight() * scale.Y;
			}

			ret.X = std::lerp(minX, maxX, _lerp.X);
			ret.Y = std::lerp(minY, maxY, _lerp.Y);

			return ret;
		}
		Vector2 GetScale() { return scale; };
		float GetAngle() { return angle; };
		float GetAlpha() { return alpha; };
		bool GetTurnLR() { return turnLR; };
		bool GetTurnUD() { return turnUD; };
		bool GetCenter() { return center; }
		UINT GetWidth() { return resource->GetWidth(); };
		UINT GetHeight() { return resource->GetHeight(); };

		void SetAll(Vector2 _pos = Vector2(0, 0), Vector2 _scale = Vector2(1, 1), float _angle = 0.0f, float _alpha = 1.0f);

		void DrawImGUI();
		void Update();
		void Draw();
		bool Hit(int _x, int _y);
	};

}