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
#include "source/Raytracer/particles.h"

class RayTracer
{
public:
    RayTracer();
    QVector<AbstractRayObject*> m_objects, m_objectsFlattened;
    Camera m_camera;
    RayTracerGlobals m_globals;
    Particles m_particles;
    enum Pass { Image, Reflect, Shadow};
//    void Raytrace(QImage& img);
    void Raymarch(QImage& img, int w, int h);

    void LoadMesh(QString fn, float scale, QVector3D orgPos, Material mat, QString name, bool invertN);

    void SetParticles() {
        for (Particle& p:m_particles.m_particles) {
            m_objects[p.id]->m_position = p.P;
        }
    }

    AbstractRayObject* Find(QString name) {
        for (AbstractRayObject* aro: m_objects) {
            AbstractRayObject* c = aro->Find(name);
            if (c!=nullptr)
                return c;

        }
        return nullptr;
    }

    int FindID(AbstractRayObject* aro) {
        for (int i=0;i<m_objects.count();i++)
            if (m_objects[i]==aro)
                return i;
        return -1;
    }

    bool RayMarchSingle(Ray& ray, Pass pass, AbstractRayObject* ignore, int cnt, int tid, QPoint point);

    void Compile2DList(QString fileOutput, int base, int maxx, QVector<QPoint>& killList,QImage &img);
};

#endif // RAYTRACER_H
