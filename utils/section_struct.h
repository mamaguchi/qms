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

uint8_t getSectItemCount() 
{
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

void removeSectByIdx(GtkButton* section[], uint8_t n)
{
    uint8_t newCount = --sectItemCount;
    GtkButton* newSection[newCount];
    uint8_t newCtr = 0;

    for(uint8_t i=0;i<=sectItemCount;i++) {
        if(i==n) {
            gtk_widget_destroy(GTK_WIDGET(section[n]));
	} else {
            newSection[newCtr] = section[i];
	    section[i] = NULL;
	    newCtr++;
	}
    }
    sectRear = -1;
    sectFront = 0;

    //return newSection; /* Error: assignment to expression with array type */
    //section = newSection; /* Error: assignment to expression with array type */
    memmove(section, newSection, sizeof(GtkButton*) * sectItemCount); /* Correct */
}
