#include <ncurses.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "MapObject.h"
#include "Bowl.h"
#include "Bed.h"
#include "Cat.h"
#include "Supplier.h"

int main(int argc, char *argv[])
{
	// default parameters
	int catsNum = 6;
	int bowlsNum = 4;
	int bedsNum = 3;
	int sleepTime = 3000;
	int eatTime = 2000;

	// user parameters
	std::cout << "Number of cats: ";
	std::cin >> catsNum;
	std::cout << "Number of bowls: ";
	std::cin >> bowlsNum;
	std::cout << "Number of beds: ";
	std::cin >> bedsNum;
	std::cout << "Sleep time (ms; +/-25%): ";
	std::cin >> sleepTime;
	std::cout << "Eat time (ms; +/-25%): ";
	std::cin >> eatTime;

	// start of the program
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, true);
	if (!has_colors())
	{
		printw("Terminal does not support colors");
		getch();
		return -1;
	}
	// init colors
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_GREEN);	  // wall
	init_pair(2, COLOR_WHITE, COLOR_RED);	  // cat
	init_pair(3, COLOR_WHITE, COLOR_MAGENTA); // bowl
	init_pair(4, COLOR_WHITE, COLOR_CYAN);	  // bed
	init_pair(5, COLOR_WHITE, COLOR_YELLOW);  // supplier
	init_pair(6, COLOR_BLACK, COLOR_BLACK);	  // background
	init_pair(7, COLOR_WHITE, COLOR_BLACK);	  // default

	// layout
	int objectsNumber = std::max(catsNum, std::max(bowlsNum, bedsNum));
	int objectsRowWidth = objectsNumber * 3;

	const int MAP_WIDTH = objectsRowWidth + 5;
	const int MAP_HEIGHT = 15;
	const int INFO_WIDTH = MAP_WIDTH;
	const int INFO_HEIGHT = catsNum;
	const int WINDOW_WIDTH = getmaxx(stdscr);
	const int WINDOW_HEIGHT = getmaxy(stdscr);

	const int WALL_ROW = (int)MAP_HEIGHT / 2;
	const int BOWLS_ROW = 1;
	const int EAT_ROW = 2;
	const int WAIT_EAT_ROW = WALL_ROW - 1;
	const int WAIT_SLEEP_ROW = WALL_ROW + 1;
	const int SLEEP_ROW = MAP_HEIGHT - 3;
	const int BEDS_ROW = MAP_HEIGHT - 2;

	// init bowls
	std::vector<Bowl *> bowls;
	for (int i = 0; i < bowlsNum; i++)
	{
		bowls.push_back(new Bowl(BOWLS_ROW, 1 + i * 3));
	}

	// init beds
	std::vector<Bed *> beds;
	for (int i = 0; i < bedsNum; i++)
	{
		beds.push_back(new Bed(BEDS_ROW, 1 + i * 3));
	}

	// bowls supplier
	Supplier *supplier = new Supplier(bowls);

	// init cats
	std::vector<Cat *> cats;
	for (int i = 0; i < catsNum; i++)
	{
		cats.push_back(new Cat(eatTime, sleepTime, bowls, beds, WAIT_EAT_ROW, 1 + i * 3, WAIT_SLEEP_ROW, 1 + i * 3));
	}

	// main loop
	while (getch() != 'q')
	{
		// clear background
		attron(COLOR_PAIR(6));
		for (int i = 1; i < MAP_HEIGHT - 1; i++)
		{
			move(i, 1);
			hline(' ', MAP_WIDTH);
		}
		attroff(COLOR_PAIR(6));

		// draw constant map
		attron(COLOR_PAIR(1));
		// border
		box(stdscr, 0, 0);
		// inner horizontal walls
		move(WALL_ROW, 1);
		hline(' ', objectsRowWidth);
		move(MAP_HEIGHT - 1, 1);
		hline(' ', WINDOW_WIDTH - 2);
		//text
		mvprintw(0, 2, " CATS ");
		mvprintw(WINDOW_HEIGHT - 1, 2, " Exit - Q ");
		mvprintw(MAP_HEIGHT - 1, 2, " Info ");
		attroff(COLOR_PAIR(1));

		// draw bowls
		attron(COLOR_PAIR(3));
		for (auto &b : bowls)
		{
			DrawInfo di = b->drawInfo();
			mvprintw(di.y, di.x, di.str.c_str(), di.data);
		}
		attroff(COLOR_PAIR(3));

		// draw beds
		attron(COLOR_PAIR(4));
		for (auto &b : beds)
		{
			DrawInfo di = b->drawInfo();
			mvprintw(di.y, di.x, di.str.c_str(), di.data);
		}
		attroff(COLOR_PAIR(4));

		attron(COLOR_PAIR(2));
		for (auto &c : cats)
		{
			DrawInfo di = c->drawInfo();
			mvprintw(di.y, di.x, di.str.c_str(), di.data);
		}
		attroff(COLOR_PAIR(2));

		// draw supplier
		attron(COLOR_PAIR(5));
		DrawInfo di = supplier->drawInfo();
		if (di.y != 0 && di.x != 0)
		{
			mvprintw(di.y, di.x, di.str.c_str(), di.data);
		}
		attroff(COLOR_PAIR(5));

		// draw table info
		attron(COLOR_PAIR(7));
		int positionY = MAP_HEIGHT;
		mvprintw(positionY, 1, "%5s | %10s | %10s | %10s | %10s | %10s", "Cat", "State", "Eats", "Sleeps", "EatWaits", "SleepWaits");
		for (auto &c : cats)
		{
			// positionY++;
			CatStats cs = c->currentStats();
			mvprintw(++positionY, 1, "%5d | %10s | %10d | %10d | %10d | %10d", c->getId(), cs.currentState.c_str(), cs.eats, cs.sleeps, cs.eatWaits, cs.sleepWaits);
		}
		attroff(COLOR_PAIR(7));

		// draw helpers
		++positionY;
		attron(COLOR_PAIR(2));
		mvprintw(++positionY, 2, "Cat(id)");
		attroff(COLOR_PAIR(2));

		attron(COLOR_PAIR(3));
		mvprintw(positionY, 11, "Bowl(food)");
		attroff(COLOR_PAIR(3));

		attron(COLOR_PAIR(4));
		mvprintw(positionY, 23, "Bed");
		attroff(COLOR_PAIR(4));

		attron(COLOR_PAIR(5));
		mvprintw(positionY, 28, "Supplier");
		attroff(COLOR_PAIR(5));

		// refresh window
		refresh();
	}

	// clear and free window
	clear();
	endwin();

	// free memory
	delete supplier;

	for (auto &c : cats)
		delete c;

	for (auto &b : beds)
		delete b;

	for (auto &b : bowls)
		delete b;

	std::cout << "Program closed" << std::endl;
	return 0;
}