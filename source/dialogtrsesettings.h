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

#ifndef DIALOGTRSESETTINGS_H
#define DIALOGTRSESETTINGS_H

#include <QDialog>
#include "source/LeLib/util/cinifile.h"
#include "dialogabout.h"
#include <QFontDatabase>
#include <QComboBox>
#include "dialogsimplelineedit.h"

namespace Ui {
class DialogTRSESettings;
}

class DialogTRSESettings : public QDialog
{
    Q_OBJECT


public:
    explicit DialogTRSESettings(QWidget *parent = 0);

    QSharedPointer<CIniFile> m_ini;
    void SetInit(QSharedPointer<CIniFile> ini) {
        m_ini = ini;
        FillFromIni();
    }

    QString fromStringList(QStringList lst) {
        QString whole ="";
        for (QString s: lst) {
            whole+=s;
            if (lst.last()!=s)
                whole=whole+", ";
        }
        return whole;
    }

    QStringList toStringList(QString s) {
        QStringList lst = s.split(",");
        return lst;
    }


    void FillFromIni();
    void FillToIni();

    ~DialogTRSESettings();

    void FillFont(QComboBox* cbmFont, QString keep);
    void BuildEmulatorHelp();
    void Help(QString tit, QString text);

    void SetupExtras();

private slots:
    void on_pushButton_clicked();

    void on_btnEmulator_clicked();

    void on_btnDasm_clicked();

    void on_btnExomizer_clicked();

    void on_btnEmulatorC128_clicked();

    void on_btnVic20Emulator_clicked();

    void on_btnNesEmulatr_clicked();

    void on_btnC1541Emulator_clicked();

    void on_btnTinyCrunch_clicked();

    void on_btnVasmm_clicked();

    void on_btn68kTargetDir_clicked();

    void on_btnOKEmulator_clicked();

    void on_btnPlus4Select_clicked();

    void on_btnX16Emulator_clicked();

    void on_cbmFont_currentIndexChanged(const QString &arg1);

    void on_btnDosbox_clicked();

    void on_btnNasm_clicked();

    void on_btnPetEmulator_clicked();

    void on_cmbFontSymbols_currentIndexChanged(const QString &arg1);

    void on_btnRgbAsmDir_clicked();

    void on_btnGameboyEmulator_clicked();

    void on_btnZXSpectrumEmulator_clicked();

    void on_btnPasmo_clicked();

    void on_btnTiki100_clicked();


    void on_btnHelpC64_clicked();



    void on_btnHelpC128_clicked();

    void on_btnHelpVIC20_clicked();

    void on_btnHelpPET_clicked();

    void on_btnHelpPlus4_clicked();

    void on_btnHelpNES_clicked();

    void on_btnHelpGB_clicked();

    void on_btnHelpZX_clicked();

    void on_btnHelpTiki_clicked();

    void on_btnHelpDosbox_clicked();

    void on_btnHelpOK64_clicked();

    void on_btnHelpAtari_clicked();

    void on_btnAtari2600_clicked();

    void on_btn68kTargetDirAtariST_clicked();

    void on_btnAmstradCMC464_clicked();

    void on_btnHelpAmstradCPC464_clicked();

    void on_btnColecoEmulator_clicked();

    void on_btnCPCDIsk_clicked();

    void on_btnSidPlayer_clicked();

    void on_btnMega65Emulator_clicked();

private:
    Ui::DialogTRSESettings *ui;
};

#endif // DIALOGTRSESETTINGS_H
