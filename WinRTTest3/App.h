#pragma once
#include "App.xaml.g.h"
#include "GamepadHandler.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Controls;

namespace winrt::WinRTTest3::implementation {
struct App : AppT<App> {
 public:
  App();

  void OnLaunched(LaunchActivatedEventArgs const&);
  void OnSuspending(IInspectable const&, SuspendingEventArgs const&);
  void OnNavigationFailed(IInspectable const&,
                          NavigationFailedEventArgs const&);

  fire_and_forget RunGamepadManager();

 private:
  GamepadHandler m_gamepadHandler{};
  TextBlock m_textBlock{};
};
}  // namespace winrt::WinRTTest3::implementation
