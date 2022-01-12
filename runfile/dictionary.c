
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#include "bus.h"
#include <stdlib.h>
#include <stdbool.h>
#define TABLE 10
#define INT_MAX 9999
#define DATA 1000
#define STATION 150
typedef struct _Widgets WTables;
struct _Widgets
{
    GtkWidget *checkbox;
    GtkWidget *entry[TABLE-1];
    GtkWidget *entries[STATION];
};

typedef struct _dictionaryFunctions {
    //*** BUTTONS
    GtkWidget *add_word_btn;
    GtkWidget *search_word_btn;
    GtkWidget *set_default_btn;
    GtkWidget *remove_word_btn;
    //*** LABElS
    GtkWidget * word_label;
    GtkWidget * mean_label;
    GtkWidget * table_label;
    //*** INPUTS
    GtkWidget * word_entry;
    GtkWidget * mean_entry;
    GtkWidget * table_entry;
    GtkWidget * table_entry_1;
} Dict;
Dict dict;

typedef struct BusFunction {
  GtkWidget * busesTable;
  GtkWidget * stationsTable;
  GtkWidget * option_1;
  GtkWidget * busLabel;
  GtkWidget * stationLabel;
  GtkWidget * option_2;
  GtkWidget * option_3;
  GtkWidget * resultLabel;
  GtkWidget * resultAnswer;
  GtkWidget * inputLabel;
  GtkWidget * inputEntry;
} Bus;

Bus bus;
GtkWidget *window, *grid;
// **    GLOBAL VARIABLES    **//
char word[256];
char mean[256];
char soundex_word[256];
char n[10];
char m[10];
int soundex_index = 0;
WTables widgets;
/*       GRAPH         */
typedef struct distance
{
	char v1[256];
	char v2[256];
	double d;
} Distance;
char *buses[DATA];
char *readHolder1[DATA]; // go
char *readHolder2[DATA]; // back
Distance data[DATA];
int sizeData = 0;
int busLine[DATA][DATA]; // keep the number of lines that a bus takes
int sizeHolder = 0;			 // size of above vars
int sizeBuses = 0;			 // number of buses
int row = 0, col = 0;
// Lưu trữ dữ liệu bus và các tuyến nó đi qua
Graph b_s;
//*     MAIN FUNCTIONS FOR BUSES        *//
void copyData(char *bus[], char *file1[], char *file2[]);
void readFile(FILE *f1, FILE *f2, FILE *f3);
void dataProcess(Graph g, JRB bus);

//**       GTK FUNCTION      **//
void btn_clicked(GtkWidget *widget, GtkEntry *entry);
void myCSS(void);
void createWind(GtkWidget **window, gint width, gint height);
void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name);
gboolean zero_to_nine_keys_callback ( GtkWidget *widget, GdkEventKey *event );
gboolean a_to_z_keys_callback ( GtkWidget *widget, GdkEventKey *event );
void set_fields_default(GtkEntry * widget);
void add_word_handler(void);
void search_word_handler(void);
void remove_word_handler(void);
void question_1(void);
gboolean input_word_handler( GtkWidget *widget, GdkEventKey *event );
void question_3(void);
void question_2(void);

// GLOBAL GRAPH DATA
Graph g;
JRB busTable; // Luu tru bus
JRB busInfo;
JRB lineTable;
int choice = 0, id1 = 0, id2 = 0, flag = 0;
char *name1;
char *name2;
char *busHolder[DATA];
char *file1[DATA];
char *file2[DATA];
int path[256];
int length = 0;
double total;
char busstopS[DATA], busB[DATA];
char name[256];
char allBus[1000] = "";
int t1, t2;
int id_bus;
int stations_length;

//**       HANDLE MAIN        **//
int main(int argc, char *argv[]){
    /*       Initialize Data      */
    g = createGraph();
    b_s = createGraph();
    busTable = createBusTable();
    busInfo = createBusInfo();
    lineTable = make_jrb();
    name1 = (char *)malloc(sizeof(char));
    name2 = (char *)malloc(sizeof(char));
    // File Handling
    FILE *f1, *f2, *f3;

    f1 = fopen("../data/departure.txt", "rt");
    f2 = fopen("../data/return.txt", "rt");
    f3 = fopen("../data/bus.txt", "rt");

    if (f1 == NULL)
    {
      printf("\nERROR! Departure file not found\n");
      exit(0);
    }

    if (f2 == NULL)
    {
      printf("\nERROR! Return file not found\n");
      exit(0);
    }
    if (f3 == NULL)
    {
      printf("\nERROR! Return file not found\n");
      exit(0);
    }

    // Procedures
    readFile(f1, f2, f3);
    copyData(busHolder, file1, file2);
    dataProcess(g, busTable);
    makeBusesTable(busTable, buses, sizeBuses);
    stations_length = printAllStations(g);
  
    // findAllBusesGoFromStation(b_s, busTable, allBus, "GIA_LAM");
    // printf("%s\n", allBus);
    /*       Initialize GTK       */
    gtk_init(&argc, &argv);
    myCSS();

    /*     Create the Window     */
    createWind(&window, 1200, 800);

    /*     Create a Grid     */
    createGrid(&grid, &window, "myGrid");
    /*     Create labels and inputs     */
    
    bus.stationLabel = gtk_label_new("Stations: ");
    gtk_widget_set_margin_start ( bus.stationLabel, 10 );
    bus.busLabel = gtk_label_new("Buses: ");
    gtk_widget_set_margin_start ( bus.busLabel, 10 );
    bus.inputLabel = gtk_label_new("Input:");
    gtk_widget_set_margin_start ( bus.inputLabel, 10 );
    bus.resultLabel = gtk_label_new("Result: ");
    gtk_widget_set_margin_start ( bus.busLabel, 10 );
    bus.resultAnswer = (GtkWidget *)gtk_entry_new();
    bus.inputEntry = (GtkWidget *)gtk_entry_new();
    // dict.word_entry = (GtkWidget *)gtk_entry_new();
    // dict.mean_entry = (GtkWidget *)gtk_entry_new();
    // dict.word_label = gtk_label_new("Word: ");
    // gtk_widget_set_margin_start ( dict.word_label, 10 );
    // dict.mean_label = gtk_label_new("Mean: ");
    // gtk_widget_set_margin_start ( dict.mean_label, 10 );
    // dict.table_label = gtk_label_new("Hints: ");
    // gtk_widget_set_margin_start ( dict.table_label, 10 );
    
    /*     Create functional buttons     */
    createGridComponents(&bus.stationsTable, "stationTable");
    createGridComponents(&bus.busesTable, "busTable");  

    //bus.stationsTable = (GtkWidget *)gtk_table_new (TABLE, TABLE, FALSE);
    //gtk_widget_set_name(bus.stationsTable , "stationTable");
    //gtk_widget_set_size_request(bus.stationsTable , 400, 400);
    //g_object_set(bus.stationsTable , "margin", 2, NULL); 
    
    bus.option_1 = gtk_button_new_with_label("Shortest path between 2 stations");
    gtk_widget_set_name(bus.option_1, "addButton");
    gtk_widget_set_size_request(bus.option_1, 390, 40);
    g_object_set (bus.option_1, "margin", 2, NULL);
    
    bus.option_2 = gtk_button_new_with_label("All stations from 1 bus");
    gtk_widget_set_name(bus.option_2, "searchButton");
    gtk_widget_set_size_request(bus.option_2, 390, 40);
    g_object_set (bus.option_2, "margin", 2, NULL);
    
    bus.option_3 = gtk_button_new_with_label("All buses from 1 station");
    gtk_widget_set_name(bus.option_3, "defaultButton");
    gtk_widget_set_size_request(bus.option_3, 390, 40);
    g_object_set (bus.option_3, "margin", 2, NULL);
    
    /*     Putting all together      */
    gtk_grid_attach(GTK_GRID(grid), bus.busLabel, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.busesTable, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.stationLabel, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.stationsTable, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.option_1, 1, 2, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.option_2, 1, 3, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.option_3, 1, 4, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.inputLabel, 0, 5 , 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.inputEntry, 1, 5, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.resultLabel, 0, 6 , 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bus.resultAnswer, 1, 6, 3, 1);
    /*        Event Handler         */
    g_signal_connect(bus.option_1, "clicked", G_CALLBACK(question_1), NULL);
    g_signal_connect(bus.option_2, "clicked", G_CALLBACK(question_2), busTable);
    g_signal_connect(bus.option_3, "clicked", G_CALLBACK(question_3), busTable);
    //g_signal_connect_swapped(dict.word_entry, "key_press_event", G_CALLBACK(input_word_handler), dict.word_entry);
    //g_signal_connect(dict.add_word_btn, "clicked", G_CALLBACK(add_word_handler), NULL);
    //g_signal_connect(dict.search_word_btn, "clicked", G_CALLBACK(search_word_handler), NULL);
    //g_signal_connect(dict.remove_word_btn, "clicked", G_CALLBACK(remove_word_handler), NULL);
    //g_signal_connect(dict.set_default_btn, "clicked", G_CALLBACK(set_fields_default), NULL);
    //g_signal_connect_swapped(window, "key_press_event", G_CALLBACK(a_to_z_keys_callback), window);

    /*     Putting rows to table      */
    JRB ptr = NULL;
    int x_pos = 0;
    jrb_traverse(ptr, busTable){
      widgets.entry[x_pos] = gtk_label_new("");
      gtk_label_set_text(widgets.entry[x_pos], jval_s(ptr->val));
      gtk_grid_attach (bus.busesTable, widgets.entry[x_pos] , x_pos, 0, 1, 1);
      x_pos++;
    }

    JRB node = NULL;
    int col_current = 0;
    int row_current = 0;
    char text[256];
    jrb_traverse(node, g.vertices){
      widgets.entries[row_current * TABLE + col_current] = gtk_label_new("");
      strcpy(text, jval_s(node->val));
      trim(text);
      gtk_label_set_text(widgets.entries[row_current * TABLE + col_current], text);
      gtk_grid_attach (bus.stationsTable, widgets.entries[row_current * TABLE + col_current] , col_current, row_current, 1, 1);
      col_current++;
      if (col_current == 7){
        col_current = 0;
        row_current++;
      }
    }


    // for (int x = 0; x < TABLE; x++)
    // {
    //   for (int y = 0; y < STATION/ TABLE;y++){
    //     widgets.entries[x * TABLE + y] = gtk_label_new("A");
    //     gtk_label_set_text(widgets.entries[x * TABLE + y], "Van Lam");
    //     gtk_grid_attach (bus.stationsTable, widgets.entries[x * TABLE + y] , y, x, 1, 1);
    //   }
    // }
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}

void myCSS(void){
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *myCssFile = "dictionary.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
    g_object_unref (provider);
}

void createWind(GtkWidget **window, gint width, gint height){
    *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(*window), "DDT-Dictionary");
    gtk_window_set_default_size(GTK_WINDOW(*window), width, height);
    //gtk_window_set_resizable (GTK_WINDOW(*window), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(*window), 5);
    g_signal_connect(*window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name){
    *grid = gtk_grid_new ();
    gtk_grid_set_row_homogeneous(GTK_GRID(*grid), FALSE);
    gtk_grid_set_column_homogeneous(GTK_GRID(*grid), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER (*grid), 15);
    gtk_grid_set_column_spacing(GTK_GRID(*grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(*grid), 5);
    gtk_widget_set_name(*grid, name);
    g_object_set (*grid, "margin", 22, NULL);
    gtk_container_add (GTK_CONTAINER (*window), *grid);
}

void createGridComponents(GtkWidget **grid, const gchar *name){
    *grid = gtk_grid_new ();
    gtk_grid_set_row_homogeneous(GTK_GRID(*grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(*grid), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER (*grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(*grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(*grid), 5);
    gtk_widget_set_name(*grid, name);
    g_object_set (*grid, "margin", 22, NULL);
}

gboolean zero_to_nine_keys_callback ( GtkWidget *widget, GdkEventKey *event )
{
    ( void ) widget;
    guint keys[11] =
    {
        GDK_KEY_0, GDK_KEY_1, GDK_KEY_2, GDK_KEY_3, GDK_KEY_4,
        GDK_KEY_5, GDK_KEY_6, GDK_KEY_7, GDK_KEY_8, GDK_KEY_9,
        GDK_KEY_BackSpace
    };

    for ( guint i = 0 ; i < 11 ; i++ )
    {
        if ( event->keyval == keys[i] )
        {
          g_print("Key %c pressed\n", keys[i]);
            return FALSE;
        }
    }

    return TRUE;
}
void question_1(void){
  char fullInput[256];
  char start[256] = "";
  char stop[256] = "";
  char fullPath[1000] = "";
  int path_length = 0;
  strcpy(fullInput, gtk_entry_get_text(bus.inputEntry));
  sscanf(fullInput, "%s %s", start,stop);
  trim(start);
  trim(stop);
  if (strlen(start) == 0 || strlen(stop) == 0){
    gtk_entry_set_text(bus.resultAnswer, "Vui lòng nhập đủ 2 bến ( ví dụ: GIA_LAM BEN_TRE)");
  }
  else {
    int s = getIdStation(g, start);
    int t = getIdStation(g, stop);
    double checkRoute = shortestPath(g, s, t, path, &path_length);
    if (checkRoute == INT_MAX)
    {
      gtk_entry_set_text(bus.resultAnswer, "Không có đường đi nào giữa 2 điểm này");
    }
    else
    {
      char minRoute[256];
      snprintf(minRoute, 50, "%g", checkRoute);
      printf("Độ dài ngắn nhất: %g\n", checkRoute);
      strcat(fullPath,"MIN{");
      strcat(fullPath, minRoute);
      strcat(fullPath,"}: ");
      for (int i = path_length - 1; i >= 0; i--)
      {
        printf("%d->", path[i]);
        strcat(fullPath, getNameStation(g, path[i]));
        strcat(fullPath, "->");
      }
      printf("%d\n", t);
      strcat(fullPath, stop);
      gtk_entry_set_text(bus.resultAnswer, fullPath);
    }
  }
}
void question_3(void){
  strcpy(allBus, "");
  JRB busTabler = createBusTable();
  makeBusesTable(busTabler, buses, sizeBuses);
  findAllBusesGoFromStation(b_s, busTabler, allBus, gtk_entry_get_text(bus.inputEntry));
  gtk_entry_set_text(bus.resultAnswer, allBus);
}
void question_2(void){
  strcpy(allBus, "");
  JRB busTabler = createBusTable();
  printf("%s\n", buses[0]);
  makeBusesTable(busTabler, buses, sizeBuses);
  findStationsFromBus(b_s, busTabler, gtk_entry_get_text(bus.inputEntry), allBus);
  gtk_entry_set_text(bus.resultAnswer, allBus);
}
gboolean a_to_z_keys_callback ( GtkWidget *widget, GdkEventKey *event ) {
    ( void ) widget;
    guint keys[54] =
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
        GDK_KEY_N,GDK_KEY_M
        
    };

    for ( guint i = 0 ; i < 54 ; i++ )
    {
        if ( event->keyval == keys[i] )
        {
          g_print("Key %c pressed\n", keys[i]);
          return FALSE;
        }
    }

    return TRUE;
}

void change_color( GtkWidget *widget,
gpointer data ){
  printf("Hi");
  printf("%d\n", (int) data);
}
void btn_clicked(GtkWidget *widget, GtkEntry *entry){
    (void)widget;
    const gchar *gstrTexto;
    gstrTexto = gtk_entry_get_text(entry);
    g_print("%s\n", gstrTexto);
    gtk_editable_select_region(GTK_EDITABLE(entry) , 0, 3);
}
void copyData(char *bus[], char *file1[], char *file2[])
{
	for (int i = 0; i < sizeHolder; i++)
	{
		file1[i] = strdup(readHolder1[i]);
		file2[i] = strdup(readHolder2[i]);
	}

	for (int i = 0; i < sizeBuses; i++)
	{
		bus[i] = strdup(buses[i]);
	}
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void readFile(FILE *f1, FILE *f2, FILE *f3)
{
	int i = 0, k = 0, flag = 0;
	char *reader = (char *)malloc(sizeof(char));
	int fscanfChecker = fscanf(f1, "%[^\n]%*c", reader);
	buses[k] = strdup(reader);
	k++;
	busLine[row][col] = k;

	while (fscanfChecker == 1)
	{
		flag = 0;
		fscanfChecker = fscanf(f1, "%[^\n]%*c", reader);
		if (fscanfChecker != 1)
			break;
		if (strchr(strdup(reader), '-') != NULL)
		{
			readHolder1[i] = strdup(reader);
			flag = 1;
			i++;
			busLine[row][col + 1] = busLine[row][col + 1] + 1;
		}
		if (flag == 0)
		{
			buses[k] = strdup(reader);
			row++;
			k++;
			busLine[row][col] = k;
		}
	}
	sizeHolder = i;
	sizeBuses = k;
	i = 0;
	fscanfChecker = fscanf(f2, "%[^\n]%*c", reader);
	while (fscanfChecker == 1)
	{
		fscanfChecker = fscanf(f2, "%[^\n]%*c", reader);
		if (fscanfChecker != 1)
			break;
		if (strchr(strdup(reader), '-') != NULL)
		{
			readHolder2[i] = strdup(reader);
			i++;
		}
	}

	// File distance
	char v1[256];
	char v2[256];
	char distance[25];
	while (fscanf(f3, "%s / %s / %s%*c", v1, v2, distance) != -1)
	{
		double d;
		sscanf(distance, "%lf", &d);
		// printf("%s %s %g\n", v1, v2, d);
		strcpy(data[sizeData].v1, v1);
		strcpy(data[sizeData].v2, v2);
		data[sizeData].d = d;
		sizeData++;
	};
}
//-------------------------------------------------------------------
void dataProcess(Graph g, JRB bus)
{
	int i = 0, count = 0;
	int vertexID = 0;
	char *linesplitter[DATA];
	char *temp = "-";
	for (i = 0; i <= row; i++)
	{
		for (int m = 0; m < busLine[i][col + 1]; m++)
		{
			int j = 0, k = 0;
			linesplitter[j] = strtok(readHolder1[count], temp);
			while (linesplitter[j] != NULL)
			{
				j++;
				linesplitter[j] = strtok(NULL, temp);
			}
			linesplitter[j] = strtok(readHolder2[count], temp);
			while (linesplitter[j] != NULL)
			{
				j++;
				linesplitter[j] = strtok(NULL, temp);
			}

			char weight[10];
			weight[0] = (i + 1) + '0';
			weight[1] = '\0';
			for (int k = 0; k < j; k++)
			{
				trim(linesplitter[k]);
				addStations(g, linesplitter[k], &vertexID);
				makeBusStation(b_s, i + 1, vertexID - 1, linesplitter[k]);
			}

			for (int k = 0; k < j - 1; k++)
			{
				// Thêm cạnh giữa 2 vertices liên tiếp
				double a = -1;
				for (int i = 0; i < sizeData; i++)
				{
					if ((strcmp(data[i].v1, linesplitter[k]) == 0) && (strcmp(data[i].v2, linesplitter[k + 1]) == 0))
					{
						a = data[i].d;
					}
				}
				if (a != -1 && strcmp(linesplitter[k], linesplitter[k + 1]) != 0)
				{
					// printf("Thêm edge giữa %s và %s\n", linesplitter[k], linesplitter[k + 1]);
					int x1 = getVertexID(g, linesplitter[k]);
					int x2 = getVertexID(g, linesplitter[k + 1]);
					if (x1 != -1 && x2 != -1 && x1 != x2)
					{
						addEdge(g, x1, x2, a);
					}
					a = -1;
				}
			}
			count++;
		}
	}
}
//-------------------------------------------------------------------
void printFileData(char *bus[], char *file1[], char *file2[])
{
	int count = 0;
	for (int i = 0; i < sizeBuses; i++)
	{
		printf("BUS: %s\n", bus[i]);
		for (int j = 0; j < busLine[i][col + 1]; j++)
		{
			printf("CHIEU DI: %s\n\n", file1[count]);
			printf("CHIEU VE: %s\n\n", file2[count]);
			count++;
		}
	}
}