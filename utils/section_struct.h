#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#define MAX_SECT 5


typedef struct
{
    uint8_t rowStart;
    uint8_t rowEnd;
} SectRowInfo;

uint8_t sectFront = 0;
uint8_t sectRear = -1;
uint8_t sectItemCount = 0;

uint8_t getSectItemCount() {
    return sectItemCount;
}

uint8_t getSectFront()
{
    return sectFront;
}

GtkButton* peekSect(GtkButton* section[])
{
    return section[sectFront];
}

bool isSectEmpty()
{
    return sectItemCount == 0;
}

bool isSectFull()
{
    return sectItemCount == MAX_SECT;
}

uint8_t sizeSect()
{
    return sectItemCount;
}

void insertSect(GtkButton* section[], GtkButton* btnPtr)
{
    if(!isSectFull()) {
        if(sectRear == MAX_SECT-1) {
	    sectRear = -1;
	}
	section[++sectRear] = btnPtr;
	sectItemCount++;
    }
}

GtkButton* removeSect(GtkButton* section[])
{
    GtkButton* sectBtn = section[sectFront++];

    if(sectFront == MAX_SECT) sectFront = 0;

    sectItemCount--;
    return sectBtn;
}
