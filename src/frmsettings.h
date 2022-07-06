/**
 * @file frmsettings.h
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef FRMSETTINGS_H
#define FRMSETTINGS_H

#include <QWidget>
#include <QtGlobal>
#include <memory>
#include "GameSettings.hpp"

using namespace std;

namespace Ui {
class frmSettings;
}

class frmSettings : public QWidget
{
    Q_OBJECT

public:
    explicit frmSettings(QWidget *parent, QSharedPointer<GameSettings>& config);
    ~frmSettings();
    void connectAllSignals();

public slots:
    void OnButtonSave_Clicked(bool b);
    void OnSliderMoved(int v);

private:
    Ui::frmSettings *ui;
    QSharedPointer<GameSettings> configAlias;
};


#endif // FRMSETTINGS_H
