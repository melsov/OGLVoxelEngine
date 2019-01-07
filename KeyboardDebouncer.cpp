#include "KeyboardDebouncer.h"



namespace VEInput
{

	void KeyboardDebouncer::CheckKeyPress()
	{
		KeyInfo foundKey = unknownKey;
		float time = glfwGetTime();
		auto it = keys.begin();
		for (; it != keys.end(); ++it)
		{
			int k = it->first;
			if (glfwGetKey(window, k) == GLFW_PRESS)
			{
				auto info = it->second;
				if (time - info.downTime > pressIntervalSeconds)
				{
					info.downTime = time;
					foundKey = info;
					break;
				}
			}
		}

		if (foundKey.keyCode != GLFW_KEY_UNKNOWN)
		{
			lastKey = foundKey;
			keys[it->first] = foundKey;
		}
		else
		{
			lastKey = unknownKey;
		}
	}

	void KeyboardDebouncer::Track(const int * key, int howMany)
	{
		for (int i = 0; i < howMany; ++i) Track(*(key + i));
	}

	KeyInfo KeyboardDebouncer::GetLastKey()
	{
		return lastKey;
	}

}
