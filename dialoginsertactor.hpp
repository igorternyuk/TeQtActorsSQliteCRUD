#pragma once

#include <QDialog>
#include "actor.hpp"
//#include <memory>

namespace Ui
{
    class DialogInsertActor;
}

class ComboBoxSqlModel;
class DialogInsertActor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInsertActor(ComboBoxSqlModel *comboModel,
                               QWidget *parent = nullptr);
    ~DialogInsertActor();
    Actor actor() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_btnLoadImage_clicked();

private:
    Ui::DialogInsertActor *ui;
    Actor mActor;
    ComboBoxSqlModel *mComboModel;
};
