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

// Vbox1 Widgets
GtkWidget* newSection;
GtkWidget* newRoom;
GtkWidget* delSection;
GtkWidget* printTicketAuto;

// Section
GtkButton* sections[MAX_SECT];

// Debug Label
//TODO:To remove debugLbl in production
GtkWidget* debugLbl;
GtkWidget* hiddenEntry;

// Room
Room* roomsOfSect[MAX_SECT][MAX_ROOM];
GtkWidget* roomsOfSect2[MAX_SECT];
RoomRowInfo* roomRowInfos[5];
uint8_t roomPrintTicketInfos[5];
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

void clear_tbl(GtkWidget* wid, gpointer ptr)
{
    g_object_ref(wid);
    gtk_container_remove(GTK_CONTAINER(roomsOfSect2[curSect]), wid);
}

void clear_tbl_perma(GtkWidget* wid, gpointer ptr)
{
    //gtk_container_remove(GTK_CONTAINER(roomsOfSect2[curSect]), wid);
    gtk_container_remove(GTK_CONTAINER(ptr), wid);
}


void reprint_rooms_tbl()
{
    if(tbl) {
	gtk_container_foreach(GTK_CONTAINER(tbl), clear_tbl, NULL);
    }
    //tbl = gtk_table_new(MAX_ROOM, 7, FALSE);
    //gtk_table_set_row_spacings(GTK_TABLE(tbl), 50);
    //gtk_table_set_col_spacings(GTK_TABLE(tbl), 1);
    
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
    
    // For Debugging
    char curRowLbl[3];
    gint delBtnRvrsCellIdx = g_list_index(roomLst, wid);
    sprintf(curRowLbl, "%d", delBtnRvrsCellIdx);
    gtk_label_set_text(GTK_LABEL(debugLbl), curRowLbl);
    
    GtkWidget* newTbl = gtk_table_new(10,7,FALSE);
    gtk_table_set_row_spacings(GTK_TABLE(newTbl), 50);
    gtk_table_set_col_spacings(GTK_TABLE(newTbl), 1);

    uint8_t totalRowNum = roomRowInfos[curSect]->rowStart;
    int rowToDelRvrsIdx = delBtnRvrsCellIdx/5;
    int rowToDelIdx = (totalRowNum-1) - rowToDelRvrsIdx;
    uint8_t rowStart = 0;
    uint8_t rowEnd = 1;
    
    for(int i=0;i<totalRowNum;i++) {
        if(i==rowToDelIdx) {
    	    for(guint n=0;n<5;n++) {
                GList* roomRow = g_list_nth(roomLst, (delBtnRvrsCellIdx+1-n));
                gtk_widget_destroy(GTK_WIDGET(roomRow->data));
    	    }
	} else {
            int firstCellInRowRvrsIdx = (totalRowNum*5-1) - (i*5);
            GList* roomNum = g_list_nth(roomLst, firstCellInRowRvrsIdx);
            g_object_ref(roomNum->data);
	    gtk_container_remove(GTK_CONTAINER(roomsOfSect2[curSect]), GTK_WIDGET(roomNum->data));
            gtk_table_attach(GTK_TABLE(newTbl), GTK_WIDGET(roomNum->data), 0,2,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);

	    //TODO: reduce firstCellInRowRvrsIdx to self descrement
            firstCellInRowRvrsIdx = (totalRowNum*5-1) - (i*5) - 1;
            GList* delRoomBtn = g_list_nth(roomLst, firstCellInRowRvrsIdx);
            g_object_ref(delRoomBtn->data);
	    gtk_container_remove(GTK_CONTAINER(roomsOfSect2[curSect]), GTK_WIDGET(delRoomBtn->data));
            gtk_table_attach(GTK_TABLE(newTbl), GTK_WIDGET(delRoomBtn->data), 2,3,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);

	    firstCellInRowRvrsIdx = (totalRowNum*5-1) - (i*5) - 2;
            GList* seqNum = g_list_nth(roomLst, firstCellInRowRvrsIdx);
            g_object_ref(seqNum->data);
	    gtk_container_remove(GTK_CONTAINER(roomsOfSect2[curSect]), GTK_WIDGET(seqNum->data));
            gtk_table_attach(GTK_TABLE(newTbl), GTK_WIDGET(seqNum->data), 3,4,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
            
            firstCellInRowRvrsIdx = (totalRowNum*5-1) - (i*5) - 3;
            GList* decrSeqBtn = g_list_nth(roomLst, firstCellInRowRvrsIdx);
            g_object_ref(decrSeqBtn->data);
	    gtk_container_remove(GTK_CONTAINER(roomsOfSect2[curSect]), GTK_WIDGET(decrSeqBtn->data));
	    gtk_table_attach(GTK_TABLE(newTbl), GTK_WIDGET(decrSeqBtn->data), 4,5,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);
            
            firstCellInRowRvrsIdx = (totalRowNum*5-1) - (i*5) - 4;
            GList* printTicket = g_list_nth(roomLst, firstCellInRowRvrsIdx);
            g_object_ref(printTicket->data);
	    gtk_container_remove(GTK_CONTAINER(roomsOfSect2[curSect]), GTK_WIDGET(printTicket->data));
	    gtk_table_attach(GTK_TABLE(newTbl), GTK_WIDGET(printTicket->data), 5,6,rowStart,rowEnd, GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0,0);

            rowStart++;
            rowEnd++;

            g_signal_connect(delRoomBtn->data, "clicked", G_CALLBACK(del_room_row), NULL); 
	}
    }

    roomRowInfos[curSect]->rowStart = rowStart;
    roomRowInfos[curSect]->rowEnd = rowEnd;
    
    gtk_container_remove(GTK_CONTAINER(vport), roomsOfSect2[curSect]); 

    roomsOfSect2[curSect] = newTbl;
    gtk_container_add(GTK_CONTAINER(vport), roomsOfSect2[curSect]); 
    gtk_widget_show_all(roomsOfSect2[curSect]);

    g_list_free(roomLst);
}

void toggle_del_section_btn(GtkWidget* wid, gpointer ptr)
{
    if(gtk_button_get_relief(GTK_BUTTON(wid))==GTK_RELIEF_NORMAL) {
        gtk_button_set_relief(GTK_BUTTON(wid), GTK_RELIEF_NONE);
    } else {
        gtk_button_set_relief(GTK_BUTTON(wid), GTK_RELIEF_NORMAL);
    }
}

void edit_room_num(GtkWidget* wid, gpointer ptr)
{
    const char* seqNumStr = gtk_entry_get_text(GTK_ENTRY(wid));

    GtkWidget* dlg = gtk_dialog_new_with_buttons(
		    "",
		    GTK_WINDOW(win),
		    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		    "Yes", 1, "No", 0, NULL);
    
    GtkWidget* entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 5);
    gtk_entry_set_text(GTK_ENTRY(entry), seqNumStr);

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

    gtk_window_set_type_hint(GTK_WINDOW(dlg), GDK_WINDOW_TYPE_HINT_TOOLBAR);
    gtk_widget_show_all(dlgVbox);

    int result = gtk_dialog_run(GTK_DIALOG(dlg));
    if(result == 0) {
	//do nothing
    } else {
        const char* userInput = gtk_entry_get_text(GTK_ENTRY(entry));
        gtk_entry_set_text(GTK_ENTRY(wid), userInput);
    }

    gtk_widget_destroy(dlg);
    gtk_window_set_focus(GTK_WINDOW(win), GTK_WIDGET(hiddenEntry));
}

void edit_seq_num(GtkWidget* wid, gpointer ptr)
{
    const char* seqNumStr = gtk_entry_get_text(GTK_ENTRY(wid));

    GtkWidget* dlg = gtk_dialog_new_with_buttons(
		    "",
		    GTK_WINDOW(win),
		    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		    "Yes", 1, "No", 0, NULL);

    GtkWidget* lbl = gtk_label_new("");
    GdkColor color;
    gdk_color_parse("#ff0000", &color); //red color
    gtk_widget_modify_fg(lbl, GTK_STATE_NORMAL, &color);

    GtkWidget* entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 5);
    gtk_entry_set_text(GTK_ENTRY(entry), seqNumStr);

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
    gtk_box_pack_start(GTK_BOX(dlgVbox), lbl, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(
				    GTK_DIALOG(dlg))), dlgVbox);

    gtk_window_set_type_hint(GTK_WINDOW(dlg), GDK_WINDOW_TYPE_HINT_TOOLBAR);
    gtk_widget_show_all(dlgVbox);

    while(1) {
        int result = gtk_dialog_run(GTK_DIALOG(dlg));
        if(result == 0) {
	    break;
        } else {
	    const char* userInput = gtk_entry_get_text(GTK_ENTRY(entry));
	    int goNextLoop = 0;
	    int p = 0;
            while(userInput[p]) {
	        if(!g_ascii_isdigit(userInput[p])) {
		    gtk_label_set_text(GTK_LABEL(lbl), "Sequence must be numeric digit only!");
		    goNextLoop = 1;
		    break;
	        }
	        p++;
	    }
	    if(goNextLoop) continue;

            gtk_entry_set_text(GTK_ENTRY(wid), userInput);
	    break;
        }
    }

    gtk_widget_destroy(dlg);
    gtk_window_set_focus(GTK_WINDOW(win), GTK_WIDGET(hiddenEntry));
}

void decr_seq_num(GtkWidget* wid, gpointer ptr)
{
    const char* seqNumStr = gtk_entry_get_text(GTK_ENTRY(ptr));
    if(strcmp(seqNumStr, "\0")==0 || strcmp(seqNumStr, "0\0")==0) return;

    GtkWidget* dlg = gtk_dialog_new_with_buttons(
		    "",
		    GTK_WINDOW(win),
		    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		    "Yes", 1, "No", 0, NULL);

    GtkWidget* lbl = gtk_label_new("Are you sure to decrease sequence by 1?");

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(
				    GTK_DIALOG(dlg))), lbl);

    gtk_window_set_type_hint(GTK_WINDOW(dlg), GDK_WINDOW_TYPE_HINT_TOOLBAR);
    gtk_widget_show_all(lbl);

    int result = gtk_dialog_run(GTK_DIALOG(dlg));
    if(result == 0) {
        gtk_widget_destroy(dlg);
        return; //user clicked 'No' btn.
    } else {
        int seqNumInt = atoi(seqNumStr);
        seqNumInt--;
        char strBuf[10];
        sprintf(strBuf, "%d", seqNumInt);
        gtk_entry_set_text(GTK_ENTRY(ptr), strBuf);

        gtk_widget_destroy(dlg);
    }
}

void add_new_room(GtkWidget* wid, gpointer ptr)
{
    ////////////////////////
    /* 2nd Implementation */

    //Check if there exist any section before adding a room
    if(getSectItemCount() == 0) {
        GtkWidget* dlg = gtk_dialog_new_with_buttons(
	    	        "",
		        GTK_WINDOW(win),
		        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		        "OK", 1, NULL);

        GtkWidget* lbl = gtk_label_new("Add a new section first");

        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(
				    GTK_DIALOG(dlg))), lbl);

        //gtk_window_set_decorated(GTK_WINDOW(dlg), FALSE); /* remove titlebar & window decorations */
	//gtk_window_set_type_hint(GTK_WINDOW(dlg), GDK_WINDOW_TYPE_HINT_MENU);/* same effect as TOOLBAR */
	gtk_window_set_type_hint(GTK_WINDOW(dlg), GDK_WINDOW_TYPE_HINT_TOOLBAR);
        gtk_widget_show_all(lbl);
        gtk_dialog_run(GTK_DIALOG(dlg));
        gtk_widget_destroy(dlg);
	return;
    }

    //Create a new room
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

    g_signal_connect(delRoomBtn, "clicked", G_CALLBACK(del_room_row), NULL); 
    g_signal_connect(decrSeqBtn, "clicked", G_CALLBACK(decr_seq_num), seqNum); 
    g_signal_connect(roomNum, "focus-in-event", G_CALLBACK(edit_room_num), NULL); 
    g_signal_connect(seqNum, "focus-in-event", G_CALLBACK(edit_seq_num), NULL); 

    gtk_widget_show_all(roomsOfSect2[curSect]);
}

void print_ticket_auto(GtkWidget* wid, gpointer ptr)
{ 
    GList* roomLst = gtk_container_get_children(GTK_CONTAINER(roomsOfSect2[curSect]));
    
    uint8_t roomRowCountCurSect = roomRowInfos[curSect]->rowStart;
    if(roomRowCountCurSect==0) return;

    int roomRowToPrintTicket = roomPrintTicketInfos[curSect];
    int prevRoomRow = -1;
    if(roomRowCountCurSect > 1) {
	if((roomRowToPrintTicket-1)<0) {
            prevRoomRow = roomRowCountCurSect-1;
	} else {
            prevRoomRow = roomRowToPrintTicket-1;
	}
    }
        
    int firstCellInRoomRowPrntTicRvrsIdx = (roomRowCountCurSect*5-1) - (roomRowToPrintTicket*5);
    GList* roomNumPrntTicLst = g_list_nth(roomLst, firstCellInRoomRowPrntTicRvrsIdx);
    GList* seqNumPrntTicLst = g_list_nth(roomLst, firstCellInRoomRowPrntTicRvrsIdx-2);

    //Adjust Sequence Number
    const char* seqNumStr = gtk_entry_get_text(GTK_ENTRY(seqNumPrntTicLst->data));
    int seqNumInt = -1;
    if(strcmp(seqNumStr,"\0")==0 || strcmp(seqNumStr,"0\0")==0) {
        seqNumInt = 1;
    } else {
        seqNumInt = atoi(seqNumStr);
        seqNumInt++;
    }
    char strBuf[10];
    sprintf(strBuf, "%d", seqNumInt);
    gtk_entry_set_text(GTK_ENTRY(seqNumPrntTicLst->data), strBuf);
    
    //TODO:Print Ticket
    
    //Adjust Room Number Color
    GdkColor color;
    gdk_color_parse("#53cc31", &color); //green color
    gtk_widget_modify_text(GTK_WIDGET(roomNumPrntTicLst->data), GTK_STATE_NORMAL, &color);
    gtk_widget_modify_text(GTK_WIDGET(seqNumPrntTicLst->data), GTK_STATE_NORMAL, &color);
    if(prevRoomRow!=-1) {/* change prev row, if present, back to normal color */
        int firstCellInPrevRoomRowRvrsIdx = (roomRowCountCurSect*5-1) - (prevRoomRow*5);
        GList* roomNumPrevRowLst = g_list_nth(roomLst, firstCellInPrevRoomRowRvrsIdx);
        GList* seqNumPrevRowLst = g_list_nth(roomLst, firstCellInPrevRoomRowRvrsIdx-2);
        gtk_widget_modify_text(GTK_WIDGET(roomNumPrevRowLst->data), GTK_STATE_NORMAL, NULL);
        gtk_widget_modify_text(GTK_WIDGET(seqNumPrevRowLst->data), GTK_STATE_NORMAL, NULL);
    }

    //Adjust roomPrintTicketInfos sequence
    roomPrintTicketInfos[curSect] = (roomPrintTicketInfos[curSect]+1) % roomRowCountCurSect;

    g_list_free(roomLst);
}

void update_cur_sect(GtkWidget* wid, gpointer ptr)
{
    // Update curSect
    GtkButton* selectedSectBtn = GTK_BUTTON(wid);
    const char* selectedSectBtnLbl = gtk_button_get_label(selectedSectBtn);
    for(uint8_t n=getSectFront();n<sizeSect();n++) {
        const char* sectBtnLbl = gtk_button_get_label(sections[n]);
	if(strcmp(sectBtnLbl, selectedSectBtnLbl)==0) {
            //If delSection btn is ON,then delete this section
            if(gtk_button_get_relief(GTK_BUTTON(delSection))==GTK_RELIEF_NORMAL) {
                //=======================
		//    Delete Section
		//=======================

		/* Delete Confirmation Dialog */
                GtkWidget* dlg = gtk_dialog_new_with_buttons(
		                     "",
		                     GTK_WINDOW(win),
		                     GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		                     "Yes", 1, "No", 0, NULL);
                GtkWidget* lbl = gtk_label_new("Delete this section?");
                gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(
				        GTK_DIALOG(dlg))), lbl);
	        gtk_window_set_type_hint(GTK_WINDOW(dlg), GDK_WINDOW_TYPE_HINT_TOOLBAR);
                gtk_widget_show_all(lbl);
                int result = gtk_dialog_run(GTK_DIALOG(dlg));
                if(result == 0) {
                    gtk_widget_destroy(dlg);
                    return; //user clicked 'No' btn.
                } else {
                    gtk_widget_destroy(dlg);
                }

		/* Begin Section Deletion */
		if(n==curSect) {
		    //we clicked on same section already in viewport
                    gtk_container_remove(GTK_CONTAINER(vport), roomsOfSect2[n]); 
                } else {
		    //we clicked on a section not in viewport
	            gtk_container_forall(GTK_CONTAINER(roomsOfSect2[n]), clear_tbl_perma,
				    roomsOfSect2[n]);
		    //remove the section which is already in viewport but keep a reference
                    g_object_ref(roomsOfSect2[curSect]);
                    gtk_container_remove(GTK_CONTAINER(vport), roomsOfSect2[curSect]); 
		}
		//recreate a new table to replace the earlier one we removed
                roomsOfSect2[n] = gtk_table_new(10,7,FALSE);
                gtk_table_set_row_spacings(GTK_TABLE(roomsOfSect2[n]), 50);
                gtk_table_set_col_spacings(GTK_TABLE(roomsOfSect2[n]), 1);

		//remove the section entry in our bookeeping structure
		removeSectByIdx(sections, n);

	        //update currently selected sect. Set it to zero
                curSect = 0;
                gtk_container_add(GTK_CONTAINER(vport), roomsOfSect2[curSect]);
                gtk_widget_show_all(roomsOfSect2[curSect]);
	        break;

            } else {
		//=======================
		//    Update curSect
		//=======================
                if(n == curSect) break;/* we selected the same section btn */

                g_object_ref(roomsOfSect2[curSect]);
                gtk_container_remove(GTK_CONTAINER(vport), roomsOfSect2[curSect]); 
	        //update currently selected sect
	        curSect = n; 	    
                gtk_container_add(GTK_CONTAINER(vport), roomsOfSect2[curSect]);
                gtk_widget_show_all(roomsOfSect2[curSect]);
	        break;
            }
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

    gtk_window_set_type_hint(GTK_WINDOW(dlg), GDK_WINDOW_TYPE_HINT_TOOLBAR);
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
    gtk_button_set_relief(GTK_BUTTON(delSection), GTK_RELIEF_NONE);

    GtkWidget* sect = gtk_button_new_with_label(sectName);
    g_signal_connect(sect, "clicked", G_CALLBACK(update_cur_sect), NULL);

    insertSect(sections, GTK_BUTTON(sect));

    gtk_widget_set_size_request(sect, -1, 100);
    
    GtkWidget* vbox3 = (GtkWidget*)ptr;

    gtk_box_pack_start(GTK_BOX(vbox3), sect, FALSE, FALSE, 0); 

    gtk_widget_destroy(dlg);

    gtk_widget_show_all(vbox3);

    update_cur_sect(sect, NULL);
}

void main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    gtk_rc_parse("gtkrc");

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
    //GtkWidget* newSection = (GtkWidget*)gtk_builder_get_object(builder, "newSection");
    newSection = (GtkWidget*)gtk_builder_get_object(builder, "newSection");
    //GtkWidget* newRoom = (GtkWidget*)gtk_builder_get_object(builder, "newRoom");
    newRoom = (GtkWidget*)gtk_builder_get_object(builder, "newRoom");
    //GtkWidget* delSection = (GtkWidget*)gtk_builder_get_object(builder, "delSection");
    delSection = (GtkWidget*)gtk_builder_get_object(builder, "delSection");
    gtk_button_set_relief(GTK_BUTTON(delSection), GTK_RELIEF_NONE);
    //GtkWidget* printTicketAuto = (GtkWidget*)gtk_builder_get_object(builder, "printTicketAuto");
    printTicketAuto = (GtkWidget*)gtk_builder_get_object(builder, "printTicketAuto");

    // Debug Label Widget
    //debugLbl = gtk_label_new("Debugging Label");
    debugLbl = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox1), debugLbl, FALSE, FALSE, 0);

    // Hidden Entry
    hiddenEntry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox1), hiddenEntry, FALSE, FALSE, 0);

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

    //Initialize Room PrintTicket Info for Each Section
    roomPrintTicketInfos[0] = 0;
    roomPrintTicketInfos[1] = 0;
    roomPrintTicketInfos[2] = 0;
    roomPrintTicketInfos[3] = 0;
    roomPrintTicketInfos[4] = 0;

    // Toplevel Windows Setup
    gtk_window_set_title(GTK_WINDOW(win), "QMS App");
    gtk_widget_set_size_request(win, 800, 600);

    // Signals Setup
    g_signal_connect(win, "delete-event", G_CALLBACK(close_program), NULL);
    g_signal_connect(newRoom, "clicked", G_CALLBACK(add_new_room), NULL);
    g_signal_connect(newSection, "clicked", G_CALLBACK(add_new_section), vbox3);
    g_signal_connect(printTicketAuto, "clicked", G_CALLBACK(print_ticket_auto), NULL);
    g_signal_connect(delSection, "clicked", G_CALLBACK(toggle_del_section_btn), NULL);
     
    gtk_window_set_decorated(GTK_WINDOW(win), FALSE); /* remove titlebar & window decorations */
    gtk_widget_show_all(win);
    gtk_widget_hide(GTK_WIDGET(hiddenEntry));
    gtk_window_fullscreen(GTK_WINDOW(win));
    gtk_main();
}
