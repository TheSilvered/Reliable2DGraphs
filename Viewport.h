#pragma once

class Viewport {
public:
    Viewport(int w, int h, double l, double r, double b, double t);

    int getWidth();
    int getHeight();
    double xToPlane(int x);
    double yToPlane(int y);
    int xFromPlane(double x);
    int yFromPlane(double y);

private:
    int m_w, m_h;
    double m_l, m_r, m_b, m_t;
};
