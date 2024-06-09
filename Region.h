#pragma once
#include <tuple>
#include "Viewport.h"
#include "Interval.h"

struct PlaneRegion {
    IVal x, y;
};

struct RegionQuads;

class Region {
public:
    Region(int x, int y, int side, Viewport vp);
    PlaneRegion planeRegion();
    bool canSubdivide();
    RegionQuads subdivide();
    void getRectangle(int& x, int& y, int& w, int& h);

private:
    Viewport m_vp;
    int m_x, m_y, m_side;
};

struct RegionQuads {
    Region q1, q2, q3, q4;
};
