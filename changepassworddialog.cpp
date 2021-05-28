#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"

#include <QPushButton>

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePasswordDialog)
{
    UtilHandle = new util();
    MaterialSqlHandle = new MaterialSql();

    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &ChangePasswordDialog::btnOkCliked);
}

void ChangePasswordDialog::btnOkCliked()
{
    QString name = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    QString password_ = ui->lineEdit_3->text();

    if(name.isEmpty() || password.isEmpty() || password_.isEmpty())
    {
        return;
    }

    if(password == password_)
    {
        qDebug() << "password is same";
        return;
    }

    if(MaterialSqlHandle->ChangePassword(name, password, password_))
    {
        qDebug() << "password changed!";
        UtilHandle->saveJson(name, password_);
    }
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}
