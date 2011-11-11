#include <gtkmm/main.h>

#include "HelloWorldWindow.hpp"

int main( int argc, char *argv[] )
{
  // Initialization
  Gtk::Main kit( argc, argv );

  // Create a hello world window object
  HelloWorldWindow example;

  // gtkmm main loop
  Gtk::Main::run( example );
  return 0;
}
