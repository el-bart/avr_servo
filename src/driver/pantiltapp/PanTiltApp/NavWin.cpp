#include <iostream>

#include "NavWin.hpp"

using namespace std;


NavWin::NavWin(void)
{
  set_title("mouse position event");
  add_events( Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
  show_all_children();
}

NavWin::~NavWin(void)
{
  cout<<"exit..."<<endl;
}

bool NavWin::on_motion_notify_event(GdkEventMotion* event)
{
  if(event!=NULL)
    cout<<"mouse pos: "<<event->x<<" "<<event->y<<endl;
  return true;
}

bool NavWin::on_button_press_event(GdkEventButton* event)
{
  if(event!=NULL)
    cout<<"press: "<<event->x<<" "<<event->y<<endl;
  return true;
}

bool NavWin::on_button_release_event(GdkEventButton* event)
{
  if(event!=NULL)
    cout<<"release: "<<event->x<<" "<<event->y<<endl;
  return true;
}
