#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

struct TrackData {
   long double first_value;
   long double second_value;
   long double result;
   char first_value_str [64];
   char second_value_str [64];
   char result_str [64];
   double status;
   int nr_digits;
   char operator;
   char new_operator;
   char label [4];
};



struct UpdateBuffer {
   GtkEntryBuffer *bufy;
   struct TrackData Tdata;
};

/*
 * Receives a pointer to an UpdateBuffer obj
 * Initializes the UpdateBuffer obj but not its buffer
 */
void initialize_up_buffer (struct UpdateBuffer *pUpbuffer) {
   pUpbuffer->Tdata.first_value = 0;
   pUpbuffer->Tdata.second_value = 0;
   pUpbuffer->Tdata.result = 0;
   pUpbuffer->Tdata.first_value_str [0] = '0';
   pUpbuffer->Tdata.second_value_str [0] = '0';
   pUpbuffer->Tdata.result_str [0] = '0';
   int cells;
   for (cells = 1; cells < 64; cells++) {
      pUpbuffer->Tdata.first_value_str [cells] = '\0';
      pUpbuffer->Tdata.second_value_str [cells] = '\0';
      pUpbuffer->Tdata.result_str [cells] = '\0';
   }
   pUpbuffer->Tdata.status = 1;
   pUpbuffer->Tdata.nr_digits = -1;
   pUpbuffer->Tdata.operator = ' ';
   pUpbuffer->Tdata.new_operator = ' ';
   for (cells = 0; cells < 4; cells++) {
      pUpbuffer->Tdata.label[cells] = '\0';
   }
}

/*
 * The string receives the conversion of the float number to a string
 * str, a pointer to a string where the result of the conversion are stored
 * size, should match the length of the string
 * floaty, the number to be converted
 */

void long_double_to_string (char *str, size_t size, long double floaty) {
   snprintf (str, size, "%0.12Lf", floaty);
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * Sets the nr_digits value
 */
void set_nr_digits (struct UpdateBuffer *pUpbuffer) {
   if (pUpbuffer->Tdata.status == 2) {
      pUpbuffer->Tdata.nr_digits = -1;
   } else {
      pUpbuffer->Tdata.nr_digits++;
   }
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * Does calculations with first_value and second_value
 * Puts the result in result
 */
void computes (struct UpdateBuffer *pUpbuffer) {
   switch (pUpbuffer->Tdata.operator) {
      case '+':
         pUpbuffer->Tdata.result = pUpbuffer->Tdata.first_value +
            pUpbuffer->Tdata.second_value;
         break;
      case '-':
         pUpbuffer->Tdata.result = pUpbuffer->Tdata.first_value -
            pUpbuffer->Tdata.second_value;
         break;
      case '*':
         pUpbuffer->Tdata.result = pUpbuffer->Tdata.first_value *
            pUpbuffer->Tdata.second_value;
         break;
      case '/':
         if (pUpbuffer->Tdata.second_value != 0) {
            pUpbuffer->Tdata.result = pUpbuffer->Tdata.first_value /
               pUpbuffer->Tdata.second_value;
            break;
         } else {
            // treats this case in some way
         }
   }
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * sets the Tdata.first_value of the 'object'
 */
void set_first_value (struct UpdateBuffer *pUpbuffer) {
   pUpbuffer->Tdata.first_value = atof (pUpbuffer->Tdata.first_value_str);
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * Sets the Tdata.first_value_str of the 'object'
 */
void set_first_value_str (struct UpdateBuffer *pUpbuffer) {
   if (pUpbuffer->Tdata.status < 2) {
      pUpbuffer->Tdata.first_value_str [pUpbuffer->Tdata.nr_digits] = 
         pUpbuffer->Tdata.label [0];
   } else if (pUpbuffer->Tdata.status == 4) {
      strcpy (pUpbuffer->Tdata.first_value_str, pUpbuffer->Tdata.result_str);
   }
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * sets the Tdata.second_value of the 'object'
 */
void set_second_value (struct UpdateBuffer *pUpbuffer) {
   pUpbuffer->Tdata.second_value = atof (pUpbuffer->Tdata.second_value_str);
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * Sets the Tdata.second_value_str of the 'object'
 */
void set_second_value_str (struct UpdateBuffer *pUpbuffer) {
   if (pUpbuffer->Tdata.status == 2 || pUpbuffer->Tdata.status == 2.5) {
      pUpbuffer->Tdata.second_value_str [pUpbuffer->Tdata.nr_digits] = 
         pUpbuffer->Tdata.label [0];
   }
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * sets the Tdata.result of the 'object'
 */
void set_result (struct UpdateBuffer *pUpbuffer) {
   computes (pUpbuffer);
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * Sets the Tdata.result_str of the 'object'
 */
void set_result_str (struct UpdateBuffer *pUpbuffer) {
   if (pUpbuffer->Tdata.status == 1 || 
       pUpbuffer->Tdata.status == 1.5 ||
       pUpbuffer->Tdata.status == 4) {
      strcpy (pUpbuffer->Tdata.result_str, pUpbuffer->Tdata.first_value_str);
   } else if (pUpbuffer->Tdata.status == 2 ||
              pUpbuffer->Tdata.status == 2.5) {
      strcpy (pUpbuffer->Tdata.result_str, pUpbuffer->Tdata.second_value_str);
   } else if (pUpbuffer->Tdata.status == 3) {
      long_double_to_string (pUpbuffer->Tdata.result_str, 64, pUpbuffer->Tdata.result); 
   }
}




/*
 * Receives a pointer to an UpdateBuffer 'object'
 * Sets the object; well, some parts of it
 */
void set_upbuffer_obj (struct UpdateBuffer *pUpbuffer) {
   // set_nr_digits (pUpbuffer);
   set_first_value_str (pUpbuffer);
   set_first_value (pUpbuffer);
   set_second_value_str (pUpbuffer);
   set_second_value (pUpbuffer);
   set_result (pUpbuffer);
   set_result_str (pUpbuffer);
   set_first_value_str (pUpbuffer);
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * Updates the buffer
 * Every time the result_str variable will update the buffer
 */
void update_buffer ( struct UpdateBuffer *pUpbuffer) {
   gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (pUpbuffer->bufy), pUpbuffer->Tdata.result_str,
      -1);
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * Sets the status and passes it to the function set_upbuffer_obj
 * After the updating of the object, calls update_buffer function
 * ( !recursive function )
 */
void input_manager (struct UpdateBuffer *pUpbuffer) {
   char figures [] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
                      'A', 'B', 'C', 'D', 'E', 'F', '.'};
   char operators [] = {'+', '-', '*', '/'};
   char unar_operators [] = {'b', 'o', 'd', 'h'};
   char bases [4][4] = {"Bin", "Oct", "Dec", "Hex"};

   if (strchr (figures, pUpbuffer->Tdata.label [0]) != NULL) {
      if (pUpbuffer->Tdata.label [0] == '.') {
         if (pUpbuffer->Tdata.status == 1) {
            pUpbuffer->Tdata.status = 1.5;
         } else if (pUpbuffer->Tdata.status == 2) {
            pUpbuffer->Tdata.status = 2.5;
         } else if (pUpbuffer->Tdata.status == 1.5 || pUpbuffer->Tdata.status == 2.5) {
            return; // one floating point in a single number is enough
         }
      }
      pUpbuffer->Tdata.nr_digits++; // <--------------
   } else if (strchr (operators, pUpbuffer->Tdata.label [0]) != NULL) {
      pUpbuffer->Tdata.new_operator = pUpbuffer->Tdata.label [0];

      if (pUpbuffer->Tdata.operator == ' ') {
         pUpbuffer->Tdata.operator = pUpbuffer->Tdata.new_operator;
      }

      if (pUpbuffer->Tdata.status < 2) {
         pUpbuffer->Tdata.status = 2;
         pUpbuffer->Tdata.nr_digits = -1; // <--------
         return; // wait for figures for the second value
      } else if (pUpbuffer->Tdata.status < 3) {
         pUpbuffer->Tdata.status = 3;
      }
   } else if (strchr (unar_operators, pUpbuffer->Tdata.label [0]) != NULL) {
      // operation on the first value 
   } else if (pUpbuffer->Tdata.label [0] == '#') {
      // reset the calculator
      initialize_up_buffer (pUpbuffer);
   } else if (strcmp (pUpbuffer->Tdata.label, "Bin") == 0 ||
              strcmp (pUpbuffer->Tdata.label, "Dec") == 0 ||
              strcmp (pUpbuffer->Tdata.label, "Hex") == 0) {
      // change the base
   } else if (strcmp (pUpbuffer->Tdata.label, "cs4") == 0) {
      pUpbuffer->Tdata.status = 4;
   } else if (strcmp (pUpbuffer->Tdata.label, "cs2") == 0) {
      pUpbuffer->Tdata.status = 2;
      pUpbuffer->Tdata.nr_digits = -1;
      return;
   }

   set_upbuffer_obj (pUpbuffer);
   update_buffer (pUpbuffer);

   /*
    * "Input" provided not by the user
    * cs<number> aka change status to <number>
    * Example: cs4 means change status to 4
    */
   if (pUpbuffer->Tdata.status == 3) {
      strcpy (pUpbuffer->Tdata.label, "cs4");
      input_manager (pUpbuffer);
   } else if (pUpbuffer->Tdata.status == 4) {
      strcpy (pUpbuffer->Tdata.label, "cs2");
      input_manager (pUpbuffer);
   }
}

/*
 * Receives a pointer to a widget button and a pointer to a UpdateBuffer 'object'
 * Gets the input from the user and passes the input to the input_manager function
 */
void cbb_get_input (GtkWidget *button, struct UpdateBuffer *pUpbuffer) {
   // the input from the user
   strcpy (pUpbuffer->Tdata.label, gtk_button_get_label (GTK_BUTTON (button)));
   input_manager (pUpbuffer);
}

int main (int argc, char *argv[]) {
   gtk_init (&argc, &argv);
   
   GtkWidget *gui; // the main window
   GtkWidget *display; // the display of the minicalculator, an entry widget
   GtkWidget *box; // contains the display and the table a grid widget
   GtkWidget *table; // a grid that ordonates the buttons of the minicalculator
   GtkEntryBuffer *bufy;

   struct UpdateBuffer Upbuffer;
   struct UpdateBuffer *pUpbuffer;
   pUpbuffer = &Upbuffer;

   pUpbuffer->bufy = gtk_entry_buffer_new ("0", -1);   
   initialize_up_buffer (pUpbuffer); 

   GtkWidget *butt [4][7]; // contains the buttons
   // labels for the buttons, that's why it matches butt's design
   gchar *butt_label [4][7] = {
      "1", "2", "3", "4", "+", "^", "Bin",
      "5", "6", "7", "8", "-", "\u221A", "Dec",
      "9", "0", "A", "B", "*", ".", "Hex",
      "C", "D", "E", "F", "/", "=", "#"
   };

   gui = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (gui), "minicalculator");
   // gtk_window_set_default_size (GTK_WINDOW (gui), 400, 300);
   gtk_window_set_position (GTK_WINDOW (gui), GTK_WIN_POS_CENTER);
   g_signal_connect (gui, "destroy", G_CALLBACK (gtk_main_quit), NULL);

   box = gtk_grid_new ();
   table = gtk_grid_new ();

   // bufy = gtk_entry_buffer_new ("0", -1);
   display = gtk_entry_new_with_buffer (pUpbuffer->bufy);
   gtk_entry_set_alignment (GTK_ENTRY (display), 1);
   gtk_editable_set_editable (GTK_EDITABLE (display), FALSE);

   // Initializes the buttons with the corresponding labels and attaches them to the grid table
   // Also, connects the callbacks to the buttons with the "clicked" signal
   int i, j;
   for (i = 0; i < 4; i++) {
      for (j = 0; j < 7; j++) {
         butt [i][j] = gtk_button_new_with_label (butt_label [i][j]);
         gtk_grid_attach (GTK_GRID (table), butt [i][j], j, i, 1, 1);
         g_signal_connect (butt [i][j], "clicked", G_CALLBACK (cbb_get_input), pUpbuffer);
      }
   }
 
   gtk_grid_attach (GTK_GRID (box), display, 0, 0, 1, 1);
   gtk_grid_attach (GTK_GRID (box), table, 0, 1, 1, 1);

   gtk_container_add (GTK_CONTAINER (gui), box);
   gtk_widget_show_all (gui);

   gtk_main ();
   return 0;  
}

