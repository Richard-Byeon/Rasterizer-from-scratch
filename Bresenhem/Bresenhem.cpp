#include "Bresenhem.h"
#include "Barycentric/Barycentric.h"

void Bresenhem(int x0, int y0, int x1, int y1, std::vector<Color>& dest)
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
//void ClearFrame(std::vector<Color>& fBuffer)
//{
//    for (int y = 0; y < HEIGHT; y++)
//    {
//        for (int x = 0; x < WIDTH; x++)
//            fBuffer[y * HEIGHT + x] = { WHITE, WHITE, WHITE, WHITE };
//    }
//}
// 

bool isBackFace(const Vec4& v1, const Vec4& v2, const Vec4& v3)
{
    float det = (v2.v[0] - v1.v[0]) * (v3.v[1] - v1.v[1])
              - (v2.v[1] - v1.v[1]) * (v3.v[0] - v1.v[0]);

    return (det < 0.0f);
}

// Draws triangle
void Draw(std::vector<Vertex>& vbuffer, std::vector<uint32_t>& ibuffer, std::vector<Color>& fbuffer)
{
    // ClearFrame(fbuffer);
    
    // Divide index buffer's size by 3 so that we can stride(or traverse) index buffer by 3.
    int triangleCount = ibuffer.size() / 3;

    for (int t = 0; t < triangleCount; t++)
    {
        uint32_t i0 = ibuffer[3 * t + 0];
        uint32_t i1 = ibuffer[3 * t + 1];
        uint32_t i2 = ibuffer[3 * t + 2];

        if (isBackFace(vbuffer[i0].Pos, vbuffer[i1].Pos, vbuffer[i2].Pos))
            continue;

        Bresenhem(vbuffer[i0].Pos.v[0], vbuffer[i0].Pos.v[1], vbuffer[i1].Pos.v[0], vbuffer[i1].Pos.v[1], fbuffer);
        Bresenhem(vbuffer[i1].Pos.v[0], vbuffer[i1].Pos.v[1], vbuffer[i2].Pos.v[0], vbuffer[i2].Pos.v[1], fbuffer);
        Bresenhem(vbuffer[i2].Pos.v[0], vbuffer[i2].Pos.v[1], vbuffer[i0].Pos.v[0], vbuffer[i0].Pos.v[1], fbuffer);

        //    // z-test?

    }
}

void Draw(std::vector<Vertex>& vbuffer, std::vector<uint32_t>& ibuffer, std::vector<Color>& fbuffer, std::vector<float>& zbuffer)
{
    int triangleCount = (int)ibuffer.size() / 3;

    for (int t = 0; t < triangleCount; t++)
    {
        uint32_t i0 = ibuffer[3 * t + 0];
        uint32_t i1 = ibuffer[3 * t + 1];
        uint32_t i2 = ibuffer[3 * t + 2];

        const Vertex& a = vbuffer[i0];
        const Vertex& b = vbuffer[i1];
        const Vertex& c = vbuffer[i2];

        if (isBackFace(a.Pos, b.Pos, c.Pos))
            continue;

        BoundingBox bbox = ComputeBoundingBox(a.Pos, b.Pos, c.Pos);
       
        RasterizeTriangle(a, b, c, bbox, fbuffer, zbuffer, WIDTH, HEIGHT);
        /*Bresenhem(vbuffer[i0].Pos.v[0], vbuffer[i0].Pos.v[1], vbuffer[i1].Pos.v[0], vbuffer[i1].Pos.v[1], fbuffer);
        Bresenhem(vbuffer[i1].Pos.v[0], vbuffer[i1].Pos.v[1], vbuffer[i2].Pos.v[0], vbuffer[i2].Pos.v[1], fbuffer);
        Bresenhem(vbuffer[i2].Pos.v[0], vbuffer[i2].Pos.v[1], vbuffer[i0].Pos.v[0], vbuffer[i0].Pos.v[1], fbuffer);
    */
    }

}