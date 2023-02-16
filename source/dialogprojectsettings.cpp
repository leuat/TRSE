/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/
#include "dialogprojectsettings.h"
#include "ui_dialogprojectsettings.h"
#include <QFileDialog>

DialogProjectSettings::DialogProjectSettings(QString dir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProjectSettings)
{
    m_currentDir = dir;
    ui->setupUi(this);

}

DialogProjectSettings::~DialogProjectSettings()
{
    delete ui;
}

void DialogProjectSettings::on_btnClose_clicked()
{
    FillToIni();
    m_ini->Save();
    close();

}

void DialogProjectSettings::FillFromIni()
{
    ui->leZeropageScreenMemory->setText(m_ini->getString("zeropage_screenmemory"));
    ui->leZeropageColorMemory->setText(m_ini->getString("zeropage_colormemory"));

    ui->leDecrunchZp1->setText(m_ini->getString("zeropage_decrunch1"));
    ui->leDecrunchZp2->setText(m_ini->getString("zeropage_decrunch2"));
    ui->leDecrunchZp3->setText(m_ini->getString("zeropage_decrunch3"));
    ui->leDecrunchZp4->setText(m_ini->getString("zeropage_decrunch4"));

    ui->leInternalZp1->setText(m_ini->getString("zeropage_internal1"));
    ui->leInternalZp2->setText(m_ini->getString("zeropage_internal2"));
    ui->leInternalZp3->setText(m_ini->getString("zeropage_internal3"));
    ui->leInternalZp4->setText(m_ini->getString("zeropage_internal4"));



    ui->leZeropages->setText(  fromStringList(m_ini->getStringList("zeropages")));
    //    ui->leDiskname->setText( m_ini->getString("d64name"));

    ui->txtGlobalDefines->setPlainText(Util::fromStringList( m_ini->getStringList("global_defines")));

    ui->leTempZP->setText(  fromStringList(m_ini->getStringList("temp_zeropages")));
    ui->levarZP->setText(  fromStringList(m_ini->getStringList("var_zeropages")));
    ui->leUserDefinedtempVars->setText(fromStringList(m_ini->getStringList("zeropages_userdefined")));

    ui->cmbSystem->setCurrentText(m_ini->getString("system"));
    ui->cmbQuemu->setCurrentText(m_ini->getString("qemu"));
    ui->cmbUseTripe->setCurrentText(m_ini->getdouble("use_tripe")?"Yes":"No");
    //    on_cmbSystem_currentIndexChanged(ui->cmbSystem)
    UpdateSystem();

    ui->cmbOutputType->setCurrentText(m_ini->getString("output_type"));
    //ui->leMainFile->setText(m_ini->getString("main_ras_file"));


    //    ui->chkUseViceC1541->setChecked(m_ini->getdouble("use_vice_c1541")==1);
    ui->chkTrack19->setChecked(m_ini->getdouble("use_track_19")==1);
    ui->chkCompilerComments->setChecked(m_ini->getdouble("disable_compiler_comments")==1);

    ui->chkPassLda->setChecked(m_ini->getdouble("post_optimizer_passlda")==1);
    ui->chkPassJmp->setChecked(m_ini->getdouble("post_optimizer_passjmp")==1);
    ui->chkPassLdaTax->setChecked(m_ini->getdouble("post_optimizer_passldatax")==1);

    ui->chkPassStaLda->setChecked(m_ini->getdouble("post_optimizer_passstalda")==1);
    ui->chkPassLdx->setChecked(m_ini->getdouble("post_optimizer_passldx")==1);

    ui->chkLocalVariables->setChecked(m_ini->getdouble("pascal_settings_use_local_variables")==1);


    ui->chkPassPhaPla->setChecked(m_ini->getdouble("post_optimizer_passphapla")==1);
    ui->chkPassCmp0->setChecked(m_ini->getdouble("post_optimizer_passcmp")==1);

    //    if (m_ini->getString("system")=="C128") {
    ui->cmbColumns->setCurrentText(QString::number(m_ini->getdouble("columns")));
    //  }

    if (m_ini->getString("system")=="NES") {
        ui->cmbNES16KBlocks->setCurrentText(QString::number(m_ini->getdouble("nes_16k_blocks")));
        ui->cmbNES8KBlocks->setCurrentText(QString::number(m_ini->getdouble("nes_8k_blocks")));
        ui->leNESCharFile->setText(m_ini->getString("nes_8k_file"));
        ui->leNESCharFile_2->setText(m_ini->getString("nes_8k_file_2"));
        ui->leNESCodeStart->setText(m_ini->getString("nes_code_start"));
    }


    //    if (m_ini->getString("system")=="VIC20") {
    ui->tabConfigs->setCurrentWidget(ui->tabVic20);
    ui->leViaZP->setText(  fromStringList(m_ini->getStringList("via_zeropages")));
    //   }

    ui->cmbVic20Config->setCurrentText(m_ini->getString("vic_memory_config"));

    ui->cmbX86Type->setCurrentText(m_ini->getString("dosbox_x86_system"));

    ui->cbmPetSystem->setCurrentText(m_ini->getString("petmodel"));

    ui->cmbAmstradCPCModel->setCurrentText(m_ini->getString("amstradcpc_model"));
    ui->leAmstradCPCOptions->setText(m_ini->getString("amstradcpc_options"));

    ui->cbmSpectrumModel->setCurrentIndex(m_ini->getdouble("spectrum_model"));
    ui->cmbX86CPU->setCurrentText(m_ini->getString("cpu_x86_system"));
    ui->cmbX86OutputType->setCurrentText(m_ini->getString("cpu_x86_output"));


    ui->leDosboxCycles->setText(m_ini->getString("dosbox_cycles"));

    //    ui->cmbDiskType->setCurrentText(m_ini->getString("cc1541_disk_type"));


    //    qDebug() <<"PROJECTSETTINGS OUT" << m_ini->getString("cpu_x86_system");


    QStringList files = m_ini->getStringList("disk_files");
    QStringList names = m_ini->getStringList("disk_names");



    ui->chkOverrideTargetSettings->setChecked(m_ini->getdouble("override_target_settings")==1);

    ui->leProgramStartAddress->setText(m_ini->getString("override_target_settings_org"));
    ui->leBasicStartAddress->setText(m_ini->getString("override_target_settings_basic"));
    ui->chkIgnoreBasic->setChecked(m_ini->getdouble("override_target_settings_sys")==1);
    ui->chkStripPrg->setChecked(m_ini->getdouble("override_target_settings_prg")==1);

    ui->chkDebugSymbols->setChecked(m_ini->getdouble("output_debug_symbols")==1);
    ui->chkIgnoreJump->setChecked(m_ini->getdouble("ignore_initial_jump")==1);



    /*    for (int r=0;r<names.count();r++) {
        ui->tabData->insertRow(r);
        ui->tabData->setItem(r,0,new QTableWidgetItem(names[r]));
        ui->tabData->setItem(r,1,new QTableWidgetItem(files[r]));
    }

*/
    //    ui->chkPOEnabled->setChecked(m_ini->getdouble("post_optimize")==1);


    QStringList ras = Util::FindFilesOfType(m_currentDir+"/","*.ras");
    QStringList flf = Util::FindFilesOfType(m_currentDir+"/","*.flf");
    //    qDebug() << ras << m_currentDir;
    ui->cmbMainRas->clear();
    ui->cmbMainRas->addItem("none");
    ui->cmbMainRas->addItems(ras);

    /*    ui->cmbDirArt->clear();
    ui->cmbDirArt->addItem("none");
    ui->cmbDirArt->addItems(flf);
*/
    ui->cmbMainRas->setCurrentText(m_ini->getString("main_ras_file"));
    //  ui->cmbDirArt->setCurrentText(m_ini->getString("dirart_flf_file"));


    ui->leInitMachineState->setText(m_ini->getString("machine_state"));

    if (m_ini->getString("system")=="THOMSON") {
        ui->cmbThomsonMedia->setCurrentText(m_ini->getString("thomson_media"));
        ui->cmbThomsonSubtype->setCurrentText(m_ini->getString("thomson_subtype"));

    }

    /*    QStringList paw = Util::FindFilesOfType(m_currentDir+"/","*.paw");
//    qDebug() << ras << m_currentDir;
    ui->cmbPawInclude->clear();
    ui->cmbPawInclude->addItem("none");
    ui->cmbPawInclude->addItems(paw);

    ui->cmbPawInclude2->clear();
    ui->cmbPawInclude2->addItem("none");
    ui->cmbPawInclude2->addItems(paw);



    ui->cmbPawInclude->setCurrentText(m_ini->getString("d64_paw_file"));

    ui->cmbPawInclude2->setCurrentText(m_ini->getString("d64_paw_file_disk2"));
*/
    ui->teBuildList->document()->setPlainText(Util::fromStringList(    m_ini->getStringList("build_list")));
    ui->leBackgroundColor->setText(QString::number(m_ini->getdouble("background_color")));
    ui->leBorderColor->setText(QString::number(m_ini->getdouble("border_color")));


    ui->cmbCustomCPU->setCurrentText(m_ini->getString("custom_system_cpu"));
    ui->cmbCustomMethods->setCurrentText(m_ini->getString("custom_system_methods"));
    ui->cmbCustomAssembler->setCurrentText(m_ini->getString("custom_system_assembler"));
    ui->leCustomAssembler->setText(m_ini->getString("custom_system_assembler_custom"));
    ui->leCustomEmulator->setText(m_ini->getString("custom_system_emulator"));
    ui->leCustomEmulatorParameters->setText(m_ini->getString("custom_system_emulator_parameters"));
    ui->leCustomAssemblerParameters->setText(m_ini->getString("custom_system_assembler_parameters"));
    ui->leCustomEnding->setText(m_ini->getString("custom_system_ending"));

    on_cmbSystem_currentIndexChanged(ui->cmbSystem->currentIndex());
    PopulateDiskList();

}



void DialogProjectSettings::FillToIni()
{
    m_ini->setStringList("zeropages", toStringList(ui->leZeropages->text()));
    m_ini->setStringList("temp_zeropages", toStringList(ui->leTempZP->text()));
    m_ini->setStringList("var_zeropages", toStringList(ui->levarZP->text()));
    m_ini->setStringList("zeropages_userdefined", toStringList(ui->leUserDefinedtempVars->text()));

    m_ini->setString("zeropage_screenmemory", Util::numToHex(Util::NumberFromStringHex(ui->leZeropageScreenMemory->text())));
    m_ini->setString("zeropage_colormemory", Util::numToHex(Util::NumberFromStringHex(ui->leZeropageColorMemory->text())));

    m_ini->setFloat("border_color", ui->leBorderColor->text().toInt());
    m_ini->setFloat("background_color", ui->leBackgroundColor->text().toInt());
    m_ini->setString("machine_state", ui->leInitMachineState->text());

    m_ini->setStringList("global_defines", ui->txtGlobalDefines->toPlainText().split("\n"));
    //    qDebug() << ui->txtGlobalDefines->toPlainText().split("\n");
    m_ini->setString("zeropage_decrunch1", Util::numToHex(Util::NumberFromStringHex(ui->leDecrunchZp1->text())));
    m_ini->setString("zeropage_decrunch2", Util::numToHex(Util::NumberFromStringHex(ui->leDecrunchZp2->text())));
    m_ini->setString("zeropage_decrunch3", Util::numToHex(Util::NumberFromStringHex(ui->leDecrunchZp3->text())));
    m_ini->setString("zeropage_decrunch4", Util::numToHex(Util::NumberFromStringHex(ui->leDecrunchZp4->text())));


    m_ini->setString("custom_system_cpu",ui->cmbCustomCPU->currentText());
    m_ini->setString("custom_system_methods",ui->cmbCustomMethods->currentText());
    m_ini->setString("custom_system_assembler",ui->cmbCustomAssembler->currentText());
    m_ini->setString("custom_system_assembler_custom",ui->leCustomAssembler->text());
    m_ini->setString("custom_system_emulator",ui->leCustomEmulator->text());
    m_ini->setString("custom_system_emulator_parameters",ui->leCustomEmulatorParameters->text());
    m_ini->setString("custom_system_assembler_parameters",ui->leCustomAssemblerParameters->text());
    m_ini->setString("custom_system_ending",ui->leCustomEnding->text());



    m_ini->setString("petmodel", ui->cbmPetSystem->currentText());

    m_ini->setString("amstradcpc_model", ui->cmbAmstradCPCModel->currentText());
    m_ini->setString("qemu",ui->cmbQuemu->currentText());

    m_ini->setFloat("use_tripe",(ui->cmbUseTripe->currentText()=="Yes")?1:0);
    ui->cmbUseTripe->setCurrentText(m_ini->getdouble("use_tripe")?"Yes":"No");


    m_ini->setString("amstradcpc_options", ui->leAmstradCPCOptions->text());

    m_ini->setFloat("spectrum_model", ui->cbmSpectrumModel->currentIndex());

    m_ini->setString("zeropage_internal1", Util::numToHex(Util::NumberFromStringHex(ui->leInternalZp1->text())));
    m_ini->setString("zeropage_internal2", Util::numToHex(Util::NumberFromStringHex(ui->leInternalZp2->text())));
    m_ini->setString("zeropage_internal3", Util::numToHex(Util::NumberFromStringHex(ui->leInternalZp3->text())));
    m_ini->setString("zeropage_internal4", Util::numToHex(Util::NumberFromStringHex(ui->leInternalZp4->text())));

    m_ini->setString("dosbox_cycles",ui->leDosboxCycles->text());


    m_ini->setString("system", ui->cmbSystem->currentText());

    //    m_ini->setFloat("use_vice_c1541", ui->chkUseViceC1541->isChecked());
    m_ini->setFloat("disable_compiler_comments", ui->chkCompilerComments->isChecked());
    m_ini->setFloat("use_track_19", ui->chkTrack19->isChecked());

    m_ini->setFloat("post_optimizer_passlda", ui->chkPassLda->isChecked());
    m_ini->setFloat("post_optimizer_passjmp", ui->chkPassJmp->isChecked());
    m_ini->setFloat("post_optimizer_passldatax", ui->chkPassLdaTax->isChecked());
    m_ini->setFloat("post_optimizer_passstalda", ui->chkPassStaLda->isChecked());
    m_ini->setFloat("post_optimizer_passldx", ui->chkPassLdx->isChecked());

    m_ini->setFloat("post_optimizer_passphapla", ui->chkPassPhaPla->isChecked());
    m_ini->setFloat("post_optimizer_passcmp", ui->chkPassCmp0->isChecked());

    m_ini->setString("dosbox_x86_system", ui->cmbX86Type->currentText());
    m_ini->setString("cpu_x86_system", ui->cmbX86CPU->currentText());
    m_ini->setString("cpu_x86_output", ui->cmbX86OutputType->currentText());

    m_ini->setFloat("pascal_settings_use_local_variables", ui->chkLocalVariables->isChecked());




    //if (m_ini->getString("system")=="C128") {
    m_ini->setString("columns", ui->cmbColumns->currentText());

    // }
    //    if (m_ini->getString("system")=="VIC20") {

    m_ini->setString("vic_memory_config",ui->cmbVic20Config->currentText());
    m_ini->setStringList("via_zeropages", toStringList(ui->leViaZP->text()));

    //  }
    if (m_ini->getString("system")=="THOMSON") {
        m_ini->setString("thomson_media",ui->cmbThomsonMedia->currentText());
        m_ini->setString("thomson_subtype",ui->cmbThomsonSubtype->currentText());

    }

    if (m_ini->getString("system")=="NES") {
        m_ini->setFloat("nes_16k_blocks",ui->cmbNES16KBlocks->currentText().toInt());
        m_ini->setFloat("nes_8k_blocks",ui->cmbNES8KBlocks->currentText().toInt());
        m_ini->setString("nes_8k_file",ui->leNESCharFile->text());
        m_ini->setString("nes_8k_file_2",ui->leNESCharFile_2->text());
        m_ini->setString("nes_code_start",ui->leNESCodeStart->text());
    }



    //    m_ini->setString("cc1541_disk_type", ui->cmbDiskType->currentText());


    m_ini->setString("output_type", ui->cmbOutputType->currentText());
    m_ini->setString("main_ras_file", ui->cmbMainRas->currentText());
    //  m_ini->setString("dirart_flf_file", ui->cmbDirArt->currentText());
    //    m_ini->setString("d64_paw_file", ui->cmbPawInclude->currentText());
    //    m_ini->setString("d64_paw_file_disk2", ui->cmbPawInclude2->currentText());


    m_ini->setFloat("override_target_settings", ui->chkOverrideTargetSettings->isChecked());
    m_ini->setString("override_target_settings_org", ui->leProgramStartAddress->text());
    m_ini->setString("override_target_settings_basic", ui->leBasicStartAddress->text());
    m_ini->setFloat("override_target_settings_sys", ui->chkIgnoreBasic->isChecked());
    m_ini->setFloat("override_target_settings_prg", ui->chkStripPrg->isChecked());

    m_ini->setStringList("build_list", ui->teBuildList->toPlainText().remove("=").split("\n"));

    m_ini->setFloat("output_debug_symbols",ui->chkDebugSymbols->isChecked());
    m_ini->setFloat("ignore_initial_jump",ui->chkIgnoreJump->isChecked());

    //    m_ini->setString("d64name", ui->leDiskname->text());

    PopulateDiskList();

    //    FillTabDataToIni();

}

/*void DialogProjectSettings::FillTabDataToIni()
{
    QStringList names;
    QStringList files;

    for (int r=0;r<ui->tabData->rowCount();r++) {
        names<< ui->tabData->item(r,0)->text();
        files<< ui->tabData->item(r,1)->text();

    }

    m_ini->setStringList("disk_files",files);
    m_ini->setStringList("disk_names",names);

}

void DialogProjectSettings::on_pushButton_clicked()
{
    int r =ui->tabData->rowCount();
    ui->tabData->insertRow(r);
    ui->tabData->setItem(r,0,new QTableWidgetItem(""));
    ui->tabData->setItem(r,1,new QTableWidgetItem(""));
    ui->tabData->setItem(r,2,new QTableWidgetItem(""));

}

void DialogProjectSettings::on_pushButton_2_clicked()
{
    ui->tabData->removeRow(ui->tabData->currentRow());

}

*/

void DialogProjectSettings::on_pushButton_clicked()
{
    Messages::messages.DisplayMessage(Messages::messages.PROJECT_BUILDER_HELP,true);
}


void DialogProjectSettings::on_btnNESLoadCharFile_2_clicked()
{

}

void DialogProjectSettings::on_btnNESLoadCharFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Tile bin file"), m_ini->getString("project_path"), "*");
    if (filename!="") {
        filename.remove(m_currentDir);
        ui->leNESCharFile->setText(filename);
        m_ini->getString("nes_8k_file") = filename;
    }

}

void DialogProjectSettings::on_btnDefaultZP_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Reset to default recommended zeropages?", "Reset zeropages to the default recommended values for this system? Any current zeropage settings will be replaced. ",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
        return;


    if (m_ini->getString("system")=="VIC20") {
        ui->leTempZP->setText("$88, $8A, $8C, $8E");
        ui->leInternalZp1->setText("$80");
        ui->leInternalZp2->setText("$82");
        ui->leInternalZp3->setText("$84");
        ui->leInternalZp4->setText("$86");
        ui->leZeropageScreenMemory->setText("$FD");
        ui->leZeropageColorMemory->setText("$FB");
        ui->leDecrunchZp1->setText("$7B");
        ui->leDecrunchZp2->setText("$7C");
        ui->leDecrunchZp3->setText("$7E");
        ui->leDecrunchZp4->setText("$7F");
        ui->leZeropages->setText("$64, $66, $68, $6A, $6C, $6E, $70, $72, $74, $76, $78");
    }


}

void DialogProjectSettings::on_cmbSystem_currentIndexChanged(int index)
{
    if (index==0)
        ui->tabConfigs->setCurrentIndex(0);
    if (index==2)
        ui->tabConfigs->setCurrentIndex(1);
    if (index==3)
        ui->tabConfigs->setCurrentIndex(2);
    if (index==4)
        ui->tabConfigs->setCurrentIndex(3);
    if (index==8)
        ui->tabConfigs->setCurrentIndex(4);
    if (index==6)
        ui->tabConfigs->setCurrentIndex(6);
    if (index==9)
        ui->tabConfigs->setCurrentIndex(8);
    if (index==25)
        ui->tabConfigs->setCurrentIndex(10);
    if (index==36) // thomson
        ui->tabConfigs->setCurrentIndex(9);

    UpdateSystem();
    /*    if (index==7)
        ui->tabConfigs->setCurrentIndex(6);*/
}
void DialogProjectSettings::UpdateSystem()
{
    QString s = ui->cmbSystem->currentText().toLower();
    if (s=="C64" || s=="C128" || s =="VIC20" || s=="PET" || s=="PLUS4") {
        ui->leProgramStartAddress->setVisible(false);
        ui->leBasicStartAddress->setVisible(true);
        ui->lblBasicStub->setVisible(false);
        ui->lblProgramStartAddress->setVisible(true);
        ui->lblProgramStartAddress->setText("Program start address");
    }
    else {
        ui->leProgramStartAddress->setVisible(true);
        ui->leBasicStartAddress->setVisible(false);
        ui->lblBasicStub->setVisible(true);
        ui->lblProgramStartAddress->setVisible(false);
        ui->lblBasicStub->setText("Program start address");

    }

}

void DialogProjectSettings::on_chkIgnoreBasic_clicked(bool checked)
{
    //    ui->leBasicStartAddress->setEnabled(!checked);
    //    ui->leBasicStartAddress->setVisible(!checked);
}

void DialogProjectSettings::CreateCombobox(QStringList lst, QString name, int r, int c)
{
    QComboBox* cmbPaw = new QComboBox();
    cmbPaw->addItems(lst);
    cmbPaw->setCurrentText(m_ini->getString(name));
    ui->grdDisks->addWidget(cmbPaw,r,c);

    QObject::connect(cmbPaw, &QComboBox::currentTextChanged, [=]() {
        m_ini->setString(name,cmbPaw->currentText());
    });

}

// Add disk
void DialogProjectSettings::on_pushButton_2_clicked()
{
    int disk = 1;
    while (m_ini->contains("disk"+QString::number(disk)+"_paw"))
        disk+=1;
    QString d = "disk"+QString::number(disk);
    m_ini->setString(d+"_paw","");
    m_ini->setString(d+"_flf","");
    m_ini->setString(d+"_type","");
    FillFromIni();
    FillToIni();
}

void DialogProjectSettings::PopulateDiskList()
{
    int disk = 1;
    Util::clearLayout(ui->grdDisks);
    int row=0;
    while (m_ini->contains("disk"+QString::number(disk)+"_paw")) {
        QString d = "disk"+QString::number(disk);

        ui->grdDisks->addWidget(new QLabel("Disk "+QString::number(disk)),row,0);
        ui->grdDisks->addWidget(new QLabel("Name"+QString::number(disk)),row,1);

        ui->grdDisks->addWidget(new QLabel("Paw"),row,3);
        ui->grdDisks->addWidget(new QLabel("Dirart"),row,5);
        ui->grdDisks->addWidget(new QLabel("Type"),row,7);

        //        cmb->setFocusPolicy( Qt::StrongFocus );
        //      cmb->installEventFilter(m_eventFilter);
        QStringList flf = QStringList()<<"none"<<Util::FindFilesOfType(m_currentDir+"/","*.flf");
        QStringList paw = Util::FindFilesOfType(m_currentDir+"/","*.paw");
        //    qDebug() << ras << m_currentDir;

        QLineEdit* le=new QLineEdit();
        le->setText(m_ini->getString(d+"_name"));
        ui->grdDisks->addWidget(le,row,2);
        QObject::connect(le, &QLineEdit::textChanged, [=]() {
            m_ini->setString(d+"_name",le->text());
        });


        CreateCombobox(paw,d+"_paw",row,4);
        CreateCombobox(flf,d+"_flf",row,6);
        CreateCombobox(QStringList()<<"d64"<<"d81",d+"_type",row,8);


        disk+=1;
        row+=1;

    }

}

// delete disk
void DialogProjectSettings::on_pushButton_3_clicked()
{
    int disk = 1;
    if (!m_ini->contains("disk1_paw"))
        return;

    while (m_ini->contains("disk"+QString::number(disk)+"_paw"))
        disk+=1;

    QString d = "disk"+QString::number(disk-1);
    m_ini->remove(d+"_paw");
    m_ini->remove(d+"_flf");
    m_ini->remove(d+"_name");
    m_ini->remove(d+"_type");
    PopulateDiskList();

}

