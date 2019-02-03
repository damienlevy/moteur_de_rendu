#ifndef POINT_H
#define POINT_H

template <typename T> class Point2D {
    public :
        Point2D(T xx , T yy) : x(xx),y(yy){}
        T getX() const{return x;}
        T getY() const{return y;}
    private :
        T x,y;

};
/*redefinition des operande*/
template <typename T>
bool operator==(Point2D<T> const& a, Point2D<T> const& b){
    return a.getX() == b.getX() && a.getY() == b.getY();// a.estEgale(b);
}
template <typename T>
bool operator!=(Point2D<T> const& a, Point2D<T> const& b){return !(a==b);}
template <typename T>
Point2D<T> operator+(Point2D<T> const& a, Point2D<T> const& b){
    Point2D<T> result(a.getX()+b.getX(),a.getY()+b.getY());
    return result;
}
template <typename T>
Point2D<T> operator-(Point2D<T> const& a, Point2D<T> const& b){
    Point2D<T> result(a.getX()-b.getX(),a.getY()-b.getY());
    return result;
}




/*
*Point3D
*
*/
template <typename T> class Point3D{
    public :
        Point3D(T xx , T yy, T zz) : x(xx),y(yy),z(zz){}
        T getX() const{return x;}
        T getY() const{return y;}
        T getZ() const{return z;}
    private :
        T x,y,z;

};

typedef Point2D<int> Point2DI;
typedef Point2D<float> Point2DF;
typedef Point3D<int> Point3DI;
typedef Point3D<float> Point3DF;



#endif