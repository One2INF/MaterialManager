#ifndef MaterialSql_H
#define MaterialSql_H

#include <QtSql>
#include <QTableWidget>
#include <QTableView>

#include "util.h"

typedef struct
{
    QString sn;
    QString name;
    int number;
    QString owner;
    QString note;
    QString ioperator;
}IN_STOCK_ST;

typedef struct
{
    QString sn;
    QString name;
    int number;
    QString community ;
    QString note;
    QString ioperator;
}OUT_STOCK_ST;

typedef struct
{
    QString sn;
    QString name;
    int number;
    QString note;
    QString transferor;
    QString transferee;
    QString ioperator;
}HOLDER_CHANGE_ST;

typedef struct
{
    QString sn;
    QString name;
    QString owner;
}STOCK_QUERY_ST;

typedef struct
{
    QString operat_type;
    QString ioperator;
    QString begin_time;
    QString end_time;
}OPERATION_RECORD_QUERY_ST;

typedef struct
{
    QString sn;
    QString name;
}MATERIAL_QUERY_ST;


typedef struct
{
    QString operat_type;
    QString sn;
    QString name;
    int number;
    QString note;
    QString ioperator;
    QString transferor;
    QString transferee;
}OPERATION_RECORD_ST;

typedef enum
{
    QUERY_TYPE_PROVINCE = 0,
    QUERY_TYPE_CITYE,
    QUERY_TYPE_COUNTY,
    QUERY_TYPE_TOWN,
    QUERY_TYPE_VILLAGEE,
    QUERY_TYPE_COMMUNITY,
    QUERY_TYPE_COUNT,
}EN_QUERY_TYPE;

typedef struct
{
    QString province;
    QString city;
    QString county;
    QString town ;
    QString village;
    QString community;
}ADDRESS_ST;

class MaterialSql
{
private:
    QString error;
    util *UtilHandle;

public:
    explicit MaterialSql();
    ~MaterialSql();

    void ConnectDB();

    /* operations */
    void AddMaterialType(const QStandardItemModel *model);
    void InStock(IN_STOCK_ST *stInStock);
    void OutStock(OUT_STOCK_ST *stOutStock);
    void HolderChange(HOLDER_CHANGE_ST *stHolderChange);
    void StockQuery(STOCK_QUERY_ST *stStockQuery, QSqlQueryModel *model);
    void OperationRecordQuery(OPERATION_RECORD_QUERY_ST *stOperationRecordQuery, QSqlQueryModel *model);
    void MaterialQuery(MATERIAL_QUERY_ST *stMatrialQuery, QSqlQueryModel *model);
    void AddCommunity(const ADDRESS_ST *stAddress);

    /* login */
    bool CheckLogin(QString &name);
    bool CheckPassword(const QString &name, const QString &password);
    bool ChangePassword(const QString &name, const QString &password, const QString &password_);

    /* data select list */
    QList<QString> GetMaterialManagersList();
    QList<QString> GetAreaList(const QString list[QUERY_TYPE_COUNT],
                               EN_QUERY_TYPE type);

private:
    void InsertRecord(OPERATION_RECORD_ST *OperationRecord);
};

#endif // MaterialSql_H
