#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "btree/inc/btree.h"
#include "soundex.h"
#define TABLE 10
//------------------------------------------------------------------------
typedef struct _widgets WTables;
struct _widgets
{
    GtkWidget* checkbox;
    GtkWidget* entry[TABLE - 1];
};
//------------------------------------------------------------------------
typedef struct _dict {
    GtkWidget *add_word_btn;
    GtkWidget *search_word_btn;
    GtkWidget *set_default_btn;
    GtkWidget *remove_word_btn;

    GtkWidget *word_label;
    GtkWidget *mean_label;
    GtkWidget *table_label;

    GtkWidget *word_entry;
    GtkWidget *mean_entry;
    GtkWidget *table_entry;
} Dict;
Dict dict;
//------------------------------------------------------------------------
char word[256];
char mean[256];
char soundex_word[256];
int soundex_index = 0;
int rsize, dsize = 256;
int check_success = -1;
int dups;
BTA* book;
//------------------------------------------------------------------------
void myCSS(void);
void createWindow(GtkWidget **window, gint width, gint height);
void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name);
gboolean input_word_handler( GtkWidget *widget, GdkEventKey *event);
void suggestionWord(BTA *book, char new_word[]);
//------------------------------------------------------------------------
int main(int argc, char *argv[])
{   
    GtkWidget *window, *grid;
    gtk_init(&argc, &argv);
    myCSS();
    
    createWindow(&window, 390, 290);
    createGrid(&grid, &window, "myGrid");
    gtk_widget_set_events(dict.word_entry, GDK_KEY_PRESS_MASK);

    dict.word_entry  = (GtkWidget*) gtk_entry_new();
    dict.mean_entry  = (GtkWidget*) gtk_entry_new();    

    dict.word_label  = gtk_label_new("Word*");
    dict.word_label  = gtk_label_new("Meaning");
    dict.table_label = gtk_label_new("Hints");

    gtk_widget_set_margin_start ( dict.word_label,  10 );
    gtk_widget_set_margin_start ( dict.mean_label,  10 );
    gtk_widget_set_margin_start ( dict.table_label, 10 );

    dict.add_word_btn    = gtk_button_new_with_label("Add");
    dict.search_word_btn = gtk_button_new_with_label("Search");
    dict.set_default_btn = gtk_button_new_with_label("Set Default");
    dict.remove_word_btn = gtk_button_new_with_label("Remove");

    gtk_widget_set_name(dict.add_word_btn,    "addButton");
    gtk_widget_set_name(dict.search_word_btn, "searchButton");
    gtk_widget_set_name(dict.set_default_btn, "defaultButton");
    gtk_widget_set_name(dict.remove_word_btn, "removeButton");
    gtk_widget_set_name(dict.word_entry,      "wordEntry");
    gtk_widget_set_name(dict.table_entry,     "tableEntry");
    gtk_widget_set_name(dict.mean_entry,      "meanEntry");

    g_object_set(dict.add_word_btn,    "margin", 2, NULL);
    g_object_set(dict.search_word_btn, "margin", 2, NULL);
    g_object_set(dict.set_default_btn, "margin", 2, NULL);
    g_object_set(dict.remove_word_btn, "margin", 2, NULL);

    dict.table_entry = (GtkWidget*) gtk_table_new (TABLE, TABLE, FALSE);
    g_object_set(dict.table_entry, "margin", 2, NULL);

    gtk_grid_attach(GTK_GRID(grid), dict.word_label,      0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.mean_label,      0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.table_label,     0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.word_entry,      1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.mean_entry,      1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.table_entry,     1, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.set_default_btn, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.search_word_btn, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.add_word_btn,    1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.remove_word_btn, 2, 4, 1, 1);

    g_signal_connect_swapped(dict.word_entry, "key_press_event", G_CALLBACK(input_word_handler), dict.word_entry);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
//------------------------------------------------------------------------
void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name)
{
    *grid = gtk_grid_new();
    
    gtk_grid_set_row_homogeneous    (GTK_GRID(*grid), FALSE);
    gtk_grid_set_column_homogeneous (GTK_GRID(*grid), FALSE);
    gtk_container_set_border_width  (GTK_CONTAINER(*grid), 15);
    gtk_grid_set_row_spacing        (GTK_GRID(*grid), 5);
    gtk_grid_set_column_spacing     (GTK_GRID(*grid), 5);
    gtk_widget_set_name             (*grid, name);
    g_object_set                    (*grid, "margin", 22, NULL);
    gtk_container_add               (GTK_CONTAINER(*window), *grid);
}
//------------------------------------------------------------------------
void createWindow(GtkWidget **window, gint width, gint height)
{
    *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title (GTK_WINDOW(*window), "DDT DICTIONARY");
    gtk_window_set_default_size (GTK_WINDOW(*window), width, height);
    gtk_window_set_resizable (GTK_WINDOW(*window), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(*window), 5);

    g_signal_connect(*window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
//------------------------------------------------------------------------
void myCSS(void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new ();
    display  = gdk_display_get_default ();
    screen   = gdk_display_get_default_screen (display);

    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *myCssFile = "dictionary.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
    g_object_unref (provider);
}
//------------------------------------------------------------------------
gboolean input_word_handler( GtkWidget *widget, GdkEventKey *event)
{
    (void) widget;
    guint keys[55]= 
    {
        GDK_KEY_a, GDK_KEY_b, GDK_KEY_c, GDK_KEY_d, GDK_KEY_e,
        GDK_KEY_f, GDK_KEY_g, GDK_KEY_h, GDK_KEY_i, GDK_KEY_k,
        GDK_KEY_t,GDK_KEY_y,GDK_KEY_u,GDK_KEY_o,GDK_KEY_p,
        GDK_KEY_q,GDK_KEY_w,GDK_KEY_e,GDK_KEY_r,GDK_KEY_s,
        GDK_KEY_j,GDK_KEY_l,GDK_KEY_z,GDK_KEY_x,GDK_KEY_v,
        GDK_KEY_n,GDK_KEY_m,
        GDK_KEY_A, GDK_KEY_B, GDK_KEY_C, GDK_KEY_D, GDK_KEY_E,
        GDK_KEY_F, GDK_KEY_G, GDK_KEY_H, GDK_KEY_I, GDK_KEY_K,
        GDK_KEY_T,GDK_KEY_Y,GDK_KEY_U,GDK_KEY_O,GDK_KEY_P,
        GDK_KEY_Q,GDK_KEY_W,GDK_KEY_E,GDK_KEY_R,GDK_KEY_S,
        GDK_KEY_J,GDK_KEY_L,GDK_KEY_Z,GDK_KEY_X,GDK_KEY_V,
        GDK_KEY_N,GDK_KEY_M, GDK_KEY_Tab
    };

    for (guint i = 0; i < 55; i++){
        if (event->keyval == keys[i] && i != 54){
            strcpy(word, gtk_entry_get_text(GTK_ENTRY(dict.word_entry)));
            strcat(word, (char[2]) { (char) keys[i], '\0' } );
            g_print("YOUR CURRENT INPUT --> %s\n", word);
            if (strlen(word) > 0) {
                suggestionWord(book, strdup(word));
            }
            return FALSE;
        }
        else if (event->keyval == keys[54] && strlen(soundex_word) > 0) {
            gtk_entry_Set_text((GtkEntry*) dict.word_entry, soundex_word);
            return FALSE;
        }
        else if (event->keyval == GDK_KEY_Tab){
            return FALSE;
        }
    }
}

void suggestionWord(BTA *book, char new_word[])
{
    char n[10], m[10];
    soundex_index = 0;

    strcpy(soundex_word, "");
    strcpy(n, soundex(new_word));

    if (btpos(book, 1) != 0){
        printf("Some errors happened!\n");
    }

    printf("--> Hints: \n");

    while (btseln(book, word, mean, dsize, &rsize) == 0) {

    }
}