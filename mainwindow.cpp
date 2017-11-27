#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "db_manager.hpp"
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>
#include <QPixmap>
#include "dialogcountry.h"
#include "dialoginsertactor.hpp"
#include "comboboxsqlmodel.hpp"
#ifdef DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("TeQtActors");
    mDB = new DBManager(this);
    mDialogCountry = new DialogCountry(this);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->addAction(ui->action_insert_new_actor);
    ui->tableView->addAction(ui->action_remove_selected_actor);
    ui->tableView->addAction(ui->action_reload_list_of_actors);
    ui->tableView->hideColumn(DBManager::ActorColumns::IMAGE);
    ui->lblImage->setText("<b>No picture</b>");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_new_database_triggered()
{
    openDBFile(true);
}

void MainWindow::on_action_open_database_triggered()
{
    openDBFile(false);
}

void MainWindow::on_action_quit_triggered()
{
    auto answer = QMessageBox::question(this, "Confirm exit",
                                        "Do you really want to exit?",
                                         QMessageBox::Yes | QMessageBox::No);
    if(answer == QMessageBox::Yes)
        this->close();
}

void MainWindow::openDBFile(bool isNewFile)
{
    QString startLocation = QStandardPaths::standardLocations(QStandardPaths::HomeLocation)
                            .value(0, QDir::homePath());
    QString filePath;
    if(isNewFile)
    {
        filePath = QFileDialog::getSaveFileName(this, "Select path to database file",
                                                startLocation,
                                                "Databases (*.db);;All files (*.*)");
    }
    else
    {
        filePath = QFileDialog::getOpenFileName(this, "Select database file",
                                                startLocation,
                                                "Databases (*.db);;All files (*.*)");
    }

    if(filePath.isEmpty()) return;
    QString errMsg;
    if(!mDB->newDatabase(filePath, errMsg))
    {
        QMessageBox::critical(this, "Error", errMsg);
        return;
    }

    if(!mDB->configurateDB(errMsg))
    {
        QMessageBox::critical(this, "Error", errMsg);
        return;
    }

    delete mModelCountry;
    mModelCountry = new QSqlTableModel(this);
    mModelCountry->setTable("country");
    mModelCountry->select();
    mDialogCountry->setModel(mModelCountry);

    delete mModelActor;
    mModelActor = new QSqlRelationalTableModel(this);
    mModelActor->setTable("actor");
    mModelActor->setHeaderData(DBManager::ActorColumns::ID, Qt::Horizontal, tr("ID"));
    mModelActor->setHeaderData(DBManager::ActorColumns::NAME, Qt::Horizontal, tr("Name"));
    mModelActor->setHeaderData(DBManager::ActorColumns::DOB, Qt::Horizontal, tr("Date of birth"));
    mModelActor->setHeaderData(DBManager::ActorColumns::BIO, Qt::Horizontal, tr("Biography"));
    mModelActor->setHeaderData(DBManager::ActorColumns::ID_COUNTRY, Qt::Horizontal, tr("Country"));
    //mModelActor->setHeaderData(DBManager::ActorColumns::IMAGE, Qt::Horizontal, tr("Image"));
    //mModel->setRelation(EDITION, QSqlRelation("edition", "id", "name"));
    mModelActor->setRelation(DBManager::ActorColumns::ID_COUNTRY, QSqlRelation("country", "id", "name"));
    mModelActor->select();
    ui->tableView->setModel(mModelActor);
    ui->tableView->hideColumn(DBManager::ActorColumns::IMAGE);
    delete mDelegate;
    mDelegate = new QSqlRelationalDelegate(this);
    ui->tableView->setItemDelegate(mDelegate);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::on_action_insert_new_actor_triggered()
{
    if(!mModelCountry || !mModelActor)
    {
        QMessageBox::critical(this, "Error", "No acive database connections");
        return;
    }

    DialogInsertActor dialog(mDB->getComboCountryModel(), this);
    if(dialog.exec() == QDialog::Rejected) return;
    QString errorMessage;
    if(!mDB->insertActor(dialog.actor(), errorMessage))
    {
        QMessageBox::critical(this, "Error", errorMessage);
    }
    else
    {
        mModelActor->select();
        QMessageBox::information(this, "Success",
                                 QString("Actor %1 was successfully inserted"
                                         " to the database")
                                 .arg(dialog.actor().name()));
    }
}

void MainWindow::on_action_remove_selected_actor_triggered()
{
    auto answer = QMessageBox::question(this, "Confirm deleting",
                                        "Do you really qant to delete"
                                        " selected actor?",
                                        QMessageBox::Yes | QMessageBox::No);
    if(answer == QMessageBox::Yes)
    {
        const int row = ui->tableView->currentIndex().row();
        if(mModelCountry)
        {
            mModelActor->removeRow(row);
            mModelActor->select();
        }
    }
}

void MainWindow::on_action_reload_list_of_actors_triggered()
{
    mModelActor->select();
}

void MainWindow::on_action_edit_countries_triggered()
{
    mDialogCountry->show();
}

void MainWindow::on_action_change_image_triggered()
{

}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if(!index.isValid() || !mModelCountry || !mModelActor) return;
    qDebug() << "Trying to load picture";
    const int id = mModelActor->index(index.row(), 0).data().toInt();
    qDebug() << "id = " << id;
    Actor actor = mDB->getActorById(id);
    qDebug() << "name = " << actor.name();
    ui->txtBio->setPlainText(actor.bio());
    QPixmap pixmap;
    if(!pixmap.loadFromData(actor.image()))
    {
        ui->lblImage->setText("<b>No picture</b>");
    }
    else
    {
        ui->lblImage->setPixmap(pixmap);
    }
}
