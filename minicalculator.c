#include <gtk/gtk.h>

int main (int argc, char *argv[]) {
   gtk_init (&argc, &argv);
   
   GtkWidget *gui; // the main window
   GtkWidget *display; // the display of the minicalculator, an entry widget
   GtkWidget *box; // contains the display and the table a grid widget
   GtkWidget *table; // a grid that ordonates the buttons of the minicalculator
   GtkEntryBuffer *bufy;

   GtkWidget *butt [4][6]; // contains the buttons
   // labels for the buttons, that's why it matches butt's design
   gchar *butt_label [4][6] = {
      "1", "2", "3", "4", "+", "Bin",
      "5", "6", "7", "8", "-", "Dec",
      "9", "0", "A", "B", "*", "Hex",
      "C", "D", "E", "F", "/", "="
   };

   gui = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (gui), "minicalculator");
   // gtk_window_set_default_size (GTK_WINDOW (gui), 400, 300);
   gtk_window_set_position (GTK_WINDOW (gui), GTK_WIN_POS_CENTER);
   g_signal_connect (gui, "destroy", G_CALLBACK (gtk_main_quit), NULL);

   box = gtk_grid_new ();
   table = gtk_grid_new ();

   bufy = gtk_entry_buffer_new ("0", -1);
   display = gtk_entry_new_with_buffer (bufy);
   gtk_entry_set_alignment (GTK_ENTRY (display), 1);
   gtk_editable_set_editable (GTK_EDITABLE (display), FALSE);

   // initializes the buttons with the corresponding labels and attaches them to the grid table  
   int i, j;
   for (i = 0; i < 4; i++) {
      for (j = 0; j < 6; j++) {
         butt [i][j] = gtk_button_new_with_label (butt_label [i][j]);
         gtk_grid_attach (GTK_GRID (table), butt [i][j], j, i, 1, 1);
      }
   }
 
   gtk_grid_attach (GTK_GRID (box), display, 0, 0, 1, 1);
   gtk_grid_attach (GTK_GRID (box), table, 0, 1, 1, 1);

   gtk_container_add (GTK_CONTAINER (gui), box);
   gtk_widget_show_all (gui); 

   gtk_main ();
   return 0;  
}
