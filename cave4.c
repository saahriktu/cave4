/* Cave 4 v0.3
 * Cave with corridors, ninjas and one exit game.
 * More rooms than in first game.
 * Author: Kurashov Artem
 * License: GNU GPLv3
 */
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static int
get_random (int g_rand_max)
{
  return (int) ((g_rand_max + 1.0) * rand () / (RAND_MAX + 1.0));
}

void
putwinmsg (char *msgptr)
{
  WINDOW *mw;
  mw = newwin (3, strlen (msgptr) + 2, LINES / 2 - 1,
	       COLS / 2 - strlen (msgptr) / 2 - 1);
  box (mw, 0, 0);
  mvwaddstr (mw, 1, 1, msgptr);
  wrefresh (mw);
  wgetch (mw);
  delwin (mw);
}

int main ()
{
  int i, j, sr, fr, h = 6, cr[5][32], cmj = 0, mff[3][32], cmjv = 0;
  srand (time (0));
  for (i = 0; i < 5; i++)
    for (j = 0; j < 32; j++)
      cr[i][j] = get_random (31);
  for (i = 0; i < 3; i++)
    for (j = 0; j < 32; j++)
      mff[i][j] = 0;
  sr = get_random (31);
  while ((fr = get_random (31)) == sr);
  initscr ();
  clear ();
  noecho ();
  curs_set (0);
  cbreak ();
  keypad (stdscr, TRUE);
  WINDOW *gw, *mapw, *mapr;
  gw = newwin (8, 26, LINES / 2 - 4, COLS / 2 - 17);
  mapr = newwin (8, 12, LINES / 2 - 4, COLS / 2 + 9);
  mapw = newpad (96, 10);
  box (gw, 0, 0);
  box (mapr, 0, 0);
  mvwprintw (mapr, 1, 1, "   Map    ");
  wrefresh (mapr);
  for (;;)
    {
      if (sr > 9)
	mvwprintw (gw, 1, 1, "Current room:       %d", sr);
      else
	mvwprintw (gw, 1, 1, "Current room:       %d ", sr);
      mvwprintw (gw, 2, 1, "Presence of ninja:  ");
      mvwprintw (gw, 3, 1, "Life:");
      for (i = 0; i < 13; i++)
	{
	  if (i <= 12 * h / 6)
	    mvwaddch (gw, 3, 7 + i, '#');
	  else
	    mvwaddch (gw, 3, 7 + i, ' ');
	}
      mvwprintw (gw, 3, 21, "%d", 100 * h / 6);
      if (100 * h / 6 < 100)
	{
	  mvwaddch (gw, 3, 23, '%');
	  mvwaddch (gw, 3, 24, ' ');
	}
      else
	mvwaddch (gw, 3, 24, '%');
      if (sr == fr)
	{
	  putwinmsg ("You found exit! Congratulation! The end.");
	  endwin ();
	  return 0;
	}
      if (cr[0][sr] < 10)
	{
	  mvwprintw (gw, 2, 21, "Yes");
	  if (--h == 0)
	    {
	      putwinmsg ("You die... Game Over...");
	      endwin ();
	      return 0;
	    }
	}
      else
	mvwprintw (gw, 2, 21, "No ");
      mvwprintw (gw, 4, 1, "Corridors:          %d", 1 + cr[4][sr] / 11);
      mvwprintw (gw, 6, 1, " Choose corridor (1-%d)", 1 + cr[4][sr] / 11);
      wrefresh (gw);
      do
	{
	  if (cmjv > 0)
	    mvwaddch (mapr, 1, 10, '^');
	  else
	    mvwaddch (mapr, 1, 10, ' ');
	  if (cmj > cmjv + 5)
	    mvwaddch (mapr, 6, 10, 'V');
	  else
	    mvwaddch (mapr, 6, 10, ' ');
	  wrefresh (mapr);
	  i = wgetch (gw);
	  if (i > 47 && i < 58)
	    i -= 48;
	  else
	    {
	      // map routines check
	      if (i == 113 || i == 81)
		{
		  clear ();
		  refresh ();
		  endwin ();
		  return 0;
		}
	      if (i == 106 || i == 74)
		cmjv++;
	      if (i == 107 || i == 75)
		cmjv--;
	      if (cmjv < 0)
		cmjv = 0;
	      if (cmjv > 92)
		cmjv = 92;
	      prefresh (mapw, cmjv, 0, LINES / 2 - 2, COLS / 2 + 10,
			LINES / 2 + 2, COLS / 2 + 20);
	      i = 0;
	    }
	}
      while (i < 1 || i > 1 + cr[4][sr] / 11);
      if (mff[i][sr] == 0)
	{
	  mvwprintw (mapw, cmj, 0, "%d,%d %d", sr, i, cr[i][sr]);
	  mff[i][sr] = 1;
	  cmj++;
	  prefresh (mapw, cmjv, 0, LINES / 2 - 2, COLS / 2 + 10,
		    LINES / 2 + 2, COLS / 2 + 20);
	}
      sr = cr[i][sr];
    }
}
