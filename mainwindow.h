#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
