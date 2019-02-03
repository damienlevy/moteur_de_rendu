#ifndef POINT_H
#define POINT_H

template <typename T> class Point2D{
    public :
        Point2D(T xx , T yy) : x(xx),y(yy){}
        T getX(){return x;}
        T getY(){return y;}
    private :
        T x,y;

};

template <typename T> class Point3D{
    public :
        Point3D(T xx , T yy, T zz) : x(xx),y(yy),z(zz){}
        T getX(){return x;}
        T getY(){return y;}
        T getZ(){return z;}
    private :
        T x,y,z;

};

typedef Point2D<int> Point2DI;
typedef Point2D<float> Point2DF;
typedef Point3D<int> Point3DI;
typedef Point3D<float> Point3DF;

#endif