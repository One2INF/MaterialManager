#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QTextEdit>
#include <QCompleter>
#include <QFileDialog>
#include <QMessageBox>

#include "logindialog.h"
#include "changepassworddialog.h"
#include "materialsql.h"

void MainWindow::drawTablewidgetButton(QTableWidget *tableWidget, int row, int column)
{
    /* add two buttons */
    QPushButton* btnPlus = new QPushButton("+");
    QPushButton* btnMinus = new QPushButton("-");
    QWidget *widget = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->addWidget(btnPlus);
    hLayout->addWidget(btnMinus);
    hLayout->setMargin(0);
    tableWidget->setCellWidget(row, column, widget);

    connect(btnPlus, &QPushButton::clicked,
            this, &MainWindow::btnOpertionClicked);
    connect(btnMinus, &QPushButton::clicked,
            this, &MainWindow::btnOpertionClicked);
}

void MainWindow::paintInStock()
{
    ui->tableWidget->setEnabled(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tableWidget->setColumnWidth(3, 50);
    drawTablewidgetButton(ui->tableWidget, 0, 3);
}

void MainWindow::paintOutStock()
{
    ui->tableWidget_2->setEnabled(true);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tableWidget_2->setColumnWidth(3, 50);
    drawTablewidgetButton(ui->tableWidget_2, 0, 3);

    QString areaList[QUERY_TYPE_COUNT] = {"浙江省", "杭州市", "余杭区", "南苑街道", "时代社区"};
    ui->comboBoxProvince->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_PROVINCE));
    ui->comboBoxProvince->setCurrentText("浙江省");
    ui->comboBoxCity->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_CITYE));
    ui->comboBoxCity->setCurrentText("杭州市");
    ui->comboBoxCounty->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_COUNTY));
    ui->comboBoxCounty->setCurrentText("余杭区");
    ui->comboBoxTown->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_TOWN));
    ui->comboBoxTown->setCurrentText("南苑街道");
    ui->comboBoxVillage->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_VILLAGEE));
    ui->comboBoxVillage->setCurrentText("时代社区");
    ui->comboBoxCommunity->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_COMMUNITY));
    ui->comboBoxCommunity->setCurrentText("村口小区");
    ui->comboBoxCommunity->setEditable(true);

    connect(ui->comboBoxProvince, QOverload<int>::of(&QComboBox::currentIndexChanged),
                      this, &MainWindow::updateCityList);
    connect(ui->comboBoxCity, QOverload<int>::of(&QComboBox::currentIndexChanged),
                      this, &MainWindow::updateCountyList);
    connect(ui->comboBoxCounty, QOverload<int>::of(&QComboBox::currentIndexChanged),
                      this, &MainWindow::updateTownList);
    connect(ui->comboBoxTown, QOverload<int>::of(&QComboBox::currentIndexChanged),
                      this, &MainWindow::updateVillageList);
    connect(ui->comboBoxVillage, QOverload<int>::of(&QComboBox::currentIndexChanged),
                      this, &MainWindow::updateCommunityList);
}

void MainWindow::paintHolderChange()
{
    ui->tableWidget_3->setEnabled(true);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tableWidget_3->setColumnWidth(3, 50);
    drawTablewidgetButton(ui->tableWidget_3, 0, 3);
}

void MainWindow::paintStockQuery()
{

}

void MainWindow::paintOperationRecordQuery()
{
    const QList<QString> list = {"全部", "入库", "出库", "转挂"};
    ui->comboBox_11->addItems(list);
}

void MainWindow::paintMaterialQuery()
{

}

void MainWindow::paintAddMaterial()
{

}

void MainWindow::updateLogin()
{
    /* if user/password is not right, do nothing */
    isLogin = MaterialSqlHandle->CheckLogin(name);
    if(isLogin)
    {
        this->setWindowTitle(QString("MaterialManager - %1").arg(name));
        ui->statusbar->showMessage("Login successful");
    }
    else
    {
        this->setWindowTitle("MaterialManager - Invalid user/password!!!");
        ui->statusbar->showMessage("Invalid user/password!!!");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MaterialSqlHandle  = new MaterialSql();
    MaterialDataHandle = new MaterialData();
    StockQueryModel           = new QSqlQueryModel();
    OperationRecordQueryModel = new QSqlQueryModel();
    MaterialQueryModel        = new QSqlQueryModel();
    AddMaterialModel          = new QStandardItemModel();

    MaterialSqlHandle->ConnectDB();

    ui->setupUi(this);

    updateLogin();

    /* paint ui */
    paintInStock();
    paintOutStock();
    paintHolderChange();
    paintStockQuery();
    paintOperationRecordQuery();
    paintMaterialQuery();
    paintAddMaterial();

    QList<QString> MaterialManagersList = MaterialSqlHandle->GetMaterialManagersList();
    ui->comboBox->addItems(MaterialManagersList);
    QCompleter *completer = new QCompleter(MaterialManagersList, ui->comboBox);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    //ui->comboBox->setEditable(true);

    ui->comboBox_8->addItem(name);
    ui->comboBox_8->setEnabled(false);

    ui->comboBox_9->addItems(MaterialManagersList);
    ui->comboBox_10->addItems(MaterialManagersList);
    ui->comboBox_12->addItems(MaterialManagersList);

    /* add item */
    ui->tableView->setModel(StockQueryModel);
    ui->tableView->show();
    ui->tableView_2->setModel(OperationRecordQueryModel);
    ui->tableView_2->setColumnWidth(0, 1);
    ui->tableView_2->setColumnWidth(1, 1);
    ui->tableView_2->show();
    ui->tableView_3->setModel(MaterialQueryModel);
    ui->tableView_3->show();
    ui->tableView_4->setModel(AddMaterialModel);
    ui->tableView_4->show();

    /* action */
    connect(ui->actionAbout, &QAction::triggered,
            this, &MainWindow::slotAboutMe);
    connect(ui->actionLogin, &QAction::triggered,
            this, &MainWindow::slotLogin);
    connect(ui->actionChangePassword, &QAction::triggered,
            this, &MainWindow::slotChangePassword);


    connect(ui->listWidget, &QListWidget::itemClicked,
            this, &MainWindow::updateWindows);

    connect(ui->pushButton, &QPushButton::clicked,
                     this, &MainWindow::btnInStockClicked);
    connect(ui->pushButton_2, &QPushButton::clicked,
                     this, &MainWindow::btnOutStockClicked);
    connect(ui->pushButton_3, &QPushButton::clicked,
                     this, &MainWindow::btnHolderChangeClicked);
    connect(ui->pushButton_4, &QPushButton::clicked,
                     this, &MainWindow::btnStockQueryClicked);
    connect(ui->pushButton_5, &QPushButton::clicked,
                     this, &MainWindow::btnOperationRecordQueryClicked);
    connect(ui->pushButton_6, &QPushButton::clicked,
                     this, &MainWindow::btnMaterialQueryClicked);

    connect(ui->pushButtonAddCommunity, &QPushButton::clicked,
                     this, &MainWindow::btnAddCommunityClicked);

    connect(ui->pushButton_8, &QPushButton::clicked,
                     this, &MainWindow::btnBrowseExcelClicked);
    connect(ui->pushButton_7, &QPushButton::clicked,
                     this, &MainWindow::btnAddMaterialTypeClicked);
}

void MainWindow::updateWindows(QListWidgetItem *item)
{
    ui->stackedWidget->setCurrentIndex(ui->listWidget->currentRow());
}

/* button clicked, tablewidget add/remove row */
void MainWindow::btnOpertionClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    /* TODO: To be optimized */
    QWidget *w = qobject_cast<QWidget *>(sender()->parent());
    QTableWidget *tableWidget = qobject_cast<QTableWidget *>(sender()->parent()->parent()->parent());

    if(nullptr == w)
    {
        qDebug() << "sender() parent is NULL";
        return;
    }

    if(nullptr == tableWidget)
    {
        qDebug() << "sender() parents are NULL";
        return;
    }

    int row = tableWidget->indexAt(w->pos()).row();
    if(btn->text() == "+")
    {
        tableWidget->insertRow(row + 1);
        drawTablewidgetButton(tableWidget, row + 1, 3);
    }
    else if(btn->text() == "-")
    {
        if(1 == tableWidget->rowCount())
        {
            qDebug() << "last item, can't delete!";
            return;
        }

        tableWidget->removeRow(row);
    }
    else
    {
        qDebug() << "Error: should not reach here!";
    }
}

void MainWindow::btnInStockClicked()
{
    /* check if login */
    if(!isLogin)
    {
        ui->statusbar->showMessage("please login first!");
        return;
    }

    /* check table is not empty */
    if(MaterialDataHandle->isTableWidgetEmpty(ui->tableWidget))
    {
        goto SHOW_ERROR;
    }

    /* insert if pass data-check */
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        IN_STOCK_ST stInStock
        {
            .sn       = ui->tableWidget->item(i, 0)->text(),
            .name     = ui->tableWidget->item(i, 1)->text(),
            .number   = ui->tableWidget->item(i, 2)->data(0).toInt(),
            .owner    = ui->comboBox->currentText(),
            .note     = ui->lineEdit->text(),
            .ioperator = name
        };

        if((MaterialDataHandle->isSnValid(stInStock.sn) &&
            MaterialDataHandle->isNamerValid(stInStock.name) &&
            MaterialDataHandle->isNumberrValid(stInStock.number) &&
            MaterialDataHandle->isOwnerValid(stInStock.owner) &&
            MaterialDataHandle->isNoteValid(stInStock.note) &&
            MaterialDataHandle->isOperaterValid(stInStock.ioperator)))
        {
            MaterialSqlHandle->InStock(&stInStock);
        }
    }

    return;

SHOW_ERROR:
    ui->statusbar->showMessage(MaterialDataHandle->lastError());
}

void MainWindow::btnOutStockClicked()
{
    /* check if login */
    if(!isLogin)
    {
        ui->statusbar->showMessage("please login first!");
        return;
    }

    if(ui->comboBoxCommunity->currentText().isEmpty() ||
       ui->comboBoxVillage->currentText().isEmpty() ||
       ui->comboBoxTown->currentText().isEmpty() ||
       ui->comboBoxCounty->currentText().isEmpty() ||
       ui->comboBoxCity->currentText().isEmpty() ||
       ui->comboBoxProvince->currentText().isEmpty())
    {
        ui->statusbar->showMessage("please select a community!");
        return;
    }

    QString community = ui->comboBoxCommunity->currentText()
                      + ui->comboBoxVillage->currentText()
                      + ui->comboBoxTown->currentText()
                      + ui->comboBoxCounty->currentText()
                      + ui->comboBoxCity->currentText()
                      + ui->comboBoxProvince->currentText();

    /* check table is not empty */
    if(MaterialDataHandle->isTableWidgetEmpty(ui->tableWidget_2))
    {
        goto SHOW_ERROR;
    }

    /* insert if pass data-check */
    for(int i = 0; i < ui->tableWidget_2->rowCount(); ++i)
    {
        OUT_STOCK_ST stOutStock
        {
            .sn        = ui->tableWidget_2->item(i, 0)->text(),
            .name      = ui->tableWidget_2->item(i, 1)->text(),
            .number    = ui->tableWidget_2->item(i, 2)->data(0).toInt(),
            .community = community,
            .note      = ui->lineEdit_2->text(),
            .ioperator  = name
        };

        if((MaterialDataHandle->isSnValid(stOutStock.sn) &&
            MaterialDataHandle->isNamerValid(stOutStock.name) &&
            MaterialDataHandle->isNumberrValid(stOutStock.number) &&
            MaterialDataHandle->isCommunityValid(stOutStock.community) &&
            MaterialDataHandle->isNoteValid(stOutStock.note) &&
            MaterialDataHandle->isOperaterValid(stOutStock.ioperator)))
        {
            // TODO: add outsock list
            MaterialSqlHandle->OutStock(&stOutStock);
        }
    }

    return;

SHOW_ERROR:
    ui->statusbar->showMessage(MaterialDataHandle->lastError());
}

void MainWindow::btnHolderChangeClicked()
{
    /* check if login */
    if(!isLogin)
    {
        ui->statusbar->showMessage("please login first!");
        return;
    }

    /* check table is not empty */
    if(MaterialDataHandle->isTableWidgetEmpty(ui->tableWidget_3))
    {
        goto SHOW_ERROR;
    }

    /* insert if pass data-check */
    for(int i = 0; i < ui->tableWidget_3->rowCount(); ++i)
    {
        HOLDER_CHANGE_ST stHolderChange
        {
            .sn         = ui->tableWidget_3->item(i, 0)->text(),
            .name       = ui->tableWidget_3->item(i, 1)->text(),
            .number     = ui->tableWidget_3->item(i, 2)->data(0).toInt(),
            .note       = ui->lineEdit_3->text(),
            .transferor = ui->comboBox_8->currentText(),
            .transferee = ui->comboBox_9->currentText(),
            .ioperator   = name
        };

        if((MaterialDataHandle->isSnValid(stHolderChange.sn) &&
            MaterialDataHandle->isNamerValid(stHolderChange.name) &&
            MaterialDataHandle->isNumberrValid(stHolderChange.number) &&
            MaterialDataHandle->isNoteValid(stHolderChange.note) &&
            MaterialDataHandle->isOperaterValid(stHolderChange.transferor) &&
            MaterialDataHandle->isOperaterValid(stHolderChange.transferee) &&
            MaterialDataHandle->isOperaterValid(stHolderChange.ioperator)))
        {
            if(stHolderChange.transferor == stHolderChange.transferee)
            {
                ui->statusbar->showMessage("transferor and transferee is same!");
                return;
            }

            // TODO: add outsock list
            MaterialSqlHandle->HolderChange(&stHolderChange);
        }
    }

    return;

SHOW_ERROR:
    ui->statusbar->showMessage(MaterialDataHandle->lastError());
}

void MainWindow::btnStockQueryClicked()
{
    STOCK_QUERY_ST stStockQuery
    {
        .sn    = ui->lineEdit_4->text(),
        .name  = ui->lineEdit_5->text(),
        .owner = ui->comboBox_10->currentText()
    };

    MaterialSqlHandle->StockQuery(&stStockQuery, StockQueryModel);

}

void MainWindow::btnOperationRecordQueryClicked()
{
    OPERATION_RECORD_QUERY_ST stOperationRecordQuery
    {
        .operat_type = ui->comboBox_11->currentText(),
        .ioperator    = ui->comboBox_12->currentText(),
        .begin_time  = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss"),
        .end_time    = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd HH:mm:ss")
    };

    MaterialSqlHandle->OperationRecordQuery(&stOperationRecordQuery, OperationRecordQueryModel);

    /* resize column width after QSqlQueryModel::setQuery */
    ui->tableView_2->setColumnWidth(0, 130);
    ui->tableView_2->setColumnWidth(1, 60);
    ui->tableView_2->setColumnWidth(2, 60);
    ui->tableView_2->setColumnWidth(4, 30);
    ui->tableView_2->setColumnWidth(6, 50);
    ui->tableView_2->setColumnWidth(7, 50);
    ui->tableView_2->setColumnWidth(8, 100);
}

void MainWindow::btnMaterialQueryClicked()
{
    MATERIAL_QUERY_ST stMatrialQuery
    {
        .sn   = ui->lineEdit_6->text(),
        .name = ui->lineEdit_7->text(),
    };

    MaterialSqlHandle->MaterialQuery(&stMatrialQuery, MaterialQueryModel);
}

void MainWindow::btnBrowseExcelClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("select excel"), ".", tr("Excel Files(*.xls *.xlsx)"));
    if(path.isEmpty())
    {
        return;
    }

    ui->label_23->setText(path);
    utilHandle->parseExcel(path, AddMaterialModel);
}

void MainWindow::btnAddMaterialTypeClicked()
{
    /* check if login */
    if(!isLogin)
    {
        ui->statusbar->showMessage("please login first!");
        return;
    }

    MaterialSqlHandle->AddMaterialType(AddMaterialModel);
}

void MainWindow::btnAddCommunityClicked()
{
    /* check if login */
    if(!isLogin)
    {
        ui->statusbar->showMessage("please login first!");
        return;
    }

    const ADDRESS_ST stAddress =
    {
        .province = ui->comboBoxProvince->currentText(),
        .city = ui->comboBoxCity->currentText(),
        .county = ui->comboBoxCounty->currentText(),
        .town = ui->comboBoxTown->currentText(),
        .village = ui->comboBoxVillage->currentText(),
        .community = ui->comboBoxCommunity->currentText(),
    };
    MaterialSqlHandle->AddCommunity(&stAddress);
}

void MainWindow::slotAboutMe()
{
  QString msgAuthor = QString("Author: One2INF (咸个和和和)<p>");
  QString msgGitee = QString("Gitee: <a href=https://gitee.com/One2INF>https://gitee.com/One2INF</a><p>");
  QString msgEmail = QString("Email: <a href=1871750676@qq.com>1871750676@qq.com</a><p>");
  QString msg = msgAuthor + msgGitee + msgEmail;

  QMessageBox::about(nullptr, "About Me", msg);
}

void MainWindow::slotLogin()
{
    LoginDialog login;
    if(QDialog::Accepted == login.exec())
    {
        updateLogin();
    }
}

void MainWindow::slotChangePassword()
{
    ChangePasswordDialog changepassword;
    changepassword.exec();
}

/*
 * QComboBox clear() and additems  temporary disable conbobox signal */
void MainWindow::comboboxBlockSignal(bool flag)
{
    ui->comboBoxProvince->blockSignals(flag);
    ui->comboBoxCity->blockSignals(flag);
    ui->comboBoxCounty->blockSignals(flag);
    ui->comboBoxTown->blockSignals(flag);
    ui->comboBoxVillage->blockSignals(flag);
    ui->comboBoxCommunity->blockSignals(flag);
}

void MainWindow::updateCityList(int idx)
{
    (void)idx;
    /* temporary disable conbobox signal */
    comboboxBlockSignal(true);

    ui->comboBoxCity->clear();
    ui->comboBoxCounty->clear();
    ui->comboBoxTown->clear();
    ui->comboBoxVillage->clear();
    ui->comboBoxCommunity->clear();

    QString areaList[QUERY_TYPE_COUNT] = {ui->comboBoxProvince->currentText(), "", "", "", ""};
    ui->comboBoxCity->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_CITYE));

    /* enable conbobox signal */
    comboboxBlockSignal(false);
}

void MainWindow::updateCountyList(int idx)
{
    (void)idx;
    /* temporary disable conbobox signal */
    comboboxBlockSignal(true);

    ui->comboBoxCounty->clear();
    ui->comboBoxTown->clear();
    ui->comboBoxVillage->clear();
    ui->comboBoxCommunity->clear();
    QString areaList[QUERY_TYPE_COUNT] = {ui->comboBoxProvince->currentText(),
                                          ui->comboBoxCity->currentText(), "", "", ""};
    ui->comboBoxCounty->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_COUNTY));

    /* enable conbobox signal */
    comboboxBlockSignal(false);
}

void MainWindow::updateTownList(int idx)
{
    (void)idx;
    /* temporary disable conbobox signal */
    comboboxBlockSignal(true);

    ui->comboBoxTown->clear();
    ui->comboBoxVillage->clear();
    ui->comboBoxCommunity->clear();
    QString areaList[QUERY_TYPE_COUNT] = {ui->comboBoxProvince->currentText(),
                                          ui->comboBoxCity->currentText(),
                                          ui->comboBoxCounty->currentText(), "", ""};
    ui->comboBoxTown->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_TOWN));

    /* enable conbobox signal */
    comboboxBlockSignal(false);
}

void MainWindow::updateVillageList(int idx)
{
    (void)idx;
    /* temporary disable conbobox signal */
    comboboxBlockSignal(true);

    ui->comboBoxVillage->clear();
    ui->comboBoxCommunity->clear();
    QString areaList[QUERY_TYPE_COUNT] = {ui->comboBoxProvince->currentText(),
                                          ui->comboBoxCity->currentText(),
                                          ui->comboBoxCounty->currentText(),
                                          ui->comboBoxTown->currentText(),
                                          ""};
    ui->comboBoxVillage->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_VILLAGEE));

    /* enable conbobox signal */
    comboboxBlockSignal(false);
}

void MainWindow::updateCommunityList(int idx)
{
    (void)idx;
    /* temporary disable conbobox signal */
    comboboxBlockSignal(true);

    ui->comboBoxCommunity->clear();
    QString areaList[QUERY_TYPE_COUNT] = {ui->comboBoxProvince->currentText(),
                                          ui->comboBoxCity->currentText(),
                                          ui->comboBoxCounty->currentText(),
                                          ui->comboBoxTown->currentText(),
                                          ui->comboBoxVillage->currentText()};
    ui->comboBoxCommunity->addItems(MaterialSqlHandle->GetAreaList(areaList, QUERY_TYPE_COMMUNITY));

    /* enable conbobox signal */
    comboboxBlockSignal(false);
}

MainWindow::~MainWindow()
{
    delete MaterialDataHandle;
    delete MaterialSqlHandle;
    delete ui;
}
