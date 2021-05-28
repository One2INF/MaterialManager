#include "logindialog.h"
#include "ui_logindialog.h"

#include <QDebug>
#include <QPushButton>

#include "util.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    UtilHandle = new util();
    MaterialSqlHandle = new MaterialSql();

    ui->setupUi(this);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &LoginDialog::btnOkCliked);
}

void LoginDialog::btnOkCliked()
{
    QString name = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    if(name.isEmpty() || password.isEmpty())
    {
        qDebug() << "name or password is empty!";
        return;
    }

    if(MaterialSqlHandle->CheckPassword(name, password))
    {
        qDebug() << "login successful!";
        UtilHandle->saveJson(name, password);
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
