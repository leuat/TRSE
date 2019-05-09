#include "raytracerglobals.h"

RayTracerGlobals::RayTracerGlobals()
{

}

void RayTracerGlobals::Sky(Ray* ray, float scale)
{

    DirectionalLight* light = static_cast<DirectionalLight*>(m_lights[0]);
        float sun = Util::minmax(QVector3D::dotProduct(light->m_direction.normalized(),ray->m_direction), 0.0f, 1.0 );
        QVector3D col = QVector3D(0.6,0.71,0.75) - ray->m_direction.y()*0.1*QVector3D(1.0,0.5,1.0) + 0.05*0.5*QVector3D(1,1,1);
        col += 1.0*QVector3D(1.0,.6,0.1)*pow( sun, 20.0 );


        // sun glare
//        col += 0.2*QVector3D(1.0,0.4,0.2)*pow( sun, 3.0 );
        ray->m_intensity = col*scale;
    return;
}

//bool Ray::::IntersectSphereFake(const QVector3D& pos, QVector3D r, QVector3D &isp1, QVector3D &isp2, double &t0, double &t1) {

bool Ray::IntersectSphere(const QVector3D& pos, QVector3D r, QVector3D &isp1, QVector3D &isp2, double &t0, double &t1) {

    QVector3D o = m_origin-pos;
    QVector3D d = m_direction;


    r.setX(1.0/(r.x()*r.x()));
    r.setY(1.0/(r.y()*r.y()));
    r.setZ(1.0/(r.z()*r.z()));


    QVector3D rD = QVector3D(d.x()*r.x(), d.y()*r.y(), d.z()*r.z());
    QVector3D rO = QVector3D(o.x()*r.x(), o.y()*r.y(), o.z()*r.z());


    float A = QVector3D::dotProduct(d,rD);
    float B = 2.0*(QVector3D::dotProduct(d, rO));
    float C = QVector3D::dotProduct(o, rO) - 1.0;

    float S = (B*B - 4.0*A*C);

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

bool Ray::IntersectBox(QVector3D pos, QVector3D bb,QVector3D& isp1,QVector3D& isp2, double& t0, double& t1)
{
    QVector3D min = bb*-1+pos;
    QVector3D max = bb+pos;


        float tmin = (min.x() - m_origin.x()) / m_direction.x();
        float tmax = (max.x() - m_origin.x()) / m_direction.x();

        if (tmin > tmax) swap(tmin, tmax);

        float tymin = (min.y() - m_origin.y()) / m_direction.y();
        float tymax = (max.y() - m_origin.y()) / m_direction.y();

        if (tymin > tymax) swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            return false;

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (min.z() - m_origin.z()) / m_direction.z();
        float tzmax = (max.z() - m_origin.z()) / m_direction.z();

        if (tzmin > tzmax) swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;

        if (tzmin > tmin)
            tmin = tzmin;

        if (tzmax < tmax)
            tmax = tzmax;


        return true;
}
