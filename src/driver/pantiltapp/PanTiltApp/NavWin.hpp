#ifndef HELLOWORLDWINDOW_HPP_FILE
#define HELLOWORLDWINDOW_HPP_FILE

#include <gtkmm/window.h>

class NavWin: public Gtk::Window
{
public:
  NavWin(void);
  ~NavWin(void);

protected:
  virtual bool on_motion_notify_event(GdkEventMotion* event);
  virtual bool on_button_press_event(GdkEventButton* event);
  virtual bool on_button_release_event(GdkEventButton* event);
};

#endif
