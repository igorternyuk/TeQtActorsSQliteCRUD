#include "dialogchangebioandphoto.hpp"
#include "ui_dialogchangebioandphoto.h"

DialogChangeBioAndPhoto::DialogChangeBioAndPhoto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChangeBioAndPhoto)
{
    ui->setupUi(this);
}

DialogChangeBioAndPhoto::~DialogChangeBioAndPhoto()
{
    delete ui;
}

void DialogChangeBioAndPhoto::on_toolButton_clicked()
{

}

void DialogChangeBioAndPhoto::on_buttonBox_accepted()
{

}

void DialogChangeBioAndPhoto::on_buttonBox_rejected()
{

}
