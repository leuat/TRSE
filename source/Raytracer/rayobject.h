#ifndef RAYOBJECT_H
#define RAYOBJECT_H

#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include "source/LeLib/util/util.h"
#include <math.h>
#include "raytracerglobals.h"
#include "material.h"
#include <QQuaternion>

class AbstractRayObject
{
public:

    QMatrix4x4 m_transform;
    QMatrix4x4 m_rotmat, m_rotmatInv;
    QVector3D m_position;
    QVector3D m_scale;
    QVector3D m_rotation;
    QVector3D m_localPos;

    Material m_material;
    QString m_name;

    Ray m_localRay[32];

    float m_bbRadius;



    void SetRotation(QVector3D v) {
        m_rotation = v;
        QMatrix3x3 rot = QQuaternion::fromEulerAngles(v).toRotationMatrix();
        m_rotmat.setToIdentity();
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++)
                m_rotmat(i,j) = rot(i,j);

        bool b;
        m_rotmatInv = m_rotmat.inverted(&b);
    }

    AbstractRayObject();

    virtual QVector3D calculateNormal(Ray* ray, QVector3D isp) {return QVector3D(0,0,0);}
    virtual bool RayTrace(Ray* ray, RayTracerGlobals& globals, QVector3D& isp, int pass,QVector<AbstractRayObject*>& objects) {return false;}

    float ApplyDirectionalLight(QVector3D normal, RayTracerGlobals& globals);
    QVector3D ApplySpecularLight(QVector3D normal, QVector3D view, RayTracerGlobals& globals, Material& mat);
    virtual QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) {return QVector3D(0,0,0);}

    void CalculateLight(Ray* ray, QVector3D& normal,  QVector3D& tangent, QVector3D& localIsp,  RayTracerGlobals& globals, QVector3D reflectDir,QVector<AbstractRayObject*>& objects, int pass);

    QVector3D GetPerturbedNormal(QVector3D pos, QVector3D normal, QVector3D tangent, RayTracerGlobals& globals);

    QVector3D Reflect(AbstractRayObject* me,  QVector3D isp, QVector3D normal, RayTracerGlobals& globals, QVector<AbstractRayObject*>& objects, int reflect);
    QVector3D ReflectMarch(AbstractRayObject* me,  QVector3D isp, QVector3D normal, RayTracerGlobals& globals, QVector<AbstractRayObject*>& objects, int reflect);

    virtual float intersect(Ray* ray) {return 110;}

    float intersect(QVector3D pos) {
        Ray r(pos,pos);
        r.m_currentPos = pos;
        return intersect(&r);
    }

    virtual QVector3D CalcMarchNormal(QVector3D& pos);

};


class RayObjectSphere : public AbstractRayObject {
public:
    RayObjectSphere(QVector3D pos, QVector3D rad, Material material) {
        m_radius = rad;
        m_position = pos;
        m_material = material;
        m_bbRadius = rad.length();
    }
    QVector3D m_radius = QVector3D(1,1,1);
    bool RayTrace(Ray* ray, RayTracerGlobals& globals, QVector3D& isp, int pass,QVector<AbstractRayObject*>& objects) override;
    float intersect(Ray* ray) override;
    QVector3D calculateNormal(Ray* ray, QVector3D isp) {return isp;}

};

class RayObjectPlane : public AbstractRayObject {
public:
    QVector3D m_pNormal = QVector3D(0,1,0);

    RayObjectPlane(QVector3D pos, QVector3D normal, Material material) {
        m_pNormal = normal;
        m_position = pos;
        m_material = material;
        m_bbRadius = 1000;
    }
    QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) override;
    float intersect(Ray* ray) override;

    bool RayTrace(Ray* ray, RayTracerGlobals& globals, QVector3D& isp, int pass,QVector<AbstractRayObject*>& objects) override;
};


class RayObjectBox : public AbstractRayObject {
public:
    QVector3D m_pNormal = QVector3D(0,1,0);
    QVector3D m_box = QVector3D(1,1,1);

    RayObjectBox(QVector3D pos, QVector3D normal, QVector3D box, Material material) {
        m_pNormal = normal;
        m_position = pos;
        m_box = box;
        m_material = material;
        m_bbRadius = m_box.length();
    }
    RayObjectBox(QVector3D pos, QVector3D normal,Material material) {
        m_pNormal = normal;
        m_position = pos;
        m_material = material;
        m_bbRadius = m_box.length();
    }
    QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) override;
    float intersect(Ray* ray) override;

};


class RayObjectTorus : public AbstractRayObject {
public:
    QVector3D m_pNormal = QVector3D(0,1,0);
    QVector3D m_radius = QVector3D(2,1,0);

    RayObjectTorus(QVector3D pos, QVector3D radius, QVector3D normal, Material material) {
        m_pNormal = normal;
        m_position = pos;
        m_material = material;
        m_radius = radius;
        m_bbRadius = (radius.x()+radius.y());
    }
    QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) override;
    float intersect(Ray* ray) override;

};


class RayObjectCylinder : public AbstractRayObject {
public:
    QVector3D m_radius = QVector3D(2,1,0);


    RayObjectCylinder(QVector3D pos, QVector3D radius, Material material) {
        m_position = pos;
        m_material = material;
        m_radius = radius;
        m_bbRadius = 1.1*radius.length();
    }
    QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) override;
    float intersect(Ray* ray) override;

};


#endif // RAYOBJECT_H
