#ifndef RAYTRACERGLOBALS_H
#define RAYTRACERGLOBALS_H

#include <QVector3D>
#include <QVector>
#include <math.h>
#include <QMatrix3x3>
#include "source/LeLib/util/SimplexNoise.h"
#include "source/LeLib/util/util.h"

#include "camera.h"
class Ray {
public:
    QVector3D m_origin;
    QVector3D m_direction;
    QVector3D m_intensity = QVector3D(0,0,0);
    float m_reflect = 3;
    double m_z = 1E20;


    float m_curStep = 0;
    QVector3D m_currentPos;
    QVector3D m_currentPosUnrotated;

    Ray() {}
    void swap(float& a, float& b) {
        float c=a;
        a=b;
        b=c;
    }

    void setCurrent(float t) {
        m_currentPos  = m_origin + m_direction*t;
    }

    Ray(QVector3D pos, QVector3D dir) {
        m_origin = pos;
        m_direction = dir;
    }
    Ray(QVector3D pos, QVector3D dir, QVector3D I) {
        m_origin = pos;
        m_direction = dir;
        m_intensity = I;
    }

    Ray Rotate(QMatrix4x4& rotmat, QVector3D pos) {
        Ray r(rotmat*(m_currentPos+pos)-pos,rotmat*m_direction);
        r.m_currentPos = r.m_origin;
        return r;
    }

    bool IntersectSphere(const QVector3D& pos, QVector3D r,QVector3D& isp1,QVector3D& isp2, double& t0, double& t1);
    bool IntersectBox(QVector3D pos, QVector3D bb,QVector3D& isp1,QVector3D& isp2, double& t0, double& t1);

};



class AbstractLight {
public:
    QVector3D m_color;
    QVector3D m_direction;
};

class DirectionalLight  : public AbstractLight{
public:

    DirectionalLight(QVector3D dir, QVector3D col) {
        m_color = col;
        m_direction = dir;
    }
};


class RayTracerGlobals
{
public:
    RayTracerGlobals();
    SimplexNoise m_noise;


    double m_steps = 120;
    double m_shadowSteps = 60;
    float m_isPaused = 0;
    float m_skyScale = 1;
    float m_c64Output = 0;
    float m_dither = 0;
    float m_shadowScale = 0.5;
    int m_width, m_height;
    QVector<int> m_c64Colors;
    QVector3D getPerlinNormal(QVector3D p, QVector3D n, QVector3D t, QVector3D bn, float s, float s2) {
        QVector3D p0 = n.normalized()+p;
        QVector3D p1 = (n+t*s).normalized()+p;
        QVector3D p2 = (n+bn*s).normalized()+p;
        p0 = p0*(1+s*m_noise.noise(s2*p0.x(), s2*p0.y(), s2*p0.z()));
        p1 = p1*(1+s*m_noise.noise(s2*p1.x(), s2*p1.y(), s2*p1.z()));
        p2 = p2*(1+s*m_noise.noise(s2*p2.x(), s2*p2.y(), s2*p2.z()));
        return QVector3D::crossProduct(p1-p0,p2-p0).normalized();
    }

    QVector3D m_ambient = QVector3D(0.3,0.3,0.3);

    QVector<AbstractLight*> m_lights;
    Camera* m_camera;


    void Sky(Ray* ray, float scale);


};

#endif // RAYTRACERGLOBALS_H
