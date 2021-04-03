#ifndef FRMLANGUAGE_H
#define FRMLANGUAGE_H

#include <QDialog>

namespace Ui {
class frmLanguage;
enum languages {
    PORTUGUESE,
    ENGLISH
};
}

class frmLanguage : public QDialog
{
    Q_OBJECT

public:
    explicit frmLanguage(QWidget *parent = nullptr);
    ~frmLanguage();
    Ui::languages GetSelectedLanguage();

public slots:
    void onIndexChanged(int index);

private:
    void SetSelectedLanguage(Ui::languages lang);
    Ui::frmLanguage *ui;
    Ui::languages selectedLang;
};

#endif // FRMLANGUAGE_H
