#include "dialoginsertactor.hpp"
#include "ui_dialoginsertactor.h"
#include "comboboxsqlmodel.hpp"
#include <QStandardPaths>
#include <QDir>
#include <QDate>
#include <QFile>
#include <QFileDialog>
#include <QCompleter>
#include <QFileSystemModel>
#include <QMessageBox>


DialogInsertActor::DialogInsertActor(ComboBoxSqlModel *comboModel,
                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInsertActor),
    mComboModel { comboModel }
{
    ui->setupUi(this);
    this->setWindowTitle("Insert new actor");
    comboModel->reload();
    ui->comboCountry->setModel(mComboModel);
    auto fileSystemModel = new QFileSystemModel(this);
    auto rootPath = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)
                    .value(0, QDir::homePath());
    fileSystemModel->setRootPath(rootPath);
    ui->txtName->setCompleter(new QCompleter(fileSystemModel, this));
}

DialogInsertActor::~DialogInsertActor()
{
    delete ui;
}

void DialogInsertActor::on_buttonBox_accepted()
{
    QFile file(ui->txtPathToImage->text());
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", file.errorString());
        return;
    }
    QString name = ui->txtName->text();
    QString dob = ui->dateEditDOB->date().toString(Qt::ISODate);
    QString bio = ui->txtBiography->toPlainText();
    int idCountry { ui->comboCountry->itemData( ui->comboCountry->currentIndex(),
                                                Qt::UserRole ).toInt() };
    auto image = file.readAll();
    /*explicit Actor(const QString &name, const QString &date,
                   const QString &bio, int idCountry,
                   const QByteArray &image = QByteArray());*/
    mActor = Actor(name, dob, bio, idCountry, image);
    accept();
}

void DialogInsertActor::on_buttonBox_rejected()
{
    reject();
}

Actor DialogInsertActor::actor() const
{
    return mActor;
}

void DialogInsertActor::on_btnLoadImage_clicked()
{
    QString startLocation
    {
        QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)
                .value(0, QDir::homePath())
    };
    QString filter { "Databases (*.jpg *.jpeg *.png *.bmp);;All files (*.*)" };
    auto filePath = QFileDialog::getOpenFileName(this, "Select a picture",
                                                 startLocation, filter);
    ui->txtPathToImage->setText(filePath);
}
