#include "raytracer.h"

RayTracer::RayTracer()
{

}

int cmpfunc (const AbstractRayObject * a, AbstractRayObject * b) {
   return ( a->m_localPos.length() > b->m_localPos.length() );
}


void RayTracer::Render(QImage &img)
{
    m_globals.m_camera = &m_camera;

    for (AbstractRayObject* aro : m_objects)
        aro->m_localPos = aro->m_position-m_camera.m_camera;

//    qSort(m_objects.begin(), m_objects.end(), cmpfunc);

//    QVector<AbstractRayObject*> objs;



#pragma omp parallel for
    for (int j=0;j<img.height();j++)
        for (int i=0;i<img.width();i++) {
            QVector3D dir = m_camera.coord2ray(i,j,img.width());
            Ray ray(m_camera.m_camera,dir);

            float m_z = 1E20;
            AbstractRayObject *winner = nullptr;
            QVector3D isp;
            for (AbstractRayObject* ro: m_objects) {
                if (ro->RayTrace(&ray, m_globals, isp,1, m_objects)) {
                    if (ray.m_z<m_z) {
                        m_z = ray.m_z;
                        winner = ro;
                    }


                }
            }
                if (winner!=nullptr) {


                    float shadow = 1;
                    QVector3D tmp;
                     ray.m_z = 1E20;
                     winner->RayTrace(&ray, m_globals, isp,0, m_objects);

                        for (AbstractLight* al: m_globals.m_lights) {
                            Ray shadowRay(isp+winner->m_position,al->m_direction);
                            for (AbstractRayObject* sh: m_objects) {
                                if (sh==winner)
                                    continue;
                                if (sh->RayTrace(&shadowRay, m_globals, tmp,1,m_objects)) {
                                    shadow*=0.25;
                                }

                            }
                        }

                        QColor c = Util::toColor(ray.m_intensity*256*shadow + m_globals.m_ambient);
        //                if (i<img.width() && j<img.height())
                        img.setPixel(i,j,c.rgba());
                    }





        }
}
