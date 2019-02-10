#include "rayobject.h"

void AbstractRayObject::SetLocalPos(QVector3D campos, QMatrix4x4 mat) {
    m_localRotmat = m_rotmat*mat;
    m_centerPos = campos;
    m_localPos = campos + mat.inverted()*m_position;
    m_localRotmatInv = m_localRotmat.inverted();
    float bbr = m_bbRadius;
    for (AbstractRayObject* aro : m_children) {
        aro->SetLocalPos(m_localPos, m_localRotmat);
        bbr = max(bbr, aro->m_position.length()+aro->m_bbRadius);
    }
    m_bbRadius = bbr;
}

void AbstractRayObject::SetLocalRay(int tid, Ray &ray) {
    m_localRay[tid] = ray.Rotate(m_localRotmat, m_localPos);
    for (AbstractRayObject* aro:m_children)
        aro->SetLocalRay(tid, ray);

}

AbstractRayObject::AbstractRayObject()
{
    m_rotmat.setToIdentity();
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
        l+=  dl->m_color*mat.m_shininess_strength*max(pow(QVector3D::dotProduct(H,normal),m_material.m_shininess),0.0f);
    }
    return l;

}

void AbstractRayObject::CalculateLight(Ray* ray, QVector3D& normal, QVector3D& tangent, QVector3D& isp, RayTracerGlobals &globals,QVector3D reflectDir, QVector<AbstractRayObject*>& objects, int pass)
{

    double l = (ray->m_origin-(isp+m_localPos)).length();

    if (l<ray->m_z)
    {

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
//            col.setX(1);
            ray->m_intensity = col*ApplyDirectionalLight(normal,globals) + globals.m_ambient;

/*            if (m_material.m_reflectivity>0 && ray->m_reflect!=0) {
                if (pass==0)
                if (pass==2)
                ray->m_intensity = ray->m_intensity*(1-m_material.m_reflectivity) + m_material.m_reflectivity*ReflectMarch(this,isp+m_localPos, reflectDir, globals, objects, ray->m_reflect-1);
            }*/
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

QVector3D AbstractRayObject::ReflectMarch(AbstractRayObject *me, QVector3D isp, QVector3D normal, RayTracerGlobals &globals, QVector<AbstractRayObject *> &objects, int reflect)
{
    QVector3D I = QVector3D(0,0,0);
    int N = 16;

    for (int i=0;i<16;i++) {
//        isp =
        Ray ray(isp,normal);
        ray.m_reflect = reflect;
        //if (rand()%100==0) qDebug() << reflect;
        for (AbstractRayObject* ar: objects) {
            if (ar==me)
                continue;


        }

        I=I+ray.m_intensity;
        return I;///(float)N;
    }
}

QVector3D AbstractRayObject::CalcMarchNormal(QVector3D &pos)
{
    QVector3D e = QVector3D(1.0,-1.0,0)*0.1773*0.05;

    QVector3D exyy = QVector3D(e.x(), e.y(),e.y());
    QVector3D eyyx = QVector3D(e.y(), e.y(),e.x());
    QVector3D eyxy = QVector3D(e.y(), e.x(),e.y());
    QVector3D exxx = QVector3D(e.x(), e.x(),e.x());


//    vec2( sdSphere(    pos-vec3( 0.0,0.25, 0.0), 0.25 ), 46.9 ) );
//    if (rand()%100==0) qDebug() << "POS: " << pos;
    return ( exyy*intersect( pos + exyy ) +
                      eyyx*intersect( pos + eyyx ) +
                      eyxy*intersect( pos + eyxy ) +
                      exxx*intersect( pos + exxx ) ).normalized();

}


bool RayObjectSphere::RayTrace(Ray *ray, RayTracerGlobals &globals, QVector3D& isp, int pass, QVector<AbstractRayObject*>& objects)
{
    QVector3D isp1, isp2;
    double t0,t1;
    if (ray->IntersectSphere(m_localPos,m_radius,isp1, isp2, t0,t1)) {
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

float RayObjectSphere::intersect(Ray *ray)
{
    return (m_localPos+ray->m_currentPos).length() - m_radius.x();


}

QVector3D RayObjectPlane::CalculateUV(QVector3D &pos, QVector3D &normal, QVector3D& tangent) {
    QVector3D bt = QVector3D::crossProduct(normal,tangent).normalized();

    return QVector3D(QVector3D::dotProduct(bt,pos),QVector3D::dotProduct(tangent,pos),0)*m_material.m_uvScale;
}

float RayObjectPlane::intersect(Ray *ray)
{
    return (-m_localPos.y() + ray->m_currentPos.y());
}

bool RayObjectPlane::RayTrace(Ray *ray, RayTracerGlobals &globals, QVector3D &isp, int pass, QVector<AbstractRayObject *> &objects)
{

    double t = QVector3D::dotProduct(m_pNormal, m_localPos-ray->m_origin)/QVector3D::dotProduct(m_pNormal,ray->m_direction);

    if (t>0) {
        QVector3D tangent = QVector3D::crossProduct(QVector3D(1.123,1.12345,45.1234),m_pNormal).normalized();

        isp = ray->m_origin+t*ray->m_direction.normalized();
        QVector3D normal = GetPerturbedNormal(isp, m_pNormal, tangent, globals);
        QVector3D p = isp - m_localPos;

        QVector3D reflectionDir = ray->m_direction-2*QVector3D::dotProduct(ray->m_direction, normal)*normal;

//        if (pass==0)
            CalculateLight(ray,normal,tangent,p,globals,reflectionDir, objects, pass);
        return true;
    }
    globals.Sky(ray, 1);
    return false;


}

QVector3D RayObjectTorus::CalculateUV(QVector3D &pos, QVector3D &normal, QVector3D &tangent)
{
    return QVector3D(0,0,0);
}

float RayObjectTorus::intersect(Ray *ray)
{

    QVector3D pos = m_localPos + ray->m_currentPos;
    QVector3D pp = pos;
    pp.setY(0);
    QVector3D q = QVector3D(pp.length()-m_radius.x(),pos.y(),0);
     return q.length()-m_radius.y();
     return 1E20;
}


QVector3D RayObjectBox::CalculateUV(QVector3D &pos, QVector3D &normal, QVector3D &tangent)
{
    return QVector3D(1,1,0);
}

float RayObjectBox::intersect(Ray *ray)
{
    QVector3D d = Util::abss(m_localPos+ray->m_currentPos) - m_box;// +ray->m_currentPos;
    float r=m_pNormal.x();

    return min(max(d.x()-r,max(d.y()-r,d.z()-r)),0.0f) + Util::maxx(d,QVector3D(0,0,0)).length();

}

QVector3D RayObjectCylinder::CalculateUV(QVector3D &pos, QVector3D &normal, QVector3D &tangent)
{
    return QVector3D(0,0,0);
}

float RayObjectCylinder::intersect(Ray *ray)
{
    QVector3D pos = m_localPos+ray->m_currentPos;
    QVector3D p = QVector3D(pos.x(), pos.z(),0);
    QVector3D d = QVector3D(p.length()-2*m_radius.x() + m_radius.y()*1.0, abs(pos.y())-m_radius.z(),0);

    return min(max(d.x(),d.y()),0.0f) + Util::maxx(d,QVector3D(0,0,0)).length() - m_radius.y();
}

float RayObjectUnion::intersect(Ray *ray) {
    float d = 0;
    for (AbstractRayObject* aro: m_objects) {
  //      c.m_origin+=aro->m_position;
//        c.m_currentPos+=aro->m_position;
//        d = (aro->intersect(&c),d);
    }
    return d;

}

float RayObjectEmpty::intersect(Ray *ray) {
    float d = 1E10;
    for (AbstractRayObject* aro: m_children) {
/*        if ((aro->m_position-ray->m_currentPos).length()>aro->m_bbRadius)
            break;*/
        Ray r = *ray;// = new Ray();
        r = r.Rotate(aro->m_localRotmat,aro->m_localPos);
        d = min(aro->intersect(&r),d);
    }
    return d;
}
