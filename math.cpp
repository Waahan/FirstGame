#include "headerMath.h"

template<typename T>
matrix<T>::matrix(std::initializer_list<std::initializer_list<T>> matrixList)
{
    for(auto& valarrayList : matrixList)
        matrixVector.push_back(std::valarray<T>{valarrayList});
}
