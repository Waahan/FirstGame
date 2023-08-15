#pragma once

#include <concepts>
#include <vector>
#include <valarray>

template<typename T>
requires std::floating_point<T>
class matrix;

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

    mathVector& operator+=(mathVector addTo){ x += addTo.x; y += addTo.y; return *this;}
    mathVector& operator-=(mathVector subtract){ x -= subtract.x; y -= subtract.y; return *this; }
    mathVector& operator*=(T scalar){ x *= scalar; y *= scalar; return *this; }
    mathVector& operator/=(T scalar){ x /= scalar; y /= scalar; return *this; }

    mathVector& operator-(){ x = -x; y = -y; return *this; }

    void dotProduct(){ return; }
    void crossProduct(){ return; }
    void magnitude(){ return; }
    void normalized(){ return; }

    private:
    T x, y;
};


template<typename T>
mathVector<T> operator+(mathVector<T> first, mathVector<T> second){ return first += second; }

template<typename T>
mathVector<T> operator-(mathVector<T> first, mathVector<T> second){ return first -= second; }

template<typename T>
mathVector<T> operator*(mathVector<T> firstVector, T scalar){ return firstVector *= scalar; }

template<typename T>
mathVector<T> operator/(mathVector<T> firstVector, T scalar){ return firstVector /= scalar; }

template<typename T>
bool operator==(mathVector<T> first, mathVector<T> second){ return ( (first.getX() == second.getX())  && (first.getY() == second.getY()) ); }

template<typename T>
bool operator!=(mathVector<T> first, mathVector<T> second){ return !(first == second); }

using Vector2f = mathVector<double>;

template<typename T>
requires std::floating_point<T>
class matrix
{
    public:
    explicit matrix<T>(std::initializer_list<std::initializer_list<T>> matrixList);

    matrix(const matrix& copyFrom) = default;
    matrix& operator=(const matrix& copyFrom) = default;

    matrix(matrix&& moveFrom) = default;
    matrix& operator=(matrix&& moveFrom) = default;

    ~matrix() = default;

    private:
    std::vector<std::valarray<T>> matrixVector;
};

using matrix2f = matrix<double>;
