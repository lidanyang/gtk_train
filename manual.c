#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <gtk/gtk.h>

#define BUTTON_NEW_INTIALIZE(n) n = gtk_button_new_with_label ("press me")
#define BUTTON_NEW_INTIALIZE_RANGE(button_array, n, m) do{\
									assert(n <= m);\
									int i = n ;\
									for(;i <= m ;i++){\
										BUTTON_NEW_INTIALIZE(button_array[i]);\
									}\
								}while(0)
static gboolean button_press (GtkWidget *, GdkEvent *);
static void menuitem_response (gchar *);
static void menuitem_response_1 (gchar *message, GtkWidget *);
GtkWidget *window;

int main( int   argc,
          char *argv[] )
{
    GtkWidget *menu[2];
    GtkWidget *menu_bar;
    GtkWidget *menu_bar_2;
    GtkWidget *root_menu[2];
    GtkWidget *menu_items;
    GtkWidget *menu_items_;
    GtkWidget *vbox;
    GtkWidget *button[4];
	GSList *group = NULL;
	//GtkWidget *item;
    char buf[128];
    int i;

    gtk_init (&argc, &argv);

    /* create a new window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (GTK_WIDGET (window), 600, 300);
    gtk_window_set_title (GTK_WINDOW (window), "GTK Menu Test");
    g_signal_connect (window, "delete-event",
                      G_CALLBACK (gtk_main_quit), NULL);

    /* Init the menu-widget, and remember -- never
     * gtk_show_widget() the menu widget!! 
     * This is the menu that holds the menu items, the one that
     * will pop up when you click on the "Root Menu" in the app */
    menu[0] = gtk_menu_new ();
    menu[1] = gtk_menu_new ();

    /* Next we make a little loop that makes three menu-entries for "test-menu".
     * Notice the call to gtk_menu_shell_append.  Here we are adding a list of
     * menu items to our menu.  Normally, we'd also catch the "clicked"
     * signal on each of the menu items and setup a callback for it,
     * but it's omitted here to save space. */

    for (i = 0; i < 4; i++)
        {
			/* Copy the names to the buf. */
			sprintf (buf, "Test-undermenu - %d", i);

			/* Create a new menu-item with a name... */
			//menu_items = gtk_menu_item_new_with_label (buf);

			menu_items = gtk_radio_menu_item_new_with_label (group, "This is an example");
			group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menu_items));
			/* ...and add it to the menu. */
			gtk_menu_shell_append (GTK_MENU_SHELL (menu[0]), menu_items);

			/* Do something interesting when the menuitem is selected */
			g_signal_connect_swapped (menu_items, "toggled",
					G_CALLBACK (menuitem_response_1), 
					(gpointer) g_strdup (buf));
			
            /* Show the widget */
            gtk_widget_show (menu_items);

		}
	for(i = 0; i < 4; i++){
			menu_items_ = gtk_menu_item_new_with_label (buf);
			/* ...and add it to the menu. */
			gtk_menu_shell_append (GTK_MENU_SHELL (menu[1]), menu_items_);

			/* Do something interesting when the menuitem is selected */
			g_signal_connect_swapped (menu_items_, "activate",
					G_CALLBACK (menuitem_response), 
					(gpointer) g_strdup (buf));
            /* Show the widget */
            gtk_widget_show (menu_items_);
	}

    /* This is the root menu, and will be the label
     * displayed on the menu bar.  There won't be a signal handler attached,
     * as it only pops up the rest of the menu when pressed. */
    root_menu[0] = gtk_menu_item_new_with_label ("Root Menu");
    root_menu[1] = gtk_menu_item_new_with_label ("Root Menu");

    gtk_widget_show (root_menu[0]);
    gtk_widget_show (root_menu[1]);

    /* Now we specify that we want our newly created "menu" to be the menu
     * for the "root menu" */
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (root_menu[0]), menu[0]);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (root_menu[1]), menu[1]);

    /* A vbox to put a menu and a button in: */
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);

    /* Create a menu-bar to hold the menus and add it to our main window */
    menu_bar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (vbox), menu_bar, FALSE, FALSE, 2);
	gtk_box_reorder_child(GTK_BOX (vbox), menu_bar , 0);
    gtk_widget_show (menu_bar);

    //menu_bar_2 = gtk_menu_bar_new ();
    //gtk_box_pack_start (GTK_BOX (vbox), menu_bar_2, FALSE, FALSE, 2);
	//gtk_box_reorder_child(GTK_BOX (vbox), menu_bar_2 , 5);
    //gtk_widget_show (menu_bar_2);

    /* Create a button to which to attach menu as a popup */
	BUTTON_NEW_INTIALIZE_RANGE(button, 0, 3);
	for(i = 0; i <= 3; i++){
		g_signal_connect_swapped (button[i], "event",
				G_CALLBACK (button_press), 
				menu[0]);
		gtk_box_pack_start (GTK_BOX (vbox), button[i], TRUE, TRUE, 2);
		gtk_box_reorder_child(GTK_BOX (vbox), button[i] , i + 1);
		gtk_widget_show (button[i]);
	}

	
	/* And finally we append the menu-item to the menu-bar -- this is the
	 * "root" menu-item I have been raving about =) */
	for(i = 0; i < 2; i++){
		gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), root_menu[i]);
	}
	//	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar_2), root_menu[1]);
	gtk_widget_show (vbox);
	/* always display the window as the last step so it all splashes on
	 * the screen at once. */
	gtk_widget_show (window);

	gtk_main ();

	return 0;
}

/* Respond to a button-press by posting a menu passed in as widget.
 *
 * Note that the "widget" argument is the menu being posted, NOT
 * the button that was pressed.
 */

static gboolean button_press( GtkWidget *widget,
		GdkEvent *event )
{

	if (event->type == GDK_BUTTON_PRESS) {
		GdkEventButton *bevent = (GdkEventButton *) event; 
		gtk_menu_popup (GTK_MENU (widget), NULL, NULL, NULL, NULL,
				bevent->button, bevent->time);
		/* Tell calling code that we have handled this event; the buck
		 * stops here. */
		return TRUE;
	}

	/* Tell calling code that we have not handled this event; pass it on. */
	return FALSE;
}


/* Print a string when a menu item is selected */

static void menuitem_response(gchar *message)
{
	GtkWidget *dialog, *label, *content_area;

	printf("%s\n", __func__);
	/* Create the widgets */
	dialog = gtk_dialog_new_with_buttons ("Message",
			GTK_WINDOW (window),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_STOCK_OK,
			GTK_RESPONSE_NONE,
			NULL);
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	label = gtk_label_new (message);
	/* Ensure that the dialog box is destroyed when the user responds. */
	g_signal_connect_swapped (dialog,
			"response",
			G_CALLBACK (gtk_widget_destroy),
			dialog);
	/* Add the label, and show:w everything we've added to the dialog. */
	gtk_container_add (GTK_CONTAINER (content_area), label);
	gtk_widget_show_all (dialog);
}

static void menuitem_response_1(gchar *message, GtkWidget *widget)
{
	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM (widget))){
		printf("%s\n", __func__);
		menuitem_response(message);
	}
}
