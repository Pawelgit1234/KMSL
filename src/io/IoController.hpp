#pragma once

#define NOMINMAX

#include <string>
#include <map>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <thread>

WORD getVirtualKeyCode(const std::string& key);

namespace kmsl
{
	class IoController
	{
	public:
		static void moveTo(int x, int y, float t);
		static void moveBy(int dx, int dy, float t);
		static void scroll(int amount, float t);
		static void type(const std::string& text, float t);
        static void press(const std::vector<std::string>& buttons, float t);
		static void hold(const std::vector<std::string>& buttons);
		static void release(const std::vector<std::string>& buttons);
		static bool getState(const std::string& button);
	};
}

