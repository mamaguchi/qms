#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>


#define MAX_ROOM 10

typedef struct
{
    GtkWidget* roomNum;
    GtkWidget* delRoomBtn;
    GtkWidget* seqNum;
    GtkWidget* decrSeqBtn;
    GtkWidget* printTicket;
} Room;

typedef struct
{
    uint8_t rowStart;
    uint8_t rowEnd;
} RoomRowInfo;

//uint8_t roomFront = 0;
//uint8_t roomRear = -1;
//uint8_t roomItemCount = 0;

#ifndef MAX_SECT
#define MAX_SECT 5
#endif

uint8_t sectRoomFront[] = {0, 0, 0, 0, 0};
uint8_t sectRoomRear[] = {-1, -1, -1, -1, -1};
uint8_t sectRoomItemCount[] = {0, 0, 0, 0, 0};

uint8_t curSect = 0;

Room* peekRoom(Room* section[], uint8_t curSect, uint8_t sectRoomFront[])
{
    return section[sectRoomFront[curSect]];
}

bool isRoomEmpty(uint8_t curSect, uint8_t sectRoomItemCount[])
{
    return sectRoomItemCount[curSect] == 0;
}

bool isRoomFull(uint8_t curSect, uint8_t sectRoomItemCount[])
{
    return sectRoomItemCount[curSect] == MAX_ROOM;
}

uint8_t sizeRoom(uint8_t curSect, uint8_t sectRoomItemCount[])
{
    return sectRoomItemCount[curSect];
}

//void insertRoom(Room* section[], Room* roomPtr)
//{
//    if(!isRoomFull()) {
//        if(sectRoomRear[curSect] == MAX_ROOM-1) {
//	    sectRoomRear[curSect] = -1;
//	}
//	section[++sectRoomRear[curSect]] = roomPtr;
//	sectRoomItemCount[curSect]++;
//    }
//}

void insertRoom(Room* section[][MAX_ROOM], Room* roomPtr)
{
    if(!isRoomFull(curSect, sectRoomItemCount)) {
        if(sectRoomRear[curSect] == MAX_ROOM-1) {
	    sectRoomRear[curSect] = -1;
	}
	section[curSect][++sectRoomRear[curSect]] = roomPtr;
	sectRoomItemCount[curSect]++;
    }
}


//void insertRoom(Room section[][MAX_ROOM], Room roomPtr, uint8_t curSect,
//		uint8_t sectRoomRear[], uint8_t sectRoomItemCount[])
//{
//    if(!isRoomFull(curSect, sectRoomItemCount)) {
//        if(sectRoomRear[curSect] == MAX_ROOM-1) {
//	    sectRoomRear[curSect] = -1;
//	}
//	section[curSect][++sectRoomRear[curSect]] = roomPtr;
//	sectRoomItemCount[curSect]++;
//    }
//}


Room* removeRoom(Room* section[])
{
    Room* room = section[sectRoomFront[curSect]++];

    if(sectRoomFront[curSect] == MAX_ROOM) sectRoomFront[curSect] = 0;

    sectRoomItemCount[curSect]--;
    return room;
}





