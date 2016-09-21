/*************************************************************************
	> File Name: file_explorer.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2016年09月20日 星期二 23时31分52秒
 ************************************************************************/

#include<gtk/gtk.h>

GtkWidget *window;

typedef struct
{
  GtkAction action;
} ToolMenuAction;

typedef struct
{
  GtkActionClass parent_class;
} ToolMenuActionClass;

G_DEFINE_TYPE(ToolMenuAction, tool_menu_action, GTK_TYPE_ACTION)

static void
tool_menu_action_class_init (ToolMenuActionClass *class)
{
  GTK_ACTION_CLASS (class)->toolbar_item_type = GTK_TYPE_MENU_TOOL_BUTTON;
}

static void
tool_menu_action_init (ToolMenuAction *action)
{
}


static void
activate_action (GtkAction *action)
{
  const gchar *name = gtk_action_get_name (action);
  const gchar *typename = G_OBJECT_TYPE_NAME (action);

  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (GTK_WINDOW (window),
                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                   GTK_MESSAGE_INFO,
                                   GTK_BUTTONS_CLOSE,
                                   "You activated action: \"%s\" of type \"%s\"",
                                    name, typename);

  /* Close dialog on user response */
  g_signal_connect (dialog,
                    "response",
                    G_CALLBACK (gtk_widget_destroy),
                    NULL);

  gtk_widget_show (dialog);
}

void close_(GtkWidget *widget, gpointer *data){
	printf("%s\n", __func__);
	gtk_main_quit();
}

static GtkActionEntry entries[] = {
  { "FileMenu", NULL, "_File" },               /* name, stock id, label */
  { "OpenMenu", NULL, "_Open" },               /* name, stock id, label */
  { "HelpMenu", NULL, "_Help" },               /* name, stock id, label */
  { "New", GTK_STOCK_NEW,                      /* name, stock id */
    "_New", "<control>N",                      /* label, accelerator */
    "Create a new file",                       /* tooltip */
    G_CALLBACK (activate_action) },
  { "Save", GTK_STOCK_SAVE,                    /* name, stock id */
    "_Save","<control>S",                      /* label, accelerator */
    "Save current file",                       /* tooltip */
    G_CALLBACK (activate_action) },
  { "SaveAs", GTK_STOCK_SAVE,                  /* name, stock id */
    "Save _As...", NULL,                       /* label, accelerator */
    "Save to a file",                          /* tooltip */
    G_CALLBACK (activate_action) },
  { "Quit", GTK_STOCK_QUIT,                    /* name, stock id */
    "_Quit", "<control>Q",                     /* label, accelerator */
    "Quit",                                    /* tooltip */
    G_CALLBACK (activate_action) },
};
static guint n_entries = G_N_ELEMENTS (entries);

static const gchar *ui_info =
"<ui>"
"  <menubar name='MenuBar'>"
"    <menu action='FileMenu'>"
"      <menuitem action='New'/>"
"      <menuitem action='Open'/>"
"      <menuitem action='Save'/>"
"      <menuitem action='SaveAs'/>"
"      <separator/>"
"      <menuitem action='Quit'/>"
"    </menu>"
"  </menubar>"
"</ui>";


int main(int argc, char *argv[]){
	GtkUIManager *merge;
	GError *error = NULL;
	GtkWidget *bar;
	GtkWidget *contents;
	GtkWidget *table;
        GtkActionGroup *action_group;
        GtkAction *open_action;	
	GtkWidget *sw;


	gtk_init(&argc, &argv);
	merge = gtk_ui_manager_new ();
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect (GTK_OBJECT(window), "destroy", 
						GTK_SIGNAL_FUNC(close_), NULL);
	gtk_window_set_title (GTK_WINDOW (window), "Application Window");
	table = gtk_table_new (1, 5, FALSE);

        gtk_container_add (GTK_CONTAINER (window), table);	

	action_group = gtk_action_group_new ("AppWindowActions");
        open_action = g_object_new (tool_menu_action_get_type (),
				  "name", "Open",
				  "label", "_Open",
				  "tooltip", "Open a file",
				  "stock-id", GTK_STOCK_OPEN,
				  NULL);
      	gtk_action_group_add_action (action_group, open_action);
      	g_object_unref (open_action);
	gtk_action_group_add_actions (action_group,
				    entries, n_entries,
				    window);


	g_object_set_data_full (G_OBJECT (window), "ui-manager", merge,
			      g_object_unref);
	gtk_ui_manager_insert_action_group (merge, action_group, 0);
	if (!gtk_ui_manager_add_ui_from_string (merge, ui_info, -1, &error))
	{
	  g_message ("building menus failed: %s", error->message);
	  g_error_free (error);
	}

	bar = gtk_ui_manager_get_widget (merge, "/MenuBar");
        gtk_widget_show (bar);
	
	gtk_table_attach (GTK_TABLE (table),
			bar,
                        /* X direction */          /* Y direction */
                        0, 1,                      0, 1,
                        GTK_EXPAND | GTK_FILL,     0,
                        0,                         0);	

	contents = gtk_text_view_new ();
      	gtk_widget_grab_focus (contents);

	sw = gtk_scrolled_window_new (NULL, NULL);
      	gtk_container_add (GTK_CONTAINER (sw),
                         contents);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                      GTK_POLICY_AUTOMATIC,
                                      GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                           GTK_SHADOW_IN);
	gtk_table_attach (GTK_TABLE (table),
                        sw,
                        /* X direction */       /* Y direction */
                        0, 1,                   3, 4,
                        GTK_EXPAND | GTK_FILL,  GTK_EXPAND | GTK_FILL,
                        0,                      0);

        gtk_window_set_default_size (GTK_WINDOW (window),
                           200, 200);
	//contents = gtk_text_view_new ();
     	//gtk_widget_grab_focus (contents);

      	//gtk_container_add (GTK_CONTAINER (sw),
      	//                   contents);
	//gtk_widget_show(window);
	gtk_widget_show_all (window);
	gtk_main();

	return 0;
}
