#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include "camera.h"
#include "raytracerglobals.h"
#include "rayobject.h"
#include <QImage>
#include "source/LeLib/objloader/objloader.h"


class RayTracer
{
public:
    RayTracer();
    QVector<AbstractRayObject*> m_objects, m_objectsFlattened;
    Camera m_camera;
    RayTracerGlobals m_globals;
    enum Pass { Image, Reflect, Shadow};
//    void Raytrace(QImage& img);
    void Raymarch(QImage& img, int w, int h);

    void LoadMesh(QString fn, float scale, QVector3D orgPos, Material mat, QString name, bool invertN);

    AbstractRayObject* Find(QString name) {
        for (AbstractRayObject* aro: m_objects) {
            AbstractRayObject* c = aro->Find(name);
            if (c!=nullptr)
                return c;

        }
        return nullptr;
    }

    bool RayMarchSingle(Ray& ray, Pass pass, AbstractRayObject* ignore, int cnt, int tid, QPoint point);

    void Compile2DList(QString fileOutput, int base, int maxx, QVector<QPoint>& killList);
};

#endif // RAYTRACER_H
