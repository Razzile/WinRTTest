#pragma once
#include "pch.h"

using namespace winrt;
using namespace Windows::Gaming::Input;
using namespace Windows::Foundation;
using namespace std::chrono_literals;

class GamepadHandler {
 public:
  GamepadHandler() : new_reading(), old_reading() {
    Gamepad::GamepadAdded({this, &GamepadHandler::GamepadAdded});
  }
  ~GamepadHandler() = default;

  bool ButtonJustPressed(const GamepadButtons selection) {
    bool new_selection_pressed =
        (selection == (new_reading.Buttons & selection));
    bool old_selection_pressed =
        (selection == (old_reading.Buttons & selection));

    return new_selection_pressed && !old_selection_pressed;
  }

  bool ButtonJustReleased(const GamepadButtons selection) {
    bool new_selection_pressed =
        (GamepadButtons::None == (new_reading.Buttons & selection));

    bool old_selection_pressed =
        (GamepadButtons::None == (old_reading.Buttons & selection));

    return new_selection_pressed && !old_selection_pressed;
  }

  void GamepadAdded(const IInspectable&, const Gamepad& gamepad) {
    std::lock_guard lock(mtx);

    auto it = std::find(begin(gamepads), end(gamepads), gamepad);

    if (it == end(gamepads)) {
      gamepads.push_back(gamepad);
    }
  }

  IAsyncOperation<winrt::hstring> Poll() {
    for (const auto& gamepad : gamepads) {
      std::lock_guard lock(mtx);
      old_reading = new_reading;
      new_reading = gamepad.GetCurrentReading();

      GamepadButtons btns = new_reading.Buttons;

      std::vector<std::wstring> buttons;
      if (true/*ButtonJustPressed(btns)*/) {
        if ((btns & GamepadButtons::A) == GamepadButtons::A) {
          buttons.push_back(L"A");
        }
        if ((btns & GamepadButtons::B) == GamepadButtons::B) {
          buttons.push_back(L"B");
        }
        if ((btns & GamepadButtons::X) == GamepadButtons::X) {
          buttons.push_back(L"X");
        }
        if ((btns & GamepadButtons::Y) == GamepadButtons::Y) {
          buttons.push_back(L"Y");
        }
        if ((btns & GamepadButtons::DPadDown) == GamepadButtons::DPadDown) {
          buttons.push_back(L"DPad Down");
        }
        if ((btns & GamepadButtons::DPadLeft) == GamepadButtons::DPadLeft) {
          buttons.push_back(L"DPad Left");
        }
        if ((btns & GamepadButtons::DPadRight) == GamepadButtons::DPadRight) {
          buttons.push_back(L"DPad Right");
        }
        if ((btns & GamepadButtons::DPadUp) == GamepadButtons::DPadUp) {
          buttons.push_back(L"DPad Up");
        }
        if ((btns & GamepadButtons::LeftShoulder) ==
            GamepadButtons::LeftShoulder) {
          buttons.push_back(L"Left Bumper");
        }
        if ((btns & GamepadButtons::RightShoulder) ==
            GamepadButtons::RightShoulder) {
          buttons.push_back(L"Right Bumper");
        }
        if ((btns & GamepadButtons::Menu) == GamepadButtons::Menu) {
          buttons.push_back(L"Start");
        }
        if ((btns & GamepadButtons::View) == GamepadButtons::View) {
          buttons.push_back(L"Select");
        }
        if ((btns & GamepadButtons::RightThumbstick) ==
            GamepadButtons::RightThumbstick) {
          buttons.push_back(L"Right Thumbstick");
        }
        if ((btns & GamepadButtons::LeftThumbstick) ==
            GamepadButtons::LeftThumbstick) {
          buttons.push_back(L"Left Thumbstick");
        }

        if (buttons.size() < 1) {
          co_return winrt::hstring{};
        }

        std::wstring text = L"Button(s) Pressed: ";
        std::wstring seperator = L"";
        for (const std::wstring& btn : buttons) {
          text += seperator + btn;
          seperator = L",";
        }

        co_return winrt::hstring{text};
      }
    }
  }

 private:
  std::mutex mtx;
  std::vector<Gamepad> gamepads;

  GamepadReading old_reading;
  GamepadReading new_reading;
};
