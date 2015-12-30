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
 * Receives a pointer to an UpdateBuffer 'object'
 * sets the Tdata.first_value of the 'object'
 */
void set_first_value (struct UpdateBuffer *pUpbuffer) {
   pUpbuffer->Tdata.first_value = atof (pUpbuffer->Tdata.first_value_str);
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
 * sets the Tdata.result of the 'object'
 */
void set_result (struct UpdateBuffer *pUpbuffer) {
   computes (pUpbuffer);
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
 * Receives a pointer to a UpdateBuffer 'object' 
 * Modifies the string corresponding to the first_value or the second_value
 * status 1: first value is being written
 * status 1.5: first value is being written but after the floating point
 * status 2: the second value is being written 
 * status 2.5: the second value is being written but after the floating point
 */
void update_value (struct UpdateBuffer *pUpbuffer) {
   if (pUpbuffer->Tdata.status < 2) {
      pUpbuffer->Tdata.first_value_str [pUpbuffer->Tdata.nr_digits] = pUpbuffer->Tdata.label [0];
   } else if (pUpbuffer->Tdata.status < 3) {
      pUpbuffer->Tdata.second_value_str [pUpbuffer->Tdata.nr_digits] =
        pUpbuffer->Tdata.label [0];
   }
}

/*
 * Receives a pointer to an UpdateBuffer 'object'
 * Sets the result_str value
 */
void set_result_str (struct UpdateBuffer *pUpbuffer) {
   if (pUpbuffer->Tdata.status < 2) {
      strcpy (pUpbuffer->Tdata.result_str, pUpbuffer->Tdata.first_value_str);
   } else if (pUpbuffer->Tdata.status < 3) {
      strcpy (pUpbuffer->Tdata.result_str, pUpbuffer->Tdata.second_value_str);
   } else if (pUpbuffer->Tdata.status == 3) {
      //strcpy (pUpbuffer->Tdata.result_str, long_double_to_string
         //(pUpbuffer->Tdata.result));
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
 * Receives a pointer to a UpdateBuffer 'object'
 * Depending on the status, the function decides what function should be called
 */
void manage_status (struct UpdateBuffer *pUpbuffer) {
   if (pUpbuffer->Tdata.status < 3) {
      update_value (pUpbuffer);
   } else if (pUpbuffer->Tdata.status == 3) {
      // do some calculations
   }
}

/*
 * Receives a pointer to a widget button and a pointer to a UpdateBuffer structure
 * Updates the buffer after the input was processed. The function does three
 * main tasks: sets the status, calls the manage_status fnction, and when all
 * this are done, calls  the function that updates the buffer 
 */
void cbb_input_manager (GtkWidget *button, struct UpdateBuffer *pUpbuffer) {

   // the input from the user
   strcpy (pUpbuffer->Tdata.label, gtk_button_get_label (GTK_BUTTON (button)));

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
      pUpbuffer->Tdata.nr_digits++;
   } else if (strchr (operators, pUpbuffer->Tdata.label [0]) != NULL) {
      pUpbuffer->Tdata.new_operator = pUpbuffer->Tdata.label [0];

      if (pUpbuffer->Tdata.operator == ' ') {
         pUpbuffer->Tdata.operator = pUpbuffer->Tdata.new_operator;
      }

      if (pUpbuffer->Tdata.status < 2) {
         pUpbuffer->Tdata.status = 2;
         pUpbuffer->Tdata.nr_digits = -1;
         return; // wait for figures for the second value
      } else if (pUpbuffer->Tdata.status < 3) {
         pUpbuffer->Tdata.status = 3;
      }
   } else if (strchr (unar_operators, pUpbuffer->Tdata.label [0]) != NULL) {
      // operation on the first value 
   } else if (pUpbuffer->Tdata.label [0] == '#') {
      // reset the calculator
      initialize_up_buffer (pUpbuffer);
   } else if (strcmp (pUpbuffer->Tdata.label, "Bin") != 0 ||
              strcmp (pUpbuffer->Tdata.label, "Dec") != 0 ||
              strcmp (pUpbuffer->Tdata.label, "Hex") != 0) {
      // change the base
   }

   manage_status (pUpbuffer);
   set_result_str (pUpbuffer);
   update_buffer (pUpbuffer); 
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
 * value - the value to be updated
 * nr_digits - the number of the digits the value has
 * status - will indicate if the figure is added before or after the comma
 * figure - the figure to add to the number
 * Returns a long double after the update was done
 */
long double update_value_2 (long double value, int nr_digits, double status, char figure) {
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
   static double status = 1; // at the begining, the first value is updating
   static long double first_value = 0;
   static long double second_value = 0;
   static int nr_digits = 0;
   char operator = ' ';
   char new_operator = ' ';
   long double result;

   if (label[0] == '+' || label[0] == '-'|| label[0] == '*' || label[0] == '/' || label[0] == '=') {
      new_operator = label [0];  /**** <<<-------   !!!! */
      if (operator == ' ') {
         operator = new_operator;
      }
      if (status == 1 || status == 1.5) {
         status = 2;
         nr_digits = 0; // start from 0 for the second value
         return; // stop here and waiting for figures for the second value
      } else if (status == 2 || status == 2.5) {
         status = 3; // <----------------------- !!!
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
      first_value = update_value_2 (first_value, nr_digits, status, label[0]);
      long_double_to_string (val_as_str, 256, first_value);
   } else if (status == 2 || status == 2.5) {
      second_value = update_value_2 (second_value, nr_digits, status, label[0]);
      long_double_to_string (val_as_str, 256, second_value);
   } else if (status == 3) {
      first_value = calculate (first_value, second_value, operator);
      long_double_to_string (val_as_str, 256, first_value);
      second_value = 0;
      status = 2;
      nr_digits = -1;
      operator = new_operator;
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
         g_signal_connect (butt [i][j], "clicked", G_CALLBACK (cbb_input_manager), pUpbuffer);
      }
   }
 
   gtk_grid_attach (GTK_GRID (box), display, 0, 0, 1, 1);
   gtk_grid_attach (GTK_GRID (box), table, 0, 1, 1, 1);

   gtk_container_add (GTK_CONTAINER (gui), box);
   gtk_widget_show_all (gui);

   gtk_main ();
   return 0;  
}

