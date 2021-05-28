#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include "util.h"
#include "materialsql.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    util *UtilHandle;
    MaterialSql *MaterialSqlHandle;

private slots:
    void btnOkCliked();
};

#endif // LOGINDIALOG_H
