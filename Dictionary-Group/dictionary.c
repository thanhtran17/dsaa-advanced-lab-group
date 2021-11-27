
#include <gtk/gtk.h>
#include <string.h>
typedef struct _dictionaryFunctions
{
  //*** BUTTONS
  GtkWidget *add_word_btn;
  GtkWidget *search_word_btn;
  //*** LABElS
  GtkWidget *word_label;
  GtkWidget *mean_label;
  //*** INPUTS
  GtkWidget *word_entry;
  GtkWidget *mean_entry;
} Dict;
Dict dict;
void btn_clicked(GtkWidget *widget, GtkEntry *entry);
void myCSS(void);
void createWind(GtkWidget **window, gint width, gint height);
void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name);
gboolean zero_to_nine_keys_callback(GtkWidget *widget, GdkEventKey *event);
gboolean a_to_z_keys_callback(GtkWidget *widget, GdkEventKey *event);
int main(int argc, char *argv[])
{
  GtkWidget *window, *grid;

  gtk_init(&argc, &argv);
  myCSS();

  /*     Create the Window     */
  createWind(&window, 390, 290);

  /*     Create a Grid     */
  createGrid(&grid, &window, "myGrid");

  /*     Create labels and inputs     */
  dict.word_entry = gtk_entry_new();
  dict.mean_entry = gtk_entry_new();
  dict.word_label = gtk_label_new("Word: ");
  gtk_widget_set_margin_start(dict.word_label, 10);
  dict.mean_label = gtk_label_new("Mean: ");
  gtk_widget_set_margin_start(dict.mean_label, 10);

  /*     Create functional buttons     */
  dict.add_word_btn = gtk_button_new_with_label("Add Word");
  gtk_widget_set_name(dict.add_word_btn, "addButton");
  gtk_widget_set_size_request(dict.add_word_btn, 390, 40);
  g_object_set(dict.add_word_btn, "margin", 2, NULL);
  dict.search_word_btn = gtk_button_new_with_label("Search Word");
  gtk_widget_set_name(dict.search_word_btn, "searchButton");
  gtk_widget_set_size_request(dict.search_word_btn, 390, 40);
  g_object_set(dict.search_word_btn, "margin", 2, NULL);

  /*     Putting all together      */
  gtk_grid_attach(GTK_GRID(grid), dict.word_label, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), dict.word_entry, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), dict.mean_label, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), dict.mean_entry, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), dict.add_word_btn, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), dict.search_word_btn, 1, 3, 1, 1);

  g_signal_connect_swapped(window, "key_press_event", G_CALLBACK(a_to_z_keys_callback), window);

  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}

void myCSS(void)
{
  GtkCssProvider *provider;
  GdkDisplay *display;
  GdkScreen *screen;

  provider = gtk_css_provider_new();
  display = gdk_display_get_default();
  screen = gdk_display_get_default_screen(display);
  gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  const gchar *myCssFile = "dictionary.css";
  GError *error = 0;

  gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
  g_object_unref(provider);
}

void createWind(GtkWidget **window, gint width, gint height)
{
  *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(*window), "DDT-Dictionary");
  gtk_window_set_default_size(GTK_WINDOW(*window), width, height);
  gtk_window_set_resizable(GTK_WINDOW(*window), TRUE);
  gtk_container_set_border_width(GTK_CONTAINER(*window), 5);
  g_signal_connect(*window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name)
{
  *grid = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(*grid), FALSE);
  gtk_grid_set_column_homogeneous(GTK_GRID(*grid), FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(*grid), 15);
  gtk_grid_set_column_spacing(GTK_GRID(*grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(*grid), 5);
  gtk_widget_set_name(*grid, name);
  g_object_set(*grid, "margin", 22, NULL);
  gtk_container_add(GTK_CONTAINER(*window), *grid);
}
gboolean zero_to_nine_keys_callback(GtkWidget *widget, GdkEventKey *event)
{
  (void)widget;
  guint keys[11] =
      {
          GDK_KEY_0, GDK_KEY_1, GDK_KEY_2, GDK_KEY_3, GDK_KEY_4,
          GDK_KEY_5, GDK_KEY_6, GDK_KEY_7, GDK_KEY_8, GDK_KEY_9,
          GDK_KEY_BackSpace};

  for (guint i = 0; i < 11; i++)
  {
    if (event->keyval == keys[i])
    {
      g_print("Key %c pressed\n", keys[i]);
      return FALSE;
    }
  }

  return TRUE;
}
gboolean a_to_z_keys_callback(GtkWidget *widget, GdkEventKey *event)
{
  (void)widget;
  guint keys[54] =
      {
          GDK_KEY_a, GDK_KEY_b, GDK_KEY_c, GDK_KEY_d, GDK_KEY_e,
          GDK_KEY_f, GDK_KEY_g, GDK_KEY_h, GDK_KEY_i, GDK_KEY_k,
          GDK_KEY_t, GDK_KEY_y, GDK_KEY_u, GDK_KEY_o, GDK_KEY_p,
          GDK_KEY_q, GDK_KEY_w, GDK_KEY_e, GDK_KEY_r, GDK_KEY_s,
          GDK_KEY_j, GDK_KEY_l, GDK_KEY_z, GDK_KEY_x, GDK_KEY_v,
          GDK_KEY_n, GDK_KEY_m,
          GDK_KEY_A, GDK_KEY_B, GDK_KEY_C, GDK_KEY_D, GDK_KEY_E,
          GDK_KEY_F, GDK_KEY_G, GDK_KEY_H, GDK_KEY_I, GDK_KEY_K,
          GDK_KEY_T, GDK_KEY_Y, GDK_KEY_U, GDK_KEY_O, GDK_KEY_P,
          GDK_KEY_Q, GDK_KEY_W, GDK_KEY_E, GDK_KEY_R, GDK_KEY_S,
          GDK_KEY_J, GDK_KEY_L, GDK_KEY_Z, GDK_KEY_X, GDK_KEY_V,
          GDK_KEY_N, GDK_KEY_M

      };

  for (guint i = 0; i < 54; i++)
  {
    if (event->keyval == keys[i])
    {
      g_print("Key %c pressed\n", keys[i]);
      return FALSE;
    }
  }

  return TRUE;
}
void btn_clicked(GtkWidget *widget, GtkEntry *entry)
{
  (void)widget;
  const gchar *gstrTexto;

  gstrTexto = gtk_entry_get_text(entry);
  g_print("%s\n", gstrTexto);
  gtk_editable_select_region(GTK_EDITABLE(entry), 0, 3);
}