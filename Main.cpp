#include <iostream>
#include <queue>
#include "Region.h"
#include "raylib.h"
#include "Interval.h"

using namespace std;

IBool func(IVal x, IVal y) {
    return x * x > y;
}

void update_regions(std::queue<Region> &regions, Image* img) {
    if (regions.empty())
        return;
    Region region = regions.front();
    regions.pop();
    int x, y, w, h;
    region.getRectangle(x, y, w, h);
    PlaneRegion planeRegion = region.planeRegion();
    IBool res = func(planeRegion.x, planeRegion.y);

    if (res.isTrue())
        ImageDrawRectangle(img, x, y, w, h, BLACK);
    else if (res.isFalse())
        ImageDrawRectangle(img, x, y, w, h, WHITE);
    else {
        ImageDrawRectangle(img, x, y, w, h, RED);

        if (region.canSubdivide()) {
            auto quads = region.subdivide();
            regions.push(quads.q1);
            regions.push(quads.q2);
            regions.push(quads.q3);
            regions.push(quads.q4);
        }
    }
}

int main()
{
    const int scale = 1;
    const int r_width = 512;
    const int r_height = 512;
    double l = -10.0, r = 10.0, b = -10.0, t = 10.0;

    SetTraceLogLevel(LOG_NONE);

    const int screenWidth = r_width * scale;
    const int screenHeight = r_height * scale;
    InitWindow(screenWidth, screenHeight, "Reliable2DGraphs");

    Image img = GenImageColor(r_width, r_height, RAYWHITE);
    Viewport vp = { r_width, r_height, l, r, b, t };

    std::queue<Region> regions;
    regions.push(Region{0, 0, r_width, vp});

    unsigned char i = 0;
    bool isPlaying = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (IsKeyPressed(KEY_P))
            isPlaying = !isPlaying;

        if (isPlaying || IsKeyPressed(KEY_SPACE) || IsKeyPressedRepeat(KEY_SPACE))
            update_regions(regions, &img);

        Texture2D tex = LoadTextureFromImage(img);
        DrawTextureEx(tex, Vector2{ 0, 0 }, 0, scale, WHITE);

        if (!regions.empty()) {
            Region topRegion = regions.front();
            int x, y, w, h;
            topRegion.getRectangle(x, y, w, h);

            DrawRectangleLines(x - 1, y - 1, w + 2, h + 2, YELLOW);
            DrawRectangleLines(x - 2, y - 2, w + 4, h + 4, BLACK);
        }

        EndDrawing();

        UnloadTexture(tex);
    }

    CloseWindow();
    return 0;
}
