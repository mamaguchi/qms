#include <time.h>
#include <string.h>
#include <gtk/gtk.h>
#include "utils/room_struct.h"
#include "utils/section_struct.h"
#include "utils/keyboard_layout.h"
#include <stdio.h>

/*** GLOBAL VARIABLES ***/
// Toplevel Window
GtkWidget* win;

// Rooms Table Viewport
GtkWidget* vport;

// Rooms Table
GtkWidget* tbl;

// Section
GtkButton* sections[MAX_SECT];

//uint8_t sectRoomFront[] = {0,0,0,0,0};
//uint8_t sectRoomRear[] = {-1,-1,-1,-1,-1};
//uint8_t sectRoomItemCount[] = {0,0,0,0,0};
//uint8_t curSect = 0;

// Debug Label
GtkWidget* debugLbl;

// Room
//Room* roomsOfSect[MAX_ROOM];
Room* roomsOfSect[MAX_SECT][MAX_ROOM];
GtkWidget* roomsOfSect2[MAX_SECT];
//Room roomsOfSect[MAX_SECT][MAX_ROOM];
RoomRowInfo* roomRowInfos[5];
//uint8_t curSect = 0;

// Image Assets
//GtkWidget* sectImg;

/************************/


void delay(int millisec)
{
    clock_t startTime = clock();
    while(clock() < startTime + millisec) {
    }
}

void close_program(GtkWidget *wid, gpointer ptr)
{
    gtk_main_quit();
}

void clear_tbl(GtkWidget* wid, gpointer ptr)
{
    //g_object_ref(wid);
    gtk_container_remove(GTK_CONTAINER(tbl), wid);
}


void reprint_rooms_tbl()
{
    if(tbl) {
	gtk_container_foreach(GTK_CONTAINER(tbl), clear_tbl, NULL);
    }
    //tbl = gtk_table_new(MAX_ROOM, 7, FALSE);
    //gtk_table_set_row_spacings(GTK_TABLE(tbl), 50);
    //gtk_table_set_col_spacings(GTK_TABLE(tbl), 1);
    
    //uint8_t rowStart = roomRowInfos[curSect]->rowStart;
    //uint8_t rowEnd = roomRowInfos[curSect]->rowEnd;
    uint8_t rowStart = 0;
    uint8_t rowEnd = 1;

    for(uint8_t n=sectRoomFront[curSect];n<sectRoomItemCount[curSect];n++) {
	
        gtk_table_attach(GTK_TABLE(tbl), (roomsOfSect[curSect][n])->roomNum, 
   		    0,2,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
        gtk_table_attach(GTK_TABLE(tbl), (roomsOfSect[curSect][n])->delRoomBtn, 
		    2,3,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
        gtk_table_attach(GTK_TABLE(tbl), (roomsOfSect[curSect][n])->seqNum, 
		    3,4,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
        gtk_table_attach(GTK_TABLE(tbl), (roomsOfSect[curSect][n])->decrSeqBtn, 
		    4,5,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
        gtk_table_attach(GTK_TABLE(tbl), (roomsOfSect[curSect][n])->printTicket, 
		    5,6,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);

	rowStart++;
	rowEnd++;
    }

    roomRowInfos[curSect]->rowStart = rowStart;
    roomRowInfos[curSect]->rowEnd = rowEnd;

    gtk_widget_show_all(tbl);
}

void del_room_row(GtkWidget* wid, gpointer ptr)
{
    GList* roomLst = gtk_container_get_children(GTK_CONTAINER(roomsOfSect2[curSect]));
    
    char curRowLbl[3];
    gint rowIdx = g_list_index(roomLst, wid);
    sprintf(curRowLbl, "%d", rowIdx);
    gtk_label_set_text(GTK_LABEL(debugLbl), curRowLbl);
    //gtk_widget_destroy(GTK_WIDGET(roomRow->data));
    
    //GList* roomRow = g_list_nth(roomLst, 0);
    //gtk_widget_destroy(GTK_WIDGET(roomRow->data));
    //roomRow = g_list_nth(roomLst, 1);
    //gtk_widget_destroy(GTK_WIDGET(roomRow->data));
    //roomRow = g_list_nth(roomLst, 2);
    //gtk_widget_destroy(GTK_WIDGET(roomRow->data));
    //roomRow = g_list_nth(roomLst, 3);
    //gtk_widget_destroy(GTK_WIDGET(roomRow->data));
    //roomRow = g_list_nth(roomLst, 4);
    //gtk_widget_destroy(GTK_WIDGET(roomRow->data));

    //guint totalRowNum = roomRowInfos[curSect]->rowStart;
    //guint row = ((totalRowNum-1)*5) - ((*(guint*)ptr)*5);
    for(guint n=0;n<5;n++) {
        GList* roomRow = g_list_nth(roomLst, (rowIdx+1-n));
        gtk_widget_destroy(GTK_WIDGET(roomRow->data));
    }
}

void add_new_room(GtkWidget* wid, gpointer ptr)
{
    ////////////////////////
    /* 1st Implementation */

    //Room room = {
	//    gtk_entry_new(),
	//    gtk_button_new_with_label("Del"),
	//    gtk_entry_new(),
	//    gtk_button_new_with_label("Decr"),
	//    gtk_button_new_with_label("Ticket")
    //};

    //insertRoom(roomsOfSect[curSect], &room);
    //insertRoom(roomsOfSect, &room);
    
    //insertRoom(roomsOfSect, &room, curSect, sectRoomRear, sectRoomItemCount);
    //insertRoom(roomsOfSect, room, curSect, sectRoomRear, sectRoomItemCount);

    //gtk_widget_set_size_request(room.roomNum, -1, 100);
    //gtk_widget_set_size_request(room.delRoomBtn, -1, 100);
    //gtk_widget_set_size_request(room.seqNum, -1, 100);
    //gtk_widget_set_size_request(room.decrSeqBtn, -1, 100);
    //gtk_widget_set_size_request(room.printTicket, -1, 100);
 
    //uint8_t rowStart = roomRowInfos[curSect]->rowStart;
    //uint8_t rowEnd = roomRowInfos[curSect]->rowEnd;

    //GtkWidget* tbl = (GtkWidget*)ptr;

    //gtk_table_attach(GTK_TABLE(tbl), room.roomNum, 0,2,rowStart,rowEnd,
	//	    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
    //gtk_table_attach(GTK_TABLE(tbl), room.delRoomBtn, 2,3,rowStart,rowEnd,
	//	    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
    //gtk_table_attach(GTK_TABLE(tbl), room.seqNum, 3,4,rowStart,rowEnd,
	//	    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
    //gtk_table_attach(GTK_TABLE(tbl), room.decrSeqBtn, 4,5,rowStart,rowEnd,
	//	    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
    //gtk_table_attach(GTK_TABLE(tbl), room.printTicket, 5,6,rowStart,rowEnd,
	//	    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);

    //roomRowInfos[curSect]->rowStart++;
    //roomRowInfos[curSect]->rowEnd++;

    //gtk_widget_show_all(tbl);


    ////////////////////////
    /* 2nd Implementation */

    GtkWidget* roomNum = gtk_entry_new();
    GtkWidget* delRoomBtn = gtk_button_new_with_label("Del");
    GtkWidget* seqNum = gtk_entry_new();
    GtkWidget* decrSeqBtn = gtk_button_new_with_label("Decr");
    GtkWidget* printTicket = gtk_button_new_with_label("Ticket");

    gtk_widget_set_size_request(roomNum, -1, 100);
    gtk_widget_set_size_request(delRoomBtn, -1, 100);
    gtk_widget_set_size_request(seqNum, -1, 100);
    gtk_widget_set_size_request(decrSeqBtn, -1, 100);
    gtk_widget_set_size_request(printTicket, -1, 100);
    
    uint8_t rowStart = roomRowInfos[curSect]->rowStart;
    uint8_t rowEnd = roomRowInfos[curSect]->rowEnd;

    gtk_table_attach(GTK_TABLE(roomsOfSect2[curSect]), roomNum, 0,2,rowStart,rowEnd,
		    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
    gtk_table_attach(GTK_TABLE(roomsOfSect2[curSect]), delRoomBtn, 2,3,rowStart,rowEnd,
		    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
    gtk_table_attach(GTK_TABLE(roomsOfSect2[curSect]), seqNum, 3,4,rowStart,rowEnd,
		    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
    gtk_table_attach(GTK_TABLE(roomsOfSect2[curSect]), decrSeqBtn, 4,5,rowStart,rowEnd,
		    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
    gtk_table_attach(GTK_TABLE(roomsOfSect2[curSect]), printTicket, 5,6,rowStart,rowEnd,
		    GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);

    roomRowInfos[curSect]->rowStart++;
    roomRowInfos[curSect]->rowEnd++;

    g_signal_connect(delRoomBtn, "clicked", G_CALLBACK(del_room_row), 
		    NULL); 


    //gtk_container_remove(GTK_CONTAINER(vport), tbl);
    //gtk_container_add(GTK_CONTAINER(vport), roomsOfSect2[curSect]);
    gtk_widget_show_all(roomsOfSect2[curSect]);
}

void send_key(GtkWidget* wid, gpointer entryPtr)
{
    if(strcmp(gtk_button_get_label(GTK_BUTTON(wid)),"<-")==0) {
        int txtLen = gtk_entry_get_text_length(GTK_ENTRY(entryPtr));
	gtk_editable_delete_text(GTK_EDITABLE(entryPtr),
		    txtLen-1, -1);
	return;
    }    
    int pos = -1;
    gtk_editable_insert_text(GTK_EDITABLE(entryPtr),
		    gtk_button_get_label(GTK_BUTTON(wid)),1,&pos);
}

void update_cur_sect(GtkWidget* wid, gpointer ptr)
{
    // Update curSect
    GtkButton* selectedSectBtn = GTK_BUTTON(wid);
    const char* selectedSectBtnLbl = gtk_button_get_label(selectedSectBtn);
    for(uint8_t n=getSectFront();n<sizeSect();n++) {
        const char* sectBtnLbl = gtk_button_get_label(sections[n]);
	if(strcmp(sectBtnLbl, selectedSectBtnLbl)==0) {
            if(curSect == n) break;

            g_object_ref(roomsOfSect2[curSect]);
            gtk_container_remove(GTK_CONTAINER(vport), roomsOfSect2[curSect]); 
	    //update currently selected sect
	    curSect = n; 	    
            gtk_container_add(GTK_CONTAINER(vport), roomsOfSect2[curSect]);
            gtk_widget_show_all(roomsOfSect2[curSect]);
	    break;
	}
    }

    // Update section button indicator image
    for(uint8_t n=getSectFront();n<sizeSect();n++) {
	if(n == curSect) {
            GtkWidget* sectImg = gtk_image_new_from_file("sect.png");
            gtk_button_set_image(sections[n], sectImg);
	} else {
            GtkWidget* oldSectImg = gtk_button_get_image(sections[n]);
	    if(oldSectImg) g_free(oldSectImg);
            gtk_button_set_image(sections[n], NULL);
	}
    }

    //reprint_rooms_tbl();
}

void add_new_section(GtkWidget* wid, gpointer ptr)
{
    if(isSectFull()) {
        // max amount of sections added,
	// so no more new section will be created.
        return;
    }

    GtkWidget* dlg = gtk_dialog_new_with_buttons(
		    "Enter a name",
		    GTK_WINDOW(win),
		    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		    "Cancel", 0, "OK", 1, NULL);

    GtkWidget* entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 5);

    // Create a virtual keyboard
    GtkWidget* keybdTbl = gtk_table_new(4, 10, FALSE);
    for(int row=0;row<4;row++) {
        for(int col=0;col<10;col++) {
            GtkWidget* key = gtk_button_new_with_label(keyboard[row][col]);
            gtk_widget_set_size_request(key, 70, 50);

	    gtk_table_attach(GTK_TABLE(keybdTbl), key, col,col+1, 
	        row,row+1, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);

	    g_signal_connect(key,"clicked",G_CALLBACK(send_key),entry);
	}
    }

    GtkWidget* dlgVbox = gtk_vbox_new(FALSE, 5);

    gtk_box_pack_start(GTK_BOX(dlgVbox), entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dlgVbox), keybdTbl, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(
				    GTK_DIALOG(dlg))), dlgVbox);

    gtk_widget_show_all(dlgVbox);

    int result = gtk_dialog_run(GTK_DIALOG(dlg));
    if(result == 0) {
        gtk_widget_destroy(dlg);
        return; //user clicked 'cancel' btn.
    }

    const char* sectName = gtk_entry_get_text(GTK_ENTRY(entry));
    if(!sectName[0]) { //empty string given
	gtk_widget_destroy(dlg);
	return;
    }

    // Create a button representing a section
    GtkWidget* sect = gtk_button_new_with_label(sectName);
    g_signal_connect(sect, "clicked", G_CALLBACK(update_cur_sect), NULL);

    insertSect(sections, GTK_BUTTON(sect));

    gtk_widget_set_size_request(sect, -1, 100);
    
    GtkWidget* vbox3 = (GtkWidget*)ptr;

    gtk_box_pack_start(GTK_BOX(vbox3), sect, FALSE, FALSE, 0); 

    gtk_widget_destroy(dlg);

    gtk_widget_show_all(vbox3);
}

void main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    roomsOfSect2[0] = gtk_table_new(10,7,FALSE);
    roomsOfSect2[1] = gtk_table_new(10,7,FALSE);
    roomsOfSect2[2] = gtk_table_new(10,7,FALSE);
    roomsOfSect2[3] = gtk_table_new(10,7,FALSE);
    roomsOfSect2[4] = gtk_table_new(10,7,FALSE);

    gtk_table_set_row_spacings(GTK_TABLE(roomsOfSect2[0]), 50);
    gtk_table_set_col_spacings(GTK_TABLE(roomsOfSect2[0]), 1);
    gtk_table_set_row_spacings(GTK_TABLE(roomsOfSect2[1]), 50);
    gtk_table_set_col_spacings(GTK_TABLE(roomsOfSect2[1]), 1);
    gtk_table_set_row_spacings(GTK_TABLE(roomsOfSect2[2]), 50);
    gtk_table_set_col_spacings(GTK_TABLE(roomsOfSect2[2]), 1);
    gtk_table_set_row_spacings(GTK_TABLE(roomsOfSect2[3]), 50);
    gtk_table_set_col_spacings(GTK_TABLE(roomsOfSect2[3]), 1);
    gtk_table_set_row_spacings(GTK_TABLE(roomsOfSect2[4]), 50);
    gtk_table_set_col_spacings(GTK_TABLE(roomsOfSect2[4]), 1);

    // Load image assets
    //sectImg = gtk_image_new_from_file("sect.png");

    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "qms.glade", NULL);

    // Widgets from qms.glade
    //GtkWidget* win = (GtkWidget*)gtk_builder_get_object(builder, "window1");
    win = (GtkWidget*)gtk_builder_get_object(builder, "window1");
    GtkWidget* vbox1 = (GtkWidget*)gtk_builder_get_object(builder, "vbox1");
    GtkWidget* vbox2 = (GtkWidget*)gtk_builder_get_object(builder, "vbox2");
    GtkWidget* vbox3 = (GtkWidget*)gtk_builder_get_object(builder, "vbox3");
    GtkWidget* swin = (GtkWidget*)gtk_builder_get_object(builder, "scolledwindow1");
    //GtkWidget* vport = (GtkWidget*)gtk_builder_get_object(builder, "viewport1");
    vport = (GtkWidget*)gtk_builder_get_object(builder, "viewport1");
    gtk_container_add(GTK_CONTAINER(vport), roomsOfSect2[curSect]);
    //GtkWidget* tbl = (GtkWidget*)gtk_builder_get_object(builder, "table1");
    //tbl = (GtkWidget*)gtk_builder_get_object(builder, "table1");
    GtkWidget* newSection = (GtkWidget*)gtk_builder_get_object(builder, "newSection");
    GtkWidget* newRoom = (GtkWidget*)gtk_builder_get_object(builder, "newRoom");
    GtkWidget* editRoom = (GtkWidget*)gtk_builder_get_object(builder, "editRoom");
    GtkWidget* printTicketAuto = (GtkWidget*)gtk_builder_get_object(builder, "printTicketAuto");

    // Debug Label Widget
    debugLbl = gtk_label_new("Debugging Label");
    gtk_box_pack_start(GTK_BOX(vbox1), debugLbl, FALSE, FALSE, 0);
    
    //Initialize Room Row Info for Each Section 
    RoomRowInfo rowInfoSect1 = {0,1};
    RoomRowInfo rowInfoSect2 = {0,1};
    RoomRowInfo rowInfoSect3 = {0,1};
    RoomRowInfo rowInfoSect4 = {0,1};
    RoomRowInfo rowInfoSect5 = {0,1};
    //roomRowInfos[curSect] = &rowInfoSect1;
    roomRowInfos[0] = &rowInfoSect1;
    roomRowInfos[1] = &rowInfoSect2;
    roomRowInfos[2] = &rowInfoSect3;
    roomRowInfos[3] = &rowInfoSect4;
    roomRowInfos[4] = &rowInfoSect5;

    // Toplevel Windows Setup
    gtk_window_set_title(GTK_WINDOW(win), "QMS App");
    gtk_widget_set_size_request(win, 800, 600);

    // Signals Setup
    g_signal_connect(win, "delete-event", G_CALLBACK(close_program), NULL);
    //g_signal_connect(newRoom, "clicked", G_CALLBACK(add_new_room), tbl);
    g_signal_connect(newRoom, "clicked", G_CALLBACK(add_new_room), NULL);
    g_signal_connect(newSection, "clicked", G_CALLBACK(add_new_section), vbox3);
     
    gtk_widget_show_all(win);
    gtk_main();
}
