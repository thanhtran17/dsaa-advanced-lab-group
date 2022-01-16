#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include<time.h>
#include <math.h>
#include <ctype.h>
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

// GTK CONTAINERS
GtkWidget *window;
GtkBuilder *builder;
GtkStack *stack1;
GtkWidget *fixed2, *fixed1, *fixed3, *fixed4; // Change state

//GTK SEARCH
GtkSearchEntry *entry1;
GtkSearchEntry *entry3, *entry4;
GtkSearchEntry *entry2;

// BUTTON AND LABELS AND GRID
GtkLabel *lb1, *lb2;
GtkButton *button1, *button3, *button2;
GtkWidget *view3;
GtkWidget *grid1;
GtkWidget *fixed[100];
GtkWidget *label[100];
GtkWidget *view1, *view2;
GtkWidget *grid1, *grid2;
//box
GtkWidget *grid5;
GtkButton *button[40];
GtkButton *btn_stations[10000];
//Autocomplete
GtkListStore *ls1,*ls2;
GtkEntryCompletion *completion1,*completion2,*completion3, *completion4;
GtkTreeIter iter;
//Other variables
// **    GLOBAL VARIABLES    **//
char word[256];
char mean[256];
char soundex_word[256];
char n[10];
char m[10];
int soundex_index = 0;
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
time_t start,end;
double duration;
//FUNCTIONS GTK
void on_clicked(GtkButton *b);
void copyData(char *bus[], char *file1[], char *file2[]);
void readFile(FILE *f1, FILE *f2, FILE *f3);
void init_grid(GtkGrid *gri);
void dataProcess(Graph g, JRB bus);
void storeStationList(Graph g,GtkListStore *ls);
void storeBusList(JRB busTable,GtkListStore *ls);
void set_completion(GtkEntryCompletion **completion,GtkListStore *ls);
void on_entry1_activate(void);
void on_entry2_activate(void);
void on_entry3_activate(void);
void on_entry4_activate(void);
void on_button1_clicked(void);
void on_button2_clicked(void);
void on_button3_clicked(void);
void on_entry1_delete(void);
void on_entry2_delete(void);
void on_entry3_delete(void);
void on_entry4_delete(void);
void on_destroy();
void on_clicked_station(GtkButton *b);
//**       HANDLE MAIN        **//

int main(int argc, char *argv[]){
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

    //GTK
    gtk_init (&argc, &argv); // init Gtk
    ls1=gtk_list_store_new(1,G_TYPE_STRING);
    ls2=gtk_list_store_new(1,G_TYPE_STRING);
    duration=0.0;
    start=clock();
    storeStationList(g,ls1);
    storeBusList(busTable, ls2);
    end=clock();
    duration=(double)(end - start) / CLOCKS_PER_SEC;
	
    printf("/**Time to push data: %g**/\n",duration);
    set_completion(&completion1,ls1);
    set_completion(&completion2,ls1);
    set_completion(&completion4, ls1);
    set_completion(&completion3,ls2);
    //color of entries
    GdkColor color;
    color.red=0x6e6e;
    color.green=0x9696;
    color.blue=0xb0b0;
     // Main container widgets
    builder = gtk_builder_new_from_file ("gui.glade");
    window = GTK_WIDGET (gtk_builder_get_object(builder, "window"));
    g_signal_connect (window, "destroy", G_CALLBACK (on_destroy), NULL);
    gtk_builder_connect_signals (builder, NULL);

    //CSS Insertion
    GtkCssProvider  *cssProvider1;
    cssProvider1  = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider1, "./gui.css", NULL);
    css_set(cssProvider1, window);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider1),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    //Set fixed for changing options
    stack1=GTK_STACK(gtk_builder_get_object(builder,"stack1"));
    fixed2=GTK_WIDGET(gtk_builder_get_object(builder,"fixed2"));
    fixed3=GTK_WIDGET(gtk_builder_get_object(builder,"fixed3"));

    //Stations
    entry1=GTK_SEARCH_ENTRY(gtk_builder_get_object(builder,"entry1"));
    gtk_entry_set_completion(GTK_ENTRY(entry1),completion1);
    entry2=GTK_SEARCH_ENTRY(gtk_builder_get_object(builder,"entry2"));
    gtk_entry_set_completion(GTK_ENTRY(entry2),completion2);
    g_signal_connect (entry1, "activate", G_CALLBACK (on_entry1_activate), NULL);
    g_signal_connect (entry1, "icon-press", G_CALLBACK (on_entry1_delete), NULL);
    g_signal_connect (entry1, "backspace", G_CALLBACK (on_entry1_delete), NULL);
    g_signal_connect (entry2, "activate", G_CALLBACK (on_entry2_activate), NULL);
    g_signal_connect (entry2, "icon-press", G_CALLBACK (on_entry2_delete), NULL);
    g_signal_connect (entry2, "backspace", G_CALLBACK (on_entry2_delete), NULL);

    lb1=GTK_LABEL(gtk_builder_get_object(builder,"station_name"));
    grid1=GTK_WIDGET(gtk_builder_get_object(builder,"grid1"));
    gtk_widget_set_name(grid1, "station_grid");
    init_grid(GTK_GRID(grid1));
    lb2=GTK_LABEL(gtk_builder_get_object(builder,"totalPath"));
    grid2=GTK_WIDGET(gtk_builder_get_object(builder,"grid2"));
    gtk_widget_set_name(grid1, "bus_grid");

    set_stations(GTK_GRID(grid2));
    view1 = GTK_WIDGET (gtk_builder_get_object (builder, "view1"));
    view2 = GTK_WIDGET (gtk_builder_get_object (builder, "view2"));
    // lb5=GTK_LABEL(gtk_builder_get_object(builder,"lb5"));
    button1=GTK_BUTTON(gtk_builder_get_object(builder,"button1"));
    g_signal_connect(button1,"clicked",G_CALLBACK(on_button1_clicked),NULL);
    gtk_widget_set_name(button1, "button1");

    button3=GTK_BUTTON(gtk_builder_get_object(builder,"button3"));
    g_signal_connect(button3,"clicked",G_CALLBACK(on_button3_clicked),NULL);
    gtk_widget_set_name(button3, "button3");

    button2=GTK_BUTTON(gtk_builder_get_object(builder,"button2"));
    g_signal_connect(button2,"clicked",G_CALLBACK(on_button2_clicked),NULL);
    gtk_widget_set_name(button2, "button2");

    //Buses
    entry3=GTK_SEARCH_ENTRY(gtk_builder_get_object(builder,"entry3"));
    gtk_entry_set_completion(GTK_ENTRY(entry3),completion3);
    g_signal_connect (entry3, "activate", G_CALLBACK (on_entry3_activate), NULL);
    g_signal_connect (entry3, "icon-press", G_CALLBACK (on_entry3_delete), NULL);
    g_signal_connect (entry3, "backspace", G_CALLBACK (on_entry3_delete), NULL);
    entry4=GTK_SEARCH_ENTRY(gtk_builder_get_object(builder,"entry4"));
    gtk_entry_set_completion(GTK_ENTRY(entry4),completion4);
    g_signal_connect (entry4, "activate", G_CALLBACK (on_entry4_activate), NULL);
    g_signal_connect (entry4, "icon-press", G_CALLBACK (on_entry4_delete), NULL);
    g_signal_connect (entry4, "backspace", G_CALLBACK (on_entry4_delete), NULL);

    gtk_widget_show_all(window);
    gtk_widget_hide(grid1);
    gtk_widget_hide(grid2);
    gtk_main();
    return EXIT_SUCCESS;
}
void set_completion(GtkEntryCompletion **completion,GtkListStore *ls){
    *completion=gtk_entry_completion_new();
    gtk_entry_completion_set_model(*completion,GTK_TREE_MODEL(ls));
    gtk_entry_completion_set_text_column(*completion,0);
}
void storeStationList(Graph g,GtkListStore *ls){
    JRB node;
    jrb_traverse(node, g.vertices){
        gtk_list_store_append(GTK_LIST_STORE(ls),&iter);
        gtk_list_store_set(ls,&iter,0,jval_s(node->val),-1);
    }
}
void storeBusList(JRB busTable,GtkListStore *ls){
    JRB node;
    jrb_traverse(node, busTable){
        // printf("%d %s\n", jval_i(node->key), jval_s(node->val));
        gtk_list_store_append(GTK_LIST_STORE(ls),&iter);
        gtk_list_store_set(ls,&iter,0,jval_s(node->val),-1);
    }
}
void on_entry1_activate(void) {
}
void on_entry1_delete(void) {
    gtk_widget_hide(grid2);
    gtk_entry_set_text(entry4, "");
    gtk_entry_set_text(entry3, "");
    gtk_label_set_text(lb2, "...");
}
void on_entry2_activate(void) {
}
void on_entry2_delete(void) {
    gtk_widget_hide(grid2);
    gtk_entry_set_text(entry4, "");
    gtk_entry_set_text(entry3, "");
    gtk_label_set_text(lb2, "...");
}
void on_entry3_activate(void) {
    // gtk_label_set_text(lb1, "");
    // gtk_widget_hide(grid1);
    gtk_widget_hide(grid2);
    gtk_entry_set_text(entry4, "");
    gtk_entry_set_text(entry1, "");
    gtk_entry_set_text(entry2, "");
}
void on_entry3_delete(void) {
    // set_stations(grid2);
    gtk_label_set_text(lb1, "");
    gtk_widget_hide(grid1);
    gtk_widget_hide(grid2);
}
void on_entry4_activate(void) {
    gtk_widget_hide(grid2);
    gtk_entry_set_text(entry3, "");
    gtk_entry_set_text(entry1, "");
    gtk_entry_set_text(entry2, "");
}
void on_entry4_delete(void) {
    gtk_label_set_text(lb1, "");
    // init_grid(grid1);
    gtk_widget_hide(grid1);
}
void on_button1_clicked(void){
  double length = getLengthStations(g);
  char fullInput[256];
  char start[256] = "";
  char stop[256] = "";
  char fullPath[1000] = "";
  int path_length = 0;
  int index = 0;
  char route[256];
  strcpy(start, gtk_entry_get_text(entry1));
  strcpy(stop, gtk_entry_get_text(entry2));
  trim(start); trim(stop);
  if (strlen(start) == 0 || strlen(stop) == 0){
    //DO NOTHING
  }
  else {
    time_t starter, ender;
    int s = getIdStation(g, start);
    int t = getIdStation(g, stop);
    starter = clock();
    double checkRoute = shortestPath(g, s, t, path, &path_length);
    ender=clock();
    double duration=(double)(ender - starter) / CLOCKS_PER_SEC;
    printf("/**Time to find shortest path: %g**/\n",duration);
    if (checkRoute == INT_MAX)
    {
      printf("Route not found!");
      gtk_label_set_text(lb2, "---");
      gtk_widget_hide(grid2);
    }
    else {
      char minRoute[256];
      snprintf(minRoute, 50, "%g", checkRoute);
      gtk_label_set_text(lb2, minRoute);
      printf("Độ dài ngắn nhất: %g\n", checkRoute);
      for (int i = path_length - 1; i > 0; i--)
      {
        printf("%d->", path[i]);
        strcpy(fullPath, getNameStation(g, path[i]));
        trim(fullPath);
        gtk_button_set_label(btn_stations[index], fullPath);
        index++;
      }
      gtk_widget_show_all(grid2);
      for (int j = index; j < length; j++){
        gtk_widget_hide(GTK_WIDGET(btn_stations[j]));
      }
    }
  }
}
void on_button2_clicked(void){
    if (strlen(gtk_entry_get_text(entry3)) == 0){
        return;
    }
    double length = getLengthStations(g);
    int index = 0;
    JRB node;
    int id_bus = 0;
    char name[256];
    strcpy(name, gtk_entry_get_text(entry3));
    trim(name);
    jrb_traverse(node, busTable){
        if (strcmp(jval_s(node->val), name) == 0){
            id_bus = jval_i(node->key);
        }
    }
    JRB tree = jval_v(jrb_find_int(b_s.edges, id_bus)->val);
    JRB ptr;
    char store_name[256];
    jrb_traverse(ptr, tree){
        strcpy(store_name, jval_s(ptr->val));
        printf("%s\n", jval_s(ptr->val));
        trim(store_name);
        gtk_button_set_label(btn_stations[index], store_name);
        index++;
    }
    gtk_widget_show_all(grid2);
    for (int j = index; j < length; j++){
        gtk_widget_hide(GTK_WIDGET(btn_stations[j]));
    }
}
void on_button3_clicked(void){
    if (strlen(gtk_entry_get_text(entry4)) == 0){
        return ;
    }
    JRB node;
    int size = 0;
    char name[100];
    strcpy(name, gtk_entry_get_text(entry4));
    trim(name);
    jrb_traverse(node, b_s.edges){
        JRB tree = jval_v(node->val);
		JRB ptr;
        jrb_traverse(ptr, tree){
            if (strcmp(jval_s(ptr->val), name) == 0)
			{
				flag = 1;
				JRB find_node = jrb_find_int(busTable, jval_i(node->key));
                gtk_button_set_label(button[size],jval_s(find_node->val));
                size++;
				printf("%s-", jval_s(find_node->val));
			}
        }
    };
    gtk_widget_show_all(grid1);
    for(int i=size;i<40;i++){
        gtk_widget_hide(GTK_WIDGET(button[i]));
    }
    gtk_label_set_text(lb1, name);
    printf("\n%s\n", gtk_entry_get_text(entry4));
}
void set_stations(GtkGrid *grid){
    double length_stations = getLengthStations(g);
    gtk_grid_set_column_spacing(grid, 7);
    gtk_grid_set_row_spacing(grid, 20);
    GdkColor color;
    gdk_color_parse("#011A4B", &color);
    GdkColor colorWhite;
    colorWhite.red=0xffff;
    colorWhite.green=0xffff;
    colorWhite.blue=0xffff;
    GdkColor colorBlack;
    for(int i=0;i < length_stations;i++){
        btn_stations[i] = GTK_BUTTON(gtk_button_new_with_label(""));
        gtk_widget_set_size_request(GTK_WIDGET(btn_stations[i]),177.5,60);
        gtk_button_set_alignment (GTK_BUTTON (btn_stations[i]), 0.5, 0.5); // hor left, ver center
        gtk_button_set_relief(GTK_BUTTON(btn_stations[i]),GTK_RELIEF_NONE);
        gtk_widget_modify_bg(GTK_WIDGET(btn_stations[i]),GTK_STATE_NORMAL,&color);
        gtk_widget_modify_fg( GTK_WIDGET(btn_stations[i]), GTK_STATE_NORMAL, &colorWhite );
        gtk_widget_modify_fg( GTK_WIDGET(btn_stations[i]), GTK_STATE_PRELIGHT, &colorBlack );
        gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(btn_stations[i]), i%2, i/2, 1, 1);
        g_signal_connect(btn_stations[i], "clicked", G_CALLBACK (on_clicked_station), NULL);
    }
}
void on_destroy(){
    gtk_main_quit();
}
void on_clicked_station(GtkButton *b){
    gtk_stack_set_visible_child(stack1,fixed3);
    const char *tmp=gtk_label_get_text (GTK_LABEL(gtk_bin_get_child(GTK_BIN(b))));
    trim(tmp);
    gtk_entry_set_text(GTK_ENTRY(entry4),tmp);
    on_button3_clicked();
}
void init_grid(GtkGrid *grid){
    gtk_grid_set_column_spacing(grid, 7);
    gtk_grid_set_row_spacing(grid, 7);
    GdkColor color;
    gdk_color_parse("#A44412", &color);
    GdkColor colorWhite;
    colorWhite.red=0xffff;
    colorWhite.green=0xffff;
    colorWhite.blue=0xffff;
    GdkColor colorBlack;
    for(int i=0;i<40;i++){
        button[i] = GTK_BUTTON(gtk_button_new_with_label("B01"));
        gtk_widget_set_size_request(GTK_WIDGET(button[i]),110,40);
        gtk_button_set_alignment (GTK_BUTTON (button[i]), 0.5, 0.5); // hor left, ver center
        gtk_button_set_relief(GTK_BUTTON(button[i]),GTK_RELIEF_NONE);
        gtk_widget_modify_bg(GTK_WIDGET(button[i]),GTK_STATE_NORMAL,&color);
        gtk_widget_modify_fg( GTK_WIDGET(button[i]), GTK_STATE_NORMAL, &colorWhite );
        gtk_widget_modify_fg( GTK_WIDGET(button[i]), GTK_STATE_PRELIGHT, &colorBlack );
        gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(button[i]), i%2, i/2, 1, 1);
        g_signal_connect(button[i], "clicked", G_CALLBACK (on_clicked), NULL);
    }
}
void on_clicked(GtkButton *b){
    const char *tmp=gtk_label_get_text (GTK_LABEL(gtk_bin_get_child(GTK_BIN(b))));
    gtk_stack_set_visible_child(stack1,fixed2);
    gtk_entry_set_text(GTK_ENTRY(entry3),tmp);
    printf("Bus name: %s\n", tmp);
    on_button2_clicked();
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
void css_set(GtkCssProvider * cssProvider, GtkWidget *g_widget) {
    GtkStyleContext *context = gtk_widget_get_style_context(g_widget);
    gtk_style_context_add_provider (context,
            GTK_STYLE_PROVIDER(cssProvider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
}
