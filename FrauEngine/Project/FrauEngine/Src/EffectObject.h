#pragma once
#include "Object.h"

#include "EffekseerDX12.h"
namespace frauEngine {

	class EffectObject : public frauEngine::Object {

	protected:
		frauEngine::Effect* resource;
		bool enable = true;
		Vector3 pos;
		Vector3 angle;
		Vector3 scale;
		int time;
		int endTime;

	public:
		void SetResource(frauEngine::Effect* _effect);


		void SetPos(Vector3 _pos);
		void SetScale(Vector3 _scale);
		void SetAngle(Vector3 _angle);
		void SetTime(int _time);
		void SetEndTime(int _endTime);
		void SetAll(Vector3 _pos, Vector3 _angle, Vector3 _scale, int _endTime);


		Vector3 GetPos() { return pos; };
		Vector3 GetScale() { return scale; };
		Vector3 GetAngle() { return angle; };
		int GetTime() { return time; }
		int GetEndTime() { return endTime; }


		void Play();

		void DrawImGUI();
		void Update();
		void Draw();
	};

}