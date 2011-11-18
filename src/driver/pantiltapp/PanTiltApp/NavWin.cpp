#include <iostream>
#include <mutex>

#include "NavWin.hpp"

using namespace std;


struct NavWin::CommThread
{
  typedef std::unique_lock<std::mutex> Lock;

  CommThread(ServoCtrl::CommDevicePtrNN dev, ServoCtrl::ServoName nameX, ServoCtrl::ServoName nameY):
    srvX_{nameX, dev, false},
    srvY_{nameY, dev, false},
    posXsent_{0},
    posYsent_{0},
    quit_{false}
  {
    setX(127);
    setY(127);
  }

  void quit(void)
  {
    Lock lock{m_};
    quit_=true;
  }

  void setX(uint8_t x)
  {
    Lock lock{m_};
    posX_=x;
  }
  uint8_t getX(void) const
  {
    Lock lock{m_};
    return posX_;
  }

  void setY(uint8_t y)
  {
    Lock lock{m_};
    posY_=y;
  }
  uint8_t getY(void) const
  {
    Lock lock{m_};
    return posY_;
  }

  void run(void)
  {
    while(!quitNow())
    {
      if( posXsent_!=getX() )
      {
        const uint8_t p=getX();
        send(srvX_, p);
        posXsent_=p;
      }

      if( posYsent_!=getY() )
      {
        const uint8_t p=getY();
        send(srvY_, p);
        posYsent_=p;
      }

      // TODO: make this conditional
      while(posXsent_==getX() && posYsent_==getY() && !quitNow())
      {
        const struct timespec ts={0, 100*1000};
        nanosleep(&ts, nullptr);
      }
    }
  }

private:
  bool quitNow(void) const
  {
    Lock lock{m_};
    return quit_;
  }

  bool send(ServoCtrl::Servo &srv, const uint8_t pos)
  {
    cout<<"sedning "<<int{pos}<<endl;
    try
    {
      srv.setPos(pos);
      return true;
    }
    catch(const std::exception &ex)
    {
      cerr<<"oops: "<<ex.what()<<" - proceeding"<<endl;
    }
    return false;
  }

  mutable std::mutex m_;

  ServoCtrl::Servo srvX_;
  ServoCtrl::Servo srvY_;

  uint8_t posXsent_;
  uint8_t posYsent_;

  uint8_t posX_;
  uint8_t posY_;

  bool quit_;
}; // struct NavWin::CommThread



NavWin::NavWin(ServoCtrl::CommDevicePtrNN dev, ServoCtrl::ServoName nameX, ServoCtrl::ServoName nameY):
  pressed_{false},
  thImpl_{ new CommThread{dev, nameX, nameY} },
  th_{ [&thImpl_]{ thImpl_->run(); } }
{
  set_title("mouse position event");
  add_events( Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
  show_all_children();
}

NavWin::~NavWin(void)
{
  cout<<"exit..."<<endl;
  thImpl_->quit();
  thImpl_->setX(166);
  thImpl_->setY(166);
  if(th_.joinable())
    th_.join();
}


bool NavWin::on_motion_notify_event(GdkEventMotion* event)
{
  if(event==NULL)
    return false;

  //cout<<"mouse pos: "<<event->x<<" "<<event->y<<endl;
  if(!pressed_)
    return true;

  posUpdate(posX_, lastX_, event->x);
  thImpl_->setX(posX_);
  //lastX_=event->x;

  posUpdate(posY_, lastY_, event->y);
  thImpl_->setY(posY_);
  //lastY_=event->y;

  return true;
}

bool NavWin::on_button_press_event(GdkEventButton* event)
{
  if(event==NULL)
    return false;

  //cout<<"press: "<<event->x<<" "<<event->y<<endl;
  pressed_=true;
  lastX_  =event->x;
  lastY_  =event->y;
  return true;
}


bool NavWin::on_button_release_event(GdkEventButton* event)
{
  if(event==NULL)
    return false;

  //cout<<"release: "<<event->x<<" "<<event->y<<endl;
  pressed_=false;
  return true;
}


void NavWin::posUpdate(uint8_t &pos, double last, double now)
{
  const int dp=posDiff(last, now);
  int       p =pos+dp;
  // limit it a bit...
  if(p<50)
    p=50;
  if(p>200)
    p=200;
  // ok
  pos=p;
}


int NavWin::posDiff(double last, double now)
{
  return (now-last)/7;

  if(last<now)
    return +1;
  if(last>now)
    return -1;
  return 0;
}
