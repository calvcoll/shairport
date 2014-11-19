static void activate_action (GtkAction *action);

static GtkActionEntry entries[] = {
  { "New",      "document-new", "_New", "<control>N",
    "Create a new file", G_CALLBACK (activate_action) },
  { "Open",     "document-open", "_Open", "<control>O",
    "Open a file", G_CALLBACK (activate_action) },
  { "Save",     "document-save", "_Save", "<control>S",
    "Save file", G_CALLBACK (activate_action) },
  { "Quit",     "application-exit", "_Quit", "<control>Q",
    "Exit the application", G_CALLBACK (gtk_main_quit) },
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
