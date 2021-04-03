#include "frmlanguage.h"
#include "ui_frmlanguage.h"

frmLanguage::frmLanguage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmLanguage)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Please select your language"));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onIndexChanged(int)));
}

frmLanguage::~frmLanguage()
{
    delete ui;
}

void frmLanguage::onIndexChanged(int index)
{
    this->SetSelectedLanguage(static_cast<Ui::languages>(index));
    close();
}

Ui::languages frmLanguage::GetSelectedLanguage()
{
    return this->selectedLang;
}


void frmLanguage::SetSelectedLanguage(Ui::languages lang)
{
    this->selectedLang = lang;
}
