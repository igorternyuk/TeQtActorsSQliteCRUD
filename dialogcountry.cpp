#include "dialogcountry.h"
#include "ui_dialogcountry.h"
#include <QSqlTableModel>
#include <QMessageBox>

DialogCountry::DialogCountry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCountry)
{
    ui->setupUi(this);
    this->setWindowTitle("Edit countries");

    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->addAction(ui->action_add_new_country);
    ui->tableView->addAction(ui->action_remove_selected_country);
    ui->tableView->setModel(mModel);

    connect(ui->btnNewCountry, &QPushButton::clicked, this,
            &DialogCountry::on_action_add_new_country_triggered);
    connect(ui->btnRemoveCountry, &QPushButton::clicked, this,
            &DialogCountry::on_action_remove_selected_country_triggered);
}

DialogCountry::~DialogCountry()
{
    delete ui;
}

void DialogCountry::on_action_add_new_country_triggered()
{
    if(mModel)
        mModel->insertRow(mModel->rowCount());
}


void DialogCountry::on_action_remove_selected_country_triggered()
{
    if(!mModel) return;
    auto answer = QMessageBox::question(this, "Confirm deleting",
                                        "Do you really want to delete selected country?",
                                        QMessageBox::Yes | QMessageBox::No);
    if(answer == QMessageBox::Yes)
    {
        const int row = ui->tableView->currentIndex().row();
        if(row == -1)
        {
            QMessageBox::critical(this, "Error",
                                  "You should select a country first");
            return;
        }
        mModel->removeRow(row);
        mModel->select();
        emit countryListUpdated();
    }
}

void DialogCountry::on_btnClose_clicked()
{
    close();
}

void DialogCountry::setModel(QSqlTableModel *model)
{
    mModel = model;
    ui->tableView->setModel(model);
}

void DialogCountry::on_btnReload_clicked()
{
    if(!mModel) return;
    mModel->select();
    emit countryListUpdated();
}
