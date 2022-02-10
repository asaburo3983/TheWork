#pragma once
#include "LowApplication.h"

namespace obj {
	class Object {
		
	protected:
		const char* name;
		bool imgui = false;
		bool enable = true;
	public:
		void SetName(const char* _name) { name = _name; };
		const char* GetName() { return name; };
		void SetEnable(bool _enable) { enable = _enable; };
		void GetEnable() { enable; };
		void SetImGuiEnable(bool _enable) { imgui = _enable; };

		bool* GetImGuiEnable() { return &imgui; };
		virtual void DrawImGUI() = 0;
		virtual void Updata() = 0;

	};
}