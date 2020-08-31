/**
 * @file frmabout.cpp
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief Janela 'sobre' do programa
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "frmabout.h"
#include "ui_frmabout.h"
#include "version.h"
#include <QMovie>
#include <windows.h>

frmAbout::frmAbout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmAbout)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->connectSignals();
    this->setWindowTitle(QString("Sobre o %1").arg(PROGNAME));

    QMovie *movie = new QMovie(":/images/lulzsec.gif");
    ui->movieLabel->setMovie(movie);
    movie->start();
}

frmAbout::~frmAbout()
{
    delete ui;
}

void frmAbout::connectSignals()
{
    QObject::connect(ui->buttonDownloadWarzone, SIGNAL(clicked(bool)),this,SLOT(OnButtonDownloadWarzone_Clicked(bool)));
}

void frmAbout::OnButtonDownloadWarzone_Clicked(bool bClicked)
{
    (void)bClicked;

    ShellExecuteW(nullptr, L"open", L"https://sourceforge.net/projects/warzone2100/", nullptr, nullptr,0);
}
