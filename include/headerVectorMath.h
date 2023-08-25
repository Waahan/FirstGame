#pragma once

#include <concepts>
#include <ostream>

#include <cmath>

template<typename T>
requires std::floating_point<T>
class mathVector
{
    public:
    explicit mathVector(T xpos = 0.0, T ypos = 0.0) : x{xpos}, y{ypos} {}
    
    mathVector(const mathVector& copyFrom) = default;
    mathVector& operator=(const mathVector& copyFrom) = default;
    
    mathVector(mathVector&& moveFrom) = default;
    mathVector& operator=(mathVector&& moveFrom) = default;
    
    ~mathVector() = default;

    T getX() const { return x; }
    T getY() const { return y; }

    mathVector& operator+=(const mathVector& addTo){ x += addTo.x; y += addTo.y; return *this;}
    mathVector& operator-=(const mathVector& subtract){ x -= subtract.x; y -= subtract.y; return *this; }

    template<typename type>
    requires std::floating_point<type>
    mathVector& operator*=(type scalar){ x *= scalar; y *= scalar; return *this; }

    template<typename type>
    requires std::floating_point<type>
    mathVector& operator/=(type scalar){ x /= scalar; y /= scalar; return *this; }

    mathVector<T> operator-() const { return mathVector<T>{-x, -y}; }

    T dotProduct(const mathVector<T>& other){ return (x * other.getX()) + (y * other.getY()); }
    T magnitude() const { return std::sqrt(std::pow(x, 2), std::pow(y, 2)); }
    T length() const { return magnitude(); }
    mathVector<T> normalized() const { mathVector<T> newVector = *this; T mag = magnitude(); if(mag > 0) newVector /= mag; return newVector; }

    private:
    T x, y;
};


template<typename T>
mathVector<T> operator+(mathVector<T> first, const mathVector<T>& second){ return first += second; }

template<typename T>
mathVector<T> operator-(mathVector<T> first, const mathVector<T>& second){ return first -= second; }

template<typename T, typename type>
requires std::floating_point<type>
mathVector<T> operator*(mathVector<T> firstVector, type scalar){ return firstVector *= scalar; }

template<typename T, typename type>
requires std::floating_point<type>
mathVector<T> operator/(mathVector<T> firstVector, type scalar){ return firstVector /= scalar; }

template<typename T>
bool operator==(const mathVector<T>& first, const mathVector<T>& second){ return ( (first.getX() == second.getX())  && (first.getY() == second.getY()) ); }

template<typename T>
bool operator!=(const mathVector<T>& first, const mathVector<T>& second){ return !(first == second); }

template<typename T>
std::ostream& operator<<(std::ostream& os, const mathVector<T>& first){ return os << first.getX() << " " << first.getY(); }

using vector2f = mathVector<double>;
