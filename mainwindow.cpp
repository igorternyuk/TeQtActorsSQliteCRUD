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
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QPushButton>
#include "dialogcountry.h"
#include "dialoginsertactor.hpp"
#include "dialogchangebioandphoto.hpp"
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
    mScene = new QGraphicsScene(this);
    mPixmap = new QGraphicsPixmapItem();
    ui->graphicsView->setScene(mScene);
    mScene->addItem(mPixmap);
    addSearchWidgetsToTheStatusBar();
    ui->graphicsView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->graphicsView->addAction(ui->action_change_photo);
}

MainWindow::~MainWindow()
{
    delete mPixmap;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Size = " << this->size();
    QMainWindow::resizeEvent(event);
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
    mModelActor->setHeaderData(DBManager::ActorColumns::ID,
                               Qt::Horizontal, tr("ID"));
    mModelActor->setHeaderData(DBManager::ActorColumns::NAME,
                               Qt::Horizontal, tr("Name"));
    mModelActor->setHeaderData(DBManager::ActorColumns::DOB,
                               Qt::Horizontal, tr("Date of birth"));
    mModelActor->setHeaderData(DBManager::ActorColumns::BIO,
                               Qt::Horizontal, tr("Biography"));
    mModelActor->setHeaderData(DBManager::ActorColumns::ID_COUNTRY,
                               Qt::Horizontal, tr("Country"));

    mModelActor->setRelation(DBManager::ActorColumns::ID_COUNTRY,
                             QSqlRelation("country", "id", "name"));

    ui->tableView->setModel(mModelActor);
    delete mDelegate;
    mDelegate = new QSqlRelationalDelegate(this);
    ui->tableView->setItemDelegate(mDelegate);
    mModelActor->select();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(DBManager::ActorColumns::IMAGE);
    ui->tableView->hideColumn(DBManager::ActorColumns::BIO);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mComboCountriesModel = mDB->getComboCountryModel();
    mComboCountry->setModel(mComboCountriesModel);
}

void MainWindow::addSearchWidgetsToTheStatusBar()
{    
    mLblName = new QLabel("Name", this);
    ui->statusBar->addPermanentWidget(mLblName);
    mLineEditSearch = new QLineEdit(this);
    ui->statusBar->addPermanentWidget(mLineEditSearch);
    mCheckBoxConsiderDOB = new QCheckBox("Consider DOB", this);
    ui->statusBar->addPermanentWidget(mCheckBoxConsiderDOB);
    mLblAgeMin = new QLabel("Min DOB");
    ui->statusBar->addPermanentWidget(mLblAgeMin);
    mSpinnerAgeMin = new QDateEdit(this);
    mSpinnerAgeMin->setDate(QDate::currentDate());
    ui->statusBar->addPermanentWidget(mSpinnerAgeMin);
    mLblAgeMax = new QLabel("Max DOB");
    ui->statusBar->addPermanentWidget(mLblAgeMax);
    mSpinnerAgeMax = new QDateEdit(this);
    mSpinnerAgeMax->setDate(QDate::currentDate());
    ui->statusBar->addPermanentWidget(mSpinnerAgeMax);
    mCheckBoxConsiderCountry = new QCheckBox("Consider country", this);
    ui->statusBar->addPermanentWidget(mCheckBoxConsiderCountry);
    mComboCountry = new QComboBox(this);

    ui->statusBar->addPermanentWidget(mComboCountry);
    mBtnSearch = new QPushButton("Search", this);
    connect(mBtnSearch, &QPushButton::clicked, this,
            &MainWindow::on_action_search_for_an_actor_triggered);
    ui->statusBar->addPermanentWidget(mBtnSearch);
    mBtnLoadFullList = new QPushButton("Load full list", this);
    connect(mBtnLoadFullList, &QPushButton::clicked, [this](){
        mModelActor->setFilter("");
        mModelActor->select();
    });
    ui->statusBar->addPermanentWidget(mBtnLoadFullList);
    mBtnReset = new QPushButton("Reset", this);
    connect(mBtnReset, &QPushButton::clicked, [this](){
        mLineEditSearch->setText("");
        mSpinnerAgeMin->setDate(QDate::currentDate());
        mSpinnerAgeMax->setDate(QDate::currentDate());
        mCheckBoxConsiderDOB->setChecked(false);
        mCheckBoxConsiderCountry->setChecked(false);
        mComboCountry->setCurrentIndex(0);
        mModelActor->setFilter("");
        mModelActor->select();
    });
    ui->statusBar->addPermanentWidget(mBtnReset);

    connect(mDialogCountry, &DialogCountry::countryListUpdated,
            [this](){
        mModelActor->relationModel(DBManager::ActorColumns::ID_COUNTRY)->select();
        mComboCountriesModel->reload();
    });

}

int MainWindow::caclAgeByDate(const QDate &dob)
{
    QDate currDate { QDate::currentDate() };
    int age = currDate.year() - dob.year();
    if(dob.month() > currDate.month() ||
       (dob.month() == currDate.month() && dob.day() > currDate.day()))
        --age;
    return age;
}

void MainWindow::on_action_insert_new_actor_triggered()
{
    if(!mModelCountry || !mModelActor)
    {
        QMessageBox::critical(this, "Error", "No active database connections");
        return;
    }

    DialogInsertActor dialog(mDB->getComboCountryModel(), this);
    if(dialog.exec() == QDialog::Rejected) return;
    QString errorMessage;
    if(!mDB->insertActor(*(dialog.actor()), errorMessage))
    {
        QMessageBox::critical(this, "Error", errorMessage);
    }
    else
    {
        mModelActor->select();
        QMessageBox::information(this, "Success",
                                 QString("Actor %1 was successfully inserted"
                                         " to the database")
                                 .arg(dialog.actor()->name()));
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

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if(!index.isValid() || !mModelCountry || !mModelActor) return;
    const int id { mModelActor->index(index.row(), 0).data().toInt() };
    auto actor = mDB->getActorById(id);
    if(actor)
    {
        ui->txtBio->setPlainText(actor->bio());
        QPixmap pixmap;
        if(pixmap.loadFromData(actor->image()))
        {
            mPixmap->setPixmap(pixmap);
        }
    }

}

void MainWindow::on_action_change_bio_and_image_triggered()
{
    if(!mModelActor) return;
    const int row { ui->tableView->currentIndex().row() };
    const int id { mModelActor->index(row, 0).data().toInt() };
    auto bio = ui->txtBio->toPlainText();
    QString erroMessage;
    if(!mDB->updateActorBioAndImage(id, bio, mImgBuffer, erroMessage))
    {
        QMessageBox::critical(this, "Failure", erroMessage);
    }
    else
    {
        QMessageBox::information(this, "Success", "Actor picture and"
                                                  " bio were successfully"
                                                  " updated");
    }
}

void MainWindow::on_action_search_for_an_actor_triggered()
{
    if(!mModelActor) return;
    QString filter { QString("actor.name LIKE '%%1%' ").arg(mLineEditSearch->text()) };

    if(mCheckBoxConsiderDOB->isChecked())
    {
        filter += QString(" AND dob >= '%1' AND dob <= '%2' ")
                  .arg(mSpinnerAgeMin->date().toString(Qt::ISODate))
                  .arg(mSpinnerAgeMax->date().toString(Qt::ISODate));
    }

    if(mCheckBoxConsiderCountry->isChecked())
    {
        auto id = mComboCountry->itemData(mComboCountry->currentIndex(), Qt::UserRole).toInt();
        filter += QString(" AND idCountry = %1").arg(id);
    }
    mModelActor->setFilter(filter);
}

void MainWindow::on_action_change_photo_triggered()
{
    QString startLocation
    {
        QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)
                .value(0, QDir::homePath())
    };
    QString filter { "Images (*.jpg *.jpeg *.png *.bmp);;All files (*.*)" };
    auto filePath = QFileDialog::getOpenFileName(this, "Select a picture",
                                                 startLocation, filter);
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QPixmap pixmap;
        mImgBuffer = file.readAll();
        if(pixmap.loadFromData(mImgBuffer))
        {
            mPixmap->setPixmap(pixmap);
        }
        file.close();
    }
}
