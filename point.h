#ifndef GEOMETRY_POINT_H
#define GEOMETRY_POINT_H
#include <type_traits>
#include <iostream>
#include <cmath>
using namespace std;

namespace Geometry
{
    template<typename _T>
    class Point
    {
    public:
        typedef typename std::enable_if<std::is_arithmetic<_T>::value, _T>::type value_type;
    public:
        Point(value_type x = 0, value_type y = 0, value_type z = 0)
                :m_data{x,y,z}
        {}
        value_type & operator[](unsigned i) {
            return m_data[i];
        }
        const value_type & operator[](unsigned i) const{
            return m_data[i];
        }
        Point operator+(const Point &p) const{
            return Point(m_data[0]+p[0], m_data[1]+p[1], m_data[2] + p[2]);
        }
        Point operator - (const Point &p) const{
            return Point(m_data[0]-p[0], m_data[1]-p[1], m_data[2] - p[2]);
        }
        Point operator *(value_type v) const{
            return Point(m_data[0] *v, m_data[1]*v, m_data[2]*v);
        }
        Point operator /(value_type v) const{
            return Point(m_data[0] /v, m_data[1]/v, m_data[2]/v);
        }

        //inner product
        value_type operator | (const Point & p) const{
            return m_data[0]*p[0] + m_data[1] * p[1] + m_data[2] *p[2];
        }
        value_type length()const{
            return sqrt(  (*this) | (*this) );
        }
        //cross product
        Point operator % (const Point &p) const{
            return Point(
                    m_data[1] * p[2] - m_data[2] *p[1],
                    m_data[2] * p[0] - m_data[0] *p[2],
                    m_data[0] * p[1] - m_data[1] *p[0]
            );
        }
    private:
        value_type m_data[3];
    };

    template<typename T>
    static inline Point<T> operator *(typename Point<T>::value_type v, const Point<T> &p){
        return p * v;
    }

    template<typename T>
    static inline ostream & operator <<(ostream &o, const Point<T> & p) {
        return o <<"("<<p[0]<<","<<p[1]<<","<<p[2]<<")";
    }


}


#endif