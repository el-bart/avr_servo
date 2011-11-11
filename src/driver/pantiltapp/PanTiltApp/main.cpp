#include <gtkmm/main.h>

#include "NavWin.hpp"

int main(int argc, char **argv)
{
  Gtk::Main kit(false);
  NavWin    win;
  Gtk::Main::run(win);
  return 0;
}
