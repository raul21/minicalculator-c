#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>


// TO DELETE
struct UpdateBuffer {
   GtkEntryBuffer *buffer;
   char text[256];
};

/*
 * Receives a pointer to a widget button and a pointer to a UpdateBuffer structure
 * Updates the buffer withe the text provided by the structure
 */
void cbb_update_buffer (GtkWidget *button, struct UpdateBuffer *buffer) {
   gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (button), buffer->text, -1);
}
// END TO DELETE 

/*
 * The string receives the conversion of the float number to a string
 * str, a pointer to a string where the conversion are stored
 * size, should match the length of the string
 * floaty, the number to be converted
 */
void long_double_to_string (char *str, size_t size, long double floaty) {
   snprintf (str, size, "%0.12Lf", floaty);
}

/* 
 * value - the value to be updated
 * nr_digits - the number of the digits the value has
 * status - will indicate if the figure is added before or after the comma
 * figure - the figure to add to the number
 * Returns a long double after the update was done
 */
long double update_value (long double value, int nr_digits, double status, char figure) {
   if (figure == '.') {
      return value;
   }
   char value_as_string [256];
   long_double_to_string (value_as_string, 256, value);

   if (value_as_string [nr_digits] == '.' && (status == 1.5 || status == 2.5)) {
      // write after the comma
      value_as_string [nr_digits + 1] = figure;
   } else if (value_as_string [nr_digits] == '.' && (status == 1 || status == 2)) {
      // write in the place of the comma and move the comma one right
      value_as_string [nr_digits] = figure;
      value_as_string [nr_digits + 1] = '.';
   } else {
      value_as_string [nr_digits] = figure;
   }

   return atof (value_as_string);
}

/*
 * Receives two numbers, an operator and returns the result of the calculation
 *
 */
long double calculate (long double first_value, long double second_value, char operator) {
   switch (operator) {
      case '+':
         return first_value + second_value;
         break;
      case '-':
         return first_value - second_value;
         break;
      case '*':
         return first_value + second_value;
         break;
      case '/':
         if (second_value != 0) {
            return first_value / second_value;
            break;
         } else {
            return 0;
            break;
         }
      }
}

/*
 * Receives a pointer to a button widget and a pointer to an entry's buffer
 * Updates the buffer
 */
void cbb_update_buffer_2 (GtkWidget *button, GtkEntryBuffer *buffer) {
   
   char label[4];
   strcpy (label, gtk_button_get_label (GTK_BUTTON(button)));
   label [3] = '\0';
   
   /* 
    * status == 1, the first value is updating before the comma
    * status == 1.5, the first value is updating after the comma
    * status == 2, the second value is updating before the comma
    * status == 2.5, the second value is updating after the comma
    * status == 3, both values are ready. Calculation time
    */
   static double status = 1; // at the begining, first value is updating
   static long double first_value = 0;
   static long double second_value = 0;
   static int nr_digits = 0;
   char operator = ' ';
   long double result;

   if (label[0] == '+' || label[0] == '-'|| label[0] == '*' || label[0] == '/' || label[0] == '=') {
      operator = label [0];  /**** <<<-------   !!!! */
      if (status == 1 || status == 1.5) {
         status = 2;
         nr_digits = 0; // start from 0 for the second value
         return; // stop here and waiting for figures for the second value
      } else if (status == 2 || status == 2.5) {
         status = 3;
      }
   } else if (strcmp (label, "Bin") == 0 || strcmp (label, "Dec") == 0 || strcmp (label, "Hex") == 0) {
      status = 1;
   } else if (label [0] == '.' && (status == 1 || status == 2)) {
      status += 0.5;
   } else if (label [0] == '.' && (status == 1.5 || status == 2.5)) {
      return; // one comma in one floating point number is enough
   }

   // Stores the interpretation as string of the value to be displayed
   char val_as_str [256];


   if (status == 1 || status == 1.5) {
      first_value = update_value (first_value, nr_digits, status, label[0]);
      long_double_to_string (val_as_str, 256, first_value);
   } else if (status == 2 || status == 2.5) {
      second_value = update_value (second_value, nr_digits, status, label[0]);
      long_double_to_string (val_as_str, 256, second_value);
   } else if (status == 3) {
      first_value = calculate (first_value, second_value, operator);
      long_double_to_string (val_as_str, 256, first_value);
      second_value = 0;
      status = 2;
      nr_digits = -1;
   }

   gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (buffer), val_as_str, -1);
   nr_digits++;

}


/*
 * Receives the first value, the second value and the status and returns the result
 * Depending on the status, the values are updated
 */
double manager (double first_value, double second_value, int status) {
   // nothing yet

}

/*
 * Receives two numbers (double) and returns the sum of them
 */
double sum (double first_value, double second_value) {
   return first_value + second_value;
}

/*
 * Receives two numbers (double) and returns the difference of them
 */
double difference (double first_value, double second_value) {
   return first_value - second_value;
}

/*
 * Receives two numbers (double) and returns the multiplication of them
 */
double multiplication (double first_value, double second_value) {
   return first_value * second_value;
}

/*
 * Receives two numbers (double) and returns the division of them
 */
double division (double first_value, double second_value) {
   if (second_value != 0) {
      return first_value / second_value;
   }
}


int main (int argc, char *argv[]) {
   gtk_init (&argc, &argv);
   
   GtkWidget *gui; // the main window
   GtkWidget *display; // the display of the minicalculator, an entry widget
   GtkWidget *box; // contains the display and the table a grid widget
   GtkWidget *table; // a grid that ordonates the buttons of the minicalculator
   GtkEntryBuffer *bufy;

   GtkWidget *butt [4][7]; // contains the buttons
   // labels for the buttons, that's why it matches butt's design
   gchar *butt_label [4][7] = {
      "1", "2", "3", "4", "+", "^", "Bin",
      "5", "6", "7", "8", "-", "\u221A", "Oct",
      "9", "0", "A", "B", "*", ".", "Dec",
      "C", "D", "E", "F", "/", "=", "Hex"
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

   // Initializes the buttons with the corresponding labels and attaches them to the grid table
   // Also, connects the callbacks to the buttons with the "clicked" signal
   int i, j;
   for (i = 0; i < 4; i++) {
      for (j = 0; j < 7; j++) {
         butt [i][j] = gtk_button_new_with_label (butt_label [i][j]);
         gtk_grid_attach (GTK_GRID (table), butt [i][j], j, i, 1, 1);
         g_signal_connect (butt [i][j], "clicked", G_CALLBACK (cbb_update_buffer_2), bufy);
      }
   }
 
   gtk_grid_attach (GTK_GRID (box), display, 0, 0, 1, 1);
   gtk_grid_attach (GTK_GRID (box), table, 0, 1, 1, 1);

   gtk_container_add (GTK_CONTAINER (gui), box);
   gtk_widget_show_all (gui);

   gtk_main ();
   return 0;  
}

