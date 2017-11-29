#pragma once

#include <QDialog>

namespace Ui
{
    class DialogChangeBioAndPhoto;
}

class DialogChangeBioAndPhoto : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChangeBioAndPhoto(QWidget *parent = nullptr);
    ~DialogChangeBioAndPhoto();

private slots:
    void on_toolButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::DialogChangeBioAndPhoto *ui;
};
