#ifndef DVECTOR3D_H
#define DVECTOR3D_H


#include <QtGui/qtguiglobal.h>
#include <QtCore/qpoint.h>
#include <QtCore/qmetatype.h>

QT_BEGIN_NAMESPACE
class QMatrix4x4;
class QRect;
#ifndef QT_NO_VECTOR3D
class Q_GUI_EXPORT DVector3D
{
public:
    Q_DECL_CONSTEXPR DVector3D();
    explicit DVector3D(Qt::Initialization) {}
    Q_DECL_CONSTEXPR DVector3D(double xpos, double ypos, double zpos) : v{xpos, ypos, zpos} {}
    Q_DECL_CONSTEXPR explicit DVector3D(const QPoint& point);
    Q_DECL_CONSTEXPR explicit DVector3D(const QPointF& point);
#ifndef QT_NO_VECTOR2D
    DVector3D(const QVector2D& vector);
    DVector3D(const QVector2D& vector, double zpos);
#endif
#ifndef QT_NO_VECTOR4D
    explicit DVector3D(const QVector4D& vector);
#endif
    bool isNull() const;
    Q_DECL_CONSTEXPR double x() const;
    Q_DECL_CONSTEXPR double y() const;
    Q_DECL_CONSTEXPR double z() const;
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    double &operator[](int i);
    double operator[](int i) const;
    double length() const;
    double lengthSquared() const;
    DVector3D normalized() const;
    void normalize();
    DVector3D &operator+=(const DVector3D &vector);
    DVector3D &operator-=(const DVector3D &vector);
    DVector3D &operator*=(double factor);
    DVector3D &operator*=(const DVector3D& vector);
    DVector3D &operator/=(double divisor);
    inline DVector3D &operator/=(const DVector3D &vector);
    static double dotProduct(const DVector3D& v1, const DVector3D& v2); //In Qt 6 convert to inline and constexpr
    static DVector3D crossProduct(const DVector3D& v1, const DVector3D& v2); //in Qt 6 convert to inline and constexpr
    static DVector3D normal(const DVector3D& v1, const DVector3D& v2);
    static DVector3D normal
        (const DVector3D& v1, const DVector3D& v2, const DVector3D& v3);
    DVector3D project(const QMatrix4x4 &modelView, const QMatrix4x4 &projection, const QRect &viewport) const;
    DVector3D unproject(const QMatrix4x4 &modelView, const QMatrix4x4 &projection, const QRect &viewport) const;
    double distanceToPoint(const DVector3D& point) const;
    double distanceToPlane(const DVector3D& plane, const DVector3D& normal) const;
    double distanceToPlane(const DVector3D& plane1, const DVector3D& plane2, const DVector3D& plane3) const;
    double distanceToLine(const DVector3D& point, const DVector3D& direction) const;
    Q_DECL_CONSTEXPR friend inline bool operator==(const DVector3D &v1, const DVector3D &v2);
    Q_DECL_CONSTEXPR friend inline bool operator!=(const DVector3D &v1, const DVector3D &v2);
    Q_DECL_CONSTEXPR friend inline const DVector3D operator+(const DVector3D &v1, const DVector3D &v2);
    Q_DECL_CONSTEXPR friend inline const DVector3D operator-(const DVector3D &v1, const DVector3D &v2);
    Q_DECL_CONSTEXPR friend inline const DVector3D operator*(double factor, const DVector3D &vector);
    Q_DECL_CONSTEXPR friend inline const DVector3D operator*(const DVector3D &vector, double factor);
    Q_DECL_CONSTEXPR friend const DVector3D operator*(const DVector3D &v1, const DVector3D& v2);
    Q_DECL_CONSTEXPR friend inline const DVector3D operator-(const DVector3D &vector);
    Q_DECL_CONSTEXPR friend inline const DVector3D operator/(const DVector3D &vector, double divisor);
    Q_DECL_CONSTEXPR friend inline const DVector3D operator/(const DVector3D &vector, const DVector3D &divisor);
    Q_DECL_CONSTEXPR friend inline bool qFuzzyCompare(const DVector3D& v1, const DVector3D& v2);
#ifndef QT_NO_VECTOR2D
    QVector2D toVector2D() const;
#endif
#ifndef QT_NO_VECTOR4D
    QVector4D toVector4D() const;
#endif
    Q_DECL_CONSTEXPR QPoint toPoint() const;
    Q_DECL_CONSTEXPR QPointF toPointF() const;
    operator QVariant() const;
private:
    double v[3];
    friend class QVector2D;
    friend class QVector4D;
#ifndef QT_NO_MATRIX4X4
    friend DVector3D operator*(const DVector3D& vector, const QMatrix4x4& matrix);
    friend DVector3D operator*(const QMatrix4x4& matrix, const DVector3D& vector);
#endif
};
Q_DECLARE_TYPEINFO(DVector3D, Q_PRIMITIVE_TYPE);
Q_DECL_CONSTEXPR inline DVector3D::DVector3D() : v{0.0f, 0.0f, 0.0f} {}
Q_DECL_CONSTEXPR inline DVector3D::DVector3D(const QPoint& point) : v{double(point.x()), double(point.y()), double(0.0f)} {}
Q_DECL_CONSTEXPR inline DVector3D::DVector3D(const QPointF& point) : v{double(point.x()), double(point.y()), 0.0f} {}
inline bool DVector3D::isNull() const
{
    return qIsNull(v[0]) && qIsNull(v[1]) && qIsNull(v[2]);
}
Q_DECL_CONSTEXPR inline double DVector3D::x() const { return v[0]; }
Q_DECL_CONSTEXPR inline double DVector3D::y() const { return v[1]; }
Q_DECL_CONSTEXPR inline double DVector3D::z() const { return v[2]; }
inline void DVector3D::setX(double aX) { v[0] = aX; }
inline void DVector3D::setY(double aY) { v[1] = aY; }
inline void DVector3D::setZ(double aZ) { v[2] = aZ; }
inline double &DVector3D::operator[](int i)
{
    Q_ASSERT(uint(i) < 3u);
    return v[i];
}
inline double DVector3D::operator[](int i) const
{
    Q_ASSERT(uint(i) < 3u);
    return v[i];
}
inline DVector3D &DVector3D::operator+=(const DVector3D &vector)
{
    v[0] += vector.v[0];
    v[1] += vector.v[1];
    v[2] += vector.v[2];
    return *this;
}
inline DVector3D &DVector3D::operator-=(const DVector3D &vector)
{
    v[0] -= vector.v[0];
    v[1] -= vector.v[1];
    v[2] -= vector.v[2];
    return *this;
}
inline DVector3D &DVector3D::operator*=(double factor)
{
    v[0] *= factor;
    v[1] *= factor;
    v[2] *= factor;
    return *this;
}
inline DVector3D &DVector3D::operator*=(const DVector3D& vector)
{
    v[0] *= vector.v[0];
    v[1] *= vector.v[1];
    v[2] *= vector.v[2];
    return *this;
}
inline DVector3D &DVector3D::operator/=(double divisor)
{
    v[0] /= divisor;
    v[1] /= divisor;
    v[2] /= divisor;
    return *this;
}
inline DVector3D &DVector3D::operator/=(const DVector3D &vector)
{
    v[0] /= vector.v[0];
    v[1] /= vector.v[1];
    v[2] /= vector.v[2];
    return *this;
}
QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wdouble-equal")
QT_WARNING_DISABLE_GCC("-Wdouble-equal")
QT_WARNING_DISABLE_INTEL(1572)
Q_DECL_CONSTEXPR inline bool operator==(const DVector3D &v1, const DVector3D &v2)
{
    return v1.v[0] == v2.v[0] && v1.v[1] == v2.v[1] && v1.v[2] == v2.v[2];
}
Q_DECL_CONSTEXPR inline bool operator!=(const DVector3D &v1, const DVector3D &v2)
{
    return v1.v[0] != v2.v[0] || v1.v[1] != v2.v[1] || v1.v[2] != v2.v[2];
}
QT_WARNING_POP
Q_DECL_CONSTEXPR inline const DVector3D operator+(const DVector3D &v1, const DVector3D &v2)
{
    return DVector3D(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2]);
}
Q_DECL_CONSTEXPR inline const DVector3D operator-(const DVector3D &v1, const DVector3D &v2)
{
    return DVector3D(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2]);
}
Q_DECL_CONSTEXPR inline const DVector3D operator*(double factor, const DVector3D &vector)
{
    return DVector3D(vector.v[0] * factor, vector.v[1] * factor, vector.v[2] * factor);
}
Q_DECL_CONSTEXPR inline const DVector3D operator*(const DVector3D &vector, double factor)
{
    return DVector3D(vector.v[0] * factor, vector.v[1] * factor, vector.v[2] * factor);
}
Q_DECL_CONSTEXPR inline const DVector3D operator*(const DVector3D &v1, const DVector3D& v2)
{
    return DVector3D(v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v1.v[2] * v2.v[2]);
}
Q_DECL_CONSTEXPR inline const DVector3D operator-(const DVector3D &vector)
{
    return DVector3D(-vector.v[0], -vector.v[1], -vector.v[2]);
}
Q_DECL_CONSTEXPR inline const DVector3D operator/(const DVector3D &vector, double divisor)
{
    return DVector3D(vector.v[0] / divisor, vector.v[1] / divisor, vector.v[2] / divisor);
}
Q_DECL_CONSTEXPR inline const DVector3D operator/(const DVector3D &vector, const DVector3D &divisor)
{
    return DVector3D(vector.v[0] / divisor.v[0], vector.v[1] / divisor.v[1], vector.v[2] / divisor.v[2]);
}
Q_DECL_CONSTEXPR inline bool qFuzzyCompare(const DVector3D& v1, const DVector3D& v2)
{
    return qFuzzyCompare(v1.v[0], v2.v[0]) &&
           qFuzzyCompare(v1.v[1], v2.v[1]) &&
           qFuzzyCompare(v1.v[2], v2.v[2]);
}
Q_DECL_CONSTEXPR inline QPoint DVector3D::toPoint() const
{
    return QPoint(qRound(v[0]), qRound(v[1]));
}
Q_DECL_CONSTEXPR inline QPointF DVector3D::toPointF() const
{
    return QPointF(qreal(v[0]), qreal(v[1]));
}
#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug dbg, const DVector3D &vector);
#endif
#ifndef QT_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const DVector3D &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, DVector3D &);
#endif
#endif
QT_END_NAMESPACE
#endif

