#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include <menu.h>
#include "nc.h"
#include "cpans.h"

// XXX: 
#define ARRAY_SIZE(a) ( 100 )
#define CTRLD 	4

char **choices;

ITEM **cpans_menu_items;
MENU *cpans_menu;
ITEM *cur_item;


void cpans_nc_init( moduledata_t ** mlist , size_t mlistsize )
{
    int i;

    /* Initialize curses */
    initscr ();
    cbreak ();
    noecho ();
    keypad (stdscr, TRUE);

    /* Initialize items */
    cpans_menu_items = (ITEM **) calloc ( mlistsize + 1, sizeof (ITEM *));
    for (i = 0; i < mlistsize; ++i)
        cpans_menu_items[i] = new_item( strdup(mlist[i]->name) , strdup(mlist[i]->version) );


    cpans_menu_items[ mlistsize ] = (ITEM *) NULL;

    cpans_menu = new_menu ((ITEM **) cpans_menu_items);

    set_menu_format( cpans_menu , LINES - 6 , 1 );
    set_menu_mark(   cpans_menu , " * ");
    set_menu_pad(    cpans_menu , 10 );
    set_menu_spacing(    cpans_menu , 5 , 0 , 0 );

    /* Make the menu multi valued */
    menu_opts_off( cpans_menu, O_ONEVALUE);

    mvprintw (LINES - 3, 0, "<SPACE>: select item.  <Q>: quit. <J/K>: move item.");
    mvprintw (LINES - 2, 0, "<ENTER> to install selected modules.");
    post_menu (cpans_menu);
    refresh ();
}


void cpans_nc_loop()
{
    int i;
    int c;
    while ((c = getch ()) != 'q' )
    {
        switch (c)
        {

        case 'j':
        case KEY_DOWN:
            menu_driver(cpans_menu, REQ_DOWN_ITEM);
            break;

        case 'k':
        case KEY_UP:
            menu_driver(cpans_menu, REQ_UP_ITEM);
            break;

        // toggle item
        case ' ':
            menu_driver(cpans_menu, REQ_TOGGLE_ITEM);
            break;

        case 10:               /* Enter */
            {
                // scan selected items
                char temp[200];
                ITEM **items;
                items = menu_items (cpans_menu);
                temp[0] = '\0';
                for (i = 0; i < item_count (cpans_menu); ++i)
                    if (item_value (items[i]) == TRUE)
                    {
                        strcat (temp, item_name (items[i]));
                        strcat (temp, " ");
                    }
                //move (20, 0);
                //clrtoeol ();
                mvprintw ( LINES - 5 , 0, temp);
                refresh ();
            }
            break;
        }
    }
}

void cpans_nc_end() 
{ 
    free_item (cpans_menu_items[0]);
    free_item (cpans_menu_items[1]);
    free_menu (cpans_menu);
    endwin ();
}
