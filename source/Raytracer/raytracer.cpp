#include "raytracer.h"

#ifndef TARGET_OS_MAC
#include <omp.h>
#endif

RayTracer::RayTracer()
{

}

int cmpfunc (const AbstractRayObject * a, AbstractRayObject * b) {
   return ( a->m_localPos.length() > b->m_localPos.length() );
}


/*void RayTracer::Raytrace(QImage &img)
{
    m_globals.m_camera = &m_camera;

    for (AbstractRayObject* aro : m_objects) {
        aro->SetLocalPos(m_camera.m_camera);
//        aro->m_localPos = aro->m_position-m_camera.m_camera;
    }

//    qSort(m_objects.begin(), m_objects.end(), cmpfunc);

//    QVector<AbstractRayObject*> objs;



#pragma omp parallel for
    for (int j=0;j<img.height();j++)
        for (int i=0;i<img.width();i++) {
            QVector3D dir = m_camera.coord2ray(i,j,img.width(), img.height());
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
            float shadow = 1;

            if (winner!=nullptr) {


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



            }
            else
                m_globals.Sky(&ray, m_globals.m_skyScale);

            QColor c = Util::toColor(ray.m_intensity*256*shadow + m_globals.m_ambient);
            //                if (i<img.width() && j<img.height())
            img.setPixel(i,j,c.rgba());





        }
}

*/





void RayTracer::Raymarch(QImage &img, int w, int h)
{

    m_globals.m_camera = &m_camera;

    m_objectsFlattened.clear();
    QMatrix4x4 id;
    id.setToIdentity();
    for (AbstractRayObject* aro : m_objects) {
        aro->SetLocalPos(QVector3D(0,0,0), id);
//        aro->SetLocalRotation();
        aro->SetMaterial(aro->m_material);
        aro->AddToFlattened(m_objectsFlattened);
    }
  //  for (AbstractRayObject* aro : m_objectsFlattened)
//        qDebug() << aro->m_name;
//    m_objectsFlattened = m_objects;

//        aro->m_localPos = aro->m_position-m_camera.m_camera;

//    qSort(m_objects.begin(), m_objects.end(), cmpfunc);

//    QVector<AbstractRayObject*> objs;


       float aspect = w/(float)h;
       if (aspect>1) aspect = h/(float)w;
       float ah = 1;
     //  QVector3D tmp[w*h];
//       qDebug() << w << h;
 //      aspect = 0.5;


#pragma omp parallel for
       for (int j=0;j<h;j++)
           for (int i=0;i<w;i++)
        {

            float x = i*m_globals.m_aspect;//*aspect;
            float y = j;//*aspect;

            QVector3D dir = m_camera.coord2ray(x,y,w,h);;
            Ray ray(m_camera.m_camera,dir);
            ray.m_reflect=3;

//            float m_z = 1E20;
#ifndef TARGET_OS_MAC
            int tid = omp_get_thread_num();
#else
        int tid = 0;
#endif
            //int tid = 0;
            RayMarchSingle(ray, Image, nullptr,m_globals.m_steps,tid, QPoint(x,y));
  //          tmp[i+j*w] = ray.m_intensity;
            QColor c = Util::toColor(ray.m_intensity*256 + m_globals.m_ambient);
            img.setPixel((i+(int)m_globals.m_translate.x())%img.width(),
                         (j+(int)m_globals.m_translate.y())%img.height(),
                         c.rgba());

        }

/*        for (int i=0;i<w;i++)
            for (int j=0;j<h;j++) {
                QColor c = Util::toColor(tmp[i+j*w]*256 + m_globals.m_ambient);
                img.setPixel(i,j,c.rgba());

            }

*/
}

void RayTracer::LoadMesh(QString fn, float scale, QVector3D orgPos, Material mat, QString name, bool invertN)
{
        ObjLoader ol(fn);

        ol.Parse();
        RayObjectEmpty* parent = new RayObjectEmpty(orgPos);
        parent->m_name = name;
        parent->m_material = mat;
        m_objects.append(parent);
        for (Face& f: ol.m_faces) {
    //        qDebug() << "Face " << f.v1;
//            if (f.v1>=ol.m_vertices.count() || f.v2>=ol.m_vertices.count() || f.v3>=ol.m_vertices.count())
  //              continue;
            QVector3D pos = ol.m_vertices[ f.v1  ]*scale+ol.m_vertices[ f.v2  ]*scale+ol.m_vertices[ f.v3  ]*scale;



/*            parent->m_children.append(new RayObjectSphere(ol.m_vertices[ f.v1  ]*scale,QVector3D(0.1,0,0),mat));
            parent->m_children.append(new RayObjectSphere(ol.m_vertices[ f.v2  ]*scale,QVector3D(0.1,0,0),mat));
            parent->m_children.append(new RayObjectSphere(ol.m_vertices[ f.v3  ]*scale,QVector3D(0.1,0,0),mat));*/
                RayObjectTriangle *rt = new RayObjectTriangle();
                rt->m_pos[0] = ol.m_vertices[ f.v1  ]*scale;
                rt->m_pos[1] = ol.m_vertices[ f.v2  ]*scale;
                rt->m_pos[2] = ol.m_vertices[ f.v3  ]*scale;
                rt->m_centerPos = (rt->m_pos[0]+ rt->m_pos[1]+rt->m_pos[2])/3;
                rt->m_position = -rt->m_centerPos;//rt->m_centerPos;
                rt->m_localPos = -rt->m_centerPos;//rt->m_centerPos;

                for (int i=0;i<3;i++) {
                    rt->m_pos[i]-=rt->m_centerPos;
                    rt->m_pos[i]*=1.04;
                }
                rt->m_normal = QVector3D::crossProduct(rt->m_pos[1]-rt->m_pos[0],rt->m_pos[2]-rt->m_pos[0]).normalized();
                if (invertN)
                    rt->m_normal*=-1;
                rt->m_bbRadius = max(max((rt->m_pos[0]).length(),
                                     (rt->m_pos[1]).length()),
                                     (rt->m_pos[2]).length());

                rt->m_pos[0]+=rt->m_centerPos;
                rt->m_pos[1]+=rt->m_centerPos;
                rt->m_pos[2]+=rt->m_centerPos;

  //          if (parent->m_children.count()<200)
                parent->m_children.append(rt);

            //qDebug() << f.v1 << f.v2 << f.v3;
        /*    RayObjectTriangle *rt = new RayObjectTriangle();
            rt->m_pos[0] = ol.m_vertices[ f.v1  ]*scale;
            rt->m_pos[1] = ol.m_vertices[ f.v2  ]*scale;
            rt->m_pos[2] = ol.m_vertices[ f.v3  ]*scale;
            rt->m_centerPos = (rt->m_pos[0]+ rt->m_pos[1]+rt->m_pos[2])/3;
            rt->m_position = rt->m_centerPos;
            rt->m_normal = QVector3D::crossProduct(rt->m_pos[1]-rt->m_pos[0],rt->m_pos[2]-rt->m_pos[0]).normalized();
            if (m_children.count()<30)
                m_children.append(rt);
                */

        }


    }

//}

bool RayTracer::RayMarchSingle(Ray& ray, Pass pass, AbstractRayObject* ignore, int cnt, int tid, QPoint point)
{
    QVector3D isp;
    float t = 1;
    AbstractRayObject *winner = nullptr;
 //   Ray rotated;
    ray.m_currentPos = ray.m_origin;
    // Generate list with bb
    QVector<AbstractRayObject*> culled;
//    f2 = m_objectsFlattened;
  //  if (pass==Pass::Shadow)
    //    return false;
    int cur = 0;



//    ray.m_intensity = ray.m_cu;
  //  return true;

    for (AbstractRayObject* o: m_objectsFlattened) {
        QVector3D isp1, isp2;
        double t0, t1;
//        qDebug() << o->m_localPos;
//        if (culled.size()>10)
  //          break;
        if (o->m_hasNormal) {
            if (QVector3D::dotProduct(ray.m_direction, o->m_normal)>0)
                continue;
        }

        if (o->m_bbRadius==0) {
           // culled.append(o);

        }
        else
//        if (ray.IntersectSphere((o->m_localPos)*-1,QVector3D(1,1,1)*o->m_bbRadius,isp1, isp2, t0,t1))
        if (ray.IntersectBox((o->m_localPos)*-1,QVector3D(1,1,1)*o->m_bbRadius,isp1, isp2, t0,t1))
        {
            //if (( t1>0) || dynamic_cast<RayObjectPlane*>(o)!=nullptr)
            if (pass==Pass::Shadow) {
                if (o->m_sendsShadow)
                    culled.append(o);
            } else
//                o->AddToFlattened(culled);
                culled.append(o);
        }

    }
//    if (culled.size()>2)
  //      qDebug() << culled.size();
//    culled = f2;
//    return false;

//    culled = m_objectsFlattened;

    if (culled.length()==0) {
        m_globals.Sky(&ray, m_globals.m_skyScale);
        return false;
    }

    for (AbstractRayObject* ro: culled) {
        ro->SetLocalRay(tid,ray);
//        ro->m_localRay[tid] = ray.Rotate(ro->m_localRotmat, ro->m_localPos);
    }

//    return false;
    for (int i=0;i<cnt;i++) {
        float precis = 0.004*t;
        float keep=1000.0;
        ray.m_curStep =t; //(ray.m_origin-m_objects[j]->m_position).length();
        ray.setCurrent(t);




        //rotated = ray;
        AbstractRayObject* w= nullptr;
        for (AbstractRayObject* ro: culled) {

            if (ro==ignore)
                continue;

            ro->m_localRay[tid].setCurrent(t);

            float keep2 = ro->intersect(&ro->m_localRay[tid]);
//            float keep2 = ro->intersect(&ray);
            if (keep2<keep) {
                keep = keep2;
                w = ro;

            }

            if (keep2<precis) {
                winner = w;
                i=cnt;
                if (pass==Shadow)
                    return true;
                break;

            }
        }
            t=t+keep;

    }

    if (winner!=nullptr) {
  //      m_objectsFlattened.removeAll(winner);
    //    m_objectsFlattened.insert(0,winner);

        Ray rotated = winner->m_localRay[tid];//ray.Rotate(winner->m_rotmat, winner->m_position);

        //ray.m_currentPos = isp;
//                exit(1);
        isp = rotated.m_currentPos;
        QVector3D normal;
         normal = winner->CalcMarchNormal(rotated.m_currentPos);

        normal = winner->m_localRotmatInv*normal;
        QVector3D tt(1,2,-213.123);
        QVector3D tangent = QVector3D::crossProduct(tt,normal).normalized();
        QVector3D bi = QVector3D::crossProduct(tangent,normal).normalized();



//        normal  = winner->GetPerturbedNormal(isp,normal,tangent,m_globals);
        if (pass!=Pass::Shadow)
            winner->m_2Dpoints[tid].append(point);


//        ray.m_reflect = 0;
        QVector3D reflectionDir = ray.m_direction-2*QVector3D::dotProduct(ray.m_direction, normal)*normal;
        QVector3D lp = ray.m_currentPos;//-winner->m_localPos;



        QVector<float> shadows;
        for (int i=0;i<m_globals.m_lights.count();i++)
            shadows.append(1);
        //        if (pass==Image)
        int cnt = 0;
        if (winner->m_receivesShadow)
            for (AbstractLight* al: m_globals.m_lights) {
                //            Ray shadowRay(isp,winner->m_rotmat*al->m_direction*1);
                Ray shadowRay(lp,al->m_direction.normalized());
                AbstractRayObject* o= nullptr;
                if (dynamic_cast<RayObjectBox*>(winner)!=nullptr)
                    o=winner;
                float shadow = 1;
                if (RayMarchSingle(shadowRay, Shadow, nullptr,14,tid, point)) {
                    shadow = m_globals.m_shadowScale;
                }
                shadows[cnt] = shadow;
                cnt++;
            }

        //  qDebug() << shadows;

        winner->CalculateLight(&ray,normal,tangent,lp,m_globals,reflectionDir,m_objects,0,shadows);

        if (winner->m_material.m_reflectivity>0 && ray.m_reflect>0) {
            Ray nxt(lp,reflectionDir);
            nxt.m_reflect=ray.m_reflect-1;
            RayMarchSingle(nxt, Reflect, winner,m_globals.m_shadowSteps,tid, point);

            ray.m_intensity = ray.m_intensity*(1-winner->m_material.m_reflectivity) + winner->m_material.m_reflectivity*nxt.m_intensity;
        }


        //ray.m_intensity*=shadow;


        return true;

    }
    m_globals.Sky(&ray,m_globals.m_skyScale);

    return false;
}

void RayTracer::Compile2DList(QString fileOutput, int base, int maxx, QVector<QPoint>& killList)
{
    QByteArray data;
    data.append(m_objects.count());
    QVector<QPoint> total;
    for (AbstractRayObject* aro : m_objects) {

        QVector<QPoint> reduced;
        int j=0;
  //      qDebug() << aro->m_2Dpoints.count();

        // Combine all 2D points from instances
        QVector<QPoint> all;
        QVector<QPoint> perhapsKill;
        for (int i=0;i<32;i++)
            all.append(aro->m_2Dpoints[i]);

        for (QPoint p: all) {
            QPoint org = p;
            p.setX(p.x()/4);
            p.setY(p.y()/8);
            bool ok=true;
            bool kill = true;
            for (QPoint op:reduced)
                if (op==p) {
                    ok=false;
                    kill = false;
                }

/*            for (QPoint op:total)
                if (op==p) {
                    ok=false;
                    kill = false;
                }
*/
            if (ok && reduced.count()<maxx) {
                reduced.append(p);
    //            perhapsKill.append(org);
//                total.append(p);
            }
            else if (kill) perhapsKill.append(org);
        }

//        if (reduced.count()>)
  //      if (reduced.count()>maxx) reduced.resize(maxx);
        // Kill avereything above
//        perhapsKill.remove(0,std::min(maxx,perhapsKill.count()));
        killList.append(perhapsKill);
        total.append(reduced);
        char cnt = reduced.count();
        data.append(cnt);
        QVector<int> positions;
        for (QPoint p: reduced) {
            int i = base + p.x() + p.y()*40;
            positions.append(i);
  //          qDebug() << Util::numToHex(i);
//            data.append((char)((i)&0xFF));
  //          data.append((char)((i>>8)&0xFF));
        }
        if (cnt!=0) {
            qSort(positions);
            //        qDebug() << positions;
            int org=positions[0];
            data.append((char)((org)&0xFF));
            data.append((char)((org>>8)&0xFF));
            data.append((char)0);
            for (int i=1; i<positions.count();i++) {
                data.append((uchar)(positions[i]-org));
            }
        }
        else {
            qDebug() << "CNT iS ZERO : " << QString::number(cnt);
            int org = base;
        }
        for (int i=0;i<32;i++)
        aro->m_2Dpoints[i].clear();
    }
    Util::SaveByteArray(data,fileOutput);
}
