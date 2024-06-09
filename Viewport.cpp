#include "Viewport.h"

static constexpr inline double map(double v, double istart, double istop,
                                   double ostart, double ostop) {
    return ostart + (ostop - ostart) * ((v - istart) / (istop - istart));
}

Viewport::Viewport(int w, int h, double l, double r, double b, double t)
    : m_w(w), m_h(h), m_l(l), m_r(r), m_b(b), m_t(t) {}

int Viewport::getWidth() {
    return this->m_w;
}

int Viewport::getHeight() {
    return this->m_h;
}

double Viewport::xToPlane(int x) {
    return map(double(x), 0.0, double(this->m_w), this->m_l, this->m_r);
}

double Viewport::yToPlane(int y) {
    return map(double(y), double(this->m_h), 0.0, this->m_b, this->m_t);
}

int Viewport::xFromPlane(double x) {
    return int(map(x, this->m_l, this->m_r, 0.0, double(this->m_w)));
}

int Viewport::yFromPlane(double y) {
    return int(map(y, this->m_b, this->m_t, double(this->m_h), 0.0));
}
