#pragma once

#include <QMainWindow>

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_new_database_triggered();
    void on_action_open_database_triggered();
    void on_action_quit_triggered();
    void on_action_insert_new_actor_triggered();
    void on_action_remove_selected_actor_triggered();
    void on_action_reload_list_of_actors_triggered();
    void on_action_edit_countries_triggered();
    void on_action_change_image_triggered();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    DialogCountry *mDialogCountry;
    DBManager *mDB;
    QSqlTableModel *mModelCountry { nullptr };
    QSqlRelationalTableModel *mModelActor { nullptr };
    QSqlRelationalDelegate *mDelegate { nullptr };
    void openDBFile(bool isNewFile = false);
};
