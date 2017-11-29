#pragma once

#include <QMainWindow>
#include <QByteArray>
#include <QDate>

namespace Ui
{
    class MainWindow;
}

class DBManager;
class QSqlTableModel;
class QSqlRelationalTableModel;
class QSqlRelationalDelegate;
class DialogCountry;
class ComboBoxSqlModel;
class QGraphicsScene;
class QGraphicsPixmapItem;
class QLabel;
class QLineEdit;
class QComboBox;
class QDateEdit;
class QCheckBox;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_action_new_database_triggered();
    void on_action_open_database_triggered();
    void on_action_quit_triggered();
    void on_action_insert_new_actor_triggered();
    void on_action_remove_selected_actor_triggered();
    void on_action_reload_list_of_actors_triggered();
    void on_action_edit_countries_triggered();
    void on_tableView_clicked(const QModelIndex &index);
    void on_action_change_bio_and_image_triggered();
    void on_action_search_for_an_actor_triggered();
    void on_action_change_photo_triggered();

private:
    Ui::MainWindow *ui;
    //DialogCountry *mDialogCountry;
    DBManager *mDB;
    QSqlTableModel *mModelCountry { nullptr };
    QSqlRelationalTableModel *mModelActor { nullptr };
    QSqlRelationalDelegate *mDelegate { nullptr };
    ComboBoxSqlModel *mComboCountriesModel { nullptr };
    DialogCountry *mDialogCountry;
    QGraphicsScene *mScene;
    QGraphicsPixmapItem *mPixmap;
    QByteArray mImgBuffer;
    QLabel *mLblName;
    QLineEdit *mLineEditSearch;
    QCheckBox *mCheckBoxConsiderDOB;
    QLabel *mLblAgeMin;
    QDateEdit *mSpinnerAgeMin;
    QLabel *mLblAgeMax;
    QDateEdit *mSpinnerAgeMax;
    QCheckBox *mCheckBoxConsiderCountry;
    QComboBox *mComboCountry;
    QPushButton *mBtnSearch;
    QPushButton *mBtnLoadFullList;
    QPushButton *mBtnReset;
    void openDBFile(bool isNewFile = false);
    void addSearchWidgetsToTheStatusBar();
    int caclAgeByDate(const QDate &dob);
};
