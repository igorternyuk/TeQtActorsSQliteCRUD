#pragma once

#include <QDialog>


namespace Ui
{
    class DialogCountry;
}

class QSqlTableModel;
class DialogCountry : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCountry(QWidget *parent = nullptr);
    ~DialogCountry();

    QSqlTableModel *model() const;
    void setModel(QSqlTableModel *model);
signals:
    void countryListUpdated();

private slots:
    void on_action_add_new_country_triggered();
    void on_action_remove_selected_country_triggered();
    void on_btnClose_clicked();
    void on_btnReload_clicked();

private:
    Ui::DialogCountry *ui;
    QSqlTableModel *mModel { nullptr };

};
