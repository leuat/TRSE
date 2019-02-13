#include "dialogeffects.h"
#include "ui_dialogeffects.h"

LuaScript* m_script = nullptr;
QString m_infoText = "", m_error="";
QString m_currentDir;
QByteArray m_screenData, m_charData;
AbstractDemoEffect* m_effect = nullptr;


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
    if (object=="mesh") {
        QString fn = m_currentDir+"/"+ lua_tostring(L,N);
        float meshscale = lua_tonumber(L,N+1);
        N+=2;
        if (!QFile::exists(fn)) {
            m_error = "Could not find 3d object file: "+fn;
            return 0;
        }

        QVector3D orgPos = QVector3D(lua_tonumber(L,N),lua_tonumber(L,N+1),lua_tonumber(L,N+2));

        mat.m_color = m_script->getVec(material+".color");

        mat.m_reflectivity = m_script->get<float>(material+".reflection");

        mat.m_shininess = m_script->get<float>(material+".shininess");

        mat.m_shininess_strength = m_script->get<float>(material+".shininess_intensity");

        m_rt.LoadMesh(fn, meshscale,orgPos, mat,name);
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
                        QVector3D(lua_tonumber(L,N+6),1,0),
                        QVector3D(lua_tonumber(L,N+3),lua_tonumber(L,N+4),lua_tonumber(L,N+5)),
                        mat);


    }

    if (object=="char") {

        CharsetImage* charset = new CharsetImage(LColorList::C64);
        QString charName = lua_tostring(L,N);
        if (charName.toLower()=="rom") {
            charset->LoadCharset(":resources/character.rom");
        }
        else {
            QString fname = m_currentDir+"/"+QString(lua_tostring(L,N));
            if (!QFile::exists(fname)) {
                m_error += "Could not open file: " + fname + "\n";
                return 0;
            }
            charset->LoadCharset(fname);
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

    if (obj!=nullptr) {
        obj->m_name= name;
        if (parent=="")
            m_rt.m_objects.append(obj);
        else {
            AbstractRayObject* aro = m_rt.Find(parent);
            if (aro==nullptr) {
                m_error +="Error in AddObject: Could not find parent object '" + parent+  "'\n";
                return 0;
            }
            aro->m_children.append(obj);
        }



        mat.m_color = m_script->getVec(material+".color");

        mat.m_reflectivity = m_script->get<float>(material+".reflection");

        mat.m_shininess = m_script->get<float>(material+".shininess");

        mat.m_shininess_strength = m_script->get<float>(material+".shininess_intensity");


        obj->SetMaterial(mat);

    }
    else m_error +="Error in AddObject: Unkown type '" + object+  "'\n";

    return 0;


}


static int SetRotation(lua_State *L)
{
//    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="Error in SetRotation : Could not find object '" + name+ "'\n";
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
        m_error +="Error in SetRotation : Could not find object '" + name+ "'\n";
        return 0;
    }

    aro->m_position = (QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)));
    return 0;
}

static int SetY(lua_State *L)
{
//    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);
    AbstractRayObject* aro = m_rt.Find(name);
    if (aro==nullptr) {
        m_error +="Error in SetRotation : Could not find object '" + name+ "'\n";
        return 0;
    }

    aro->m_position.setY(lua_tonumber(L,2));
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

static int AddScreen(lua_State* L) {

    if (m_effect!=nullptr)
       m_effect->AddScreen(m_screenData, lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4), lua_tonumber(L,5),lua_tonumber(L,6));

    return 0;
}

static int CompressCharset(lua_State* L) {
    // 0, 40, 13, 25
    int noChars;
    m_effect->m_mc->CompressAndSave(m_charData, m_screenData, lua_tonumber(L,1),lua_tonumber(L,2), lua_tonumber(L,3),lua_tonumber(L,4),noChars,lua_tonumber(L,5),  lua_tonumber(L,6));
    m_infoText+="Compressed chars: " + QString::number(noChars) + "\n";
    return 0;
}

static int SaveScreenAndCharset(lua_State* L) {
    QFile f(m_currentDir+"/"+ lua_tostring(L,2));
    f.open(QFile::WriteOnly);
    f.write(m_charData);
    f.close();
    m_charData.clear();

    QFile f2(m_currentDir+"/"+lua_tostring(L,1));
    f2.open(QFile::WriteOnly);
    f2.write(m_screenData);
    f2.close();
    m_screenData.clear();
    return 0;
}


static int CompressAndSaveHorizontalData(lua_State* L) {

    QByteArray packedData, table;
    table.clear();
//    qDebug() <<m_count*16 << " but is " <<m_screenData.count()/ww;
    m_effect->OptimizeAndPackCharsetData(m_screenData, packedData, table, lua_tonumber(L,1), lua_tonumber(L,2));
  //  qDebug() << "Table should be : " << (m_noChars-1)*1024;
    //qDebug() << "Table is : " << table.count();


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
    lua_register(m_script->L, "CompressAndSaveHorizontalData", CompressAndSaveHorizontalData);
    lua_register(m_script->L, "CompressCharset", CompressCharset);
    lua_register(m_script->L, "SaveScreenAndCharset", SaveScreenAndCharset);
    lua_register(m_script->L, "AddScreen", AddScreen);
    lua_register(m_script->L, "SetRotation", SetRotation);
    lua_register(m_script->L, "SetPosition", SetPosition);
    lua_register(m_script->L, "sin", LuaSin);
    lua_register(m_script->L, "SetY", SetY);
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

    m_rt.m_camera.m_fov = m_script->get<float>("globals.fov");
    m_rt.m_camera.m_camera = m_script->getVec("globals.camera");
    m_rt.m_camera.m_target = m_script->getVec("globals.target");
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
//    m_effect->Render(m_effect->m_img);
    m_effect->start();
}

/*void DialogEffects::on_pushButton_clicked()
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
*/
