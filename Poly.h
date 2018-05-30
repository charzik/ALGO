//
//  Poly.h
//  LiAl
//
//  Created by Михаил Чарзавакян on 02.02.2018.
//  Copyright © 2018 Михаил Чарзавакян. All rights reserved.
//

#ifndef Poly_h
#define Poly_h


#endif /* Poly_h */

#include <cmath>
#include <iostream>
#include <vector>

template<typename T>
class Polynomial {
public:
    using ConstIter = typename std::vector<T>::const_iterator;
    
    Polynomial(const T& a = T(0)) : v(std::vector<T> {a}) {
        this->normalize();
    }
    
    Polynomial(const std::vector<T>& _v) {
        v = _v;
        this->normalize();
    }
    
    template<typename Iter>
    Polynomial(Iter st, Iter fn) {
        v.resize(0);
        for (Iter it = st; it != fn; ++it)
            v.push_back(*it);
        this->normalize();
    }
    
    bool operator==(const Polynomial<T>& other) const {
        return v == other.v;
    }
    
    bool operator!=(const Polynomial<T>& other) const {
        return !(v == other.v);
    }
    
    Polynomial<T>& operator+=(const Polynomial<T>& other) {
        size_t newSize = std::max(v.size(), other.v.size());
        v.resize(newSize);
        for (size_t i = 0; i < other.v.size(); ++i)
            v[i] += other.v[i];
        this->normalize();
        return *this;
    }
    
    Polynomial<T>& operator-=(const Polynomial<T>& other) {
        size_t newSize = std::max(v.size(), other.v.size());
        v.resize(newSize);
        for (size_t i = 0; i < other.v.size(); ++i)
            v[i] -= other.v[i];
        this->normalize();
        return *this;
    }
    
    Polynomial<T>& operator*=(const Polynomial<T>& other) {
        int newSize = other.v.size() + v.size() - 1;
        Polynomial<T> res;
        if (newSize <= 0) {
            *this = res;
            return *this;
        }
        res.v.resize(newSize, T(0));
        for (size_t i = 0; i < v.size(); ++i)
            for (size_t j = 0; j < other.v.size(); ++j)
                res.v[i + j] += (v[i] * other.v[j]);
        res.normalize();
        *this = res;
        return *this;
    }
    
    T operator()(const T& x) const {
        T ans = T(0);
        for (int i = static_cast<int>(v.size()) - 1; i >= 0; --i)
            ans = ans * x + v[i];
        return ans;
    }
    
    T operator[](int i) const {
        if (i < 0 || i >= static_cast<int>(v.size()))
            return T(0);
        return v[static_cast<size_t>(i)];
    }
    
    int Degree() const {
        return (static_cast<int>(v.size()) - 1);
    }
    
    ConstIter begin() const {
        return v.begin();
    }
    
    ConstIter end() const {
        return v.end();
    }
    
    Polynomial<T> operator&(const Polynomial<T>& other) const {
        Polynomial<T> ans;
        for (int i = static_cast<int>(v.size()) - 1; i >= 0; --i)
            ans = ans * other + v[i];
        ans.normalize();
        return ans;
    }
    
    std::pair<Polynomial<T>, Polynomial<T>> division(const Polynomial<T>& other) const {
        auto mod = *this;
        Polynomial<T> div(T(0));
        while (mod.Degree() >= other.Degree()) {
            int curPow = mod.Degree() - other.Degree();
            T k = mod[mod.Degree()] / other[other.Degree()];
            std::vector<T> helpV(curPow, T(0));
            helpV.push_back(k);
            Polynomial<T> curP(helpV);
            div += curP;
            mod -= (other * curP);
        }
        return {div, mod};
    }
    
    Polynomial<T> operator,(const Polynomial<T>& other) const {
        auto a = *this;
        auto b = other;
        while (b != Polynomial<T> (T(0))) {
            a = a % b;
            std::swap(a, b);
        }
        auto k = a[a.Degree()];
        for (size_t i = 0; i < a.v.size(); ++i)
            a.v[i] = a.v[i] / k;
        return a;
    }
    
private:
    void normalize() {
        while (!v.empty() && v.back() == T(0))
            v.pop_back();
    }
    
    std::vector<T> v;
};

template<typename T>
Polynomial<T> operator+(const Polynomial<T>& rhs, const Polynomial<T>& lhs) {
    auto res = rhs;
    res += lhs;
    return res;
}

template<typename T>
Polynomial<T>& operator+=(Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return (lhs += cast);
}

template<typename T>
Polynomial<T> operator+(const Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return lhs + cast;
}

template<typename T>
Polynomial<T> operator+(const T& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> cast(lhs);
    return cast + rhs;
}

template<typename T>
Polynomial<T>& operator-=(Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return (lhs -= cast);
}

template<typename T>
Polynomial<T> operator-(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    auto res = lhs;
    res -= rhs;
    return res;
}

template<typename T>
Polynomial<T> operator-(const Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return lhs - cast;
}

template<typename T>
Polynomial<T> operator-(const T& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> cast(lhs);
    return cast - rhs;
}

template<typename T>
Polynomial<T>& operator*=(Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return (lhs *= cast);
}

template<typename T>
Polynomial<T> operator*(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    auto res = lhs;
    res *= rhs;
    return res;
}

template<typename T>
Polynomial<T> operator*(const Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return lhs * cast;
}

template<typename T>
Polynomial<T> operator*(const T& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> cast(lhs);
    return cast * rhs;
}

template<typename T>
bool operator==(const T& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> cast(lhs);
    return cast == rhs;
}

template<typename T>
bool operator==(const Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return cast == lhs;
}

template<typename T>
bool operator!=(const T& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> cast(lhs);
    return cast != rhs;
}

template<typename T>
bool operator!=(const Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return cast != lhs;
}

template<typename T>
Polynomial<T> operator&(const Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return lhs & cast;
}

template<typename T>
Polynomial<T> operator&(const T& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> cast(lhs);
    return cast & rhs;
}

template<typename T>
Polynomial<T> operator/(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    return lhs.division(rhs).first;
}

template<typename T>
Polynomial<T> operator%(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    return lhs.division(rhs).second;
}

template<typename T>
Polynomial<T> operator/(const Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return lhs / cast;
}

template<typename T>
Polynomial<T> operator/(const T& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> cast(lhs);
    return cast / rhs;
}

template<typename T>
Polynomial<T> operator%(const Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return lhs % cast;
}

template<typename T>
Polynomial<T> operator%(const T& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> cast(lhs);
    return cast % rhs;
}

template<typename T>
Polynomial<T> operator,(const T& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> cast(lhs);
    return (cast , rhs);
}

template<typename T>
Polynomial<T> operator,(const Polynomial<T>& lhs, const T& rhs) {
    Polynomial<T> cast(rhs);
    return (lhs , cast);
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p) {
    bool fl = false;
    for (int i = static_cast<int>(p.Degree()); i >= 0; --i) {
        char sign;
        if (p[i] == T(0))
            continue;
        else if (p[i] > T(0))
            sign = '+';
        else
            sign = '-';
        
        if (!(sign == '+' && i == static_cast<int>(p.Degree())))
            out << sign;
        
        if (!(p[i] == T(1) || p[i] == T(-1)) || i == 0) {
            out << std::max(p[i] * T(-1), p[i]);
            if (i != 0)
                out << '*';
        }
        
        if (i != 0)
            out << 'x';
        
        if (i > 1)
            out << '^' << i;
        fl = true;
    }
    if (!fl)
        out << 0;
    return out;
}
