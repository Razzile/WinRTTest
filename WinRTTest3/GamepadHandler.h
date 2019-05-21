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

      if (ButtonJustPressed(btns)) {
        switch (btns) {
          case GamepadButtons::A:
            co_return L"Button Pressed: A";
          case GamepadButtons::B:
             co_return L"Button Pressed: B";
          case GamepadButtons::X:
             co_return L"Button Pressed: X";
          case GamepadButtons::Y:
             co_return L"Button Pressed: Y";
          case GamepadButtons::DPadDown:
             co_return L"Button Pressed: DPad Down";
          case GamepadButtons::DPadLeft:
             co_return L"Button Pressed: DPad Left";
          case GamepadButtons::DPadRight:
             co_return L"Button Pressed: DPad Right";
          case GamepadButtons::DPadUp:
             co_return L"Button Pressed: DPad Up";
          case GamepadButtons::LeftShoulder:
             co_return L"Button Pressed: Left Bumper";
          case GamepadButtons::RightShoulder:
             co_return L"Button Pressed: Right Bumper";
          case GamepadButtons::LeftThumbstick:
             co_return L"Button Pressed: Left Thumbstick";
          case GamepadButtons::RightThumbstick:
             co_return L"Button Pressed: Right Thumbstick";
          case GamepadButtons::Menu:
             co_return L"Button Pressed: Start";
          case GamepadButtons::View:
             co_return L"Button Pressed: Select";
          default:
             co_return L"Button Pressed: {Unknown}";
            break;
        }
      }
    }
  }

 private:
  std::mutex mtx;
  std::vector<Gamepad> gamepads;

  GamepadReading old_reading;
  GamepadReading new_reading;
};
