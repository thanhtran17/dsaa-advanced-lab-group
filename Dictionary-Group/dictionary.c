
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "btree/inc/btree.h"
#include "soundex.h"
typedef struct _dictionaryFunctions
{
    //*** BUTTONS
    GtkWidget *add_word_btn;
    GtkWidget *search_word_btn;
    GtkWidget *set_default_btn;
    GtkWidget *remove_word_btn;
    //*** LABElS
    GtkWidget *word_label;
    GtkWidget *mean_label;
    //*** INPUTS
    GtkWidget *word_entry;
    GtkWidget *mean_entry;
} Dict;
Dict dict;

// **    GLOBAL VARIABLES    **//
char word[256];
char mean[256];
char soundex_word[256];
int soundex_index = 0;
/*       BTREE         */
char pb[] = "language.db";
int choice;
int rsize;       // current length of data
int dsize = 256; // length of data
int check_success = -1;
int dups;

//**       GTK FUNCTION      **//
void btn_clicked(GtkWidget *widget, GtkEntry *entry);
void myCSS(void);
void createWind(GtkWidget **window, gint width, gint height);
void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name);
gboolean zero_to_nine_keys_callback(GtkWidget *widget, GdkEventKey *event);
gboolean a_to_z_keys_callback(GtkWidget *widget, GdkEventKey *event);
void set_fields_default(GtkEntry *widget);
void add_word_handler(void);
void search_word_handler(void);
void remove_word_handler(void);
gboolean input_word_handler(GtkWidget *widget, GdkEventKey *event);

//**        BTREE FUNCTIONS      **//
int insertWord(BTA *head_node, char word[], char mean[], int dsize);
void findWord(BTA *head_node, int dsize, int *rsize);
void deleteWord(BTA *head_node, char word[]);
void printList(BTA *head_node, int pos, char word[], char mean[], int dsize, int *rsize);
BTA *createBtree(BTA *root, char *pb);
void readFile(BTA *head_node);
void suggestionWord(BTA *book, char new_word[]);

/*     Initialize Btree    */
btinit();
BTA *book;

//**       HANDLE MAIN        **//
int main(int argc, char *argv[])
{
    book = createBtree(book, pb);
    btdups(book, dups);
    readFile(book);
    //printList(book, 1, word, mean, dsize, &rsize);
    /*     Initialize Soundex     */
    init();
    /*       Initialize GTK       */
    GtkWidget *window, *grid;
    gtk_init(&argc, &argv);

    myCSS();

    /*     Create the Window     */
    createWind(&window, 390, 290);

    /*     Create a Grid     */
    createGrid(&grid, &window, "myGrid");
    gtk_widget_set_events(dict.word_entry, GDK_KEY_PRESS_MASK);
    /*     Create labels and inputs     */
    dict.word_entry = (GtkWidget *)gtk_entry_new();
    dict.mean_entry = (GtkWidget *)gtk_entry_new();
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

    dict.set_default_btn = gtk_button_new_with_label("Set Default");
    gtk_widget_set_name(dict.set_default_btn, "defaultButton");
    gtk_widget_set_size_request(dict.set_default_btn, 390, 40);
    g_object_set(dict.set_default_btn, "margin", 2, NULL);

    dict.remove_word_btn = gtk_button_new_with_label("Remove Word");
    gtk_widget_set_name(dict.remove_word_btn, "removeButton");
    gtk_widget_set_size_request(dict.remove_word_btn, 390, 40);
    g_object_set(dict.remove_word_btn, "margin", 2, NULL);

    /*     Putting all together      */
    gtk_grid_attach(GTK_GRID(grid), dict.word_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.word_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.mean_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.mean_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.add_word_btn, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.search_word_btn, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.remove_word_btn, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dict.set_default_btn, 1, 5, 1, 1);

    /*        Event Handler         */
    g_signal_connect_swapped(dict.word_entry, "key_press_event", G_CALLBACK(input_word_handler), dict.word_entry);
    g_signal_connect(dict.add_word_btn, "clicked", G_CALLBACK(add_word_handler), NULL);
    g_signal_connect(dict.search_word_btn, "clicked", G_CALLBACK(search_word_handler), NULL);
    g_signal_connect(dict.remove_word_btn, "clicked", G_CALLBACK(remove_word_handler), NULL);
    g_signal_connect(dict.set_default_btn, "clicked", G_CALLBACK(set_fields_default), NULL);
    //g_signal_connect_swapped(window, "key_press_event", G_CALLBACK(a_to_z_keys_callback), window);

    gtk_widget_show_all(window);
    gtk_main();
    btcls(book);
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
gboolean input_word_handler(GtkWidget *widget, GdkEventKey *event)
{
    (void)widget;
    guint keys[55] =
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
            GDK_KEY_N, GDK_KEY_M, GDK_KEY_Tab

        };
    for (guint i = 0; i < 55; i++)
    {
        if (event->keyval == keys[i] && i != 54)
        {
            strcpy(word, gtk_entry_get_text(GTK_ENTRY(dict.word_entry)));
            strcat(word, (char[2]){(char)keys[i], '\0'});
            g_print("Current Input: %s\n", word);
            if (strlen(word) > 0)
            {
                suggestionWord(book, strdup(word));
            }
            return FALSE;
        }
        else if (event->keyval == keys[54] && strlen(soundex_word) > 0)
        {
            gtk_entry_set_text((GtkEntry *)dict.word_entry, soundex_word);
            return FALSE;
        }
        else if (event->keyval == GDK_KEY_Tab)
        {
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
void set_fields_default(GtkEntry *widget)
{
    (void)widget;
    gtk_entry_set_text((GtkEntry *)dict.word_entry, "");
    gtk_entry_set_text((GtkEntry *)dict.mean_entry, "");
}
void add_word_handler(void)
{
    strcpy(word, gtk_entry_get_text((GtkEntry *)dict.word_entry));
    strcpy(mean, gtk_entry_get_text((GtkEntry *)dict.mean_entry));
    if (strlen(word) > 0 && strlen(mean) > 0)
    {
        insertWord(book, strdup(word), strdup(mean), dsize);
        g_print("Insert new word successfully\n");
        gtk_entry_set_text((GtkEntry *)dict.word_entry, "");
        gtk_entry_set_text((GtkEntry *)dict.mean_entry, "");
    }
    else
    {
        g_print("Please input word and mean for adding\n");
    }
}
void search_word_handler(void)
{
    strcpy(word, gtk_entry_get_text((GtkEntry *)dict.word_entry));
    if (strlen(word) > 0)
    {
        findWord(book, dsize, &rsize);
    }
    else
    {
        g_print("Please input word for searching\n");
    }
}
void remove_word_handler(void)
{
    strcpy(word, gtk_entry_get_text((GtkEntry *)dict.word_entry));
    if (strlen(word) > 0)
    {
        deleteWord(book, word);
        gtk_entry_set_text((GtkEntry *)dict.word_entry, "");
        gtk_entry_set_text((GtkEntry *)dict.mean_entry, "");
    }
}
int insertWord(BTA *head_node, char word[], char mean[], int dsize)
{
    int rsize = btupd(head_node, word, mean, dsize);
    if (rsize == 0)
    {
        return rsize;
    }
    return btins(head_node, word, mean, dsize);
}
void findWord(BTA *head_node, int dsize, int *rsize)
{
    if (btsel(head_node, word, mean, dsize, rsize) == 0)
    {
        printf("Meaning of Word is %s\n", mean);
        gtk_entry_set_text((GtkEntry *)dict.mean_entry, mean);
    }
    else
    {
        printf("There is no word like that in dictionary. Please try again!\n");
    };
}
void deleteWord(BTA *head_node, char word[])
{
    if (btdel(head_node, word) == 0)
    {
        printf("Delete %s out of list successfully\n", word);
    }
    else
    {
        printf("There is no word like that in dictionary. Please try again!\n");
    }
}
void printList(BTA *head_node, int pos, char word[], char mean[], int dsize, int *rsize)
{
    if (btpos(head_node, pos) != 0)
    {
        printf("Some errors happened!\n");
    }
    while (btseln(head_node, word, mean, dsize, rsize) == 0)
    {
        printf("Word: %s Mean: %s\n", word, mean);
    }
}
BTA *createBtree(BTA *root, char *pb)
{
    root = btopn(pb, 0, 0);
    if (root == NULL)
    {
        root = btcrt(pb, 0, 0);
    }
    return root;
}
void readFile(BTA *head_node)
{
    FILE *fp;
    char word[256];
    char mean[256];
    int dsize = 256;
    if ((fp = fopen("language.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while (!feof(fp))
    {
        fscanf(fp, "%s %[^\n]", word, mean);
        //strcpy(container_word[container_index], word);
        //container_index++;
        insertWord(head_node, word, mean, dsize);
    }
    fclose(fp);
}
void suggestionWord(BTA *book, char new_word[])
{
    strcpy(soundex_word, "");
    soundex_index = 0;
    char n[10];
    char m[10];
    strcpy(n, soundex(new_word));
    if (btpos(book, 1) != 0)
    {
        printf("Some errors happened!\n");
    }
    printf("\nHint: \n");
    while (btseln(book, word, mean, dsize, &rsize) == 0)
    {
        //printf("Compare: %s %s\n", word, new_word);
        //printf(" %s\n", new_word);
        strcpy(m, soundex(strdup(word)));
        if (strcmp(n, m) == 0)
        {
            if (soundex_index == 0)
            {
                strcpy(soundex_word, word);
            }
            if (soundex_index < 10)
            {
                printf("%s\n", word);
            }
            //strcpy(soundex_word[soundex_index], word);
            soundex_index++;
        }
    }
}
void btn_clicked(GtkWidget *widget, GtkEntry *entry)
{
    (void)widget;
    const gchar *gstrTexto;
    gstrTexto = gtk_entry_get_text(entry);
    g_print("%s\n", gstrTexto);
    gtk_editable_select_region(GTK_EDITABLE(entry), 0, 3);
}