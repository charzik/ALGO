#ifndef Rational_h
#define Rational_h


#endif /* Rational_h */

#include <algorithm>
#include <cstdio>
#include <cmath>
#include <iostream>

class Rational {
public:
    Rational() : m(0), n(1)
    {}
    
    Rational(int m_, int n_ = 1) : m(m_), n(n_) {
        recalc();
    }
    
    int numerator() const {
        return m;
    }
    
    int denominator() const {
        return n;
    }
    
    Rational& operator+=(const Rational& u) {
        m = m * u.n + n * u.m;
        n *= u.n;
        recalc();
        return *this;
    }
    
    Rational& operator-=(const Rational& u) {
        m = m * u.n - n * u.m;
        n *= u.n;
        recalc();
        return *this;
    }
    
    Rational& operator*=(const Rational& u) {
        m *= u.m;
        n *= u.n;
        recalc();
        return *this;
    }
    
    Rational& operator/=(const Rational& u) {
        int c = u.m;
        m *= u.n;
        n *= c;
        recalc();
        return *this;
    }
    
    Rational& operator++() {
        return *this += 1;
    }
    
    Rational operator++(int) {
        Rational res = *this;
        *this += 1;
        return res;
    }
    
    Rational& operator--() {
        return *this -= 1;
    }
    
    Rational operator--(int) {
        Rational res = *this;
        *this -= 1;
        return res;
    }
    
    Rational operator-() const {
        return {-m, n};
    }
    
    Rational operator+() const {
        return {m, n};
    }
    
private:
    int m, n;
    
    static int gcd(int a, int b) {
        while (a != 0) {
            b %= a;
            std::swap(a, b);
        }
        return b;
    }
    
    void recalc() {
        if (n < 0) {
            n = -n;
            m = -m;
        }
        int gcd = this->gcd(abs(m), abs(n));
        m /= gcd;
        n /= gcd;
    }
};

Rational operator+(const Rational& v, const Rational& u) {
    auto res = v;
    res += u;
    return res;
}

Rational operator-(const Rational& v, const Rational& u) {
    auto res = v;
    res -= u;
    return res;
}

Rational operator*(const Rational& v, const Rational& u) {
    auto res = v;
    res *= u;
    return res;
}

Rational operator/(const Rational& v, const Rational& u) {
    auto res = v;
    res /= u;
    return res;
}

bool operator==(const Rational& v, const Rational& u) {
    return v.denominator() == u.denominator() &&
    v.numerator() == u.numerator();
}

bool operator!=(const Rational& v, const Rational& u) {
    return !(v == u);
}

std::ostream& operator<<(std::ostream& out, const Rational& a) {
    if (a.numerator() == 0)
        return (out << 0);
    if (a.denominator() == 1)
        return (out << a.numerator());
    out << a.numerator() << "/" << a.denominator();
    return out;
}
