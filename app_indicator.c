#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include "app_indicator.h"
#include "common.h"
#include "pthread.h"

static GtkActionEntry entries[] = {
  { "New",      "document-new", "_New", "<control>N",
    "Create a new file", G_CALLBACK (activate_action) },
  { "Open",     "document-open", "_Open", "<control>O",
    "Open a file", G_CALLBACK (activate_action) },
  { "Save",     "document-save", "_Save", "<control>S",
    "Save file", G_CALLBACK (activate_action) },
  { "Quit",     "application-exit", "_Quit", "<control>Q",
    "Exit the application", G_CALLBACK (shutdown_gtk) },
};
static guint n_entries = G_N_ELEMENTS (entries);

static const gchar *ui_info =
"<ui>"
"  <popup name='IndicatorPopup'>"
"    <menuitem action='New' />"
"    <menuitem action='Open' />"
"    <menuitem action='Save' />"
"    <menuitem action='Quit' />"
"  </popup>"
"</ui>";

static pthread_t gtk_thread;

void shutdown_gtk(void) {
  gtk_main_quit();
  shairport_shutdown(0);
  pthread_exit(&gtk_thread);
}

static void
activate_action (GtkAction *action)
{
        const gchar *name = gtk_action_get_name (action);
        GtkWidget *dialog;

        dialog = gtk_message_dialog_new (NULL,
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_INFO,
                                         GTK_BUTTONS_CLOSE,
                                         "You activated action: \"%s\"",
                                         name);

        g_signal_connect (dialog, "response",
                          G_CALLBACK (gtk_widget_destroy), NULL);

        gtk_widget_show (dialog);
}

static void *init_indicator (void *pconn)
{
  GObject *dead;
  GtkWidget *indicator_menu;
  GtkActionGroup *action_group;
  GtkUIManager *uim;
  AppIndicator *indicator;
  GError *error = NULL;

  gtk_init (0,NULL);

  action_group = gtk_action_group_new ("AppActions");
  gtk_action_group_add_actions (action_group,
                                entries, n_entries,
                                dead);

  /* Menus */
  uim = gtk_ui_manager_new ();

  gtk_ui_manager_insert_action_group (uim, action_group, 0);

  if (!gtk_ui_manager_add_ui_from_string (uim, ui_info, -1, &error))
    {
      g_message ("Failed to build menus: %s\n", error->message);
      g_error_free (error);
      error = NULL;
    }

  /* Indicator */
  indicator = app_indicator_new ("shairport",
                                 "/home/calv/tmp/test/ice.png",
                                 APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

  indicator_menu = gtk_ui_manager_get_widget (uim, "/ui/IndicatorPopup");

  app_indicator_set_status (indicator, APP_INDICATOR_STATUS_ACTIVE);
  app_indicator_set_attention_icon (indicator, "indicator-messages-new");

  app_indicator_set_menu (indicator, GTK_MENU (indicator_menu));

  gtk_main ();

  return NULL;
}

void startup_gtk(void) {
  pthread_create(&gtk_thread, NULL, &init_indicator, NULL);
}
