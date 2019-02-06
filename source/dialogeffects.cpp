#include "dialogeffects.h"
#include "ui_dialogeffects.h"

LuaScript* m_script = nullptr;
QString m_infoText = "", m_error="";
QString m_currentDir;

DialogEffects::DialogEffects(QString file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEffects)
{
    ui->setupUi(this);
    m_file = file;

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
        }
//    }
    m_effect->Initialize();
    connect(m_effect,SIGNAL(SignalImageUpdate()),this,SLOT(UpdateImage()));
//    m_effect->FillToGUI();
    m_effect->start();
}


static int LuaSin(lua_State *L) {
    lua_pushnumber(L, sin(lua_tonumber(L, 1)));
    return 1;
}

static int LuaCos(lua_State *L) {
    lua_pushnumber(L, cos(lua_tonumber(L, 1)));
    return 1;
}



static int AddObject(lua_State *L)
{
//    qDebug();// << text;
  //  int n = lua_gettop(L);
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
        obj =
                    new RayObjectTorus(
                        QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2)) ,
                        QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),0),
                        QVector3D(0,1,0),
                        mat);

    }

    if (object=="box") {
        obj =
                    new RayObjectBox(
                        QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2)) ,
                        QVector3D(0,1,0),
                        QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5)),
                        mat);


    }

/*    if (object=="char") {

        CharsetImage* charset = new CharsetImage(LColorList::C64);
        if (QString(lua_tostring(L,4)).toLower()=="rom")
            charset->LoadCharset(":resources/character.rom");
        else
            charset->LoadCharset(m_currentDir+"/"+QString(lua_tostring(L,4)));

       obj =
                    new RayObjectBox(
                        QVector3D(lua_tonumber(L,4),lua_tonumber(L,5),lua_tonumber(L,6)) ,
                        QVector3D(0,1,0),
                        QVector3D(lua_tonumber(L,7),lua_tonumber(L,8),lua_tonumber(L,9)),
                        mat);

        delete charset;

    }
*/
    if (obj!=nullptr) {
        obj->m_name= name;
        if (parent=="")
            m_rt.m_objects.append(obj);
        else {
            AbstractRayObject* aro = m_rt.Find(parent);
            if (aro==nullptr) {
                m_error +="Error in AddObject: Could not find parent object '" + parent+  "'";
                return 0;
            }
            aro->m_children.append(obj);
        }



        mat.m_color = m_script->getVec(material+".color");

        mat.m_reflectivity = m_script->get<float>(material+".reflection");

        mat.m_shininess = m_script->get<float>(material+".shininess");


        obj->SetMaterial(mat);

    }
    else m_error +="Error in AddObject: Unkown type '" + object+  "'";

    return 0;


}


static int SetRotation(lua_State *L)
{
//    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="Error in SetRotation : Could not find object '" + name+ "'";
        return 0;
    }

    aro->SetRotation(QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)));
    return 0;
}


static int SetPosition(lua_State *L)
{
//    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="Error in SetRotation : Could not find object '" + name+ "'";
        return 0;
    }

    aro->m_position = (QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)));
    return 0;
}



int DialogEffects::Message(lua_State *L)
{
    m_infoText+=lua_tostring(L,1);
    m_infoText+="\n";
    return 0;
}

static int ClearObjects(lua_State *L) {
    m_rt.m_objects.clear();
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

    m_rt.m_globals.m_lights[0]->m_color = QVector3D(1,1,0.7);
    m_rt.m_objects.clear();



    m_rt.m_camera.m_camera = QVector3D(0,0,-5);
    float a = 0.05;
    m_rt.m_globals.m_ambient = QVector3D(a,a,a);


    lua_register(m_script->L, "AddObject", AddObject);
    lua_register(m_script->L, "SetRotation", SetRotation);
    lua_register(m_script->L, "SetPosition", SetPosition);
    lua_register(m_script->L, "sin", LuaSin);
    lua_register(m_script->L, "cos", LuaCos);
    lua_register(m_script->L, "Message", Message);
    lua_register(m_script->L, "ClearAllObjects", ClearObjects);

    /* run the script */
    int ret = luaL_dostring(m_script->L, "Init()");

    UpdateGlobals();
}

void DialogEffects::Init(QString dir)
{
    m_currentDir = dir;
    if (m_file!="")
        LoadScript(m_file);

    Create();



}

void DialogEffects::UpdateGlobals()
{
    if (m_script==nullptr)
        return;
    m_rt.m_camera.m_camera = m_script->getVec("globals.camera");
    m_rt.m_globals.m_lights[0]->m_direction = m_script->getVec("globals.light0.direction").normalized();
    m_rt.m_globals.m_lights[0]->m_color = m_script->getVec("globals.light0.color");
    m_rt.m_globals.m_ambient = m_script->getVec("globals.ambient");
    m_rt.m_globals.m_skyScale = m_script->get<float>("globals.sky");
    m_rt.m_globals.m_shadowScale = m_script->get<float>("globals.shadow_scale");
    m_rt.m_globals.m_isPaused = m_script->get<float>("globals.isPaused");

    m_rt.m_globals.m_width = m_script->get<float>("output.resolution.width");
    m_rt.m_globals.m_height = m_script->get<float>("output.resolution.height");

    m_rt.m_globals.m_c64Output = m_script->get<float>("output.c64_output");
    m_rt.m_globals.m_dither = m_script->get<float>("output.dither");

    m_rt.m_globals.m_c64Colors = m_script->getIntVector("output.c64_colors");
}

void DialogEffects::UpdateImage()
{
    if (m_effect==nullptr)
        return;

    if (m_error!="") {
        ui->txtOutput->setText(m_error);
        return;
    }

    QPixmap p;//
    p.convertFromImage(m_effect->m_img);
    if (m_effect->m_img.width()<321)
        p = p.scaled(320, 200, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    ui->lblImage->setPixmap( p );
    ui->txtOutput->setText(m_infoText);




    if (m_script!=nullptr && m_rt.m_globals.m_isPaused==0) {
        int ret = luaL_dostring(m_script->L, "Update()");
        UpdateGlobals();
        if (m_script->m_error!="") m_error = m_script->m_error;

    }


}

void DialogEffects::on_pushButton_clicked()
{
    Abort();
    close();
}

void DialogEffects::on_btnToggleAnim_clicked()
{
    if (m_effect==nullptr)
        return;
    m_effect->ToggleAnim();
//    m_effect->m_toggleAnim=!m_effect->m_toggleAnim;
 //   m_effect->m_time = 0;
}

void DialogEffects::on_btnToggleC64_clicked()
{
    if (m_effect==nullptr)
        return;
    m_effect->m_toggleC64=!m_effect->m_toggleC64;

}

void DialogEffects::on_btnSave_clicked()
{
    if (m_effect==nullptr)
        return;
    bool c64=m_effect->m_toggleC64;
    bool anim=m_effect->m_toggleAnim;
    m_effect->m_toggleC64 = true;
    m_effect->m_toggleAnim = false;

//    m_effect->Save("twister.bin");

    m_effect->m_toggleC64 = c64;
    m_effect->m_toggleAnim = anim;

}

void DialogEffects::on_comboBox_activated(const QString &arg1)
{
    Create();
}
