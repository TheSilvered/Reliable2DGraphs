#include <cfenv>
#include <cmath>
#include "Interval.h"

static inline double add_up(double a, double b) {
    fesetround(FE_UPWARD);
    return a + b;
}
static inline double add_down(double a, double b) {
    fesetround(FE_DOWNWARD);
    return a + b;
}

static inline double sub_up(double a, double b) {
    fesetround(FE_UPWARD);
    return a - b;
}
static inline double sub_down(double a, double b) {
    fesetround(FE_DOWNWARD);
    return a - b;
}

static inline double mul_up(double a, double b) {
    fesetround(FE_UPWARD);
    return a * b;
}
static inline double mul_down(double a, double b) {
    fesetround(FE_DOWNWARD);
    return a * b;
}

static inline double div_up(double a, double b) {
    fesetround(FE_UPWARD);
    return a / b;
}

static inline double div_down(double a, double b) {
    fesetround(FE_DOWNWARD);
    return a / b;
}

static inline double sqrt_up(double x) {
    fesetround(FE_UPWARD);
    return sqrt(x);
}

static inline double sqrt_down(double x) {
    fesetround(FE_DOWNWARD);
    return sqrt(x);
}

constexpr double min4(double a, double b, double c, double d) {
    if (a < b && a < c && a < d)
        return a;
    else if (b < c && b < d)
        return b;
    return c < d ? c : d;
}

constexpr double max4(double a, double b, double c, double d) {
    if (a > b && a > c && a > d)
        return a;
    else if (b > c && b > d)
        return b;
    return c > d ? c : d;
}

IBool::IBool(bool lower, bool upper) {
    if (lower) {
        m_lower = upper;
        m_upper = lower;
    } else {
        m_lower = lower;
        m_upper = upper;
    }
}

IBool IBool::operator!() {
    return IBool(!m_lower, !m_upper);
}

IBool IBool::operator||(const IBool& other) {
    return IBool(m_lower || other.m_lower, m_upper || other.m_upper);
}

IBool IBool::operator&&(const IBool& other) {
    return IBool(m_lower && other.m_lower, m_upper && other.m_upper);
}

IVal::IVal(double lower, double upper) : m_domain(true) {
    if (upper < lower) {
        m_lower = upper;
        m_upper = lower;
    } else {
        m_lower = lower;
        m_upper = upper;
    }
}

IVal::IVal(double lower, double upper, IBool domain) : m_domain(domain) {
    if (upper < lower) {
        m_lower = upper;
        m_upper = lower;
    } else {
        m_lower = lower;
        m_upper = upper;
    }
}

IVal::IVal(double value) : m_lower(value), m_upper(value), m_domain(true) {}

// Arithmetic operators

IVal operator+(IVal lhs, IVal rhs) {
    IBool domain = lhs.domain() && rhs.domain();
    if (domain.isFalse())
        return i_undefined;

    return IVal(
        add_down(lhs.lower(), rhs.lower()),
        add_up(lhs.upper(), rhs.upper()),
        domain
    );
}

IVal operator-(IVal lhs, IVal rhs) {
    IBool domain = lhs.domain() && rhs.domain();
    if (domain.isFalse())
        return i_undefined;

    return IVal(
        sub_down(lhs.lower(), rhs.upper()),
        sub_up(lhs.upper(), rhs.lower()),
        domain
    );
}

IVal operator*(IVal lhs, IVal rhs) {
    IBool domain = lhs.domain() && rhs.domain();
    if (domain.isFalse())
        return i_undefined;

    double x1 = lhs.lower();
    double y1 = lhs.upper();
    double x2 = rhs.lower();
    double y2 = rhs.upper();

    if ((x1 >= 0 && x2 >= 0) || (x1 <= 0 && x2 <= 0))
        return IVal(mul_down(x1, x2), mul_up(y1, y2), domain);

    return IVal(
        min4(mul_down(x1, x2), mul_down(x1, y2), mul_down(y1, x2), mul_down(y1, y2)),
        max4(mul_up(x1, x2), mul_up(x1, y2), mul_up(y1, x2), mul_up(y1, y2)),
        domain
    );
}

IVal operator/(IVal lhs, IVal rhs) {
    IBool domain = lhs.domain() && rhs.domain();
    if (domain.isFalse())
        return i_undefined;

    double c = rhs.lower();
    double d = rhs.upper();

    if (c == 0 && d == 0)
        return i_undefined;

    if (c <= 0 && d >= 0)
        return IVal(-INFINITY, INFINITY, { false, true });

    double inv_lower = c == 0 ? -INFINITY : div_down(1, c);
    double inv_upper = d == 0 ? INFINITY : div_up(1, d);

    IVal inv_rhs = IVal(inv_lower, inv_upper, { c != 0 && d != 0, true });

    return lhs * inv_rhs;
}

// Comparisons

IBool operator>(IVal lhs, IVal rhs) {
    IBool true_domain = lhs.domain() && rhs.domain();
    if (true_domain.isFalse())
        return IBool(false);

    if (lhs.lower() > rhs.upper())
        return true_domain;
    else if (lhs.upper() < rhs.lower())
        return IBool(false);
    return IBool(false, true);
}

IBool operator>=(IVal lhs, IVal rhs) {
    IBool true_domain = lhs.domain() && rhs.domain();
    if (true_domain.isFalse())
        return IBool(false);

    if (lhs.lower() >= rhs.upper())
        return true_domain;
    else if (lhs.upper() <= rhs.lower())
        return IBool(false);
    return IBool(false, true);
}

IBool operator<(IVal lhs, IVal rhs) {
    IBool true_domain = lhs.domain() && rhs.domain();
    if (true_domain.isFalse())
        return IBool(false);

    if (lhs.upper() < rhs.lower())
        return true_domain;
    else if (lhs.lower() > rhs.lower())
        return IBool(false);
    return IBool(false, true);
}

IBool operator<=(IVal lhs, IVal rhs) {
    IBool true_domain = lhs.domain() && rhs.domain();
    if (true_domain.isFalse())
        return IBool(false);

    if (lhs.upper() <= rhs.lower())
        return true_domain;
    else if (lhs.lower() >= rhs.lower())
        return IBool(false);
    return IBool(false, true);
}

IBool operator==(IVal lhs, IVal rhs) {
    IBool true_domain = lhs.domain() && rhs.domain();
    if (true_domain.isFalse())
        return IBool(false);

    if (lhs.lower() == rhs.lower() &&
        lhs.upper() == rhs.upper() &&
        lhs.lower() == lhs.upper())
    {
        return true_domain;
    }

    if ((lhs < rhs || lhs > rhs).isTrue())
        return IBool(false);
    return IBool(false, true);
}

IBool operator!=(IVal lhs, IVal rhs) {
    if (lhs.domain().isFalse() || rhs.domain().isFalse())
        return IBool(false);

    return !(lhs == rhs);
}

// Functions

IVal sqrt(IVal val) {
    if (val.domain().isFalse())
        return val;

    if (val.upper() < 0)
        return i_undefined;
    else if (val.lower() >= 0)
        return IVal(sqrt_down(val.lower()), sqrt_up(val.upper()), val.domain());
    return IVal(0, sqrt_up(val.upper()), { false, true });
}

IVal abs(IVal val) {
    if (val.domain().isFalse())
        return val;

    if (val.upper() <= 0)
        return IVal(-val.upper(), -val.lower(), val.domain());
    else if (val.lower() >= 0)
        return val;
    return IVal(0, val.upper(), val.domain());
}

std::ostream& operator<<(std::ostream &out, IVal val) {
    if (val.domain().isFalse())
        out << "[]";
    else if (val.domain().isTrue())
        out << '[' << val.lower() << ", " << val.upper() << ']';
    else
        out << "([" << val.lower() << ", " << val.upper() << "])";
    return out;
}
