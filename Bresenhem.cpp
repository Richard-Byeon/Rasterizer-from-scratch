#include "Bresenhem.h"

void Bresenhem(int x0, int y0, int x1, int y1, std::vector<Pixel>& dest)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int sx = (dx > 0) ? 1 : -1;
    int sy = (dy > 0) ? 1 : -1;

    dx = sx * dx;
    dy = sy * dy;

    int x = x0;
    int y = y0;

    // |m| <= 1
    if (dx > dy)
    {
        int D = 2 * dy - dx;

        for (int i = 0; i < dx; i++)
        {
            if (D > 0)
            {
                y += sy;
                D += 2 * (dy - dx);
            }

            else
                D += 2 * dy;

            x += sx;

            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                dest[y * WIDTH + x] = { BLACK, BLACK, BLACK, 255 }; // DRAW AS A BLACK
        }

    }

    // |m| > 1 -> swap x and y.
    else
    {
        int D = 2 * dx - dy;
        for (int i = 0; i < dy; i++)
        {
            if (D > 0)
            {
                x += sx;
                D += 2 * (dx - dy);
            }

            else
                D += 2 * dx;

            y += sy;

            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                dest[y * WIDTH + x] = { BLACK, BLACK, BLACK, 255 };
        }
    }


}