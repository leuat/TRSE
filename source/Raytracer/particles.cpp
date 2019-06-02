#include "particles.h"

void Particles::CollideSphere(float s) {
    #pragma omp parallel for
    for (int i=0;i<m_particles.count();i++)
        for (int j=i+1;j<m_particles.count();j++)
            m_particles[i].CollideSphere(m_particles[j],s);
}


Particles::Particles()
{

}

void Particle::CollideSphere(Particle &o, float s) {
    QVector3D dir = o.P - P;
    float l = dir.length();
    dir = dir.normalized();
    if (l<radius + o.radius) {
        float diff = (radius + o.radius)-l;
        o.P = P + dir*(radius+o.radius+e-0*diff/2);
        //  P = P - dir*(1+diff/2);
        o.ApplyForce(dir*o.mass*s);
        ApplyForce(dir*mass*-1*s);
    }
}
