/*#include<gtk/gtk.h>

#include "objects.h"
#include "gui_util.h"
#include "button_callbacks.h"

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *fixed;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "gtk life sim on c++");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    fixed = gtk_fixed_new();

    add_button(fixed, "hello", 0, 0, 50, 50, (void*)test);
    add_button(fixed, "hello", 200, 200, 50, 50, (void*)test);

    gtk_container_add(GTK_CONTAINER(window), fixed);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}*/

#include<gtkmm/application.h>
#include "gui/window.hpp"

int main(int argc, char *argv[]){
    auto app = Gtk::Application::create(argc, argv, "my.life.sim");

    GameWindow window;
    return app->run(window);
}