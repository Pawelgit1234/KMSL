#include "IoController.hpp"

namespace kmsl
{
	void IoController::moveTo(int x, int y, float t)
	{
        POINT startPos;
        GetCursorPos(&startPos);

        int startX = startPos.x;
        int startY = startPos.y;

        int deltaX = x - startX;
        int deltaY = y - startY;

        int steps = 100;
        float stepTime = t / steps; 

        for (int i = 0; i <= steps; ++i)
        {
            float factor = static_cast<float>(i) / steps;
            int currentX = static_cast<int>(startX + deltaX * factor);
            int currentY = static_cast<int>(startY + deltaY * factor);

            SetCursorPos(currentX, currentY);

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(stepTime * 1000)));
        }
	}

    void IoController::moveBy(int dx, int dy, float t)
    {
        POINT startPos;
        GetCursorPos(&startPos);
        moveTo(dx + startPos.x, dy + startPos.y, t);
    }

    void IoController::scroll(int amount, float t) // -amound down, amount up
    {
        int steps = 100;
        float stepTime = t / steps;
        int scrollAmountPerStep = amount / steps;

        INPUT input;
        input.type = INPUT_MOUSE;
        input.mi.dx = 0;
        input.mi.dy = 0;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;

        for (int i = 0; i < steps; ++i)
        {
            input.mi.mouseData = scrollAmountPerStep;
            SendInput(1, &input, sizeof(INPUT));

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(stepTime * 1000)));
        }

        int remainingScroll = amount % steps;
        if (remainingScroll != 0)
        {
            input.mi.mouseData = remainingScroll;
            SendInput(1, &input, sizeof(INPUT));
        }
    }

    void IoController::type(const std::string& text, float t)
    {
        for (char c : text)
        {
            std::string s(1, c);
            if (std::isupper(c))
                press({ "SHIFT", s }, 0.f);
            else
                press({ s }, 0.f);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(t * 1000)));
        }
    }


    void IoController::press(const std::vector<std::string>& buttons, float t)
    {
        hold(buttons);

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(t * 1000)));

        release(buttons);
    }

    void IoController::hold(const std::vector<std::string>& buttons) {
        std::vector<INPUT> inputs;

        for (const auto& button : buttons) {
            WORD keyCode = getVirtualKeyCode(button);
            if (keyCode == 0) continue;

            if (keyCode >= VK_LBUTTON && keyCode <= VK_XBUTTON2) {
                DWORD mouseFlags;
                switch (keyCode) {
                case VK_LBUTTON: mouseFlags = MOUSEEVENTF_LEFTDOWN; break;
                case VK_RBUTTON: mouseFlags = MOUSEEVENTF_RIGHTDOWN; break;
                case VK_MBUTTON: mouseFlags = MOUSEEVENTF_MIDDLEDOWN; break;
                case VK_XBUTTON1: mouseFlags = MOUSEEVENTF_XDOWN; break;
                case VK_XBUTTON2: mouseFlags = MOUSEEVENTF_XDOWN; break;
                default: continue;
                }
                inputs.push_back(createMouseInput(keyCode, mouseFlags));
            }
            else {
                inputs.push_back(createKeyboardInput(keyCode, 0));
            }
        }

        SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
    }

    void IoController::release(const std::vector<std::string>& buttons) {
        std::vector<INPUT> inputs;

        for (const auto& button : buttons) {
            WORD keyCode = getVirtualKeyCode(button);
            if (keyCode == 0) continue;

            if (keyCode >= VK_LBUTTON && keyCode <= VK_XBUTTON2) {
                DWORD mouseFlags;
                switch (keyCode) {
                case VK_LBUTTON: mouseFlags = MOUSEEVENTF_LEFTUP; break;
                case VK_RBUTTON: mouseFlags = MOUSEEVENTF_RIGHTUP; break;
                case VK_MBUTTON: mouseFlags = MOUSEEVENTF_MIDDLEUP; break;
                case VK_XBUTTON1: mouseFlags = MOUSEEVENTF_XUP; break;
                case VK_XBUTTON2: mouseFlags = MOUSEEVENTF_XUP; break;
                default: continue;
                }
                inputs.push_back(createMouseInput(keyCode, mouseFlags));
            }
            else {
                inputs.push_back(createKeyboardInput(keyCode, KEYEVENTF_KEYUP));
            }
        }

        SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
    }

    bool IoController::getState(const std::string& button)
    {
        WORD keyCode = getVirtualKeyCode(button);
        if (keyCode >= 0x01 && keyCode <= 0xFE)
        {
            SHORT state = GetAsyncKeyState(keyCode);
            return (state & 0x8000) != 0;
        }

        if (keyCode == VK_LBUTTON || keyCode == VK_RBUTTON || keyCode == VK_MBUTTON || keyCode == VK_XBUTTON1 || keyCode == VK_XBUTTON2)
        {
            SHORT state = GetAsyncKeyState(keyCode);
            return (state & 0x8000) != 0;
        }

        return false;
    }

    WORD IoController::getVirtualKeyCode(const std::string& key)
    {
        static std::map<std::string, WORD> keyMap = {
            // alphabet keys
            {"A", 0x41}, {"B", 0x42}, {"C", 0x43}, {"D", 0x44},
            {"E", 0x45}, {"F", 0x46}, {"G", 0x47}, {"H", 0x48},
            {"I", 0x49}, {"J", 0x4A}, {"K", 0x4B}, {"L", 0x4C},
            {"M", 0x4D}, {"N", 0x4E}, {"O", 0x4F}, {"P", 0x50},
            {"Q", 0x51}, {"R", 0x52}, {"S", 0x53}, {"T", 0x54},
            {"U", 0x55}, {"V", 0x56}, {"W", 0x57}, {"X", 0x58},
            {"Y", 0x59}, {"Z", 0x5A},
            {"a", 0x41}, {"b", 0x42}, {"c", 0x43}, {"d", 0x44},
            {"e", 0x45}, {"f", 0x46}, {"g", 0x47}, {"h", 0x48},
            {"i", 0x49}, {"j", 0x4A}, {"k", 0x4B}, {"l", 0x4C},
            {"m", 0x4D}, {"n", 0x4E}, {"o", 0x4F}, {"p", 0x50},
            {"q", 0x51}, {"r", 0x52}, {"s", 0x53}, {"t", 0x54},
            {"u", 0x55}, {"v", 0x56}, {"w", 0x57}, {"x", 0x58},
            {"y", 0x59}, {"z", 0x5A},

            // number keys
            {"0", 0x30}, {"1", 0x31}, {"2", 0x32}, {"3", 0x33},
            {"4", 0x34}, {"5", 0x35}, {"6", 0x36}, {"7", 0x37},
            {"8", 0x38}, {"9", 0x39},

            // control keys
            {"ENTER", VK_RETURN}, {"SPACE", VK_SPACE}, {"SHIFT", VK_SHIFT},
            {"CTRL", VK_CONTROL}, {"ALT", VK_MENU}, {"ESC", VK_ESCAPE},
            {"TAB", VK_TAB}, {"CAPSLOCK", VK_CAPITAL}, {"BACKSPACE", VK_BACK},
            {"DELETE", VK_DELETE}, {"INSERT", VK_INSERT}, {"HOME", VK_HOME},
            {"END", VK_END}, {"PAGEUP", VK_PRIOR}, {"PAGEDOWN", VK_NEXT},
            {"UP", VK_UP}, {"DOWN", VK_DOWN}, {"LEFT", VK_LEFT}, {"RIGHT", VK_RIGHT},

            // functional keys
            {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4},
            {"F5", VK_F5}, {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8},
            {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},

            // keys on mouse
            {"LBUTTON", VK_LBUTTON}, {"RBUTTON", VK_RBUTTON},
            {"MBUTTON", VK_MBUTTON}, {"XBUTTON1", VK_XBUTTON1},
            {"XBUTTON2", VK_XBUTTON2},

            // modificators
            {"LCTRL", VK_LCONTROL}, {"RCTRL", VK_RCONTROL},
            {"LSHIFT", VK_LSHIFT}, {"RSHIFT", VK_RSHIFT},
            {"LALT", VK_LMENU}, {"RALT", VK_RMENU}
        };

        auto it = keyMap.find(key);
        if (it != keyMap.end())
            return it->second;

        return 0;
    }

    INPUT IoController::createKeyboardInput(WORD keyCode, DWORD dwFlags)
    {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = keyCode;
        input.ki.dwFlags = dwFlags;
        return input;
    }

    INPUT IoController::createMouseInput(WORD keyCode, DWORD dwFlags)
    {
        INPUT input = { 0 };
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = dwFlags;
        return input;
    }
};
