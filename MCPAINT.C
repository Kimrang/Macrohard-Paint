#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <graphics.h>
#define BRUSHSIZE 1
#define BRUSHCOLOR RED
#define ERASERSIZE 5
#define ERASERCOLOR BLACK
#pragma warn -par

union REGS in, out;

void initdisplay(int, int, struct canvas);
int initmouse();
void showmouseptr();
void getmousepos(int*, int*, int*);
void paint(int, int, int, int);

struct canvas
{
	int x1, y1;
	int x2, y2;
};

int main()
{
	char ch;
	int gdriver, gmode;
	int button, x, y, midx, midy, modex, modey, cirx, ciry;
	int brushsize, brushcolor, erasersize, erasercolor;
	int ind, score;
	char comm[10], mode[10];
	struct canvas c;

	clrscr();
	srand((unsigned int)time(NULL));	/* seeding random number */
	gdriver = DETECT;
	initgraph(&gdriver, &gmode, "C:\\TURBOC3\\BGI");
	if(initmouse() == 0)
	{
		outtextxy(180, 200, "Error: Mouse Support Unavailable");
		outtextxy(180, 210, "Enter any key to exit...");
		getch();
		return 1;
	}
	x = y = 0;
	midx = getmaxx()/2;
	midy = getmaxy()/2;
	/* current mode coordinates */
	modex = getmaxx()-50;
	modey = getmaxy()-50;
	/* color picker coordinates */
	cirx = getmaxx()-30;
	ciry = getmaxy()-15;
	/* canvas coordinates */
	c.x1 = midx-250;
	c.y1 = midy-100;
	c.x2 = midx+250;
	c.y2 = midy+180;
	brushsize = BRUSHSIZE;
	brushcolor = BRUSHCOLOR;
	erasersize = ERASERSIZE;
	erasercolor = ERASERCOLOR;

	initdisplay(midx, midy, c);
	showmouseptr();
	while(!kbhit())
	{
		getmousepos(&button, &x, &y);
		if(button != 0)
		{
			setcolor(RED);
			if((button==1) || (button==2))	/* for left/right click */
			{
				if(((c.x1+5<x)&&(x<c.x2-5)) && ((c.y1+5<y)&&(y<c.y2-5)))
				{
					setcolor(BLACK);
					outtextxy(modex, modey, mode);
					setcolor(RED);
					strcpy(mode, (button==1)?"Paint":"Erase");
					outtextxy(modex, modey, mode);
					if(button==1)
						paint(x, y, brushsize, brushcolor);
					else
						paint(x, y, erasersize, erasercolor);
				}
				else
				{
					setcolor(BLACK);
					outtextxy(getmaxx()-50, getmaxy()-50, mode);
				}
			}
			else	/* for middle mouse click */
			{
				++brushcolor;
				if((brushcolor==BLINK) || (brushcolor==BLACK))
					brushcolor = 1;
				setcolor(brushcolor);
				circle(cirx, ciry, 8);
				circle(cirx, ciry, 6);
			}
		}
	}
	restorecrtmode();
	score = rand() % 10;	/* emulating score */
	while(1)
	{
		ch = getchar();
		printf("> ");
		scanf("%s", comm);
		if(strcmp(comm, "submit") == 0)
		{
			if((0 <= score) && (score <= 3))
				puts("Practice make things perfect! :)");
			else if((3 < score) && (score <= 6))
				puts("Beautiful picture! :0");
			else if((6 < score) && (score <= 8))
				puts("Incredible Art!!!");
			else
				puts("Masterpiece!");
			printf("Rating: %d/10%c", score, ch);
		}
		else if(strcmp(comm, "help") == 0)
			printf(
			"submit\t\tsubmit art\n"
			"exit\t\texit program\n");
		else if(strcmp(comm, "exit") == 0)
			break;
		else
		{
			printf("%s: command not found\n", comm);
			puts("enter 'help' to list commands");
		}
	}
	closegraph();
	return 0;
}

void paint(int x, int y, int brushsize, int brushcolor)
{
	setcolor(brushcolor);
	circle(x, y, brushsize);
}

void initdisplay(int midx, int midy, struct canvas c)
{
	int ind;
	char quest[20];
	char item[][15] = {"Scenery", "Apple", "Watermelon", "Cat", "Tree",
			"Man", "Flower", "Dog", "Bee", "Football"};

	/*displaying logo*/
	setcolor(LIGHTCYAN);
	settextstyle(BOLD_FONT, HORIZ_DIR, 2);
	outtextxy(midx-150, 5, "MacroHard");
	setcolor(LIGHTMAGENTA);
	settextstyle(SCRIPT_FONT, HORIZ_DIR, 5);
	outtextxy(midx+20, 10, "Paint");

	/*displaying canvas*/
	setcolor(WHITE);
	rectangle(c.x1-3, c.y1-3, c.x2+3, c.y2+3);
	rectangle(c.x1, c.y1, c.x2, c.y2);

	/*displaying todo*/
	setcolor(GREEN);
	settextstyle(SIMPLEX_FONT, HORIZ_DIR, 1);
	ind = rand() % 10;
	sprintf(quest, "Draw a %s", item[ind]);
	outtextxy(midx-250, 100, quest);

	/*diplaying instructions*/
	setcolor(LIGHTGRAY);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(midx-130, getmaxy()-20, "LMB:Paint RMB:Erase MMB:Change Color");
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
}

int initmouse()
{
	in.x.ax = 0;
	int86(0X33, &in, &out);
	return out.x.ax;
}

void showmouseptr()
{
	in.x.ax = 1;
	int86(0X33, &in, &out);
}

void getmousepos(int *button, int *x, int *y)
{
	in.x.ax = 3;
	int86(0X33, &in, &out);
	*button = out.x.bx;
	*x = out.x.cx;
	*y = out.x.dx;
}
