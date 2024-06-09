#pragma once

#include <iostream>
#include <type_traits>

class IBool {
public:
    IBool(bool lower, bool upper);
    IBool(bool val) : m_lower(val), m_upper(val) {}

    bool lower() { return m_lower; }
    bool upper() { return m_upper; }
    bool isTrue() { return m_lower && m_upper; }
    bool isFalse() { return !m_lower && !m_upper; }
    bool isMixed() { return !isTrue() && !isFalse(); }

    IBool operator!();
    IBool operator||(const IBool& other);
    IBool operator&&(const IBool& other);

private:
    bool m_lower, m_upper;
};

class IVal {
public:
    IVal(double lower, double upper)
        : m_lower(lower), m_upper(upper), m_domain(true) {}
    IVal(double lower, double upper, IBool domain)
        : m_lower(lower), m_upper(upper), m_domain(domain) {}

    IVal(double value)
        : m_lower(value), m_upper(value), m_domain(true) {}

    double lower() { return m_lower; }
    double upper() { return m_upper; }
    IBool domain() { return m_domain; }

private:
    const double m_lower, m_upper;
    const IBool m_domain;
};

const IVal i_undefined = IVal(0.0, 0.0, {false, false});

// Arithmetic operators

IVal operator+(IVal lhs, IVal rhs);
IVal operator-(IVal lhs, IVal rhs);
IVal operator*(IVal lhs, IVal rhs);
IVal operator/(IVal lhs, IVal rhs);

// Comparisons

IBool operator>(IVal lhs, IVal rhs);
IBool operator>=(IVal lhs, IVal rhs);
IBool operator<(IVal lhs, IVal rhs);
IBool operator<=(IVal lhs, IVal rhs);
IBool operator==(IVal lhs, IVal rhs);
IBool operator!=(IVal lhs, IVal rhs);

// IO streams

std::ostream& operator<<(std::ostream &out, IVal val);
