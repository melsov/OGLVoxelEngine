#pragma once
#ifndef _KEYBOARD_DEBOUNCER_H_
#define _KEYBOARD_DEBOUNCER_H_

#include <map>
#include <glfw3.h>

namespace VEInput
{
	enum KeyState
	{
		KEY_PRESSED, KEY_DOWN, KEY_RELEASED
	};

	struct KeyInfo
	{
		float downTime;
		int state = KEY_RELEASED;
		int keyCode;
	};

	class KeyboardDebouncer
	{
	public:
		KeyboardDebouncer(GLFWwindow* _window, float _pressIntervalSeconds = 0.2) :
			window(_window),
			pressIntervalSeconds(_pressIntervalSeconds) 
		{
			unknownKey = { 0.0, KEY_RELEASED, GLFW_KEY_UNKNOWN };
			lastKey = unknownKey;
		}

		~KeyboardDebouncer() {}

		void CheckKeyPress();

		void Track(int key)
		{
			KeyInfo kinfo;
			kinfo.downTime = 0;
			kinfo.state = KEY_RELEASED;
			kinfo.keyCode = key;
			keys[key] = kinfo;
		}

		void Track(const int* key, int howMany);

		KeyInfo GetLastKey();



	private:
		GLFWwindow* window;
		std::map<int, KeyInfo> keys;
		float pressIntervalSeconds;
		KeyInfo unknownKey;
		KeyInfo lastKey;
	};
}

#endif

