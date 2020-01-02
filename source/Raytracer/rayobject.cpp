#include "rayobject.h"

SimplexNoise AbstractRayObject::m_sn;


void AbstractRayObject::SetLocalPos(QVector3D campos, QMatrix4x4 mat) {
    m_localRotmat = m_rotmat*mat;
//    m_centerPos = campos;
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

QVector3D AbstractRayObject::ApplyDirectionalLight(QVector3D normal, RayTracerGlobals &globals,QVector<float>& shadows)
{
    QVector3D l = QVector3D(0,0,0);
    int cnt = 0;
    for (AbstractLight* al : globals.m_lights) {
        DirectionalLight* dl = static_cast<DirectionalLight*>(al);
        if (dl==nullptr)
            continue;

        l+= dl->m_color*max(QVector3D::dotProduct(dl->m_direction.normalized(),normal),0.0f)*shadows[cnt];
        cnt++;
    }
    return l;
}

QVector3D AbstractRayObject::ApplySpecularLight(QVector3D normal, QVector3D view, RayTracerGlobals &globals, Material &mat,QVector<float>& shadows)
{
    QVector3D l = QVector3D(0,0,0);
    int cnt = 0;
    for (AbstractLight* al : globals.m_lights) {
        DirectionalLight* dl = static_cast<DirectionalLight*>(al);
        if (dl==nullptr)
            continue;


        QVector3D H = ((dl->m_direction.normalized()-view.normalized())).normalized();
        l+=  dl->m_color*mat.m_shininess_strength*max(pow(QVector3D::dotProduct(H,normal),m_material.m_shininess),0.0f)*shadows[cnt];
        cnt++;
    }
    return l;

}

void AbstractRayObject::CalculateLight(Ray* ray, QVector3D& normal, QVector3D& tangent, QVector3D& isp, RayTracerGlobals &globals,QVector3D reflectDir, QVector<AbstractRayObject*>& objects, int pass, QVector<float>& shadows)
{

    double l = (ray->m_origin-(isp+m_localPos)).length();

    if (l<ray->m_z)
    {

        QVector3D col = m_material.m_color;
        if (m_material.m_checkerBoard.x()!=0) {
            float x = m_material.m_checkerBoard.x();
            //QVector3D mul = QVector3D(1,1,1);
            QVector3D n = normal.normalized();
            Ray r = *ray;// = new Ray();
            r = r.Rotate(m_localRotmat,m_position);
            //r = r.Rotate(m_localRotmatInv,m_localPos);

            n = CalculateUV( r.m_currentPos,normal,tangent) + m_uvShift;
//            n = QVector3D(asin(n.y()), atan2(n.z(), n.x()),0);
            //mul.setX(fmod(abs(n.x()), x)>x/2);
            //mul.setY(fmod(abs(n.y()), x)>x/2);
            //mul.setZ(fmod(abs(n.z()), x)>x/2);
            float mul = fmod(abs(n.x()+100), x)>x/3;
            mul = mul*fmod(abs(n.y()+100), x)>x/3;
//            mul = mul*fmod(abs(n.z()), x)>x/2;
            col = col*mul;
        }
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



            ray->m_intensity = col*ApplyDirectionalLight(normal,globals,shadows);
            ray->m_intensity.setX(max(ray->m_intensity.x(),globals.m_ambient.x()*col.x()));
            ray->m_intensity.setY(max(ray->m_intensity.y(),globals.m_ambient.y()*col.y()));
            ray->m_intensity.setZ(max(ray->m_intensity.z(),globals.m_ambient.z()*col.z()));
            ray->m_intensity += ApplySpecularLight(normal,ray->m_direction,  globals, m_material, shadows);
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
    if (m_hasNormal)
        return m_normal;

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
//            CalculateLight(ray,normal,tangent, isp,globals, isp.normalized(),objects,pass);

        return true;
    }
    return false;
}

float RayObjectSphere::intersect(Ray *ray)
{
    return ((m_localPos+ray->m_currentPos).length() - m_radius.x());


}

QVector3D RayObjectSphere::CalculateUV(QVector3D &pos, QVector3D &normal, QVector3D &tangent)
{
    return QVector3D(asin(normal.y()), atan2(normal.z(), normal.x()),0);

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
            //CalculateLight(ray,normal,tangent,p,globals,reflectionDir, objects, pass);
        return true;
    }
    globals.Sky(ray, 1);
    return false;


}

QVector3D RayObjectTorus::CalculateUV(QVector3D &pos, QVector3D &normal, QVector3D &tangent)
{
    float z = pos.z();
    float x = pos.x();
    float y = pos.y();
    float u = (1.0 - (atan2(z, x) + M_PI) / (M_PI*2));

    float len = sqrt(x * x + z * z);

        // Now rotate about the y-axis to get the point P into the x-z plane.
    x = len - m_radius.x();
    float  v = (atan2(y, x) + M_PI) / (M_PI*2);


    return QVector3D(u*2,v,0);
}

float RayObjectTorus::intersect(Ray *ray)
{

    QVector3D pos = m_localPos + ray->m_currentPos;
//    pos = pos + QVector3D(0.2,0,0)*sin(pos.x()*4.0);
    QVector3D pp = pos;
    pp.setY(0);
    QVector3D q = QVector3D(pp.length()-m_radius.x(),pos.y(),0);
     return q.length()-m_radius.y();
}


QVector3D RayObjectBox::CalculateUV(QVector3D &pos, QVector3D &normal, QVector3D &tangent)
{
    return QVector3D(1,1,0);
}

float RayObjectBox::intersect(Ray *ray)
{
    QVector3D d = Util::abss(m_localPos+ ray->m_currentPos) - m_box;// +ray->m_currentPos;
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

    if (m_flatten)
        return d;

    for (AbstractRayObject* aro: m_children) {
/*        if ((aro->m_position-ray->m_currentPos).length()>aro->m_bbRadius)
            break;*/
        Ray r = *ray;// = new Ray();
        r = r.Rotate(aro->m_localRotmat,aro->m_localPos);
        d = min(aro->intersect(&r),d);
        if (d<=0)
            return d;
    }
    return d;
}

RayObjectTriangle::RayObjectTriangle() {
    m_sendsShadow = false;
    m_receivesShadow = false;
    m_hasNormal = true;
}

float RayObjectTriangle::intersect(Ray *ray)
{
/*    return 100;
    if (QVector3D::dotProduct(m_normal,ray->m_direction)>0)
        return 100;*/
    QVector3D p = ray->m_currentPos;
    float h = 0.06;
    float dtop = abs(p.distanceToPlane(m_pos[1], m_normal));
    float d = dtop;//dtop-2;
    if (d>h) {
        return d;
    }

    bool sameside =
            Util::SameSide(p, m_pos[0], m_pos[1], m_pos[2]) &&
            Util::SameSide(p, m_pos[2], m_pos[0], m_pos[1]) &&
            Util::SameSide(p, m_pos[1], m_pos[0], m_pos[2]);


    if (!sameside) {
        return 100;
    }

    return d;

}


/*    float sdEquilateralTriangle(  in vec2 p )
    {
        const float k = 1.73205;//sqrt(3.0);
        p.x = abs(p.x) - 1.0;
        p.y = p.y + 1.0/k;
        if( p.x + k*p.y > 0.0 ) p = vec2( p.x - k*p.y, -k*p.x - p.y )/2.0;
        p.x += 2.0 - 2.0*clamp( (p.x+2.0)/2.0, 0.0, 1.0 );
        return -length(p)*sign(p.y);
    }

    float sdTriPrism( vec3 p, vec2 h )
    {
        vec3 q = abs(p);
        float d1 = q.z-h.y;
        h.x *= 0.866025;
        float d2 = sdEquilateralTriangle(p.xy/h.x)*h.x;
        return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.);
    }
  */

float RayObjectGenMesh::intersect(Ray *ray)
{
    if (m_type=="duck")
        return Duck(ray);
    return 1;
}

float RayObjectGenMesh::Duck(Ray *ray)
{
    m_bbRadius = m_radius.x()*2;
    float cur = 100;
    QVector3D cp = m_localPos+ray->m_currentPos;
    //cp.setY(cp.y()/0.8);
    cp = cp*QVector3D(1,1.2,1);

    QVector3D cpBottom = m_localPos+ray->m_currentPos;
    cpBottom = cpBottom*QVector3D(0.7, 1.3 *(1+0.3* pow(1.2*sin(cpBottom.x()),3)), 1.0);


    float top = (cp - QVector3D(0.3,1.4,0)).length() - m_radius.x()*0.8;
    float bottom = (cpBottom).length() - m_radius.x();


    cur = min(top, bottom);



    return cur;
}

float RayObjectOperation::intersect(Ray *ray)
{
    if (m_type == "blend") {
        return (m_blend*m_o1->intersect(ray) + (1-m_blend)*m_o2->intersect(ray));
    }
    if (m_type == "max")
        return max(m_o1->intersect(ray),m_o2->intersect(ray));
    if (m_type == "min")
        return max(m_o1->intersect(ray),m_o2->intersect(ray));
    if (m_type == "sub")
        return m_o1->intersect(ray)*max(-m_o2->intersect(ray),0.0f);
}

float RayObjectPerlin::intersect(Ray *ray)
{
    float amp=m_perlinVals.x();
    float scale=m_perlinVals.y();
//    qDebug() << m_obj->m_children.count();
    AbstractRayObject* m_obj = m_children[0];
    Ray r = *ray;// = new Ray();
    r = r.Rotate(m_obj->m_localRotmat,m_obj->m_localPos);

    QVector3D d2 =  ray->m_currentPos;// +ray->m_currentPos;
  //  if (m_obj->m_children.count()==0) {
        float mm = m_obj->intersect(ray);
        if (mm>0.2) return mm;
        return mm + amp*m_sn.noise(d2.x()*scale, d2.y()*scale, d2.z()*scale);

    /*
    float d = 0;

    for (AbstractRayObject* aro: m_obj->m_children) {
        Ray r = *ray;// = new Ray();
        r = r.Rotate(aro->m_localRotmat,aro->m_localPos);
        d = min(aro->intersect(&r),d);
        if (d<=0)
            return d+ amp*m_sn.noise(d2.x()*scale, d2.y()*scale, d2.z()*scale);;
    }
*/

/*    if (m_children.count()!=0)
        for (AbstractRayObject* ro: m_children)
            ro->intersect()
  */

}

float RayObjectHoles::intersect(Ray *ray)
{
    QVector3D d2 =  ray->m_currentPos;// +ray->m_currentPos;
//    d2.setX(fmod(d2.x(),m_vals.x()));
//    if (d2.x()>m_vals.x()/2) d2.setX(1000);

    ray->m_currentPos = d2;
    float mm = m_obj->intersect(ray);

    QVector3D d = Util::abss(m_localPos+ ray->m_currentPos) - m_vals;// +ray->m_currentPos;
    d.setX(fmodf(abs(d.x()),m_vals.x())-m_vals.x()*0.50f);
    d.setY(fmodf(abs(d.y()),m_vals.y())-m_vals.y()*0.50f);
    d.setZ(fmodf(abs(d.z()),m_vals.z())-m_vals.z()*0.50f);
    float cut= min(max(d.x(),max(d.y(),d.z())),0.0f) + Util::maxx(d,QVector3D(0,0,0)).length();


/*    if (mm>0.2) return mm;
    float amp=0.9;
    float deltay = fmod(abs(d2.x()),m_vals.x())>m_vals.x()/5.0;
*/
    return max(mm,cut);// + amp*deltay;

}

QVector3D RayObjectTrianglePrism::CalculateUV(QVector3D &pos, QVector3D &normal, QVector3D &tangent)
{
    return QVector3D(0,0,0);
}

float RayObjectTrianglePrism::intersect(Ray *ray)
{
    QVector3D p = m_localPos+ ray->m_currentPos;
    QVector3D q = Util::abss(m_localPos+ ray->m_currentPos);// +ray->m_currentPos;
      return max(q.z()-m_box.y(),max(q.x()*0.866025f+p.y()*0.5f,-p.y())-m_box.x()*0.5f);

}
