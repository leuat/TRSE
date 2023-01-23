#include "dialogeffects.h"
#include "ui_dialogeffects.h"

LuaScript* m_script = nullptr;
QString m_infoText = "", m_error="";
QString m_currentDir;
QByteArray m_charData;
QByteArray m_extraData;
QVector<int> m_screenData;
AbstractDemoEffect* m_effect = nullptr;
Compression m_compression;
LImage* m_maskImage;


DialogEffects::DialogEffects(QString file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEffects)
{
    ui->setupUi(this);
    m_file = file;
    m_effect = nullptr;
    m_screenData.clear();
    m_charData.clear();
}

DialogEffects::~DialogEffects()
{

    Abort();

    delete ui;
}

void DialogEffects::Abort()
{
    if (m_effect!=nullptr) {
        m_effect->m_abort = true;
        m_effect->quit();
        m_effect->wait();
        m_effect->terminate();
        m_effect->wait();
        m_effect->deleteLater();
        //        delete m_effect;
    }

}
static RayTracer m_rt;



static bool VerifyFjongParameters(lua_State* L, QString method) {
    BuiltInFunction f = Syntax::s.builtinFunctionsFjong[method.toLower()];
    int N = 0;
    //    qDebug() <<"HERE" << f.m_params.count();
    for (BuiltInFunction::Type t : f.m_params) {
        N++;
        if (t==BuiltInFunction::IGNOREPARAM)
            continue;
        if (t==BuiltInFunction::STRING) {
            if (!lua_isstring(L,N)) {
                m_error += "<br>Method '"+method+"' requires parameter "+QString::number(N) + " to be a string";
                return false;
            }
        }
        if (t==BuiltInFunction::NUMBER) {
            if (!lua_isnumber(L,N)) {
                m_error += "<br>Method '"+method+"' requires parameter "+QString::number(N) + " to be a number";
                return false;
            }
        }
    }

    return true;
}


void DialogEffects::Create()
{
    if (m_effect!=nullptr) {
        Abort();
        delete m_effect;
        return;
    }
    if (m_error!="") {
        ui->txtOutput->setText(m_error);

        return;
    }

    /*if (ui->comboBox->currentText()=="Twister")
        m_effect = new DemoEffectTwister(ui->glParams);
*/

    //  if (ui->comboBox->currentText()=="Raytracer") {
    m_effect = new DemoEffectRaytracer();
    if (m_script!=nullptr) {

        dynamic_cast<DemoEffectRaytracer*>(m_effect)->m_rt = &m_rt;
        dynamic_cast<DemoEffectRaytracer*>(m_effect)->m_script = m_script;
    }


    //    }
    m_effect->Initialize();
    connect(m_effect,SIGNAL(SignalImageUpdate()),this,SLOT(UpdateImage()));



    //    m_effect->FillToGUI();
    m_effect->start();
}


static bool ValidateNoParameters(QString p, lua_State *L, int num)
{
    int params = lua_gettop (L);
    if (params <num) {
        m_error += "<br>Method '"+p+"' requires "+QString::number(num)+" parameters, but has only "+QString::number(params);
        return false;
    }
    return true;
}



static int LuaSin(lua_State *L) {

    lua_pushnumber(L, sin(lua_tonumber(L, 1)));
    return 1;
}

static int LuaCos(lua_State *L) {
    lua_pushnumber(L, cos(lua_tonumber(L, 1)));
    return 1;
}

static int LuaSqrt(lua_State *L) {
    lua_pushnumber(L, sqrt(lua_tonumber(L, 1)));
    return 1;
}


static int AddLight(lua_State *L) {
    QVector3D dir = QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    QVector3D col = QVector3D(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7));
    int no = lua_tonumber(L,1);
    if (m_rt.m_globals.m_lights.count()<=no)
        m_rt.m_globals.m_lights.resize(no+1);

    //    qDebug() << m_rt.m_globals.m_lights.count();
    if (m_rt.m_globals.m_lights[no]==nullptr)
        m_rt.m_globals.m_lights[no] = new DirectionalLight(dir,col);

    m_rt.m_globals.m_lights[no]->m_color = col;
    m_rt.m_globals.m_lights[no]->m_direction = dir;
    return 1;
}


static int AddObjectRegular(lua_State *L)
{
    if (!VerifyFjongParameters(L,"AddObjectRegular"))
        return 0;

    int args = lua_gettop(L);
    QString object = QString::fromStdString(lua_tostring(L,1)).toLower();
    Material mat = Material(QVector3D(1,1,1),50,0.5, 0,0,"");
    QString name = lua_tostring(L,2);
    QString parent = lua_tostring(L,3);
    QString material = lua_tostring(L,4);
    RayObjectRegular3D* obj = nullptr;
    int N = 5;

    if (object=="torus_wire") {
        ValidateNoParameters("AddObject (torus)",L,N+4);
        obj = new RayObjectRegular3D();
        //        qDebug() << object << lua_tonumber(L,N);
        obj->GenerateTorus(lua_tonumber(L,N),lua_tonumber(L,N+1),
                           lua_tonumber(L,N+2),
                           lua_tonumber(L,N+3),true,lua_tonumber(L,N+4),lua_tonumber(L,N+5),lua_tonumber(L,N+6),lua_tonumber(L,N+7));


    }
    if (object=="torus_poly") {
        ValidateNoParameters("AddObject (torus)",L,N+4);
        obj = new RayObjectRegular3D();
        //        qDebug() << object << lua_tonumber(L,N);
        obj->GenerateTorus(lua_tonumber(L,N),lua_tonumber(L,N+1),
                           lua_tonumber(L,N+2),
                           lua_tonumber(L,N+3),false,lua_tonumber(L,N+4),lua_tonumber(L,N+5),lua_tonumber(L,N+6),lua_tonumber(L,N+7));

    }
    if (obj==nullptr) return 1;

    obj->m_name= name;
    if (parent=="") {
        obj->m_id = m_rt.m_objects.count();
        m_rt.m_objects.append(obj);
    }
    else {
        AbstractRayObject* aro = m_rt.Find(parent);
        if (aro==nullptr) {
            m_error +="<br>Error in AddObject: Could not find parent object '" + parent+  "'\n";
            return 0;
        }
        aro->m_children.append(obj);
    }
    return 1;
}

static int AddObject(lua_State *L)
{
    if (!VerifyFjongParameters(L,"AddObject"))
        return 0;

    //    qDebug();// << text;
    int args = lua_gettop(L);
    //  if (n!=5) {
    //       return 0;
    // }
    QString object = QString::fromStdString(lua_tostring(L,1)).toLower();



    Material mat = Material(QVector3D(1,1,1),50,0.5, 0,0,"");
    QString name = lua_tostring(L,2);
    QString parent = lua_tostring(L,3);
    QString material = lua_tostring(L,4);

    AbstractRayObject* obj = nullptr;
    int N = 5;
    if (object=="torus") {
        ValidateNoParameters("AddObject (torus)",L,N+4);
        obj =
                new RayObjectTorus(
                    QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2)) ,
                    QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),0),
                    QVector3D(0,1,0),
                    mat);

    }

    if (object=="cylinder") {
        obj =
                new RayObjectCylinder(
                    QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2)) ,
                    QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5)),
                    mat);

    }
    if (object=="sphere") {
        obj =
                new RayObjectSphere(
                    QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2)) ,
                    QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5)),
                    mat);



    }
    if (object=="genmesh") {
        QString type = lua_tostring(L,N);
        N++;
        obj =
                new RayObjectGenMesh(type,
                                     QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2)) ,
                                     QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5)),
                                     mat);
    }

    if (object=="plane") {
        obj =
                new RayObjectPlane(
                    QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2)) ,
                    QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5)),
                    mat);

    }


    if (object=="operation") {
        QString type = lua_tostring(L,N);
        float blend = lua_tonumber(L,N+1);

        AbstractRayObject* o1 = m_rt.Find(lua_tostring(L,N+2));
        AbstractRayObject* o2 = m_rt.Find(lua_tostring(L,N+3));
        obj =
                new RayObjectOperation(type, blend, o1, o2);

        m_rt.m_objects.removeAll(o1);
        m_rt.m_objects.removeAll(o2);
    }


    if (object=="perlin") {
        AbstractRayObject* o1 = m_rt.Find(lua_tostring(L,N));
        N++;
        QVector3D vals = QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2));
        obj =
                new RayObjectPerlin(vals,o1);
        obj->m_position = o1->m_position;
        obj->m_rotation = o1->m_rotation;
        obj->m_bbRadius = o1->m_bbRadius;
        m_rt.m_objects.removeAll(o1);
    }

    if (object=="holes") {
        AbstractRayObject* o1 = m_rt.Find(lua_tostring(L,N));
        N++;
        QVector3D vals = QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2));
        obj =
                new RayObjectHoles(vals,o1);
        obj->m_position = o1->m_position;
        obj->m_rotation = o1->m_rotation;
        obj->m_bbRadius = o1->m_bbRadius;
        m_rt.m_objects.removeAll(o1);
    }


    if (object=="mesh") {
        QString fn = m_currentDir+"/"+ lua_tostring(L,N);
        float meshscale = lua_tonumber(L,N+1);
        float invertN = lua_tonumber(L,N+2);
        N+=3;
        if (!QFile::exists(fn)) {
            m_error += "<br>Could not find 3d object file: "+fn;
            return 0;
        }

        QVector3D orgPos = QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2));

        mat.m_color = m_script->getVec(material+".color");

        mat.m_reflectivity = m_script->get<float>(material+".reflection");

        mat.m_shininess = m_script->get<float>(material+".shininess");

        mat.m_shininess_strength = m_script->get<float>(material+".shininess_intensity");


        if (m_script->lua_exists(material+".texture")) {
            mat.m_hasTexture = true;
            mat.m_texture.Load(m_currentDir + "/"+ m_script->get<QString>(material+".texture"));
        }

        m_rt.LoadMesh(fn, meshscale,orgPos, mat,name, invertN==1);
        return 0;

        //        QVector3D scale = QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5));
        //        QVector3D size = QVector3D(lua_tonumber(L,N+6),lua_tonumber(L,N+7),lua_tonumber(L,N+8));
        //obj = new RayObjectEmpty(orgPos);
        //obj->LoadMesh(fn, meshscale);


    }

    if (object=="box") {
        obj =
                new RayObjectBox(
                    QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2)) ,
                    QVector3D(lua_tonumber(L,N+6),lua_tonumber(L,N+7),lua_tonumber(L,N+8)),
                    QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5)),
                    mat);

        //        obj->m_flatten = false;

    }

    if (object=="triangleprism") {
        obj =
                new RayObjectTrianglePrism(
                    QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2)) ,
                    QVector3D(lua_tonumber(L,N+6),lua_tonumber(L,N+7),lua_tonumber(L,N+8)),
                    QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5)),
                    mat);

        //        obj->m_flatten = false;

    }
    if (object=="triangle") {
        auto o =
                new RayObjectTriangle();
        o->m_material = mat;
        o->m_pos[0] =   QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2));
        o->m_pos[1] =   QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5));
        o->m_pos[2] =   QVector3D(lua_tonumber(L,N+6),lua_tonumber(L,N+7),lua_tonumber(L,N+8));

        o->m_position = QVector3D(0,0,0);
        o->m_normal = QVector3D::crossProduct(o->m_pos[0]-o->m_pos[1],o->m_pos[2]-o->m_pos[1]).normalized();
        o->m_bbRadius = 10;
        //        obj->m_flatten = false;
        obj = o;

    }


    if (object=="char") {

        CharsetImage* charset = new CharsetImage(LColorList::C64);
        QString charName = lua_tostring(L,N);
        if (charName.toLower()=="rom") {
            charset->LoadCharset(":resources/character.rom",0);
        }
        else {
            QString fname = m_currentDir+"/"+QString(lua_tostring(L,N));
            if (!QFile::exists(fname)) {
                m_error += "<br>Could not open file: " + fname;
                return 0;
            }

            charset->LoadCharset(fname,lua_tonumber(L,N+11));
        }
        N++;

        int chr = lua_tonumber(L, N);
        N++;

        QVector3D orgPos = QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2));
        QVector3D scale = QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5));
        QVector3D size = QVector3D(lua_tonumber(L,N+6),lua_tonumber(L,N+7),lua_tonumber(L,N+8));
        obj = new RayObjectEmpty(orgPos);
        obj->m_flatten = false;
        //        obj = new RayObjectUnion(orgPos);
        PixelChar& pc = charset->m_data[chr];
        for (int y=0;y<8;y++)
            for (int x=0;x<8;x++) {
                if (((pc.p[y]>>x) & 1)==1) {
                    float xx = (x-4+0.5)*scale.x();
                    float yy = (y-4+0.5)*scale.y();
                    float s = 0.5;
                    QVector3D pos = QVector3D(0,yy,xx);

                    AbstractRayObject* aro = new RayObjectBox(pos,QVector3D(0,1,0),
                                                              size, mat);
                    aro->m_name = name + "_"+QString::number(x) +"_"+QString::number(y);
                    //                  qDebug() << aro->m_name;
                    //((RayObjectUnion*)obj)->m_objects.append(aro);
                    obj->m_children.append(aro);
                }
            }




        /*       obj =
                    new RayObjectBox(
                        QVector3D(lua_tonumber(L,4),lua_tonumber(L,5),lua_tonumber(L,6)) ,
                        QVector3D(0,1,0),
                        QVector3D(lua_tonumber(L,7),lua_tonumber(L,8),lua_tonumber(L,9)),
                        mat);
*/
        delete charset;

    }

    if (object=="image") {
        QImage img;
        QString fname = m_currentDir+"/"+QString(lua_tostring(L,N));
        if (!QFile::exists(fname)) {
            m_error += "<br>Could not open image file: " + fname;
            return 0;
        }
        img.load(fname);
        N++;

        int boxwidth = lua_tonumber(L, N);
        N++;
        int boxheight = lua_tonumber(L, N);
        N++;

        QVector3D orgPos = QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2));
        QVector3D scale = QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5));
        QVector3D size = QVector3D(lua_tonumber(L,N+6),lua_tonumber(L,N+7),lua_tonumber(L,N+8));
        obj = new RayObjectEmpty(orgPos);
        obj->m_flatten = false;
        //        obj = new RayObjectUnion(orgPos);

        for (int y=0;y<boxwidth;y++)
            for (int x=0;x<boxheight;x++) {
                int xp = x/(float)boxwidth*img.width();
                int yp = y/(float)boxwidth*img.height();
                QVector3D c = Util::fromColor(img.pixelColor(xp,yp));

                if (c.length()!=0) {
                    float xx = (x-boxwidth/2+0.5)*scale.x();
                    float yy = (y-boxheight/2+0.5)*scale.y();
                    QVector3D pos = QVector3D(0,yy,xx);

                    AbstractRayObject* aro = new RayObjectBox(pos,QVector3D(0,1,0),
                                                              size, mat);
                    aro->m_name = name + "_"+QString::number(x) +"_"+QString::number(y);
                    //                  qDebug() << aro->m_name;
                    //((RayObjectUnion*)obj)->m_objects.append(aro);
                    obj->m_children.append(aro);
                }
            }




    }


    if (obj!=nullptr) {
        obj->m_name= name;
        if (parent=="") {
            obj->m_id = m_rt.m_objects.count();
            m_rt.m_objects.append(obj);
        }
        else {
            AbstractRayObject* aro = m_rt.Find(parent);
            if (aro==nullptr) {
                m_error +="<br>Error in AddObject: Could not find parent object '" + parent+  "'\n";
                return 0;
            }
            aro->m_children.append(obj);
        }



        mat.m_color = m_script->getVec(material+".color");

        mat.m_reflectivity = m_script->get<float>(material+".reflection");

        mat.m_shininess = m_script->get<float>(material+".shininess");

        mat.m_shininess_strength = m_script->get<float>(material+".shininess_intensity");

        if (m_script->lua_exists(material+".texture")) {
            mat.m_hasTexture = true;
            mat.m_texture.Load(m_currentDir + "/"+ m_script->get<QString>(material+".texture"));
        }
        if (m_script->lua_exists(material+".uv_scale")) {
            mat.m_uvScale = m_script->getVec(material+".uv_scale");
        }
        if (m_script->lua_exists(material+".uv_rotation")) {
            mat.m_uv_rotation = m_script->get<float>(material+".uv_rotation");
        }
        if (m_script->lua_exists(material+".uvtype")) {

            if (m_script->get<QString>(material+".uvtype")=="sphere_map") {
                mat.m_type = Material::Type::UV_SPHERE_MAP;
                //    qDebug() << "HERE";
            }
        }

        if (m_script->lua_exists(material+".drawuvcoord")) {
            mat.m_drawuvcoord = true;
        }
        /*        if (m_script->lua_exists(material+".uv_type")) {
            int t = (int)m_script->get<float>(material+".uv_type");
            mat.m_type = Material::Type::UV_CUBE;
            if (t==1)
                mat.m_type = Material::Type::UV_SPHERE_MAP;
            if (t==2)
                mat.m_type = Material::Type::UV_RAW;

        }*/


        mat.m_shininess_strength = m_script->get<float>(material+".shininess_intensity");
        if (m_script->lua_exists(material+".checkerboard")) {
            mat.m_checkerBoard.setX(m_script->get<float>(material+".checkerboard"));
        }
        if (m_script->lua_exists(material+".checkerboard_color")) {
            mat.m_checkerBoard_color = m_script->getVec(material+".checkerboard_color");
        }
        if (m_script->lua_exists(material+".light_type")) {
            mat.m_lightningType = m_script->get<float>(material+".light_type");
        }
        if (m_script->lua_exists(material+".uv_shift")) {
            mat.m_uvShift = m_script->getVec(material+".uv_shift");
        }
        obj->SetMaterial(mat);

    }
    else m_error +="<br>Error in AddObject: Unkown type '" + object+  "'";

    return 0;


}


static int SetRotation(lua_State *L)
{
    //    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in SetRotation : Could not find object '" + name;;
        return 0;
    }

    aro->SetRotation(QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)));
    return 0;
}

static int Invert(lua_State *L)
{
    //    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in Invert : Could not find object '" + name;;
        return 0;
    }
    aro->m_inverted = !aro->m_inverted;
    //    aro->SetRotation(QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)));
    return 0;
}

static int Save3DObject(lua_State *L)
{
    //    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in SetRotation : Could not find object '" + name;;
        return 0;
    }
    QString fname = m_currentDir+"/"+lua_tostring(L,2);

    aro->Save6502(fname,lua_tonumber(L,3));
    return 0;
}

static int SetID(lua_State *L)
{
    //    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in SetRotation : Could not find object '" + name;;
        return 0;
    }
    aro->m_id = lua_tonumber(L,2);
    return 0;
}


static int SetQuatAxisAngle(lua_State *L)
{
    //    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in SetRotation : Could not find object '" + name+ "'";
        return 0;
    }

    aro->SetQuatAxisAngle(QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)),lua_tonumber(L,5));
    return 0;
}

static int SetPosition(lua_State *L)
{
    //    int n = lua_gettop(L);
    if (!VerifyFjongParameters(L,"SetPosition"))
        return 0;

    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in SetPosition : Could not find object '" + name+ "'";
        return 0;
    }

    aro->m_position = (QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)));
    return 0;
}
static int SetScale(lua_State *L)
{
    //    int n = lua_gettop(L);
    if (!VerifyFjongParameters(L,"SetScale"))
        return 0;

    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in SetPosition : Could not find object '" + name+ "'";
        return 0;
    }

    aro->m_scale = (QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)));
    return 0;
}

static int RemoveObject(lua_State *L)
{
    //    int n = lua_gettop(L);
    if (!VerifyFjongParameters(L,"RemoveObject"))
        return 0;

    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        return 0;
    }
    m_rt.m_objects.removeAll(aro);

    return 0;
}


static int DeleteFile(lua_State *L)
{
    //    int n = lua_gettop(L);

    QString fname = m_currentDir+"/"+lua_tostring(L,1);
    if (QFile::exists(fname))
        QFile::remove(fname);
    return 0;
}


static int AddPosition(lua_State *L)
{
    //    int n = lua_gettop(L);
    if (!VerifyFjongParameters(L,"AddPosition"))
        return 0;

    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        //m_error +="Error in AddPosition : Could not find object '" + name+ "'\n";
        return 0;
    }

    aro->m_position += (QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)));
    return 0;
}

static int SetUVShift(lua_State *L)
{
    //    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in SetUVShift : Could not find object '" + name+ "'";
        return 0;
    }

    aro->m_uvShift= (QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),0));
    aro->m_material.m_uvShift = aro->m_uvShift;
    return 0;
}

static int SetY(lua_State *L)
{
    //    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in SetRotation : Could not find object '" + name+ "'";
        return 0;
    }

    aro->m_position.setY(lua_tonumber(L,2));
    return 0;
}



int DialogEffects::Message(lua_State *L)
{
    if (!VerifyFjongParameters(L,"Message"))
        return 0;

    m_infoText=QString(lua_tostring(L,1)) +"\n"+ m_infoText;
    //    m_infoText+="\n";
    return 0;
}

static int ClearObjects(lua_State *L) {
    m_rt.m_objects.clear();
    return 0;
}

static int AddScreen(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddScreen"))
        return 0;

    if (m_effect!=nullptr) {
        QByteArray ba;
        m_compression.AddScreen(ba, m_effect->m_img,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));//, lua_tonumber(L,5),lua_tonumber(L,6));
        for (char c:ba)
            m_screenData.append(c);
    }

    return 0;
}

static int AddC64FullScreen(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddC64FullScreen"))
        return 0;

    if (m_effect!=nullptr) {
        C64FullScreenChar* img = dynamic_cast<C64FullScreenChar*>(m_effect->m_mc);
        if (img==nullptr)
            return 0;

        //        if (img->m_items.count()==0)
        //          return 0;

        C64Screen* screen = dynamic_cast<C64Screen*>((img->m_items)[img->m_current].get());
        if (screen==nullptr)
            return 0;


        for (char c : screen->m_rawData)
            m_screenData.append(c);


        //QByteArray ba;
        //      qDebug() << "Image size : " << img->m_height << img->m_width;
        /*        for (int y=0;y<img->m_height;y++)
            for (int x=0;x<img->m_width;x++) {

                m_screenData.append(img->getPixel(x,y));
            }
//        m_compression.AddScreen(ba, m_effect->m_img,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));//, lua_tonumber(L,5),lua_tonumber(L,6));
//        for (char c:ba)
  //          m_screenData.append(c);

  */
    }

    return 0;
}

static int Translate(lua_State* L) {
    if (!VerifyFjongParameters(L,"Translate"))
        return 0;

    if (m_effect!=nullptr) {
        m_effect->m_mc->Transform(lua_tonumber(L,1),lua_tonumber(L,2));
        ///m_compression.AddScreen(ba, m_effect->m_img,lua_tonumber(L,1),lua_tonumber(L,2));
    }

    return 0;
}

static int AddCharsetScreen(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddCharsetScreen"))
        return 0;


    CharsetImage* charset = new CharsetImage(LColorList::C64);
    int N = 3;
    QString charName = lua_tostring(L,N);
    if (charName.toLower()=="rom") {
        charset->LoadCharset(":resources/character.rom",0);
    }
    else {
        QString fname = m_currentDir+"/"+QString(lua_tostring(L,N));
        if (!QFile::exists(fname)) {
            m_error += "<br>Could not open file: " + fname;
            return 0;
        }

        charset->LoadCharset(fname,0);
    }

    if (m_effect!=nullptr) {
        QByteArray ba = Util::toQByteArray(m_screenData);
        //m_compression.AddScreen(ba, m_effect->m_img,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));//, lua_tonumber(L,5),lua_tonumber(L,6));
        m_compression.AddCharsetScreen(ba, m_effect->m_img, charset, lua_tonumber(L,1),lua_tonumber(L,2));
    }

    return 0;
}

static int AddToData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddToData"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddToDataX(m_charData, *((MultiColorImage*)m_effect->m_mc) ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));

    return 0;
}

static int AddVZ200Data(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddVZ200ToData"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddToVZ200Data(m_charData, *m_effect->m_mc ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));

    return 0;
}
static int Add4PixelData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddVZ200ToData"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddTo4PixelData(m_charData, *m_effect->m_mc ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));

    return 0;
}
static int AddSpecialC64bitmapModeToData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddSpecialC64bitmapModeToData"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddSpecialC64bitmapModeToData(m_charData, *m_effect->m_mc ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));

    return 0;
}
static int AddBBCMode5LineToData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddBBCMode5LineToData"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddToDataBBCMode5(m_charData, m_effect->m_mc ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));

    return 0;
}

static int AddToDataVGA(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddToData"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddToDataVGA(m_charData, *((LImageQImage*)m_effect->m_mc) ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));

    return 0;
}

static int AddBitplaneToData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddBitplaneTodata"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddBitplaneToData(m_charData, *((MultiColorImage*)(m_effect->m_mc)) ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4), lua_tonumber(L,5), lua_tonumber(L,6));

    return 0;
}

static int AddAmstradCPCToData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddAmstradCPCToData"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddAmstradCPCToData(m_charData, m_effect->m_mc ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));

    return 0;
}

static int AddAtariBitplaneToData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddAtariBitplaneTodata"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddAtariBitplaneToData(m_charData, *((MultiColorImage*)(m_effect->m_mc)) ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));

    return 0;
}

static int AddAtariSingleBitplaneToData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddAtariBitplaneTodata"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddSingleAtariBitplaneToData(m_charData, *((MultiColorImage*)(m_effect->m_mc)) ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4),lua_tonumber(L,5));

    return 0;
}


static int AddGameboyData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddBitplaneTodata"))
        return 0;

    if (m_effect!=nullptr)
        m_compression.AddGameboyData(m_charData, *((MultiColorImage*)(m_effect->m_mc)) ,lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4));

    return 0;
}


static int CompressCharset(lua_State* L) {
    // 0, 40, 13, 25
    if (!VerifyFjongParameters(L,"CompressCharset"))
        return 0;

    int noChars;
    MultiColorImage* mc = dynamic_cast<MultiColorImage*>(m_effect->m_mc);
    if (mc==nullptr)
        return 0;

    int type =0;
    int args = lua_gettop(L);
    if (args>=7)
        type = lua_tonumber(L,7);

    mc->CompressAndSave(m_charData, m_screenData, lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3),lua_tonumber(L,4),noChars,lua_tonumber(L,5),  lua_tonumber(L,6), type,false);
    m_infoText+="Compressed chars: " + QString::number(noChars) + "\n";
    return 0;
}



static int SaveSinusScrollerData(lua_State* L) {
    if (!VerifyFjongParameters(L,"SaveSinusScrollerData"))
        return 0;


    int type =0;
    int args = lua_gettop(L);
    if (m_effect == nullptr || m_effect->m_mc==nullptr) {
        qDebug() << "NULL POINTERS";
        return 0;
    }
    MultiColorImage* mc = dynamic_cast<MultiColorImage*>(m_effect->m_mc);
    if (mc==nullptr)
        return 0;


    Compression::SaveSinusScrollerData(mc,lua_tonumber(L,1),lua_tonumber(L,2),m_currentDir+"/"+lua_tostring(L,3));
    //    mc->CompressAndSave(m_charData, m_screenData, lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3),lua_tonumber(L,4),noChars,lua_tonumber(L,5),  lua_tonumber(L,6), type,false);
    return 0;
}


static int SaveScreenAndCharset(lua_State* L) {
    if (!VerifyFjongParameters(L,"SaveScreenAndCharset"))
        return 0;

    QFile f(m_currentDir+"/"+ lua_tostring(L,2));
    f.open(QFile::WriteOnly);
    f.write(m_charData);
    f.close();
    m_charData.clear();

    QFile f2(m_currentDir+"/"+lua_tostring(L,1));
    f2.open(QFile::WriteOnly);
    QByteArray d = Util::toQByteArray(m_screenData);
    f2.write(d);
    f2.close();
    m_screenData.clear();
    return 0;

}


static int ExportDiffAsUnrolledBitmap(lua_State* L) {
    QString file = m_currentDir+"/"+ lua_tostring(L,1);
    QString name = lua_tostring(L,3);
    QString bg = m_currentDir+"/"+ lua_tostring(L,2);
    QString waitFunc = lua_tostring(L,4);
    m_effect->ExportDiffAsUnrolledBitmap6502(file,bg, name,waitFunc,lua_tonumber(L,5),
                                             lua_tonumber(L,6),
                                             lua_tonumber(L,7),
                                             lua_tonumber(L,8),
                                             lua_tonumber(L,9));
    // void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502(QString file, QString name, int base, int xp, int yp, int w, int h) {
    return 0;
}

static int ExportDiffAsUnrolledBitmapIn(lua_State* L) {
    QString file = m_currentDir+"/"+ lua_tostring(L,1);
    QString name = lua_tostring(L,3);
    QString bg = m_currentDir+"/"+ lua_tostring(L,2);
    QString waitFunc = lua_tostring(L,4);
    m_effect->ExportDiffAsUnrolledBitmap6502In(file,bg, name,waitFunc,lua_tonumber(L,5),
                                               lua_tonumber(L,6),
                                               lua_tonumber(L,7),
                                               lua_tonumber(L,8),
                                               lua_tonumber(L,9));
    // void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502(QString file, QString name, int base, int xp, int yp, int w, int h) {
    return 0;
}

static int ExportDiffAsUnrolledBitmapColorOut(lua_State* L) {
    QString file = m_currentDir+"/"+ lua_tostring(L,1);
    QString name = lua_tostring(L,3);
    QString bg = m_currentDir+"/"+ lua_tostring(L,2);
    QString waitFunc = lua_tostring(L,4);
    m_effect->ExportDiffAsUnrolledBitmap6502ColorOut(file,bg, name,waitFunc,lua_tonumber(L,5),
                                                     lua_tonumber(L,6),
                                                     lua_tonumber(L,7),
                                                     lua_tonumber(L,8),
                                                     lua_tonumber(L,9),lua_tonumber(L,10),lua_tonumber(L,11));
    // void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502(QString file, QString name, int base, int xp, int yp, int w, int h) {
    return 0;
}

static int ExportDiffAsUnrolledBitmapColorIn(lua_State* L) {
    QString file = m_currentDir+"/"+ lua_tostring(L,1);
    QString name = lua_tostring(L,3);
    QString bg = m_currentDir+"/"+ lua_tostring(L,2);
    QString waitFunc = lua_tostring(L,4);
    m_effect->ExportDiffAsUnrolledBitmap6502ColorIn(file,bg, name,waitFunc,lua_tonumber(L,5),
                                                    lua_tonumber(L,6),
                                                    lua_tonumber(L,7),
                                                    lua_tonumber(L,8),
                                                    lua_tonumber(L,9),lua_tonumber(L,10),lua_tonumber(L,11));
    // void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502(QString file, QString name, int base, int xp, int yp, int w, int h) {
    return 0;
}

static int UpdateScreenDataFrame(lua_State* L) {
    m_effect->UpdateScreenDataFrame(m_screenData,lua_tonumber(L,1),
                                    lua_tonumber(L,2),
                                    lua_tonumber(L,3),
                                    lua_tonumber(L,4),
                                    lua_tonumber(L,5));
    // void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502(QString file, QString name, int base, int xp, int yp, int w, int h) {
    return 0;
}


static int ExportDiffAsUnrolledBitmapColorInAddress(lua_State* L) {
    QString file = m_currentDir+"/"+ lua_tostring(L,1);
    QString name = lua_tostring(L,3);
    //    QString bg = m_currentDir+"/"+ lua_tostring(L,2);
    QString waitFunc = lua_tostring(L,4);
    m_effect->ExportDiffAsUnrolledBitmap6502ColorInAddress(file,lua_tonumber(L,2), name,waitFunc,lua_tonumber(L,5),
                                                           lua_tonumber(L,6),
                                                           lua_tonumber(L,7),
                                                           lua_tonumber(L,8),
                                                           lua_tonumber(L,9),lua_tonumber(L,10));
    // void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502(QString file, QString name, int base, int xp, int yp, int w, int h) {
    return 0;
}

static int Save2DInfo(lua_State* L) {
    QString file = m_currentDir+"/"+ lua_tostring(L,1);
    QString fileUnroll = m_currentDir+"/"+ lua_tostring(L,4);
    int base = lua_tonumber(L,2);
    int maxx = lua_tonumber(L,3);
    QVector<QPoint> killList;
    m_rt.Compile2DList(file,base,maxx, killList, m_effect->m_post,fileUnroll ,m_maskImage,lua_tonumber(L,5));

    // MultiColorImage mc(m_effect->m_mc->m_colorList.m_type);
    //  mc.m_data[0].C = m_charData;
    // m_effect->m_mc
    int bg = m_effect->m_mc->getBackground();
    for (QPoint k: killList)
        m_effect->m_mc->setPixel(k.x(),k.y(),bg);

    return 0;
}


static int AddProjectedLineList(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddProjected2DLineRenderList"))
        return 0;

    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in AddProjected2DLineRenderList : Could not find object '" + name;;
        return 0;
    }
    m_charData.append(aro->getProjectedLineList());
    //    Util::SaveByteArray(m_charData,m_currentDir+"/"+ lua_tostring(L,1));
    //  m_charData.clear();
    return 0;
}


static int AddProjected2DCoordinatesToData(lua_State* L) {
    if (!VerifyFjongParameters(L,"AddProjected2DCoordinatesToData"))
        return 0;

    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="<br>Error in AddProjected2DCoordinatesToData : Could not find object '" + name;;
        return 0;
    }
    m_charData.append(aro->getProjected8bitData());
    //    Util::SaveByteArray(m_charData,m_currentDir+"/"+ lua_tostring(L,1));
    //  m_charData.clear();
    return 0;
}

static int SaveData(lua_State* L) {
    if (!VerifyFjongParameters(L,"SaveData"))
        return 0;

    Util::SaveByteArray(m_charData,m_currentDir+"/"+ lua_tostring(L,1));
    m_charData.clear();
    return 0;
}
static int SaveExtraData(lua_State* L) {
    if (!VerifyFjongParameters(L,"SaveExtraData"))
        return 0;

    Util::SaveByteArray(m_extraData,m_currentDir+"/"+ lua_tostring(L,1));
    m_extraData.clear();
    return 0;
}

static int SaveDataScreen(lua_State* L) {
    if (!VerifyFjongParameters(L,"SaveData"))
        return 0;

    QFile f(m_currentDir+"/"+ lua_tostring(L,1));
    f.open(QFile::WriteOnly);

    QByteArray ba = Util::toQByteArray(m_screenData);

    f.write(ba);
    f.close();
    m_screenData.clear();
    return 0;
}

static int SaveCompressedSpriteData(lua_State* L) {

    m_compression.SaveCompressedSpriteData(m_charData, m_currentDir+"/"+ lua_tostring(L,1), m_currentDir+"/"+ lua_tostring(L,2), lua_tonumber(L,3),lua_tonumber(L,4));
    m_charData.clear();
    return 0;
}


static int SaveMulticolorImage(lua_State* L) {
    QString fname = m_currentDir+"/"+  lua_tostring(L,1);
    if (QFile::exists(fname))
        QFile::remove(fname);
    QFile f(fname);
    //    f.open(QFile::WriteOnly);
    //    exit(1);
    m_effect->m_mc->setMultiColor(false);
    m_effect->m_mc->m_exportParams["StartX"]=0;
    m_effect->m_mc->m_exportParams["StartY"]=0;
    m_effect->m_mc->m_exportParams["EndX"]=40;
    m_effect->m_mc->m_exportParams["EndY"]=25;

    dynamic_cast<MultiColorImage*>(m_effect->m_mc)->ForceExportBin(f);
    //    f.open()
    //  m_effect->m_mc->SaveBin(f);
    m_charData.clear();
    //  f.close();
    return 0;
}

static int SaveKoalaImage(lua_State* L) {
    QString fname = m_currentDir+"/"+  lua_tostring(L,1);
    if (QFile::exists(fname))
        QFile::remove(fname);
    QFile f(fname);
    f.open(QFile::WriteOnly);
    //    exit(1);
    m_effect->m_mc->setMultiColor(false);
    m_effect->m_mc->m_exportParams["StartX"]=0;
    m_effect->m_mc->m_exportParams["StartY"]=0;
    m_effect->m_mc->m_exportParams["EndX"]=40;
    m_effect->m_mc->m_exportParams["EndY"]=25;

    dynamic_cast<MultiColorImage*>(m_effect->m_mc)->ExportKoa(f);
    //    f.open()
    //  m_effect->m_mc->SaveBin(f);
    m_charData.clear();
    //  f.close();
    return 0;
}

static int SaveImage(lua_State* L) {
    if (!VerifyFjongParameters(L,"SaveImage"))
        return 0;

    QString fname = m_currentDir+"/"+  lua_tostring(L,1);
    if (QFile::exists(fname))
        QFile::remove(fname);


    if (fname.toLower().endsWith(".png") ||fname.toLower().endsWith(".jpg") )
        m_effect->m_img.save(fname);
    if (fname.toLower().endsWith(".bin"))
        m_effect->SaveImageBin(fname);
    if (fname.toLower().endsWith(".flf")) {
        LImageIO::Save(fname,m_effect->m_mc);
    }

    //    f.open()
    //  m_effect->m_mc->SaveBin(f);
    //    m_charData.clear();
    //  f.close();
    return 0;
}

static int AddParticle(lua_State* L) {
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_infoText+="Could not find object: " + name;
        return 0;
    }

    Particle p(aro->m_position,QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)),lua_tonumber(L,5),lua_tonumber(L,6),m_rt.FindID(aro));
    m_rt.m_particles.AddParticle(p);

    return 0;
}

static int MoveParticles(lua_State* L) {
    m_rt.m_particles.Move(lua_tonumber(L,1));
    m_rt.SetParticles();
    return 0;
}

static int ConstrainParticlesBox(lua_State* L) {
    QVector3D min = QVector3D(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3));
    QVector3D max = QVector3D(lua_tonumber(L,4),lua_tonumber(L,5),lua_tonumber(L,6));
    m_rt.m_particles.ConstrainBox(min,max, lua_tonumber(L,7));
    return 0;
}

static int ApplyForce(lua_State* L) {
    QVector3D F = QVector3D(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3));
    m_rt.m_particles.ApplyForce(F);
    return 0;
}



static int CollideSphere(lua_State* L) {
    m_rt.m_particles.CollideSphere(lua_tonumber(L,1));
    return 0;
}


static int AddBinaryScreen(lua_State* L) {
    m_rt.m_particles.CollideSphere(lua_tonumber(L,1));
    return 0;
}

static int AddScreenPetscii(lua_State* L) {
    m_compression.AddPetsciiScreen(m_charData, m_effect->m_img);
    //    m_rt.m_particles.CollideSphere(lua_tonumber(L,1));
    m_infoText+="Added petscii screen";
    return 0;
}

static int AddScreenBinary(lua_State* L) {

    m_compression.AddBinaryScreen(m_charData, m_effect->m_img);
    //    m_rt.m_particles.CollideSphere(lua_tonumber(L,1));
    return 0;
}

/*lua_register(m_script->L, "AddParticle", AddParticle);
lua_register(m_script->L, "MoveParticles", MoveParticles);
lua_register(m_script->L, "ConstrainParticlesBox", ConstrainParticlesBox);
lua_register(m_script->L, "ApplyForce", ApplyForce);

*/


static int OptimizeScreenAndCharset(lua_State* L) {
    if (!VerifyFjongParameters(L,"OptimizeScreenAndCharset"))
        return 0;

    QByteArray cOut;
    QVector<int> sOut;
    int type = 0;
    int scale = 1;
    //    if (lua_isuserdata(L, 5))
    type = lua_tonumber(L,5);


    if (dynamic_cast<LImageGamboy*>(m_effect->m_mc)!=nullptr) {
        scale=2;
        m_compression.OptimizeScreenAndCharsetGB(m_screenData, m_charData, sOut, cOut,  lua_tonumber(L,1), lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4),type,m_effect->m_mc->m_colorList,m_effect->m_mc->m_bitMask);
    }
    else
        m_compression.OptimizeScreenAndCharset(m_screenData, m_charData, sOut, cOut,  lua_tonumber(L,1), lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4),type,m_effect->m_mc->m_colorList,m_effect->m_mc->m_bitMask);
    //    m_charData.clear();
    m_charData = cOut;
    m_infoText="Total no. chars : "+QString::number(m_charData.length()/(8*scale)) +"\n"+ m_infoText;

    m_screenData = sOut;
    return 0;
}

static int CompressScreenAndCharset(lua_State* L) {
    if (!VerifyFjongParameters(L,"CompressScreenAndCharset"))
        return 0;

    QByteArray cOut;
    QVector<int> sOut;
    int scale = 1;
    m_compression.CompressScreenAndCharset(m_screenData, m_charData, sOut, cOut,  lua_tonumber(L,1), lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4),m_effect->m_mc->m_bitMask);
    //    m_charData.clear();
    m_charData = cOut;
    m_infoText="Total no. chars : "+QString::number(m_charData.length()/(8*scale)) +"\n"+ m_infoText;

    m_screenData = sOut;
    return 0;
}


static int DrawLine(lua_State* L) {

    QPainter painter;
    painter.begin(&m_effect->m_img);
    painter.setPen(QPen(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)), lua_tonumber(L,8), Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    painter.drawLine(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    painter.end();
    painter.begin(&m_effect->m_post);
    painter.setPen(QPen(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)), lua_tonumber(L,8), Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    painter.drawLine(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    painter.end();
    //    m_effect->m_img.fill((Qt::red));
    return 0;
}

static int DrawRect(lua_State* L) {

    QPainter painter;
    painter.begin(&m_effect->m_img);
    painter.setPen(QPen(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)), lua_tonumber(L,8), Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    painter.setBrush(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)));
    painter.drawRect(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    painter.end();
    painter.begin(&m_effect->m_post);
    painter.setPen(QPen(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)), lua_tonumber(L,8), Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    painter.setBrush(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)));
    painter.drawRect(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    painter.end();
    //    m_effect->m_img.fill((Qt::red));
    return 0;
}


static int DrawCircle(lua_State* L) {

    QPainter painter;
    painter.begin(&m_effect->m_img);
    painter.setPen(QPen(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)), lua_tonumber(L,8), Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin));
    painter.setBrush(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)));
    painter.drawEllipse(lua_tonumber(L,1)-lua_tonumber(L,3)/(float)2,
                        lua_tonumber(L,2)-lua_tonumber(L,4)/(float)2,
                        lua_tonumber(L,3),lua_tonumber(L,4));
    painter.end();
    painter.begin(&m_effect->m_post);
    painter.setPen(QPen(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)), lua_tonumber(L,8), Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin));
    painter.setBrush(QColor(lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7)));
    painter.drawEllipse(lua_tonumber(L,1)-lua_tonumber(L,3)/(float)2,
                        lua_tonumber(L,2)-lua_tonumber(L,4)/(float)2,
                        lua_tonumber(L,3),lua_tonumber(L,4));
    painter.end();
    //    m_effect->m_img.fill((Qt::red));
    return 0;
}


static void DrawDiamond(QPainter& p, float x, float y, float w, float h, QColor c1, QColor c2) {


    QPen p1 = QPen(c1, 1, Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin);
    QPen p2 = QPen(c2, 1, Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin);
    QBrush b(c1, Qt::SolidPattern);
    QBrush b2(c2, Qt::SolidPattern);


    p.setPen(p1);
    p.setBrush(b);
/*    QPointF points[4] = {
        QPointF((x+w/2.0), y),
        QPointF((x+w), y+h/2.0),
        QPointF((x+w/2.0), y+h),
        QPointF((x), y+h/2.0),

    };
    p.drawPolygon(points,4);*/
        QPointF points[4] = {
            QPointF((x+w/2.0), y),
            QPointF((x+w), y+h/2.0),
            QPointF((x+w/2.0), y+h),
//            QPointF((x), y+h/2.0),

        };
        p.drawPolygon(points,3);
        p.setPen(p2);
        p.setBrush(b2);

        QPointF points2[4] = {
            QPointF((x+w/2.0), y),
            QPointF((x), y+h/2.0),
            QPointF((x+w/2.0), y+h),
//            QPointF((x), y+h/2.0),

        };
        p.drawPolygon(points2,3);

}

static void DrawHalfDiamond(QPainter& p, float x, float y, float w, float h, int top, int bottom) {

    QPointF points[4] = {
        QPointF((x+w/2.0), y),
        QPointF((x+w), y+h/2.0),
//        QPointF((x), y+h/2.0),
        QPointF((x+w/2.0), y+h),
//        QPointF((x), y+h/2.0),

    };
    p.drawPolygon(points,3);
/*
    if (top!=0)  {
        QPointF top[4] = {
            QPointF(x, y),
            QPointF((x+w), y),
            QPointF((x+w), y+h/2),
            QPointF((x), y+h/2),

        };
        p.drawPolygon(top,4);
    }
    if (bottom!=0)  {
        QPointF top[4] = {
            QPointF(x, y+h/2),
            QPointF((x+w), y+h/2),
            QPointF((x+w), y+h),
            QPointF((x), y+h),

        };
//        p.drawPolygon(top,4);

    }
    */
}




static int DrawIsometricFont(lua_State* L) {

    CharsetImage* img = (CharsetImage*)LImageIO::Load( m_currentDir +  "/"+lua_tostring(L,1));


    QPainter painter;
    painter.begin(&m_effect->m_img);
    QColor c1 = QColor(lua_tonumber(L,9),lua_tonumber(L,10),lua_tonumber(L,11));
    QColor c2 = QColor(lua_tonumber(L,12),lua_tonumber(L,13),lua_tonumber(L,14));
//    int startx = lua_tonumber(L,12);
  //  int endx = lua_tonumber(L,13);

    int ch = lua_tonumber(L,2);
    float sx = lua_tonumber(L,3);
    float sy = lua_tonumber(L,4);
    float sdx = lua_tonumber(L,7);
    float sdy = lua_tonumber(L,8);
    float orgx = lua_tonumber(L,5);
    float orgy = lua_tonumber(L,6);
    float xx = 0;
    float yy = 0;
    PixelChar pc = img->m_data[ch];
    float dy=0;
    int startx = 0;
    float dx = startx*sdx/2;
    int endx = 8;
    for (int y=0;y<8;y++) {
        xx=0;
        dy=sdy*4;
        for (int x=startx;x<endx;x++) {
            //           qDebug() << ch%40 << (ch/40.0)*40;
            //            PixelChar pc = img->getPixelChar(0,0);

//            auto down = 1;
  //          if (y<7) down = pc.get(x,y+1,0b1);
            auto left = 0;
            if (x>0 )
                left = pc.get(x-1,y,0b1);

            QColor pc1 = c1;
            QColor pc2 = c1;
            if (left==0 && x<3) pc2=c2;
                    //            if (y<3 && x>1) pc2=pc1;


            if (pc.get(x,y,0b1)!=0)
                DrawDiamond(painter,xx+dx+orgx,orgy+yy+dy,sx,sy,pc1,pc2);

            xx+=sdx/2;
            dy-=sdx/2;

        }
        yy+=sdx/2;
        dx+=sdx/2;
    }
    painter.end();
    return 0;
}

static int DrawTriangle(lua_State* L) {

    QPainter painter;
    painter.begin(&m_effect->m_img);
    painter.setPen(QPen(QColor(lua_tonumber(L,7),lua_tonumber(L,8),lua_tonumber(L,9)), lua_tonumber(L,10), Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin));
    painter.setBrush(QColor(lua_tonumber(L,7),lua_tonumber(L,8),lua_tonumber(L,9)));
    QPointF points[3] = {
        QPointF(lua_tonumber(L,1),lua_tonumber(L,2)),
        QPointF(lua_tonumber(L,3),lua_tonumber(L,4)),
        QPointF(lua_tonumber(L,5),lua_tonumber(L,6)),

    };
    painter.drawPolygon(points,3);
    painter.end();
    //    m_effect->m_img.fill((Qt::red));
    return 0;
}

static int DrawQuad(lua_State* L) {

    QPainter painter;
    painter.begin(&m_effect->m_img);
    painter.setPen(QPen(QColor(lua_tonumber(L,9),lua_tonumber(L,10),lua_tonumber(L,11)), lua_tonumber(L,12), Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin));
    painter.setBrush(QColor(lua_tonumber(L,9),lua_tonumber(L,10),lua_tonumber(L,11)));
    QPointF points[4] = {
        QPointF(lua_tonumber(L,1),lua_tonumber(L,2)),
        QPointF(lua_tonumber(L,3),lua_tonumber(L,4)),
        QPointF(lua_tonumber(L,5),lua_tonumber(L,6)),
        QPointF(lua_tonumber(L,7),lua_tonumber(L,8)),

    };
    painter.drawPolygon(points,4);
    painter.end();
    //    m_effect->m_img.fill((Qt::red));
    return 0;
}

static int GenerateSpecialClearScreen(lua_State* L) {

    QString file = m_currentDir + "/"+lua_tostring(L,1);
    QString name = lua_tostring(L,2);
    if (QFile::exists(file))
        QFile::remove(file);

    int pos = lua_tonumber(L,3);
    int sp1 = lua_tonumber(L,4);
    int sp2 = lua_tonumber(L,5);


    QString s = "procedure "+name+"();\n";
    s+="begin\n";
    s+="  asm(\"";
    for (int y=0;y<25;y++) {
        for (int x=0;x<40;x++) {
            int xp = ((39-x)/40.0*25);
            if (/*xp>y-sp1 && */xp<y+sp2)
               s+= "  sta "+Util::numToHex(pos + y*40 +x) + "\n";
        }
    }
    s+="  \");\n";
    s+="end;\n";

    Util::SaveTextFile(file,s);



    return 0;
}


static int DrawImage(lua_State* L) {

    QPainter painter;
    QString file = lua_tostring(L,1);
    auto org = file;
    if (!QFile::exists(file))
        file = m_currentDir+ "/"+file;
    if (!QFile::exists(file)) {
        m_infoText+="Could not find image file : "+org+" (global file or in project directory)";
        return 0;
    }

    QImage img(file);
    QVector3D pos(lua_tonumber(L,2),lua_tonumber(L,3),0);
    QVector3D scale(lua_tonumber(L,4),lua_tonumber(L,5),0);
    double rot = lua_tonumber(L,6);
    //   qDebug() << QFile::exists(lua_tostring(L,1)) <<lua_tostring(L,1);

    for (int y=0;y<m_effect->m_img.height();y++)
        for (int x=0;x<m_effect->m_img.width();x++) {
            double xs = ((x-pos.x())/(double)(m_effect->m_img.width()))*img.width();
            double ys = ((y-pos.y())/(double)(m_effect->m_img.height()))*img.height();
            xs = (xs - img.width()/2);//*scale.x();
            ys = (ys - img.height()/2);//*scale.y();
            double xr = xs * cos(rot) -ys*sin(rot);
            double yr = xs * sin(rot) +ys*cos(rot);
            xr*=scale.x();
            yr*=scale.y();
            xr+=img.width()/2;
            yr+=img.height()/2;
            QColor col(0,0,0,0);
            if (xr>=0 && xr<img.width() && yr>=0 && yr<img.height()) {
                col = img.pixelColor(xr,yr);
            }
            if (col.alpha()>0) {
                m_effect->m_img.setPixelColor(x,y,col);
                m_effect->m_post.setPixelColor(x,y,col);
            }
        }
    return 0;

}

static int AddCycleMaskImage(lua_State* L) {

    m_maskImage = LImageIO::Load(lua_tostring(L,1));

    return 0;

}

static int DrawMaskImage(lua_State* L) {

    QPainter painter;
    QImage img(lua_tostring(L,1));
    QVector3D pos(lua_tonumber(L,2),lua_tonumber(L,3),0);
    QVector3D scale(lua_tonumber(L,4),lua_tonumber(L,5),0);
    QColor maskColor(lua_tonumber(L,6),lua_tonumber(L,7),lua_tonumber(L,8));
    QColor drawColor(lua_tonumber(L,9),lua_tonumber(L,10),lua_tonumber(L,11));
    //   qDebug() << QFile::exists(lua_tostring(L,1)) <<lua_tostring(L,1);
    float rot = 0;
    for (int y=0;y<m_effect->m_img.height();y++)
        for (int x=0;x<m_effect->m_img.width();x++) {
            double xs = ((x-pos.x())/(double)(m_effect->m_img.width()))*img.width();
            double ys = ((y-pos.y())/(double)(m_effect->m_img.height()))*img.height();
            xs = (xs - img.width()/2);//*scale.x();
            ys = (ys - img.height()/2);//*scale.y();
            double xr = xs * cos(rot) -ys*sin(rot);
            double yr = ys * sin(rot) +ys*cos(rot);
            xr*=scale.x();
            yr*=scale.y();
            xr+=img.width()/2;
            yr+=img.height()/2;
            QColor col(0,0,0,0);
            if (xr>=0 && xr<img.width() && yr>=0 && yr<img.height()) {
                col = img.pixelColor(xr,yr);
            }
            if (rand()%100>98)
                qDebug() << col<<maskColor;
            if (!Util::isEqual(maskColor,col)) {
                m_effect->m_img.setPixelColor(x,y,drawColor);
                m_effect->m_post.setPixelColor(x,y,drawColor);
            }
        }
    return 0;

}

static int AddRawCharsetData(lua_State* L) {
    int w = lua_tonumber(L,1);
    int h = lua_tonumber(L,1);;
    MultiColorImage* mc = dynamic_cast<MultiColorImage*>(m_effect->m_mc);
    if (mc==nullptr)
        return 0;

    for (int j=0;j<h;j++)
        for (int i=0;i<w;i++) {
            PixelChar& pc = mc->m_data[j*40+i];
            for (int k=0;k<8;k++)
                m_charData.append(pc.reverse(pc.p[k]));
        }
    return 0;
}
static int AddInt(lua_State* L) {
    int w = lua_tonumber(L,1);
    Util::appendInt16Rev(m_charData,w);
    return 0;
}

static int AddByte(lua_State* L) {
    int w = lua_tonumber(L,1);
    m_charData.append(w&255);
    return 0;
}


static int CompressAndSaveHorizontalData(lua_State* L) {
    if (!VerifyFjongParameters(L,"CompressAndSaveHorizontalData"))
        return 0;

    QByteArray table,packedData;
    table.clear();
    QByteArray ba;
    if (m_screenData.count()!=0)
        ba = Util::toQByteArray(m_screenData);
    else
        ba = m_charData;

    m_compression.OptimizeAndPackCharsetData(ba, packedData, table, lua_tonumber(L,1), lua_tonumber(L,2),lua_tonumber(L,5)==1,lua_tonumber(L,6),m_effect->m_mc->m_colorList,m_effect->m_mc->m_bitMask);


    QFile f(m_currentDir+"/"+lua_tostring(L,3));
    f.open(QFile::WriteOnly);
    f.write(packedData);
    f.close();

    QFile f2(m_currentDir+"/"+  lua_tostring(L,4));
    f2.open(QFile::WriteOnly);
    f2.write(table);
    f2.close();

    return 0;

}
static int SaveCompressedTRM(lua_State* L) {
    m_compression.SaveCompressedTRM(m_charData,m_currentDir+"/"+ lua_tostring(L,1), lua_tonumber(L,2));
    return 0;
}


static int CopyFile(lua_State* L) {
    Util::CopyFile(m_currentDir+"/"+ lua_tostring(L,1), m_currentDir+"/"+lua_tostring(L,2));
    return 0;
}



static int AddToPng(lua_State* L) {
    //    Util::CopyFile(m_currentDir+"/"+ lua_tostring(L,1), m_currentDir+"/"+lua_tonumber(L,2));
    if (!VerifyFjongParameters(L,"AddToPng"))
        return 0;
    QString inFile = m_currentDir+"/"+ lua_tostring(L,1);
    QImage img(inFile);
    int xp = lua_tonumber(L,2);
    int yp = lua_tonumber(L,3);
    for (int y=0;y<m_effect->m_mc->GetHeight();y++)
        for (int x=0;x<m_effect->m_mc->GetWidth();x++) {
            img.setPixelColor(x+xp,y+yp, m_effect->m_mc->m_colorList.get(m_effect->m_mc->getPixel(x,y)).color);
        }

    img.save(inFile);
    return 0;
}




void DialogEffects::LoadScript(QString file)
{
    m_script = new LuaScript(file);
    m_infoText = "";
    m_error = "";
    if (m_script->L==nullptr) {
        m_error = m_script->m_error;
        delete m_script;
        m_script = nullptr;

        return;
    }

    //    m_rt.m_globals.m_lights[0]->m_color = QVector3D(1,1,0.7);
    m_rt.m_objects.clear();



    m_rt.m_camera.m_camera = QVector3D(0,0,-5);
    float a = 0.05;
    m_rt.m_globals.m_ambient = QVector3D(a,a,a);


    lua_register(m_script->L, "AddObject", AddObject);
    lua_register(m_script->L, "AddObjectRegular", AddObjectRegular);
    lua_register(m_script->L, "SetRotation", SetRotation);
    lua_register(m_script->L, "Invert", Invert);
    lua_register(m_script->L, "SetID", SetID);
    lua_register(m_script->L, "SetPosition", SetPosition);
    lua_register(m_script->L, "SetScale", SetScale);
    lua_register(m_script->L, "AddPosition", AddPosition);

    lua_register(m_script->L, "DeleteFile", DeleteFile);

    lua_register(m_script->L, "RemoveObject", RemoveObject);

    // Data registration
    lua_register(m_script->L, "AddC64LineToData", AddToData);
    lua_register(m_script->L, "AddVZ200ToData", AddVZ200Data);
    lua_register(m_script->L, "Add4PixelToData", Add4PixelData);
    lua_register(m_script->L, "AddSpecialC64bitmapModeToData", AddSpecialC64bitmapModeToData);
    lua_register(m_script->L, "AddBBCMode5LineToData", AddBBCMode5LineToData);
    lua_register(m_script->L, "AddVGALineToData", AddToDataVGA);

    lua_register(m_script->L, "AddProjected2DCoordinatesToData", AddProjected2DCoordinatesToData);
    lua_register(m_script->L, "AddProjectedLineList", AddProjectedLineList);

    lua_register(m_script->L, "SaveRawData", SaveData);
    lua_register(m_script->L, "SaveRawScreen", SaveDataScreen);
    lua_register(m_script->L, "SaveExtraData", SaveExtraData);

    lua_register(m_script->L, "CompressAndSaveHorizontalData", CompressAndSaveHorizontalData);
    lua_register(m_script->L, "OptimizeScreenAndCharset", OptimizeScreenAndCharset);
    lua_register(m_script->L, "CompressScreenAndCharset", CompressScreenAndCharset);
    lua_register(m_script->L, "CompressCharset", CompressCharset);
    lua_register(m_script->L, "SaveScreenAndCharset", SaveScreenAndCharset);
    lua_register(m_script->L, "SaveCompressedSpriteData", SaveCompressedSpriteData);
    lua_register(m_script->L, "AddRawCharsetData", AddRawCharsetData);

    lua_register(m_script->L, "UpdateScreenDataFrame", UpdateScreenDataFrame);

    lua_register(m_script->L, "CopyFile", CopyFile);
    lua_register(m_script->L, "AddToPng", AddToPng);

    lua_register(m_script->L,"SaveSinusScrollerData", SaveSinusScrollerData);

    lua_register(m_script->L, "AddAmstradCPCToData", AddAmstradCPCToData);
    lua_register(m_script->L, "AddAmigaBitplaneToData", AddBitplaneToData);
    lua_register(m_script->L, "AddAtariBitplaneToData", AddAtariBitplaneToData);
    lua_register(m_script->L, "AddAtariSingleBitplaneToData", AddAtariSingleBitplaneToData);
    lua_register(m_script->L, "AddGameboyData", AddGameboyData);
    lua_register(m_script->L, "Save2DInfo", Save2DInfo);
    lua_register(m_script->L, "ExportDiffAsUnrolledBitmap", ExportDiffAsUnrolledBitmap);
    lua_register(m_script->L, "ExportDiffAsUnrolledBitmapIn", ExportDiffAsUnrolledBitmapIn);
    lua_register(m_script->L, "ExportDiffAsUnrolledBitmapColorOut", ExportDiffAsUnrolledBitmapColorOut);
    lua_register(m_script->L, "ExportDiffAsUnrolledBitmapColorIn", ExportDiffAsUnrolledBitmapColorIn);
    lua_register(m_script->L, "ExportDiffAsUnrolledBitmapColorInAddress", ExportDiffAsUnrolledBitmapColorInAddress);

    lua_register(m_script->L, "Save3DObject", Save3DObject);

    lua_register(m_script->L, "SaveMulticolorImage", SaveMulticolorImage);
    lua_register(m_script->L, "SaveKoalaImage", SaveKoalaImage);
    lua_register(m_script->L, "SaveImage", SaveImage);
    lua_register(m_script->L, "SaveCompressedTRM", SaveCompressedTRM);

    lua_register(m_script->L, "AddInt", AddInt);
    lua_register(m_script->L, "AddByte", AddByte);

    lua_register(m_script->L, "AddScreen", AddScreen);
    lua_register(m_script->L, "AddC64FullScreen", AddC64FullScreen);
    lua_register(m_script->L, "Translate", Translate);
    lua_register(m_script->L, "AddCharsetScreen", AddCharsetScreen);
    lua_register(m_script->L, "AddScreenPetscii", AddScreenPetscii);
    lua_register(m_script->L, "AddScreenBinary", AddScreenBinary);
    lua_register(m_script->L, "SetQuatAxisAngle", SetQuatAxisAngle);
    lua_register(m_script->L, "SetUVShift", SetUVShift);
    lua_register(m_script->L, "sin", LuaSin);
    lua_register(m_script->L, "SetY", SetY);
    lua_register(m_script->L, "cos", LuaCos);
    lua_register(m_script->L, "sqrt", LuaSqrt);
    lua_register(m_script->L, "Message", Message);
    lua_register(m_script->L, "ClearAllObjects", ClearObjects);

    lua_register(m_script->L, "GenerateSpecialClearScreen", GenerateSpecialClearScreen);
    lua_register(m_script->L, "DrawLine", DrawLine);
    lua_register(m_script->L, "DrawRect", DrawRect);
    lua_register(m_script->L, "DrawCircle", DrawCircle);
    lua_register(m_script->L, "DrawIsometricFont", DrawIsometricFont);
    lua_register(m_script->L, "DrawTriangle", DrawTriangle);
    lua_register(m_script->L, "DrawQuad", DrawQuad);
    lua_register(m_script->L, "DrawImage", DrawImage);
    lua_register(m_script->L, "AddCycleMaskImage", AddCycleMaskImage);
    lua_register(m_script->L, "DrawMaskImage", DrawMaskImage);

    // Particle effects

    lua_register(m_script->L, "AddParticle", AddParticle);
    lua_register(m_script->L, "MoveParticles", MoveParticles);
    lua_register(m_script->L, "ConstrainParticlesBox", ConstrainParticlesBox);
    lua_register(m_script->L, "ApplyForce", ApplyForce);
    lua_register(m_script->L, "CollideSphere", CollideSphere);



    /* run the script */
    int ret = luaL_dostring(m_script->L, "Init()");

    lua_register(m_script->L, "SetLight", AddLight);

    UpdateGlobals();



}

void DialogEffects::Init(QString dir)
{
    m_currentDir = dir;
    m_rt.m_globals.m_lights.clear();
    m_rt.m_globals.m_lights.append(new DirectionalLight(QVector3D(1,1,-1).normalized(),QVector3D(1,1,1)));
    m_rt.m_particles.Clear();
    if (m_file!="")
        LoadScript(m_file);

    Create();



}

void DialogEffects::UpdateGlobals()
{
    if (m_script==nullptr)
        return;



    m_rt.m_camera.m_fov = m_script->get<float>("globals.fov");
    m_rt.m_camera.m_camera = m_script->getVec("globals.camera");
    m_rt.m_camera.m_target = m_script->getVec("globals.target");
    m_rt.m_camera.m_up = m_script->getVec("globals.up");
    m_rt.m_globals.m_lights[0]->m_direction = m_script->getVec("globals.light0.direction").normalized();
    m_rt.m_globals.m_lights[0]->m_color = m_script->getVec("globals.light0.color");
    if (m_script->lua_exists("globals.light0.power"))
        m_rt.m_globals.m_lights[0]->m_power = m_script->get<float>("globals.light0.power");

    m_rt.m_globals.m_ambient = m_script->getVec("globals.ambient");
    m_rt.m_globals.m_skyScale = m_script->get<float>("globals.sky");
    m_rt.m_globals.m_shadowScale = m_script->get<float>("globals.shadow_scale");
    m_rt.m_globals.m_isPaused = m_script->get<float>("globals.isPaused");

    m_rt.m_globals.m_width = m_script->get<float>("output.resolution.width");
    m_rt.m_globals.m_height = m_script->get<float>("output.resolution.height");

    if (m_script->lua_exists("output.ignorechars")) {
        m_rt.m_globals.m_ignoreChars = m_script->getIntVector("output.ignorechars");
    }

    if (m_script->lua_exists("output.charset")) {
        QString charName = m_script->get<QString>("output.charset");
        m_rt.m_globals.m_charset = m_script->get<QString>("output.charset");;
        //        qDebug() << "LOADING : "<< charName;
        if (m_rt.m_globals.m_charset.toLower()!="rom")
            m_rt.m_globals.m_charset = m_currentDir+"/"+m_rt.m_globals.m_charset;

    }

    if (m_script->lua_exists("globals.camera_type")) {
        int type = m_script->get<float>("globals.camera_type");
        if (type==1) m_rt.m_globals.m_type = RayTracerGlobals::fisheye;
    }


    if (m_script->lua_exists("output.orgresolution.width")) {
        m_rt.m_globals.m_orgWidth = m_script->get<float>("output.orgresolution.width");
        m_rt.m_globals.m_orgHeight = m_script->get<float>("output.orgresolution.height");

    }
    else {
        m_rt.m_globals.m_orgWidth = m_rt.m_globals.m_width;
        m_rt.m_globals.m_orgHeight = m_rt.m_globals.m_height;
    }


    m_rt.m_globals.m_outputType = m_script->get<float>("output.output_type");
    m_rt.m_globals.m_aspect = m_script->get<float>("output.aspect");

    if (m_script->lua_exists("globals.translate"))
        m_rt.m_globals.m_translate = m_script->getVec("globals.translate");

    if (m_rt.m_globals.m_outputType==RayTracerGlobals::output_type_VGA || m_rt.m_globals.m_outputType==RayTracerGlobals::output_type_AMSTRAD || m_rt.m_globals.m_outputType==RayTracerGlobals::output_type_AMIGA)  {
        if (m_script->lua_exists("output.palette_file"))
        {
            QString f = m_script->get<QString>("output.palette_file");
            //      qDebug() << "Here "<<f;
            if (m_effect!=nullptr)
                if (m_effect->m_mc!=nullptr) {
                    if (f.endsWith(".flf")) {
                        LImage* img = LImageIO::Load(m_currentDir + f);

                        //                        m_effect->m_mc->m_colorList.CopyFrom(&img->m_colorList);
                        if (img!=nullptr) {
                            //                            qDebug() << "HER " << f;
                            m_effect->m_mc->m_colorList.CopyFrom(&img->m_colorList);
                            delete img;
                        }
                        else     m_infoText+="Could not find palette file: "+f+"\n";

                    }
                    else
                        m_effect->m_mc->m_colorList.LoadFromFile(m_currentDir+"/"+f);
                }
        }

    }
    if (m_script->lua_exists("output.smooth")) {
        m_rt.m_globals.m_smooth = m_script->get<float>("output.smooth");
    }

    if (m_script->lua_exists("output.palette")) {
        //            qDebug() << "PALETTE";
        if (m_effect!=nullptr)
            if (m_effect->m_mc!=nullptr)
                m_effect->m_mc->m_colorList.fromArrayList(m_script->getIntVector("output.palette"));

    }

    if (m_script->lua_exists("output.dither"))
        m_rt.m_globals.m_dither = m_script->get<float>("output.dither");
    if (m_script->lua_exists("output.ditherStrength"))
        m_rt.m_globals.m_ditherStrength = m_script->getVec("output.ditherStrength");
    if (m_script->lua_exists("output.index_colors"))
        m_rt.m_globals.m_c64Colors = m_script->getIntVector("output.index_colors");

    //  if (m_script->lua_exists(material+".checkerboard")) {


    if (m_rt.m_globals.m_outputType==RayTracerGlobals::output_type_BINARY)  {

        m_rt.m_globals.m_dither = m_script->get<float>("output.dither");
        m_rt.m_globals.m_ditherStrength = m_script->getVec("output.ditherStrength");
        m_rt.m_globals.m_c64Colors = m_script->getIntVector("output.index_colors");
    }



    if (m_rt.m_globals.m_outputType==RayTracerGlobals::output_type_c64)  {

        m_rt.m_globals.m_multicolor = m_script->get<float>("output.c64_multicolor");
        m_rt.m_globals.m_dither = m_script->get<float>("output.dither");
        m_rt.m_globals.m_ditherStrength = m_script->getVec("output.ditherStrength");
        //   if (m_script->lua_gettostack("output.c64_imageType"))
        m_rt.m_globals.m_c64ImageType = m_script->get<float>("output.c64_imageType");

        m_rt.m_globals.m_c64Colors = m_script->getIntVector("output.index_colors");
    }


    if (m_rt.m_globals.m_c64Colors.count()==1) {
        int p = m_rt.m_globals.m_c64Colors[0];
        m_rt.m_globals.m_c64Colors.clear();
        if (m_effect)
            if (m_effect->m_mc)
                for (int i=0;i<m_effect->m_mc->m_colorList.count();i+=p)
                    m_rt.m_globals.m_c64Colors.append(i);

        CharsetImage* c = dynamic_cast<CharsetImage*>(m_effect->m_mc);
        if (c!=nullptr) {
            for (int i=0;i<std::min(4,m_effect->m_mc->m_colorList.count());i++)
                c->SetColor(m_rt.m_globals.m_c64Colors[i],i);
        }
    }


    m_rt.m_globals.m_steps = m_script->get<float>("globals.raymarch_steps");
    m_rt.m_globals.m_shadowSteps = m_script->get<float>("globals.raymarch_shadow_steps");
}


void DialogEffects::UpdateImage()
{

    if (m_effect==nullptr)
        return;

    if (m_error!="") {
        ui->txtOutput->setText(m_error);
        return;
    }



    ui->lblImage->setPixmap( m_effect->m_pixmap );
    if (ui->txtOutput->toPlainText()!=m_infoText)
        ui->txtOutput->setText(m_infoText);



    if (m_script!=nullptr && m_rt.m_globals.m_isPaused==0) {
        luaL_dostring(m_script->L, "Update()");
        UpdateGlobals();
        if (m_script->m_error!="") m_error = m_script->m_error;

    }
    //    qDebug() << "KAKE";


    m_effect->m_ready = true;
}
