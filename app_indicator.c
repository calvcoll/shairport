#ifdef __unix__

#include "app_indicator.h"

static pthread_t gtk_thread;
GtkWidget *coverart;
GtkWidget *album_cover;

void indicator_set_image_data(const char *buffer, int length, metadata *data) {
    printf("%s\n", "got to indicator");
    GdkPixbufLoader *loader;
    GdkPixbuf *pixbuf;

    loader = gdk_pixbuf_loader_new();
    gdk_pixbuf_loader_write(loader, (guchar*) &buffer, length, NULL);
    pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);

    coverart = gtk_image_new_from_pixbuf (pixbuf);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(album_cover), coverart);
    gtk_menu_item_set_label(GTK_MENU_ITEM(album_cover), data->artist);
}

void shutdown_gtk(void) {
    gtk_main_quit();
    shairport_shutdown(0);
    pthread_exit(&gtk_thread);
}

static void *init_indicator (void *pconn)
{
    // setup_icon();
    GtkWidget *indicator_menu;
    GtkWidget *quit_button;
    AppIndicator *indicator;

    gtk_init (0,NULL);

    indicator = app_indicator_new ( "shairport",
                                    "/home/calv/tmp/shairport.png",
                                    APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

    app_indicator_set_status (indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_attention_icon (indicator, "indicator-messages-new");

    indicator_menu = gtk_menu_new();
    album_cover = gtk_image_menu_item_new();
    coverart = gtk_image_new_from_file("/home/calv/tmp/shairport.png");
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(album_cover), coverart);
    gtk_menu_item_set_label(GTK_MENU_ITEM(album_cover), "Not playing");

    quit_button = gtk_menu_item_new_with_label("Quit");
    //
    // coverart = gtk_image_new_from_file("/home/calv/tmp/shairport.png");
    // printf("pixsize %d\n", gtk_image_get_pixel_size(GTK_IMAGE(coverart)));

    gtk_image_menu_item_set_always_show_image(GTK_IMAGE_MENU_ITEM(album_cover), TRUE);

    gtk_menu_attach(GTK_MENU(indicator_menu), album_cover, 1, 2, 0, 1);
    // gtk_menu_attach(GTK_MENU(indicator_menu), coverartlabel, 2, 3, 0, 1);
    gtk_menu_attach(GTK_MENU(indicator_menu), quit_button, 1, 2, 1, 2);

    app_indicator_set_menu (indicator, GTK_MENU (indicator_menu));
    gtk_widget_show_all(indicator_menu);

    g_signal_connect(GTK_OBJECT(quit_button), "activate", G_CALLBACK(shutdown_gtk), NULL);

    gtk_main ();

    return NULL;
}

void startup_gtk(void) {
    pthread_create(&gtk_thread, NULL, &init_indicator, NULL);
}

#endif
