#include <iostream>

#include "HelloWorldWindow.hpp"

using namespace std;


HelloWorldWindow::HelloWorldWindow(void)
{
  set_title("mouse position event");
  add_events( Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
  show_all_children();
}

HelloWorldWindow::~HelloWorldWindow(void)
{
  cout<<"exit..."<<endl;
}

bool HelloWorldWindow::on_motion_notify_event(GdkEventMotion* event)
{
  if(event!=NULL)
    cout<<"mouse pos: "<<event->x<<" "<<event->y<<endl;
  return true;
}

bool HelloWorldWindow::on_button_press_event(GdkEventButton* event)
{
  if(event!=NULL)
    cout<<"press: "<<event->x<<" "<<event->y<<endl;
  return true;
}

bool HelloWorldWindow::on_button_release_event(GdkEventButton* event)
{
  if(event!=NULL)
    cout<<"release: "<<event->x<<" "<<event->y<<endl;
  return true;
}
