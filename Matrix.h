//
//  Matrix.h
//  LiAl
//
//  Created by Михаил Чарзавакян on 02.02.2018.
//  Copyright © 2018 Михаил Чарзавакян. All rights reserved.
//

#ifndef Matrix_h
#define Matrix_h


#endif /* Matrix_h */

#include <algorithm>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>

template<typename T>
bool nextPer(std::vector<size_t>& v, T& sgn) {
    int pos1 = static_cast<int>(v.size()) - 2;
    int cnt = 0;
    while (pos1 != -1 && v[pos1] >= v[pos1 + 1])
        --pos1;
    if (pos1 == -1)
        return false;
    int pos2 = static_cast<int>(v.size()) - 1;
    while (v[pos1] >= v[pos2])
        --pos2;
    std::swap(v[pos1], v[pos2]);
    reverse(v.begin() + pos1 + 1, v.end());
    cnt = 1 + (v.end() - v.begin() - pos1 - 1) / 2;
    if (cnt % 2 == 1)
        sgn = sgn * T(-1);
    return true;
}

template<typename T, bool Const>
class MtxIterator;

template<typename T>
using ConstIterator = MtxIterator<T, true>;

template<typename T>
using Iterator = MtxIterator<T, false>;

template<typename T>
class Matrix {
public:
    friend Iterator<T>;
    friend ConstIterator<T>;
    
    template<typename U>
    friend std::istream& operator>>(std::istream& in, Matrix<U>& M);
    
    Matrix(const std::vector<std::vector<T>>& _mtx) : mtx(_mtx)
    {}
    
    std::pair<size_t, size_t> size() const {
        if (mtx.size() == 0)
            return {0, 0};
        return {mtx.size(), mtx[0].size()};
    }
    
    const T& get(size_t i, size_t j) const {
        return mtx[i][j];
    }
    
    Matrix<T>& operator+=(const Matrix<T>& other) {
        auto p = (*this).size();
        size_t n = p.first, m = p.second;
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < m; ++j)
                mtx[i][j] += other.get(i, j);
        
        return *this;
    }
    
    Matrix<T>& operator-=(const Matrix<T>& other) {
        auto p = (*this).size();
        size_t n = p.first, m = p.second;
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < m; ++j)
                mtx[i][j] -= other.get(i, j);
        
        return *this;
    }
    
    Matrix<T> operator+(const Matrix<T>& other) const {
        auto res = *this;
        res += other;
        return res;
    }
    
    Matrix<T> operator-(const Matrix<T>& other) const {
        auto res = *this;
        res -= other;
        return res;
    }
    
    template<typename U>
    Matrix<T>& operator*=(const U& other) {
        auto p = (*this).size();
        size_t n = p.first, m = p.second;
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < m; ++j)
                mtx[i][j] *= other;
        
        return *this;
    }
    
    template<typename U>
    Matrix<T> operator*(const U& other) const {
        auto res = *this;
        res *= other;
        return res;
    }
    
    Matrix<T>& transpose() {
        auto p = (*this).size();
        size_t n = p.first, m = p.second;
        std::vector<std::vector<T>> v(m, std::vector<T> (n));
        Matrix res(v);
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < m; ++j)
                res.mtx[j][i] = mtx[i][j];
        *this = res;
        return *this;
    }
    
    Matrix<T> transposed() const {
        auto res = *this;
        res = res.transpose();
        return res;
    }
    
    Matrix<T>& operator*=(const Matrix<T>& other) {
        auto p1 = (*this).size(), p2 = other.size();
        size_t n1 = p1.first;
        size_t n2 = p2.first, m2 = p2.second;
        std::vector<std::vector<T>> v(n1, std::vector<T> (m2));
        Matrix<T> res(v);
        for (size_t i = 0; i < n1; ++i)
            for (size_t j = 0; j < m2; ++j)
                for (size_t k = 0; k < n2; ++k)
                    res.mtx[i][j] += mtx[i][k] * other.mtx[k][j];
        *this = res;
        return *this;
    }
    
    Matrix<T> operator*(const Matrix<T>& other) const {
        auto res = *this;
        res *= other;
        return res;
    }
    
    template<typename U>
    std::vector<U> solve(const std::vector<U>& b) const {
        auto p = (*this).size();
        size_t n = p.first;
        auto M = *this;
        auto bM = b;
        for (size_t i = 0; i < n; ++i) {
            auto curEl = M.mtx[i][i];
            for (size_t g = i; g < n; ++g) {
                if (M.mtx[g][i] != 0) {
                    curEl = M.mtx[g][i];
                    std::swap(M.mtx[g], M.mtx[i]);
                    std::swap(bM[g], bM[i]);
                    break;
                }
            }
            for (size_t j = i + 1; j < n; ++j) {
                auto x = M.mtx[j][i];
                if (x == 0)
                    continue;
                for (size_t k = i; k < n; ++k) {
                    M.mtx[j][k] /= x;
                    M.mtx[j][k] *= curEl;
                    M.mtx[j][k] -= M.mtx[i][k];
                }
                bM[j] /= static_cast<U> (x);
                bM[j] *= static_cast<U> (curEl);
                bM[j] -= bM[i];
            }
        }
        for (int i = n - 1; i >= 0; --i) {
            if (M.mtx[i][i] == 0) {
                bM[i] = 0;
                continue;
                M.mtx[i][i] = 1;
            }
            bM[i] /= static_cast<U> (M.mtx[i][i]);
            auto cur = M.mtx[i][i];
            for (int j = i; j < n; ++j)
                M.mtx[i][j] /= cur;
            for (int j = i - 1; j >= 0; --j) {
                bM[j] -= (bM[i] * static_cast<U> (M.mtx[j][i]));
                cur = M.mtx[j][i];
                for (int k = i; k < n; ++k)
                    M.mtx[j][k] -= (M.mtx[i][k] * cur);
            }
        }
        std::cout << M << "\n";
        return bM;
    }
    
    T det() const {
        T res = T(0);
        std::vector<size_t> cur(mtx.size());
        for (size_t i = 0; i < mtx.size(); ++i)
            cur[i] = i;
        
        T sgn = T(1);
        T curP = T(1);
        for (size_t i = 0; i < cur.size(); ++i)
            curP = curP * mtx[i][cur[i]];
        res = res + sgn * curP;
        while (nextPer(cur, sgn)) {
            curP = T(1);
            for (size_t i = 0; i < cur.size(); ++i)
                curP = curP * mtx[i][cur[i]];
            res = res + sgn * curP;
        }
        return res;
    }
    
    Iterator<T> begin() {
        size_t zero = 0;
        return {*this, zero, zero};
    }
    
    ConstIterator<T> begin() const {
        size_t zero = 0;
        return {*this, zero, zero};
    }
    
    Iterator<T> end() {
        return {*this, (*this).size().first, (*this).size().second};
    }
    
    ConstIterator<T> end() const {
        return {*this, (*this).size().first, (*this).size().second};
    }
    
private:
    std::vector<std::vector<T>> mtx;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& M) {
    auto p = M.size();
    size_t n = p.first, m = p.second;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            out << M.get(i, j);
            if (j != m - 1)
                out << '\t';
        }
        if (i != n - 1)
            out << '\n';
    }
    return out;
}

template<typename T>
std::istream& operator>>(std::istream& in, Matrix<T>& M) {
    auto p = M.size();
    size_t n = p.first, m = p.second;
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < m; ++j) {
            T cur;
            in >> cur;
            M.mtx[i][j] = cur;
        }
    return in;
}

template<typename T, bool Const>
class MtxIterator {
public:
    friend Matrix<T>;
    
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using reference = typename std::conditional<Const, const T&, T&>::type;
    using pointer = typename std::conditional<Const, const T*, T*>::type;
    using difference_type = std::ptrdiff_t;
    using typeMatrix = typename std::conditional<Const, const Matrix<T>&,
    Matrix<T>&>::type;
    
    MtxIterator(typeMatrix m_, size_t i_, size_t j_) :
    m(m_), i(i_), j(j_)
    {}
    
    bool operator==(const MtxIterator& other) const {
        return i == other.i && j == other.j;
    }
    
    bool operator!=(const MtxIterator& other) const {
        return !(i == other.i && j == other.j);
    }
    
    reference operator*() const {
        return m.mtx[i][j];
    }
    
    MtxIterator& operator++() {
        auto p = m.size();
        if (i == p.first - 1 && j == p.second - 1) {
            ++j;
            ++i;
        } else if (j == p.second - 1) {
            ++i;
            j = 0;
        } else {
            ++j;
        }
        return *this;
    }
    
    MtxIterator operator++(int) {
        auto ans = *this;
        auto p = m.size();
        if (i == p.first - 1 && j == p.second - 1) {
            ++j;
            ++i;
        } else if (j == p.second - 1) {
            ++i;
            j = 0;
        } else {
            ++j;
        }
        return ans;
    }
    
private:
    typeMatrix m;
    size_t i, j;
};
