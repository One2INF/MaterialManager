#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTableWidget>

#include "materialsql.h"
#include "materialdata.h"
#include "util.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MaterialSql *MaterialSqlHandle;
    MaterialData *MaterialDataHandle;
    util *utilHandle;

    QString name;
    bool isLogin;

    QSqlQueryModel *StockQueryModel;
    QSqlQueryModel *OperationRecordQueryModel;
    QSqlQueryModel *MaterialQueryModel;

    QStandardItemModel *AddMaterialModel;

    /* paint ui */
    void paintInStock();
    void paintOutStock();
    void paintHolderChange();
    void paintStockQuery();
    void paintOperationRecordQuery();
    void paintMaterialQuery();
    void paintAddMaterial();

    void drawTablewidgetButton(QTableWidget *tableWidget, int row, int column);

    void updateLogin();
    void comboboxBlockSignal(bool flag);

private slots:
    void slotAboutMe();
    void slotLogin();
    void slotChangePassword();

    void updateWindows(QListWidgetItem *item);
    void btnInStockClicked();
    void btnOutStockClicked();
    void btnHolderChangeClicked();
    void btnStockQueryClicked();
    void btnOperationRecordQueryClicked();
    void btnMaterialQueryClicked();
    void btnBrowseExcelClicked();
    void btnAddMaterialTypeClicked();
    void btnAddCommunityClicked();

    void updateCityList(int idx);
    void updateCountyList(int idx);
    void updateTownList(int idx);
    void updateVillageList(int idx);
    void updateCommunityList(int idx);
public slots:
    void btnOpertionClicked();
};
#endif // MAINWINDOW_H
