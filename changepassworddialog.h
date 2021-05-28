#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>

#include "util.h"
#include "materialsql.h"

namespace Ui {
class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordDialog(QWidget *parent = nullptr);
    ~ChangePasswordDialog();

private:
    Ui::ChangePasswordDialog *ui;
    util *UtilHandle;
    MaterialSql *MaterialSqlHandle;

private slots:
    void btnOkCliked();
};

#endif // CHANGEPASSWORDDIALOG_H
