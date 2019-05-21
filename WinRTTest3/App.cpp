#include "pch.h"

#include "App.h"
#include "MainPage.h"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace WinRTTest3;
using namespace WinRTTest3::implementation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of
/// authored code executed, and as such is the logical equivalent of main() or
/// WinMain().
/// </summary>
App::App() {
  InitializeComponent();
  Suspending({this, &App::OnSuspending});

#if defined _DEBUG && \
    !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
  UnhandledException(
      [this](IInspectable const&, UnhandledExceptionEventArgs const& e) {
        if (IsDebuggerPresent()) {
          auto errorMessage = e.Message();
          __debugbreak();
        }
      });
#endif
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other
/// entry points will be used such as when the application is launched to open a
/// specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const& e) {
  Frame rootFrame{nullptr};

  m_textBlock.Text(L"Press a button on your controller");
  m_textBlock.FontSize(44.0);
  m_textBlock.HorizontalAlignment(HorizontalAlignment::Center);
  m_textBlock.VerticalAlignment(VerticalAlignment::Center);

  auto window = Window::Current();
  window.Content(m_textBlock);
  window.Activate();

  auto gamepad_runner{m_gamepadHandler.Poll()};

  RunGamepadManager();
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is
/// saved without knowing whether the application will be terminated or resumed
/// with the contents of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending([[maybe_unused]] IInspectable const& sender,
                       [[maybe_unused]] SuspendingEventArgs const& e) {
  // Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(IInspectable const&,
                             NavigationFailedEventArgs const& e) {
  throw hresult_error(
      E_FAIL, hstring(L"Failed to load Page ") + e.SourcePageType().Name);
}

fire_and_forget App::RunGamepadManager() {
  while (true) {
    // switch to background thread
    co_await winrt::resume_background();
    winrt::hstring pressed = m_gamepadHandler.Poll().get();
    if (pressed.size() > 0) {
      // return to UI thread
      co_await winrt::resume_foreground(m_textBlock.Dispatcher());
      m_textBlock.Text(pressed);
    }
    co_await 100ms;
  }
}
