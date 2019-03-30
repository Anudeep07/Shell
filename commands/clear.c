#include "../shell.h"

void clearscreen()
{
    write(1, "\33[H\33[2J", 7);
}