/*
    Hack para o warzone 2100

    Versões suportadas:
        * 2.3.9
        * 3.1.5
        * 3.2.3

    Características:
        * Energia infinita
        * Easter egg

    Descrição:
        Este programa permite que o jogador possua uma energia sempre acima do valor
        que ele quiser. Com esta ferramenta você pode, por exemplo, fazer com que
        nunca a sua energia esteja abaixo de 60000 por mais de 1 minuto.

    Aviso:
        Se for compilar no Visual Studio, ative a opção 'MultiByte'

    Autor:
        Lucas Vieira de Jesus <lucas.engen.cc@gmail.com>

    Testado no:
        Microsoft Windows [versão 10.0.16299.125] x64
*/

#include "frmabout.h"
#include "ui_frmabout.h"
#include <QMovie>
#include <windows.h>

frmAbout::frmAbout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmAbout)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->connectSignals();

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
