#ifndef PARTICLES_H
#define PARTICLES_H

#include <QVector3D>
#include <QVector>

class Particle {
public:
    QVector3D P, V, A;
    float mass;
    float radius;
    int id;

    Particle() {

    }
    void Move(float dt) {
        P = P + V*dt;
        V = V + A*dt;
        A = QVector3D(0,0,0);
    }
    float e= 0.001f;

    Particle(QVector3D p, QVector3D v, float m, float rad, int i) {
        P=p;
        V=v;
        A=QVector3D(0,0,0);
        radius = rad;
        mass = m;
        id =i ;
    }

    void ApplyForce(QVector3D F) {
        A= A + F/mass;
    }

    void ConstrainBox(QVector3D min, QVector3D max, float s) {
        if (P.x()<min.x()) { P.setX(min.x()+e); V.setX(V.x()*-s); }
        if (P.y()<min.y()) { P.setY(min.y()+e); V.setY(V.y()*-s); }
        if (P.z()<min.z()) { P.setZ(min.z()+e); V.setZ(V.z()*-s); }

        if (P.x()>max.x()) { P.setX(max.x()-e); V.setX(V.x()*-s); }
        if (P.y()>max.y()) { P.setY(max.y()-e); V.setY(V.y()*-s); }
        if (P.z()>max.z()) { P.setZ(max.z()-e); V.setZ(V.z()*-s); }

        if ((max.x()-min.x())==0) { P.setX(max.x()); V.setX(0); A.setX(0);}
        if ((max.y()-min.y())==0) { P.setY(max.y()); V.setY(0);A.setY(0);}
        if ((max.z()-min.z())==0) { P.setZ(max.z()); V.setZ(0);A.setZ(0);}
    }

    void CollideSphere(Particle& o, float s);


};


class Particles
{
public:
    QVector<Particle> m_particles;

    void AddParticle(Particle p) {
        m_particles.append(p);
    }

    void Clear() {
        m_particles.clear();
    }


    void Move(float dt) {
        for (Particle& p: m_particles)
            p.Move(dt);
    }

    void ApplyForce(QVector3D F) {
        for (Particle& p: m_particles)
            p.ApplyForce(F);
    }

    void CollideSphere(float s);

    void ConstrainBox(QVector3D min, QVector3D max, float scale) {
        for (Particle& p: m_particles)
            p.ConstrainBox(min,max,scale);
    }


    Particles();
};

#endif // PARTICLES_H
