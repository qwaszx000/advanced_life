#include <gtkmm/application.h>
#include "gui/window.hpp"

int main(int argc, char *argv[]){
    auto app = Gtk::Application::create(argc, argv, "my.life.sim");

    GameWindow window;
    return app->run(window);
}