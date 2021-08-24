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

#include "dialogtrsesettings.h"
#include "ui_dialogtrsesettings.h"
#include "source/LeLib/util/util.h"
#include <QFileDialog>
#include "source/LeLib/util/util.h"
#include "dialoghelp.h"
#include <QStringList>
#include <QDir>


DialogTRSESettings::DialogTRSESettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTRSESettings)
{
    ui->setupUi(this);

    SetupExtras();

}

void DialogTRSESettings::FillFromIni()
{
    FillFont(ui->cbmFont, m_ini->getString("editor_font"));
    FillFont(ui->cmbFontSymbols, m_ini->getString("editor_font_symbols"));

    ui->leDosbox->setText(m_ini->getString("dosbox"));
    ui->leVasmm->setText(m_ini->getString("vasmm"));
    ui->leNasm->setText(m_ini->getString("nasm"));
    ui->le68kTargetDir->setText(m_ini->getString("vasmm_target_dir"));
    ui->le68kTargetDirAtariST->setText(m_ini->getString("vasmm_target_dir_atarist"));
    ui->leDasm->setText(m_ini->getString("dasm"));
    ui->leEmulator->setText(m_ini->getString("emulator"));
    ui->leEmulatorC128->setText(m_ini->getString("c128_emulator"));
    ui->leVZ200Emulator->setText(m_ini->getString("vz200_emulator"));
    ui->leVic20Emulator->setText(m_ini->getString("vic20_emulator"));
    ui->lePETEmulator->setText(m_ini->getString("pet_emulator"));
    ui->leNesEmulator->setText(m_ini->getString("nes_emulator"));
    ui->leMega65Emu->setText(m_ini->getString("mega65_emulator"));
    ui->leAtari2600Emulator->setText(m_ini->getString("atari2600_emulator"));
    ui->leGameboyEmulator->setText(m_ini->getString("gameboy_emulator"));
    ui->leOK64Emulator->setText(m_ini->getString("ok64_emulator"));
    ui->leBBCEmulator->setText(m_ini->getString("bbc_emulator"));
    ui->leAtari800Emulator->setText(m_ini->getString("atari800_emulator"));
    ui->lePlus4Emulator->setText(m_ini->getString("plus4_emulator"));
    ui->leX16Emu->setText(m_ini->getString("x16_emulator"));
    ui->leMSXEmulator->setText(m_ini->getString("msx_emulator"));
    ui->leAppleIIEmulator->setText(m_ini->getString("appleii_emulator"));
    ui->leOricEmulator->setText(m_ini->getString("oric_emulator"));
    ui->leSNES->setText(m_ini->getString("snes_emulator"));
    ui->leAmstradCPC->setText(m_ini->getString("amstradcpc_emulator"));
    ui->leColecoEmulator->setText(m_ini->getString("coleco_emulator"));
    ui->leQemuDir->setText(m_ini->getString("qemu_directory"));
    //ui->leX16EmuParams->setText(m_ini->getString("x16_emulator_params"));
    ui->leC1541->setText(m_ini->getString("c1541"));
    ui->lePasmo->setText(m_ini->getString("pasmo"));
    ui->leCL65->setText(m_ini->getString("cl65"));
    ui->leZXSpectrumEmulator->setText(m_ini->getString("spectrum_emulator"));
    ui->leTiki100->setText(m_ini->getString("tiki100_emulator"));
    ui->leExomizer->setText(m_ini->getString("exomizer"));
    ui->leRGBAsmDir->setText(m_ini->getString("gameboy_rgbasm_dir"));
    ui->leTinyCrunch->setText(m_ini->getString("tinycrunch"));
    ui->leCursorWidth->setText(QString::number((int)m_ini->getdouble("editor_cursor_width")));
    ui->chkAutoInject->setChecked(m_ini->getdouble("auto_inject")==1.0);
    ui->leBackupFiles->setText(QString::number((int)m_ini->getdouble("backup_files_count")));

    ui->leLZ4->setText(m_ini->getString("lz4"));

    ui->leSplashSeconds->setText(QString::number((int)m_ini->getdouble("splash_seconds")));

    ui->leSidplayer->setText(m_ini->getString("sidplayer"));

    ui->chkBackup->setChecked(m_ini->getdouble("auto_backup")==1.0);
    ui->chkAutoIndent->setChecked(m_ini->getdouble("editor_autocomplete")==1.0);
    ui->chkAutoComplete->setChecked(m_ini->getdouble("editor_autocomplete_nobug")==1.0);
    ui->chkCompileThread->setChecked(m_ini->getdouble("compile_thread")==1.0);

    ui->leCPCDisk->setText(m_ini->getString("cpcdisk_location"));

    ui->chkExomizer->setChecked(m_ini->getdouble("exomizer_version_3")==1.0);

    ui->chkDisableFileColors->setChecked(m_ini->getdouble("disable_file_colors")==1.0);
    ui->chkDisplayAddresses->setChecked(m_ini->getdouble("display_addresses")==1.0);

//    if (ui->cmbPalette->currentText()=="Dark")
    if (m_ini->getdouble("windowpalette")==0)
        ui->cmbPalette->setCurrentText("Dark");
    if (m_ini->getdouble("windowpalette")==1)
        ui->cmbPalette->setCurrentText("Light");


    ui->leKeywords->setText( Util::toString(m_ini->getStringList("custom_keyword_list")));
    ui->leColour->setText( Util::toString(m_ini->getStringList("custom_keyword_colour")));
    ui->chkBold->setChecked(m_ini->getdouble("custom_keyword_bold")==1.0);
    ui->chkItalic->setChecked(m_ini->getdouble("custom_keyword_italic")==1.0);
    QDir directory(Util::path+"themes/");
    QStringList  themes = directory.entryList(QStringList() << "*.ini");
    ui->cmbTheme->clear();
  //  qDebug() << "themes : " << themes;
    QStringList washed;
    for (QString s: themes) {
        QString name = Util::getFileWithoutEnding(s);
    //    qDebug() << name;
        washed << name;
    }
    ui->cmbTheme->addItems(washed);
    ui->cmbTheme->setCurrentText(m_ini->getString("theme").split(".")[0]);

    ui->cmbThemeFjong->addItems(washed);
    ui->cmbThemeFjong->setCurrentText(m_ini->getString("theme_fjong").split(".")[0]);


    ui->leFontSize->setText(QString::number((int)m_ini->getdouble("font_size")));
    ui->leFontScale->setText(QString::number(m_ini->getdouble("font_size_scale")));
    ui->leFontSizeSymbols->setText(QString::number((int)m_ini->getdouble("font_size_symbols")));
    ui->leTabWidth->setText(QString::number((int)m_ini->getdouble("tab_width")));

    ui->leFontSizeMemoryAnalyzer->setText(QString::number((int)m_ini->getdouble("memory_analyzer_font_size")));
    ui->chkExomizerFootprint->setChecked(m_ini->getdouble("hide_exomizer_footprint")==1.0);
    //ui->chkRemoveUnusedProcedures->setChecked( m_ini->getdouble("optimizer_remove_unused_symbols")==1.0);

    ui->leUserDefined->setText(m_ini->getString("user_defined_command"));
    ui->leUserDefinedParameters->setText(m_ini->getString("user_defined_command_parameters"));

    ui->cmbAssembler->setCurrentText(m_ini->getString("assembler"));
    ui->cmbAssemblerZ80->setCurrentText(m_ini->getString("assembler_z80"));
    ui->cmbPainter->setCurrentIndex((int)m_ini->getdouble("image_painter"));

}


void DialogTRSESettings::FillToIni()
{


    m_ini->setFloat("editor_cursor_width",ui->leCursorWidth->text().toInt());
    m_ini->setString("vasmm_target_dir", ui->le68kTargetDir->text());
    m_ini->setString("vasmm_target_dir_atarist", ui->le68kTargetDirAtariST->text());
    m_ini->setString("vasmm", ui->leVasmm->text());
    m_ini->setString("nasm", ui->leNasm->text());
    m_ini->setString("dosbox", ui->leDosbox->text());
    m_ini->setString("dasm", ui->leDasm->text());
    m_ini->setString("exomizer", ui->leExomizer->text());
    m_ini->setString("tinycrunch", ui->leTinyCrunch->text());
    m_ini->setString("emulator", ui->leEmulator->text());
    m_ini->setString("vic20_emulator", ui->leVic20Emulator->text());
    m_ini->setString("pet_emulator", ui->lePETEmulator->text());
    m_ini->setString("c128_emulator", ui->leEmulatorC128->text());
    m_ini->setString("vz200_emulator", ui->leVZ200Emulator->text());
    m_ini->setString("atari2600_emulator", ui->leAtari2600Emulator->text());
    m_ini->setString("ok64_emulator", ui->leOK64Emulator->text());
    m_ini->setString("bbc_emulator", ui->leBBCEmulator->text());
    m_ini->setString("atari800_emulator", ui->leAtari800Emulator->text());
    m_ini->setString("msx_emulator", ui->leMSXEmulator->text());
    m_ini->setString("appleii_emulator", ui->leAppleIIEmulator->text());
    m_ini->setString("oric_emulator", ui->leOricEmulator->text());
    m_ini->setString("snes_emulator", ui->leSNES->text());
    m_ini->setString("plus4_emulator", ui->lePlus4Emulator->text());
    m_ini->setString("x16_emulator", ui->leX16Emu->text());
    m_ini->setString("amstradcpc_emulator", ui->leAmstradCPC->text());
    m_ini->setString("qemu_directory",ui->leQemuDir->text());
    m_ini->setString("coleco_emulator", ui->leColecoEmulator->text());
    m_ini->setString("lz4",ui->leLZ4->text());
    //m_ini->setString("x16_emulator_params", ui->leX16EmuParams->text());
    m_ini->setString("c1541", ui->leC1541->text());

    m_ini->setString("spectrum_emulator", ui->leZXSpectrumEmulator->text());
    m_ini->setString("tiki100_emulator", ui->leTiki100->text());
    m_ini->setString("pasmo", ui->lePasmo->text());
    m_ini->setString("cl65", ui->leCL65->text());


    m_ini->setString("sidplayer",ui->leSidplayer->text());

    m_ini->setString("cpcdisk_location",ui->leCPCDisk->text());

    m_ini->setFloat("backup_files_count",ui->leBackupFiles->text().toInt());

    m_ini->setFloat("splash_seconds",ui->leSplashSeconds->text().toInt());


    m_ini->setFloat("display_addresses", ui->chkDisplayAddresses->isChecked());
    m_ini->setString("gameboy_rgbasm_dir", ui->leRGBAsmDir->text());


    m_ini->setString("mega65_emulator", ui->leMega65Emu->text());

    m_ini->setString("nes_emulator", ui->leNesEmulator->text());
    m_ini->setString("gameboy_emulator", ui->leGameboyEmulator->text());
    m_ini->setString("theme", ui->cmbTheme->currentText() + ".ini");
    m_ini->setString("theme_fjong", ui->cmbThemeFjong->currentText() + ".ini");

    m_ini->setFloat("font_size", ui->leFontSize->text().toInt());
    m_ini->setFloat("font_size_scale", ui->leFontScale->text().toFloat());
    m_ini->setFloat("font_size_symbols", ui->leFontSizeSymbols->text().toInt());
    m_ini->setFloat("tab_width", ui->leTabWidth->text().toInt());
    m_ini->setFloat("memory_analyzer_font_size", ui->leFontSizeMemoryAnalyzer->text().toInt());

    m_ini->setFloat("hide_exomizer_footprint", ui->chkExomizerFootprint->isChecked()?1:0);

    m_ini->setFloat("image_painter",ui->cmbPainter->currentIndex());

    m_ini->setFloat("auto_inject", ui->chkAutoInject->isChecked()?1:0);
    m_ini->setFloat("auto_backup", ui->chkBackup->isChecked()?1:0);
    m_ini->setFloat("editor_autocomplete", ui->chkAutoIndent->isChecked()?1:0);
    m_ini->setFloat("editor_autocomplete_nobug", ui->chkAutoComplete->isChecked()?1:0);
    m_ini->setFloat("compile_thread", ui->chkCompileThread->isChecked()?1:0);

    m_ini->setString("user_defined_command",ui->leUserDefined->text());
    m_ini->setString("user_defined_command_parameters",ui->leUserDefinedParameters->text());

    m_ini->setFloat("disable_file_colors", ui->chkDisableFileColors->isChecked()?1:0);

    m_ini->setFloat("exomizer_version_3", ui->chkExomizer->isChecked()?1:0);




//    m_ini->setFloat("optimizer_remove_unused_symbols", ui->chkRemoveUnusedProcedures->isChecked()?1:0);

    if (ui->cmbPalette->currentText()=="Dark")
        m_ini->setFloat("windowpalette",0);
    if (ui->cmbPalette->currentText()=="Light")
        m_ini->setFloat("windowpalette",1);


    m_ini->setString("assembler", ui->cmbAssembler->currentText());
    m_ini->setString("assembler_z80", ui->cmbAssemblerZ80->currentText());

    m_ini->setStringList("custom_keyword_list", ui->leKeywords->text().split(","));
    m_ini->setStringList("custom_keyword_colour", ui->leColour->text().split(","));
    m_ini->setFloat("custom_keyword_bold",ui->chkBold->isChecked());
    m_ini->setFloat("custom_keyword_italic",ui->chkItalic->isChecked());



}

DialogTRSESettings::~DialogTRSESettings()
{
    delete ui;
}

void DialogTRSESettings::FillFont(QComboBox *cbmFont, QString keep) {
    //        QString keep = m_ini->getString("editor_font");
    cbmFont->clear();
    QFontDatabase d;
    QStringList fam = d.families();
    if (!fam.contains("Courier"))
        fam<<"Courier";
    cbmFont->addItems(fam);
    cbmFont->setCurrentText(keep);


}

void DialogTRSESettings::Help(QString tit, QString text)
{
    DialogInfo* da = new DialogInfo();
//    da->SetMessage(text, QImage());
    da->SetText(tit,text);
    da->exec();

    delete da;

}

void DialogTRSESettings::SetupExtras()
{
    QStringList data;
    data<<"C64"<<"C128"<<"VIC20"<<"PET"<<"PLUS4"<<"NES"<<"GAMEBOY"<<"SPECTRUM"<<"MSX" << "COLECO"<<"AMSTRADCPC"<<"ATARI2600"<<"TIKI100"<<"X86" << "OK64" << "X16" <<"MEGA65"<<"BBCM" <<"ATARI800" <<"APPLEII" <<"ORIC"<<"SNES"<<"VZ200"<<"QEMU" ;
    for (int i=0;i<ui->grdEmulators->rowCount();i++) {
        if (data[i]=="QEMU")
            continue;
        QPushButton* btn = new QPushButton("params");
        ui->grdEmulators->addWidget(btn,i,5);
        QString name = "emulator_additional_parameters_"+data[i];
        connect(btn, &QPushButton::clicked,  [=](){
            DialogSimpleLineEdit* de = new DialogSimpleLineEdit(data[i]+" emulator additional params",
                                                                "Additional parameters for the emulator.",
                                                                m_ini->getString(name)
                                                                );
            de->exec();
            if (de->m_ok)
                m_ini->setString(name,de->getValue().trimmed());

            SetupExtras();
        });
    }
}


void DialogTRSESettings::on_pushButton_clicked()
{
    FillToIni();
    m_ini->Save();
    close();
}

void DialogTRSESettings::on_btnEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("C64 Emulator location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leEmulator->setText(filename);

}

void DialogTRSESettings::on_btnExomizer_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Exomizer location"), m_ini->getString("project_path"), "*");
    if (filename!="")
    ui->leExomizer->setText(filename);

}

void DialogTRSESettings::on_btnDasm_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Dasm location"), m_ini->getString("project_path"), "*");
    if (filename!="")
    ui->leDasm->setText(filename);

}

void DialogTRSESettings::on_btnEmulatorC128_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("C128 Emulator location"), m_ini->getString("project_path"), "*");
    if (filename!="")
    ui->leEmulatorC128->setText(filename);

}

void DialogTRSESettings::on_btnVic20Emulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("VIC-20 Emulator location"), m_ini->getString("project_path"), "*");
    if (filename!="")
    ui->leVic20Emulator->setText(filename);

}

void DialogTRSESettings::on_btnNesEmulatr_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("NES Emulator location"), m_ini->getString("project_path"), "*");
    if (filename!="")
    ui->leNesEmulator->setText(filename);

}

void DialogTRSESettings::on_btnC1541Emulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("C1541 location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leC1541->setText(filename);

}

void DialogTRSESettings::on_btnTinyCrunch_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Tinycrunch script location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leTinyCrunch->setText(filename);

}

void DialogTRSESettings::on_btnVasmm_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Vasmm location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leVasmm->setText(filename);

}

void DialogTRSESettings::on_btn68kTargetDir_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Vasmm target directory"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->le68kTargetDir->setText(filename);

}

void DialogTRSESettings::on_btnOKEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("OK64 executable"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leOK64Emulator->setText(filename);

}


void DialogTRSESettings::on_btnPlus4Select_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Plus4 emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->lePlus4Emulator->setText(filename);

}


void DialogTRSESettings::on_btnX16Emulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("X16 emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leX16Emu->setText(filename);

}

void DialogTRSESettings::on_cbmFont_currentTextChanged(const QString &arg1)
{
    m_ini->setString("editor_font", arg1);
}

void DialogTRSESettings::on_btnDosbox_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Dosbox"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leDosbox->setText(filename);

}

void DialogTRSESettings::on_btnNasm_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("NASM location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leNasm->setText(filename);

}

void DialogTRSESettings::on_btnPetEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("PET emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->lePETEmulator->setText(filename);

}

void DialogTRSESettings::on_cmbFontSymbols_currentTextChanged(const QString &arg1)
{
    m_ini->setString("editor_font_symbols", arg1);

}

void DialogTRSESettings::on_btnRgbAsmDir_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(this,
        tr("RGBASM location"), m_ini->getString("project_path"));
    if (filename!="")
        ui->leRGBAsmDir->setText(filename);

}

void DialogTRSESettings::on_btnGameboyEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Gameboy emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leGameboyEmulator->setText(filename);

}

void DialogTRSESettings::on_btnZXSpectrumEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("ZX Spectrum emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leZXSpectrumEmulator->setText(filename);

}

void DialogTRSESettings::on_btnPasmo_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Pasmo assembler location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->lePasmo->setText(filename);

}

void DialogTRSESettings::on_btnTiki100_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Tiki100 emulator location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leTiki100->setText(filename);

}

void DialogTRSESettings::on_btnHelpC64_clicked()
{
    Help("C64 Emulator", "Your favorite C64 emulator. <br>We'd recommend using VICE : <a href=\"https://vice-emu.sourceforge.io/\">https://vice-emu.sourceforge.io/</a>");
}

void DialogTRSESettings::on_btnHelpC128_clicked()
{
    Help("C128 Emulator", "Your favorite C128 emulator. <br>We'd recommend using VICE : <a href=\"https://vice-emu.sourceforge.io/\">https://vice-emu.sourceforge.io/</a>");

}

void DialogTRSESettings::on_btnHelpVIC20_clicked()
{
    Help("VIC20 Emulator", "Your favorite VIC20 emulator. <br>We'd recommend using VICE : <a href=\"https://vice-emu.sourceforge.io/\">https://vice-emu.sourceforge.io/</a>");

}

void DialogTRSESettings::on_btnHelpPET_clicked()
{
    Help("Commodore PET Emulator", "Your favorite PET emulator. <br>We'd recommend using VICE : <a href=\"https://vice-emu.sourceforge.io/\">https://vice-emu.sourceforge.io/</a>");

}

void DialogTRSESettings::on_btnHelpPlus4_clicked()
{
    Help("PLUS4 Emulator", "Your favorite PLUS4 emulator. <br>We'd recommend using VICE : <a href=\"https://vice-emu.sourceforge.io/\">https://vice-emu.sourceforge.io/</a>");
}

void DialogTRSESettings::on_btnHelpNES_clicked()
{
    Help("NES Emulator", "Your favorite NES emulator. <br>We recommend using Mednafen : <a href=\"https://mednafen.github.io/\">https://mednafen.github.io/</a>");

}

void DialogTRSESettings::on_btnHelpGB_clicked()
{
    Help("Gameboy Emulator", "Your favorite Gameboy emulator. <br>We recommend using Mednafen (pretty): <a href=\"https://mednafen.github.io/\">https://mednafen.github.io/</a>. <br><br>If you plan to run on real hardware, a much more accurate emulator would be Gambatte : <a hef=\"https://sourceforge.net/projects/gambatte/\">https://sourceforge.net/projects/gambatte/</a>.");

}

void DialogTRSESettings::on_btnHelpZX_clicked()
{
    Help("ZX Spectrum Emulator", "Please use https://www.retrovirtualmachine.org/en/ ");
}

void DialogTRSESettings::on_btnHelpTiki_clicked()
{
    Help("TIKI 100 Emulator", "Your favorite Tiki 100 emulator! <br>We recommend using TIKI-Emul for windows <a href=\"http://www.djupdal.org/tiki/emulator/TIKI-100_emul_v1.1_win32.zip\">http://www.djupdal.org/tiki/emulator/TIKI-100_emul_v1.1_win32.zip</a> or compile the source for linux <a href=\"http://www.djupdal.org/tiki/emulator/TIKI-100_emul_v1.1-src.tgz\">http://www.djupdal.org/tiki/emulator/TIKI-100_emul_v1.1-src.tgz</a>. <br><br>Not that you really have any other choices in terms of selecting an emulator...");

}

void DialogTRSESettings::on_btnHelpDosbox_clicked()
{
    Help("DOSBOX Emulator", "Grab Dosbox from <a href=\"https://www.dosbox.com/\">https://www.dosbox.com/</a>");

}

void DialogTRSESettings::on_btnHelpOK64_clicked()
{
    Help("OK64 Emulator", "Since the OK64 is written by the developers of TRSE, it is included with the TRSE by default. ");
}

void DialogTRSESettings::on_btnHelpAtari_clicked()
{
    Help("Atari 2600 Emulator", "Your favorite Atari 2600 emulator! <br>We recommend using Stella <a href=\"https://stella-emu.github.io/\">https://stella-emu.github.io/</a>. Debian package 'stella' on linux (apt-get install stella).");

}

void DialogTRSESettings::on_btnAtari2600_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Atari 2600 emulator location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leAtari2600Emulator->setText(filename);

}

void DialogTRSESettings::on_btn68kTargetDirAtariST_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Atari ST target directory"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->le68kTargetDirAtariST->setText(filename);

}

void DialogTRSESettings::on_btnAmstradCPC_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Amstrad CPC emulator 'Caprice' location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leAmstradCPC->setText(filename);


}

void DialogTRSESettings::on_btnHelpAmstradCPC_clicked()
{
    Help("Amstrad CPC Emulator 'Caprice'", "TRSE needs to use the 'Caprice' emulator in order to be able to inject programs.");

}

void DialogTRSESettings::on_btnColecoEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("ColecoVision emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leColecoEmulator->setText(filename);


}

void DialogTRSESettings::on_btnCPCDIsk_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Amstrad CPCDiskXP.exe"), m_ini->getString("project_path"), "CPCDiskXP.exe");
    if (filename!="")
        ui->leCPCDisk->setText(filename);


}

void DialogTRSESettings::on_btnSidPlayer_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("SID player location"), m_ini->getString("project_path"), "");
    if (filename!="")
        ui->leSidplayer->setText(filename);


}

void DialogTRSESettings::on_btnMega65Emulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Mega 65 emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leMega65Emu->setText(filename);

}

void DialogTRSESettings::on_pushButton_2_clicked()
{
    Help("Mega65 emulator 'Xemu'", "TRSE needs to use the 'xemu' emulator in order to be able to inject programs. Download from https://github.lgb.hu/xemu/");

}

void DialogTRSESettings::on_btnBBCEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("BBC Emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leBBCEmulator->setText(filename);


}

void DialogTRSESettings::on_btnAtari800Emulator_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this,
        tr("Atari 800 Emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leAtari800Emulator->setText(filename);
}

void DialogTRSESettings::on_pushButton_4_clicked()
{
    Help("Atari 800 emulator","Download from : https://github.com/atari800/atari800/releases");
}

void DialogTRSESettings::on_pushButton_3_clicked()
{
    Help("BBC 'B2' emulator","Download from : https://github.com/tom-seddon/b2/releases");

}

void DialogTRSESettings::on_btnUserDefinedCommand_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("User-defined program"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leUserDefined->setText(filename);

}

void DialogTRSESettings::on_btnMSXEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("MSX Emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leMSXEmulator->setText(filename);

}

void DialogTRSESettings::on_btnHelpMSX_clicked()
{
    Help("MSX Emulator","For windows: http://www.emulator-zone.com/doc.php/msx/bluemsx.html. Macos : https://github.com/CocoaMSX/CocoaMSX. ");

}

void DialogTRSESettings::on_btnApple2Help_clicked()
{
    Help("Apple II emulator","For mac, use https://paleotronic.com/software/microm8/download/ (for now)");

}

void DialogTRSESettings::on_btnAppleII_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Apple II emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leAppleIIEmulator->setText(filename);

}

void DialogTRSESettings::on_btnLZ4_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("LZ4 executable"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leLZ4->setText(filename);

}

void DialogTRSESettings::on_btnQemuHelp_clicked()
{

}

void DialogTRSESettings::on_btnQemu_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Qemu directory"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leQemuDir->setText(filename);

}

void DialogTRSESettings::on_btnOric_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Oric emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leOricEmulator->setText(filename);

}


void DialogTRSESettings::on_btnSnes_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("SNES emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leSNES->setText(filename);


}


void DialogTRSESettings::on_btnCL65_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("CL65 assembler location"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leCL65->setText(filename);

}


void DialogTRSESettings::on_btnVZ200_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("VZ200 emulator"), m_ini->getString("project_path"), "*");
    if (filename!="")
        ui->leVZ200Emulator->setText(filename);

}


void DialogTRSESettings::on_btnVZ200Help_clicked()
{
    Help("Apple VZ200 emulator","Download from https://bluebilby.com/sdm_categories/tools/");

}

