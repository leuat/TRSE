#include "rayobject.h"

AbstractRayObject::AbstractRayObject()
{

}

float AbstractRayObject::ApplyDirectionalLight(QVector3D normal, RayTracerGlobals &globals)
{
    float l = 0;
    for (AbstractLight* al : globals.m_lights) {
        DirectionalLight* dl = static_cast<DirectionalLight*>(al);
        if (dl==nullptr)
            continue;

        l+= max(QVector3D::dotProduct(dl->m_direction.normalized(),normal),0.0f);
    }
    return l;
}

QVector3D AbstractRayObject::ApplySpecularLight(QVector3D normal, QVector3D view, RayTracerGlobals &globals, Material &mat)
{
    QVector3D l = QVector3D(0,0,0);
    for (AbstractLight* al : globals.m_lights) {
        DirectionalLight* dl = static_cast<DirectionalLight*>(al);
        if (dl==nullptr)
            continue;


        QVector3D H = ((dl->m_direction.normalized()-view.normalized())).normalized();
        l+=  dl->m_color*max(pow(QVector3D::dotProduct(H,normal),m_material.m_shininess),0.0f);
    }
    return l;

}

void AbstractRayObject::CalculateLight(Ray* ray, QVector3D& normal, QVector3D& tangent, QVector3D& isp, RayTracerGlobals &globals,QVector3D reflectDir, QVector<AbstractRayObject*>& objects, int pass)
{

    double l = (ray->m_origin-(isp+m_position)).length();

    if (l<ray->m_z) {

        QVector3D col = m_material.m_color;

        if (pass==0)
        {

            if (m_material.m_hasTexture) {
                QVector3D uv = CalculateUV(isp, normal, tangent);

                float lvl = pow(0.1*l,0.9);
//                lvl = 0;
                //            lvl = 4;
                QImage* img = m_material.m_texture.get(lvl);
                float uu = abs((int)(uv.x()*(float)img->width())%img->width());
                float vv = abs((int)(uv.y()*(float)img->height())%img->height());
                QVector3D c = Util::fromColor(QColor(img->pixel(uu,vv)))/256.;
                col.setX(col.x()*c.x());
                col.setY(col.y()*c.y());
                col.setZ(col.z()*c.z());
            }

            ray->m_intensity = col*ApplyDirectionalLight(normal,globals) + globals.m_ambient;

            if (m_material.m_reflectivity>0 && ray->m_reflect!=0) {
                ray->m_intensity = ray->m_intensity*(1-m_material.m_reflectivity) + m_material.m_reflectivity*Reflect(this,isp+m_position, reflectDir, globals, objects, ray->m_reflect-1);
            }
            ray->m_intensity += ApplySpecularLight(normal,ray->m_direction,  globals, m_material);
        }
        ray->m_z = l;
    }

}

QVector3D AbstractRayObject::GetPerturbedNormal(QVector3D pos, QVector3D normal, QVector3D tangent, RayTracerGlobals& globals)
{
    if (m_material.m_perlinness==0)
        return normal;
//    return normal;

            QVector3D bn = QVector3D::crossProduct(tangent, normal).normalized();
            QVector3D perlin = globals.getPerlinNormal(pos,normal,tangent,bn,0.5,m_material.m_perlinScale);
            QVector3D dir = (1-m_material.m_perlinness)*normal + (m_material.m_perlinness)*perlin;
            normal = (dir + normal).normalized();

    return normal;
}

QVector3D AbstractRayObject::Reflect(AbstractRayObject* me, QVector3D isp, QVector3D normal, RayTracerGlobals &globals, QVector<AbstractRayObject *> &objects, int reflect)
{
    QVector3D I = QVector3D(0,0,0);
//    QVector3D I = QVector3D(0,0,0);
    int N = 1;
//    for (int i=0;i<N;i++) {
        //QVector3D dir = QVector3D(rand()%100-50, rand()%100-50, rand()%100-50)/100.0;
//        QVector3D dir = normal;

//
        Ray ray(isp,normal);
        ray.m_reflect = reflect;
        //if (rand()%100==0) qDebug() << reflect;
        for (AbstractRayObject* ar: objects) {
            if (ar==me)
                continue;
            QVector3D tmp;
            ar->RayTrace(&ray,globals,tmp,0,objects);
        }

        I=I+ray.m_intensity;
  //  }
//    I= QVector3D(1,0,0);
    return I;///(float)N;
}

bool RayObjectSphere::RayTrace(Ray *ray, RayTracerGlobals &globals, QVector3D& isp, int pass, QVector<AbstractRayObject*>& objects)
{
    QVector3D isp1, isp2;
    double t0,t1;
    if (ray->IntersectSphere(m_position,m_radius,isp1, isp2, t0,t1)) {
        isp = isp1;
        if (t0<0 || t1<0)
            return false;

        QVector3D normal = isp.normalized();

        QVector3D rnd = QVector3D(isp.y(), isp.z(), isp.x());
        QVector3D tangent = QVector3D::crossProduct(rnd, isp).normalized();

        normal = GetPerturbedNormal(QVector3D(0,0,0),isp.normalized(),tangent, globals);

//        if (pass==0)
            CalculateLight(ray,normal,tangent, isp,globals, isp.normalized(),objects,pass);

        return true;
    }
    return false;
}

QVector3D RayObjectPlane::CalculateUV(QVector3D &pos, QVector3D &normal, QVector3D& tangent) {
    QVector3D bt = QVector3D::crossProduct(normal,tangent).normalized();

    return QVector3D(QVector3D::dotProduct(bt,pos),QVector3D::dotProduct(tangent,pos),0)*m_material.m_uvScale;
}

bool RayObjectPlane::RayTrace(Ray *ray, RayTracerGlobals &globals, QVector3D &isp, int pass, QVector<AbstractRayObject *> &objects)
{

    double t = QVector3D::dotProduct(m_pNormal, m_position-ray->m_origin)/QVector3D::dotProduct(m_pNormal,ray->m_direction);

    if (t>0) {
        QVector3D tangent = QVector3D::crossProduct(QVector3D(1.123,1.12345,45.1234),m_pNormal).normalized();

        isp = ray->m_origin+t*ray->m_direction.normalized();
        QVector3D normal = GetPerturbedNormal(isp, m_pNormal, tangent, globals);
        QVector3D p = isp - m_position;

        QVector3D reflectionDir = ray->m_direction-2*QVector3D::dotProduct(ray->m_direction, normal)*normal;

//        if (pass==0)
            CalculateLight(ray,normal,tangent,p,globals,reflectionDir, objects, pass);
        return true;
    }
    return false;


}

bool RayObjectTorus::RayTrace(Ray *ray, RayTracerGlobals &globals, QVector3D &isp, int pass, QVector<AbstractRayObject *> &objects)
{
    double t = QVector3D::dotProduct(m_pNormal, m_position-ray->m_origin)/QVector3D::dotProduct(m_pNormal,ray->m_direction);

    if (t>0) {
        QVector3D tangent = QVector3D::crossProduct(QVector3D(1.123,1.12345,45.1234),m_pNormal).normalized();

        isp = ray->m_origin+t*ray->m_direction.normalized();
        QVector3D normal = GetPerturbedNormal(isp, m_pNormal, tangent, globals);
        QVector3D p = isp - m_position;

        QVector3D reflectionDir = ray->m_direction-2*QVector3D::dotProduct(ray->m_direction, normal)*normal;

//        if (pass==0)
            CalculateLight(ray,normal,tangent,p,globals,reflectionDir, objects, pass);
        return true;
    }
    return false;

}
