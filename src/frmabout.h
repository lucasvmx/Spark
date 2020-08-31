/**
 * @file frmabout.h
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief Janela 'sobre'
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef FRMABOUT_H
#define FRMABOUT_H

#include <QWidget>

namespace Ui {
class frmAbout;
}

class frmAbout : public QWidget
{
    Q_OBJECT

public:
    explicit frmAbout(QWidget *parent = 0);
    ~frmAbout();
    void connectSignals();

public slots:
    void OnButtonDownloadWarzone_Clicked(bool bClicked);

private:
    Ui::frmAbout *ui;
};

#endif // FRMABOUT_H
