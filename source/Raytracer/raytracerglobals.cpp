#include "raytracerglobals.h"

RayTracerGlobals::RayTracerGlobals()
{
    m_lights.append(new DirectionalLight(QVector3D(1,1,-1).normalized(),QVector3D(1,1,1)));
}

bool Ray::IntersectSphere(const QVector3D& pos, QVector3D r, QVector3D &isp1, QVector3D &isp2, double &t0, double &t1) {

    QVector3D o = m_origin-pos;
    QVector3D d = m_direction;


    r.setX(1.0/(r.x()*r.x()));
    r.setY(1.0/(r.y()*r.y()));
    r.setZ(1.0/(r.z()*r.z()));


    QVector3D rD = QVector3D(d.x()*r.x(), d.y()*r.y(), d.z()*r.z());
    QVector3D rO = QVector3D(o.x()*r.x(), o.y()*r.y(), o.z()*r.z());


    double A = QVector3D::dotProduct(d,rD);
    double B = 2.0*(QVector3D::dotProduct(d, rO));
    double C = QVector3D::dotProduct(o, rO) - 1.0;

    double S = (B*B - 4.0*A*C);

    if (S<=0) {
        isp1 = QVector3D(0,0,0);
        isp2 = QVector3D(0,0,0);
        t0 = 0;
        t1 = 0;
        return false;
    }

    t0 =  (-B - sqrt(S))/(2.0*A);
    t1 =  (-B + sqrt(S))/(2.0*A);

    isp1 = o+d*t0;
    isp2 = o+d*t1;

    return true;
}
