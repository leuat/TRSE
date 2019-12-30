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
#include "source/LeLib/objloader/objloader.h"

class AbstractRayObject
{
public:

    QMatrix4x4 m_transform;
    QMatrix4x4 m_rotmat, m_rotmatInv;
    QMatrix4x4 m_localRotmat, m_localRotmatInv;
    QVector3D m_normal;
    QVector3D m_position;
    QVector3D m_scale;
    QVector3D m_rotation;
    QVector3D m_localPos;
    QVector3D m_centerPos;
    QVector3D m_uvShift = QVector3D(0,0,0);
    bool m_sendsShadow = true;
    bool m_receivesShadow = true;
    bool m_flatten = true;
    bool m_hasNormal = false;
    Material m_material;
    QString m_name;
    static SimplexNoise m_sn;
    Ray m_localRay[32];
    QVector<QPoint> m_2Dpoints[32];
    float m_bbRadius;

    QVector<AbstractRayObject*> m_children;

    void SetMaterial(Material m) {
        m_material = m;
        for (AbstractRayObject* aro : m_children)
            aro->SetMaterial(m);

    }


    void AddToFlattened(QVector<AbstractRayObject*>& list) {
       list.append(this);
        if (m_flatten)
        for (AbstractRayObject* aro : m_children)
            aro->AddToFlattened(list);

    }

    AbstractRayObject* Find(QString name) {
        if (m_name == name)
            return this;

        for (AbstractRayObject* aro: m_children) {
            AbstractRayObject* c = aro->Find(name);
            if (c!=nullptr)
                return c;
        }
        return nullptr;
    }


    void SetLocalPos(QVector3D campos, QMatrix4x4 mat);

    void SetLocalRay(int tid, Ray& ray);

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

    void SetQuatAxisAngle(QVector3D v, float angle) {
        m_rotation = v;
        QMatrix3x3 rot = QQuaternion::fromAxisAndAngle(v,angle).toRotationMatrix();
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

    QVector3D ApplyDirectionalLight(QVector3D normal, RayTracerGlobals& globals,QVector<float>& shadows);
    QVector3D ApplySpecularLight(QVector3D normal, QVector3D view, RayTracerGlobals& globals, Material& mat,QVector<float>& shadows);
    virtual QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) {return QVector3D(0,0,0);}

    void CalculateLight(Ray* ray, QVector3D& normal,  QVector3D& tangent, QVector3D& localIsp,  RayTracerGlobals& globals, QVector3D reflectDir,QVector<AbstractRayObject*>& objects, int pass, QVector<float>& shadows);

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


class RayObjectEmpty : public AbstractRayObject {
public:
    RayObjectEmpty(QVector3D pos) {
        m_position = pos;
        m_bbRadius = 0.01;
    }
    float intersect(Ray* ray) override;

};




class RayObjectUnion : public AbstractRayObject {
public:
    RayObjectUnion(QVector3D pos) {
        m_position = pos;

    }
    QVector<AbstractRayObject*> m_objects;


    float intersect(Ray* ray) override;

};


class RayObjectOperation : public AbstractRayObject {
public:
    QString m_type;
    float m_blend;
    AbstractRayObject* m_o1, *m_o2;
    RayObjectOperation(QString type, float blend, AbstractRayObject* o1, AbstractRayObject* o2) {
        m_type = type.toLower();
       m_blend = blend;
       m_o1 = o1;
       m_o2 = o2;

    }


    float intersect(Ray* ray) override;

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
    QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent);
};


class RayObjectGenMesh : public AbstractRayObject {
public:

    QString m_type;
    QVector3D m_radius;
    RayObjectGenMesh(QString type, QVector3D pos, QVector3D rad, Material material) {
        m_type = type.toLower();
        m_radius = rad;
        m_position = pos;
        m_material = material;
        m_bbRadius = rad.length();
    }
    float intersect(Ray* ray) override;
    float Duck(Ray* ray);
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

class RayObjectTrianglePrism : public AbstractRayObject {
public:
    QVector3D m_pNormal = QVector3D(0,1,0);
    QVector3D m_box = QVector3D(1,1,1);

    RayObjectTrianglePrism(QVector3D pos, QVector3D normal, QVector3D box, Material material) {
        m_pNormal = normal;
        m_position = pos;
        m_box = box;
        m_material = material;
        m_bbRadius = m_box.length();
    }
    RayObjectTrianglePrism(QVector3D pos, QVector3D normal,Material material) {
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

class RayObjectTriangle: public AbstractRayObject {
  public:
    QVector3D m_pos[3];

    RayObjectTriangle();
    float intersect(Ray* ray) override;


};


class RayObjectCylinder : public AbstractRayObject {
public:
    QVector3D m_radius = QVector3D(2,1,0);


    RayObjectCylinder(QVector3D pos, QVector3D radius, Material material) {
        m_position = pos;
        m_material = material;
        m_radius = radius;
        m_bbRadius = 1.5*radius.length();
    }
    QVector3D CalculateUV(QVector3D& pos, QVector3D& normal, QVector3D& tangent) override;
    float intersect(Ray* ray) override;

};

class RayObjectPerlin : public AbstractRayObject {
public:
    QVector3D m_perlinVals;
//    AbstractRayObject* m_obj;
    RayObjectPerlin(QVector3D vals, AbstractRayObject* obj) {
        m_perlinVals = vals;
  //      m_obj = obj;
        m_children.append(obj);
        m_position = obj->m_position;
        m_localPos = obj->m_localPos;
        m_rotation = obj->m_rotation;
        m_bbRadius = obj->m_bbRadius;
    }

    float intersect(Ray* ray) override;

};


class RayObjectHoles : public AbstractRayObject {
public:
    QVector3D m_vals;
    AbstractRayObject* m_obj;
    RayObjectHoles(QVector3D val, AbstractRayObject* obj) {
        m_vals = val;
        m_obj = obj;
    }

    float intersect(Ray* ray) override;

};

#endif // RAYOBJECT_H
