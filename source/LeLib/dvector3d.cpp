#include "dvector3d.h"

#include "qmatrix4x4.h"
#include <QtCore/qdatastream.h>
#include <QtCore/qmath.h>
#include <QtCore/qvariant.h>
#include <QtCore/qdebug.h>
#include <QtCore/qrect.h>
// DVector3D used to be defined as class DVector3D { double x, y, z; };,
// now instead it is defined as classs DVector3D { double v[3]; };.
// Check that binary compatibility is preserved.
// ### Qt 6: remove all of these checks.
namespace {
struct DVector3DOld
{
    double x, y, z;
};
struct DVector3DNew
{
    double v[3];
};
Q_STATIC_ASSERT_X(std::is_standard_layout<DVector3DOld>::value, "Binary compatibility break in DVector3D");
Q_STATIC_ASSERT_X(std::is_standard_layout<DVector3DNew>::value, "Binary compatibility break in DVector3D");
Q_STATIC_ASSERT_X(sizeof(DVector3DOld) == sizeof(DVector3DNew), "Binary compatibility break in DVector3D");
// requires a constexpr offsetof
#if !defined(Q_CC_MSVC) || (_MSC_VER >= 1910)
Q_STATIC_ASSERT_X(offsetof(DVector3DOld, x) == offsetof(DVector3DNew, v) + sizeof(DVector3DNew::v[0]) * 0, "Binary compatibility break in DVector3D");
Q_STATIC_ASSERT_X(offsetof(DVector3DOld, y) == offsetof(DVector3DNew, v) + sizeof(DVector3DNew::v[0]) * 1, "Binary compatibility break in DVector3D");
Q_STATIC_ASSERT_X(offsetof(DVector3DOld, z) == offsetof(DVector3DNew, v) + sizeof(DVector3DNew::v[0]) * 2, "Binary compatibility break in DVector3D");
#endif
} // anonymous namespace
/*!
    \class DVector3D
    \brief The DVector3D class represents a vector or vertex in 3D space.
    \since 4.6
    \ingroup painting-3D
    \inmodule QtGui
    Vectors are one of the main building blocks of 3D representation and
    drawing.  They consist of three coordinates, traditionally called
    x, y, and z.
    The DVector3D class can also be used to represent vertices in 3D space.
    We therefore do not need to provide a separate vertex class.
    \sa QVector2D, QVector4D, QQuaternion
*/
/*!
    \fn DVector3D::DVector3D()
    Constructs a null vector, i.e. with coordinates (0, 0, 0).
*/
/*!
    \fn DVector3D::DVector3D(Qt::Initialization)
    \since 5.5
    \internal
    Constructs a vector without initializing the contents.
*/
/*!
    \fn DVector3D::DVector3D(double xpos, double ypos, double zpos)
    Constructs a vector with coordinates (\a xpos, \a ypos, \a zpos).
*/
/*!
    \fn DVector3D::DVector3D(const QPoint& point)
    Constructs a vector with x and y coordinates from a 2D \a point, and a
    z coordinate of 0.
*/
/*!
    \fn DVector3D::DVector3D(const QPointF& point)
    Constructs a vector with x and y coordinates from a 2D \a point, and a
    z coordinate of 0.
*/
/*!
    \fn bool DVector3D::isNull() const
    Returns \c true if the x, y, and z coordinates are set to 0.0,
    otherwise returns \c false.
*/
/*!
    \fn double DVector3D::x() const
    Returns the x coordinate of this point.
    \sa setX(), y(), z()
*/
/*!
    \fn double DVector3D::y() const
    Returns the y coordinate of this point.
    \sa setY(), x(), z()
*/
/*!
    \fn double DVector3D::z() const
    Returns the z coordinate of this point.
    \sa setZ(), x(), y()
*/
/*!
    \fn void DVector3D::setX(double x)
    Sets the x coordinate of this point to the given \a x coordinate.
    \sa x(), setY(), setZ()
*/
/*!
    \fn void DVector3D::setY(double y)
    Sets the y coordinate of this point to the given \a y coordinate.
    \sa y(), setX(), setZ()
*/
/*!
    \fn void DVector3D::setZ(double z)
    Sets the z coordinate of this point to the given \a z coordinate.
    \sa z(), setX(), setY()
*/
/*! \fn double &DVector3D::operator[](int i)
    \since 5.2
    Returns the component of the vector at index position \a i
    as a modifiable reference.
    \a i must be a valid index position in the vector (i.e., 0 <= \a i
    < 3).
*/
/*! \fn double DVector3D::operator[](int i) const
    \since 5.2
    Returns the component of the vector at index position \a i.
    \a i must be a valid index position in the vector (i.e., 0 <= \a i
    < 3).
*/
/*!
    Returns the normalized unit vector form of this vector.
    If this vector is null, then a null vector is returned.  If the length
    of the vector is very close to 1, then the vector will be returned as-is.
    Otherwise the normalized form of the vector of length 1 will be returned.
    \sa length(), normalize()
*/
DVector3D DVector3D::normalized() const
{
    // Need some extra precision if the length is very small.
    double len = double(v[0]) * double(v[0]) +
                 double(v[1]) * double(v[1]) +
                 double(v[2]) * double(v[2]);
    if (qFuzzyIsNull(len - 1.0f)) {
        return *this;
    } else if (!qFuzzyIsNull(len)) {
        double sqrtLen = std::sqrt(len);
        return DVector3D(double(double(v[0]) / sqrtLen),
                         double(double(v[1]) / sqrtLen),
                         double(double(v[2]) / sqrtLen));
    } else {
        return DVector3D();
    }
}
/*!
    Normalizes the currect vector in place.  Nothing happens if this
    vector is a null vector or the length of the vector is very close to 1.
    \sa length(), normalized()
*/
void DVector3D::normalize()
{
    // Need some extra precision if the length is very small.
    double len = double(v[0]) * double(v[0]) +
                 double(v[1]) * double(v[1]) +
                 double(v[2]) * double(v[2]);
    if (qFuzzyIsNull(len - 1.0f) || qFuzzyIsNull(len))
        return;
    len = std::sqrt(len);
    v[0] = double(double(v[0]) / len);
    v[1] = double(double(v[1]) / len);
    v[2] = double(double(v[2]) / len);
}
/*!
    \fn DVector3D &DVector3D::operator+=(const DVector3D &vector)
    Adds the given \a vector to this vector and returns a reference to
    this vector.
    \sa operator-=()
*/
/*!
    \fn DVector3D &DVector3D::operator-=(const DVector3D &vector)
    Subtracts the given \a vector from this vector and returns a reference to
    this vector.
    \sa operator+=()
*/
/*!
    \fn DVector3D &DVector3D::operator*=(double factor)
    Multiplies this vector's coordinates by the given \a factor, and
    returns a reference to this vector.
    \sa operator/=()
*/
/*!
    \fn DVector3D &DVector3D::operator*=(const DVector3D& vector)
    \overload
    Multiplies the components of this vector by the corresponding
    components in \a vector.
    Note: this is not the same as the crossProduct() of this
    vector and \a vector.
    \sa crossProduct()
*/
/*!
    \fn DVector3D &DVector3D::operator/=(double divisor)
    Divides this vector's coordinates by the given \a divisor, and
    returns a reference to this vector.
    \sa operator*=()
*/
/*!
    \fn DVector3D &DVector3D::operator/=(const DVector3D &vector)
    \since 5.5
    Divides the components of this vector by the corresponding
    components in \a vector.
    \sa operator*=()
*/
/*!
    Returns the dot product of \a v1 and \a v2.
*/
double DVector3D::dotProduct(const DVector3D& v1, const DVector3D& v2)
{
    return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}
/*!
    Returns the cross-product of vectors \a v1 and \a v2, which corresponds
    to the normal vector of a plane defined by \a v1 and \a v2.
    \sa normal()
*/
DVector3D DVector3D::crossProduct(const DVector3D& v1, const DVector3D& v2)
{
    return DVector3D(v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
                     v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
                     v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]);
}
/*!
    Returns the normal vector of a plane defined by vectors \a v1 and \a v2,
    normalized to be a unit vector.
    Use crossProduct() to compute the cross-product of \a v1 and \a v2 if you
    do not need the result to be normalized to a unit vector.
    \sa crossProduct(), distanceToPlane()
*/
DVector3D DVector3D::normal(const DVector3D& v1, const DVector3D& v2)
{
    return crossProduct(v1, v2).normalized();
}
/*!
    \overload
    Returns the normal vector of a plane defined by vectors
    \a v2 - \a v1 and \a v3 - \a v1, normalized to be a unit vector.
    Use crossProduct() to compute the cross-product of \a v2 - \a v1 and
    \a v3 - \a v1 if you do not need the result to be normalized to a
    unit vector.
    \sa crossProduct(), distanceToPlane()
*/
DVector3D DVector3D::normal
        (const DVector3D& v1, const DVector3D& v2, const DVector3D& v3)
{
    return crossProduct((v2 - v1), (v3 - v1)).normalized();
}
/*!
    \since 5.5
    Returns the window coordinates of this vector initially in object/model
    coordinates using the model view matrix \a modelView, the projection matrix
    \a projection and the viewport dimensions \a viewport.
    When transforming from clip to normalized space, a division by the w
    component on the vector components takes place. To prevent dividing by 0 if
    w equals to 0, it is set to 1.
    \note the returned y coordinates are in OpenGL orientation. OpenGL expects
    the bottom to be 0 whereas for Qt top is 0.
    \sa unproject()
 */
/*!
    \since 5.5
    Returns the object/model coordinates of this vector initially in window
    coordinates using the model view matrix \a modelView, the projection matrix
    \a projection and the viewport dimensions \a viewport.
    When transforming from clip to normalized space, a division by the w
    component of the vector components takes place. To prevent dividing by 0 if
    w equals to 0, it is set to 1.
    \note y coordinates in \a viewport should use OpenGL orientation. OpenGL
    expects the bottom to be 0 whereas for Qt top is 0.
    \sa project()
 */
/*!
    \since 5.1
    Returns the distance from this vertex to a point defined by
    the vertex \a point.
    \sa distanceToPlane(), distanceToLine()
*/
double DVector3D::distanceToPoint(const DVector3D& point) const
{
    return (*this - point).length();
}
/*!
    Returns the distance from this vertex to a plane defined by
    the vertex \a plane and a \a normal unit vector.  The \a normal
    parameter is assumed to have been normalized to a unit vector.
    The return value will be negative if the vertex is below the plane,
    or zero if it is on the plane.
    \sa normal(), distanceToLine()
*/
double DVector3D::distanceToPlane
        (const DVector3D& plane, const DVector3D& normal) const
{
    return dotProduct(*this - plane, normal);
}
/*!
    \overload
    Returns the distance from this vertex to a plane defined by
    the vertices \a plane1, \a plane2 and \a plane3.
    The return value will be negative if the vertex is below the plane,
    or zero if it is on the plane.
    The two vectors that define the plane are \a plane2 - \a plane1
    and \a plane3 - \a plane1.
    \sa normal(), distanceToLine()
*/
double DVector3D::distanceToPlane
    (const DVector3D& plane1, const DVector3D& plane2, const DVector3D& plane3) const
{
    DVector3D n = normal(plane2 - plane1, plane3 - plane1);
    return dotProduct(*this - plane1, n);
}
/*!
    Returns the distance that this vertex is from a line defined
    by \a point and the unit vector \a direction.
    If \a direction is a null vector, then it does not define a line.
    In that case, the distance from \a point to this vertex is returned.
    \sa distanceToPlane()
*/
double DVector3D::distanceToLine
        (const DVector3D& point, const DVector3D& direction) const
{
    if (direction.isNull())
        return (*this - point).length();
    DVector3D p = point + dotProduct(*this - point, direction) * direction;
    return (*this - p).length();
}
/*!
    \fn bool operator==(const DVector3D &v1, const DVector3D &v2)
    \relates DVector3D
    Returns \c true if \a v1 is equal to \a v2; otherwise returns \c false.
    This operator uses an exact doubleing-point comparison.
*/
/*!
    \fn bool operator!=(const DVector3D &v1, const DVector3D &v2)
    \relates DVector3D
    Returns \c true if \a v1 is not equal to \a v2; otherwise returns \c false.
    This operator uses an exact doubleing-point comparison.
*/
/*!
    \fn const DVector3D operator+(const DVector3D &v1, const DVector3D &v2)
    \relates DVector3D
    Returns a DVector3D object that is the sum of the given vectors, \a v1
    and \a v2; each component is added separately.
    \sa DVector3D::operator+=()
*/
/*!
    \fn const DVector3D operator-(const DVector3D &v1, const DVector3D &v2)
    \relates DVector3D
    Returns a DVector3D object that is formed by subtracting \a v2 from \a v1;
    each component is subtracted separately.
    \sa DVector3D::operator-=()
*/
/*!
    \fn const DVector3D operator*(double factor, const DVector3D &vector)
    \relates DVector3D
    Returns a copy of the given \a vector,  multiplied by the given \a factor.
    \sa DVector3D::operator*=()
*/
/*!
    \fn const DVector3D operator*(const DVector3D &vector, double factor)
    \relates DVector3D
    Returns a copy of the given \a vector,  multiplied by the given \a factor.
    \sa DVector3D::operator*=()
*/
/*!
    \fn const DVector3D operator*(const DVector3D &v1, const DVector3D& v2)
    \relates DVector3D
    Multiplies the components of \a v1 by the corresponding components in \a v2.
    Note: this is not the same as the crossProduct() of \a v1 and \a v2.
    \sa DVector3D::crossProduct()
*/
/*!
    \fn const DVector3D operator-(const DVector3D &vector)
    \relates DVector3D
    \overload
    Returns a DVector3D object that is formed by changing the sign of
    all three components of the given \a vector.
    Equivalent to \c {DVector3D(0,0,0) - vector}.
*/
/*!
    \fn const DVector3D operator/(const DVector3D &vector, double divisor)
    \relates DVector3D
    Returns the DVector3D object formed by dividing all three components of
    the given \a vector by the given \a divisor.
    \sa DVector3D::operator/=()
*/
/*!
    \fn const DVector3D operator/(const DVector3D &vector, const DVector3D &divisor)
    \relates DVector3D
    \since 5.5
    Returns the DVector3D object formed by dividing components of the given
    \a vector by a respective components of the given \a divisor.
    \sa DVector3D::operator/=()
*/
/*!
    \fn bool qFuzzyCompare(const DVector3D& v1, const DVector3D& v2)
    \relates DVector3D
    Returns \c true if \a v1 and \a v2 are equal, allowing for a small
    fuzziness factor for doubleing-point comparisons; false otherwise.
*/
#ifndef QT_NO_VECTOR2D
/*!
    Returns the 2D vector form of this 3D vector, dropping the z coordinate.
    \sa toVector4D(), toPoint()
*/
QVector2D DVector3D::toVector2D() const
{
    return QVector2D(v[0], v[1]);
}
#endif
#ifndef QT_NO_VECTOR4D
/*!
    Returns the 4D form of this 3D vector, with the w coordinate set to zero.
    \sa toVector2D(), toPoint()
*/
QVector4D DVector3D::toVector4D() const
{
    return QVector4D(v[0], v[1], v[2], 0.0f);
}
#endif
/*!
    \fn QPoint DVector3D::toPoint() const
    Returns the QPoint form of this 3D vector. The z coordinate
    is dropped.
    \sa toPointF(), toVector2D()
*/
/*!
    \fn QPointF DVector3D::toPointF() const
    Returns the QPointF form of this 3D vector. The z coordinate
    is dropped.
    \sa toPoint(), toVector2D()
*/
/*!
    Returns the 3D vector as a QVariant.
*/
/*!
    Returns the length of the vector from the origin.
    \sa lengthSquared(), normalized()
*/
double DVector3D::length() const
{
    // Need some extra precision if the length is very small.
    double len = double(v[0]) * double(v[0]) +
                 double(v[1]) * double(v[1]) +
                 double(v[2]) * double(v[2]);
    return double(std::sqrt(len));
}
/*!
    Returns the squared length of the vector from the origin.
    This is equivalent to the dot product of the vector with itself.
    \sa length(), dotProduct()
*/
double DVector3D::lengthSquared() const
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
QDebug operator<<(QDebug dbg, const DVector3D &vector)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "DVector3D("
        << vector.x() << ", " << vector.y() << ", " << vector.z() << ')';
    return dbg;
}
