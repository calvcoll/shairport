#ifdef __unix__

#include "app_indicator.h"

static pthread_t gtk_thread;
GtkImage *coverart;
GtkImageMenuItem *album_cover;

void indicator_set_image_data(const char *buffer, int length) {
    GdkPixbufLoader *loader;
    GdkPixbuf *pixbuf;

    loader = gdk_pixbuf_loader_new();
    gdk_pixbuf_loader_write(loader, (guchar*) &buffer, length, NULL);
    pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);

    coverart = GTK_IMAGE(gtk_image_new_from_pixbuf (pixbuf));
    gtk_image_menu_item_set_image(album_cover, GTK_WIDGET(coverart));
}

void shutdown_gtk(void) {
    gtk_main_quit();
    shairport_shutdown(0);
    pthread_exit(&gtk_thread);
}

static void *init_indicator (void *pconn)
{
    setup_icon();
    GtkMenu *indicator_menu;
    GtkMenuItem *quit_button;
    AppIndicator *indicator;

    gtk_init (0,NULL);

    indicator = app_indicator_new ( "shairport",
                                    "/home/calv/tmp/shairport.png",
                                    APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

    app_indicator_set_status (indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_attention_icon (indicator, "indicator-messages-new");

    indicator_menu = GTK_MENU(gtk_menu_new());
    album_cover = gtk_image_menu_item_new();

    quit_button = gtk_menu_item_new_with_label("Quit");

    gtk_menu_attach(indicator_menu, album_cover, 0, 1, 1, 2);
    gtk_menu_attach(indicator_menu, quit_button, 1, 2, 2, 3);

    app_indicator_set_menu (indicator, GTK_MENU (indicator_menu));
    gtk_widget_show(album_cover);
    gtk_widget_show(quit_button);

    g_signal_connect(GTK_OBJECT(quit_button), "activate", G_CALLBACK(shutdown_gtk), NULL);

    gtk_main ();

    return NULL;
}

void startup_gtk(void) {
    pthread_create(&gtk_thread, NULL, &init_indicator, NULL);
}

#endif
