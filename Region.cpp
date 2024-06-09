#include <cfenv>
#include "Region.h"

Region::Region(int x, int y, int side, Viewport vp)
    : m_x(x), m_y(y), m_side(side), m_vp(vp) {}

PlaneRegion Region::planeRegion() {
    fesetround(FE_TONEAREST);

    int l = this->m_x, t = this->m_y;
    int r = l + this->m_side, b = t + this->m_side;
    IVal x_interval = { this->m_vp.xToPlane(l), this->m_vp.xToPlane(r) };
    IVal y_interval = { this->m_vp.yToPlane(b), this->m_vp.yToPlane(t) };
    return PlaneRegion{ x_interval, y_interval };
}

bool Region::canSubdivide() {
    return this->m_side > 1;
}

RegionQuads Region::subdivide() {
    int side1 = this->m_side >> 1;
    int side2 = this->m_side - side1;
    int x = this->m_x, y = this->m_y;
    auto vp = this->m_vp;
    Region r1 = { x, y, side1, vp };
    Region r2 = { x + side1, y, side2, vp };
    Region r3 = { x, y + side1, side2, vp };
    Region r4 = { x + side1, y + side1, side2, vp };
    return RegionQuads{ r1, r2, r3, r4 };
}

void Region::getRectangle(int& x, int& y, int& w, int& h) {
    x = this->m_x;
    y = this->m_y;
    w = h = this->m_side;
}
