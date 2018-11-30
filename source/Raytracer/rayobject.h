#ifndef RAYOBJECT_H
#define RAYOBJECT_H

#include <QVector3D>
#include <QMatrix4x4>
#include "source/LeLib/util/util.h"
#include <math.h>
#include "raytracerglobals.h"
#include "material.h"

class AbstractRayObject
{
public:

    QMatrix4x4 m_transform;
    QVector3D m_position;
    QVector3D m_scale;
    QVector3D m_rotation;
    QVector3D m_localPos;

    Material m_material;

    AbstractRayObject();


    virtual bool RayTrace(Ray* ray, RayTracerGlobals& globals, QVector3D& isp, int pass,QVector<AbstractRayObject*>& objects) = 0;

    float ApplyDirectionalLight(QVector3D normal, RayTracerGlobals& globals);
    QVector3D ApplySpecularLight(QVector3D normal, QVector3D view, RayTracerGlobals& globals, Material& mat);
    virtual QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) {return QVector3D(0,0,0);}

    void CalculateLight(Ray* ray, QVector3D& normal,  QVector3D& tangent, QVector3D& localIsp,  RayTracerGlobals& globals, QVector3D reflectDir,QVector<AbstractRayObject*>& objects, int pass);

    QVector3D GetPerturbedNormal(QVector3D pos, QVector3D normal, QVector3D tangent, RayTracerGlobals& globals);

    QVector3D Reflect(AbstractRayObject* me,  QVector3D isp, QVector3D normal, RayTracerGlobals& globals, QVector<AbstractRayObject*>& objects, int reflect);
};


class RayObjectSphere : public AbstractRayObject {
public:
    RayObjectSphere(QVector3D pos, QVector3D rad, Material material) {
        m_radius = rad;
        m_position = pos;
        m_material = material;
    }
    QVector3D m_radius = QVector3D(1,1,1);
    bool RayTrace(Ray* ray, RayTracerGlobals& globals, QVector3D& isp, int pass,QVector<AbstractRayObject*>& objects) override;
};

class RayObjectPlane : public AbstractRayObject {
public:
    QVector3D m_pNormal = QVector3D(0,1,0);

    RayObjectPlane(QVector3D pos, QVector3D normal, Material material) {
        m_pNormal = normal;
        m_position = pos;
        m_material = material;
    }
    QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) override;

    bool RayTrace(Ray* ray, RayTracerGlobals& globals, QVector3D& isp, int pass,QVector<AbstractRayObject*>& objects) override;
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
    }
    QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) override;

    bool RayTrace(Ray* ray, RayTracerGlobals& globals, QVector3D& isp, int pass,QVector<AbstractRayObject*>& objects) override;
};



#endif // RAYOBJECT_H
