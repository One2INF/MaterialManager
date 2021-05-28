#include "materialsql.h"

#include <QtSql>
#include <QMessageBox>
#include <QTableView>

MaterialSql::MaterialSql()
{
    UtilHandle = new util();
}

MaterialSql::~MaterialSql()
{
    delete UtilHandle;
}

void MaterialSql::ConnectDB()
{
    DATABASE_CONFIG_ST database;
    if(!UtilHandle->parseJsonDatabase(&database))
    {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                              QObject::tr("no database config find"));
        return;
    }


    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL3");
    db.setHostName(database.hostname);
    db.setPort(database.port);
    db.setDatabaseName(database.databasename);
    db.setUserName(database.username);
    db.setPassword(database.password);

    if (!db.open())
    {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                              db.lastError().text(), QMessageBox::Cancel);
        return;
    }
}

void MaterialSql::AddMaterialType(const QStandardItemModel *model)
{
    QString SN;
    QString Supplier;
    QString Category;
    QString Name;
    QString Specification;
    int     UnitPrice;
    QString Note;
    QString sql_cmd;
    QSqlQuery query;

    for(int i = 0; i < model->rowCount(); ++i)
    {
        SN       = model->index(i, 0).data().toString();
        Supplier = model->index(i, 1).data().toString();
        Category = model->index(i, 2).data().toString();
        Name     = model->index(i, 3).data().toString();
        Specification = model->index(i, 4).data().toString();
        UnitPrice = model->index(i, 5).data().toInt();
        Note = model->index(i, 6).data().toString();
        sql_cmd = QString("insert into MaterialList values('%1','%2','%3','%4','%5','%6','%7')")
                         .arg(SN).arg(Supplier).arg(Category).arg(Name)
                         .arg(Specification).arg(UnitPrice).arg(Note);
        query.exec(sql_cmd);
    }
}

QList<QString> MaterialSql::GetMaterialManagersList()
{
    QSqlQuery query;
    QList<QString> list;
    query.exec("SELECT 姓名 FROM MaterialManagers");
    while(query.next())
    {
        list << query.value(0).toString();
    }
    qDebug() << list;
    return list;
}

void MaterialSql::InsertRecord(OPERATION_RECORD_ST *OperationRecord)
{
    QSqlQuery query;
    QString sql_cmd = QString("insert into OperationRecords "
                      "(操作类型, 物料编号, 物料名称, 数量, 备注, 操作人, 转出人, 转入人)"
                      "values('%1', '%2', '%3', %4, '%5', '%6', '%7', '%8')")
                      .arg(OperationRecord->operat_type)
                      .arg(OperationRecord->sn).arg(OperationRecord->name)
                      .arg(OperationRecord->number).arg(OperationRecord->note)
                      .arg(OperationRecord->ioperator).arg(OperationRecord->transferor)
                      .arg(OperationRecord->transferee);
    query.exec(sql_cmd);
    qDebug() << sql_cmd;
}

void MaterialSql::InStock(IN_STOCK_ST *stInStock)
{
    qDebug() << stInStock->sn << stInStock->name << stInStock->number \
             << stInStock->owner << stInStock->note << stInStock->ioperator;

    QSqlQuery query;
    /* check if material is valid */
    QString sql_cmd = QString("SELECT * from MaterialList WHERE 物料编号='%1'").arg(stInStock->sn);
    query.exec(sql_cmd);
    if(query.next())
    {
        if(stInStock->name != query.value(3).toString())
        {
          error = "invalid name";
          return;
        }
    }
    else
    {
        error = "invalid sn";
        return;
    }

    /* instock */
    sql_cmd = QString("SELECT * from MaterialStock WHERE 物料编号='%1' AND 挂账人='%2'")\
                      .arg(stInStock->sn).arg(stInStock->owner);
    query.exec(sql_cmd);
    qDebug() << sql_cmd;
    if(query.next())
    {
        int number = stInStock->number + query.value(2).toInt();
        sql_cmd = QString("UPDATE MaterialStock SET 数量 = '%1' WHERE 物料编号='%2' AND 挂账人='%3'")\
                          .arg(number).arg(stInStock->sn).arg(stInStock->owner);
    }
    else
    {
        sql_cmd = QString("insert into MaterialStock values('%1', '%2', '%3', '%4')")\
                          .arg(stInStock->sn).arg(stInStock->name).arg(stInStock->number).arg(stInStock->owner);
    }
    query.exec(sql_cmd);
    qDebug() << sql_cmd;

    OPERATION_RECORD_ST OperationRecord =
    {
        .operat_type = "入库",
        .sn = stInStock->sn,
        .name = stInStock->name,
        .number = stInStock->number,
        .note = stInStock->note,
        .ioperator = stInStock->ioperator,
        .transferee = stInStock->owner
    };
    InsertRecord(&OperationRecord);
}

void MaterialSql::OutStock(OUT_STOCK_ST *stOutStock)
{
    qDebug() << stOutStock->sn << stOutStock->name << stOutStock->number \
             << stOutStock->community << stOutStock->note << stOutStock->ioperator;

    QSqlQuery query;
    /* check if material is valid */
    QString sql_cmd = QString("SELECT * from MaterialList WHERE 物料编号='%1'").arg(stOutStock->sn);
    query.exec(sql_cmd);
    if(query.next())
    {
        if(stOutStock->name != query.value(3).toString())
        {
          error = "invalid name";
          return;
        }
    }
    else
    {
        error = "invalid sn";
        return;
    }

    /* outstock */
    sql_cmd = QString("SELECT * from MaterialStock WHERE 物料编号='%1' AND 挂账人='%2'")\
                      .arg(stOutStock->sn).arg(stOutStock->ioperator);
    query.exec(sql_cmd);
    qDebug() << sql_cmd;

    if(query.next())
    {
        if(query.value(2).toInt() < stOutStock->number)
        {
            error = "number is bigger than stock!";
            return;
        }

        int number = query.value(2).toInt() - stOutStock->number;
        sql_cmd = QString("UPDATE MaterialStock SET 数量='%1' WHERE 物料编号='%2' AND 挂账人='%3'")\
                          .arg(number).arg(stOutStock->sn).arg(stOutStock->ioperator);
        qDebug() << sql_cmd;
        query.exec(sql_cmd);
    }
    else
    {
        error = "number is bigger than stock!";
        return;
    }

    OPERATION_RECORD_ST OperationRecord =
    {
        .operat_type = "出库",
        .sn = stOutStock->sn,
        .name = stOutStock->name,
        .number = stOutStock->number,
        .note = stOutStock->note,
        .ioperator = stOutStock->ioperator,
        .transferor = stOutStock->ioperator,
        .transferee = stOutStock->community
    };

    InsertRecord(&OperationRecord);
}

void MaterialSql::HolderChange(HOLDER_CHANGE_ST *stHolderChange)
{
    qDebug() << stHolderChange->sn << stHolderChange->name << stHolderChange->number \
             << stHolderChange->note << stHolderChange->transferor << stHolderChange->transferee \
             << stHolderChange->ioperator;

    QSqlQuery query;
    /* check if material is valid */
    QString sql_cmd = QString("SELECT * from MaterialList WHERE 物料编号='%1'").arg(stHolderChange->sn);
    query.exec(sql_cmd);
    if(query.next())
    {
        if(stHolderChange->name != query.value(3).toString())
        {
          error = "invalid name";
          return;
        }
    }
    else
    {
        error = "invalid sn";
        return;
    }

    /* transferor outstock */
    sql_cmd = QString("SELECT * from MaterialStock WHERE 物料编号='%1' AND 挂账人='%2'")\
                      .arg(stHolderChange->sn).arg(stHolderChange->transferor);
    query.exec(sql_cmd);
    if(query.next())
    {
        if(query.value(2).toInt() < stHolderChange->number)
        {
            error = "number is bigger than stock!";
            return;
        }

        int number = query.value(2).toInt() - stHolderChange->number;
        sql_cmd = QString("UPDATE MaterialStock SET 数量 = '%1' WHERE 物料编号='%2' AND 挂账人='%3'")\
                          .arg(number).arg(stHolderChange->sn).arg(stHolderChange->transferor);
        query.exec(sql_cmd);
    }
    else
    {
        error = "number is bigger than stock!";
        return;
    }

    /* transferee instock */
    sql_cmd = QString("SELECT * from MaterialStock WHERE 物料编号='%1' AND 挂账人='%2'")\
                      .arg(stHolderChange->sn).arg(stHolderChange->transferee);
    query.exec(sql_cmd);
    if(query.next())
    {
        int number = stHolderChange->number + query.value(2).toInt();
        sql_cmd = QString("UPDATE MaterialStock SET 数量 = '%1' WHERE 物料编号='%2' AND 挂账人='%3'")\
                          .arg(number).arg(stHolderChange->sn).arg(stHolderChange->transferee);
        query.exec(sql_cmd);
    }
    else
    {
        sql_cmd = QString("insert into MaterialStock values('%1', '%2', '%3', '%4')")\
                          .arg(stHolderChange->sn).arg(stHolderChange->name) \
                          .arg(stHolderChange->number).arg(stHolderChange->transferee);
        query.exec(sql_cmd);
    }

    OPERATION_RECORD_ST OperationRecord =
    {
        .operat_type = "转挂",
        .sn     = stHolderChange->sn,
        .name   = stHolderChange->name,
        .number = stHolderChange->number,
        .note   = stHolderChange->note,
        .ioperator  = stHolderChange->ioperator,
        .transferor = stHolderChange->transferor,
        .transferee = stHolderChange->transferee,
    };

    InsertRecord(&OperationRecord);
}

void MaterialSql::StockQuery(STOCK_QUERY_ST *stStockQuery, QSqlQueryModel *model)
{
    if(stStockQuery->sn.isEmpty())
    {
        stStockQuery->sn = "1";
    }
    else
    {
        stStockQuery->sn = QString("物料编号='%1'").arg(stStockQuery->sn);
    }

    if(stStockQuery->name.isEmpty())
    {
        stStockQuery->name = "1";
    }
    else
    {
        stStockQuery->name = QString("物料名称='%1'").arg(stStockQuery->name);
    }

    if(stStockQuery->owner.isEmpty() ||
       ("请选择" == stStockQuery->owner))
    {
        stStockQuery->owner = "1";
    }
    else
    {
        stStockQuery->owner = QString("挂账人='%1'").arg(stStockQuery->owner);
    }

    /* query stock */
    QString sql_cmd = QString("SELECT * from MaterialStock WHERE %1 AND %2 AND %3")\
                      .arg(stStockQuery->sn).arg(stStockQuery->name).arg(stStockQuery->owner);
    qDebug() << sql_cmd;
    model->setQuery(sql_cmd);
}

void MaterialSql::OperationRecordQuery(OPERATION_RECORD_QUERY_ST *stOperationRecordQuery, QSqlQueryModel *model)
{
    if(stOperationRecordQuery->operat_type.isEmpty() ||
       ("全部" == stOperationRecordQuery->operat_type))
    {
        stOperationRecordQuery->operat_type = "1";
    }
    else
    {
        stOperationRecordQuery->operat_type = QString("操作类型='%1'")
                .arg(stOperationRecordQuery->operat_type);
    }

    if(stOperationRecordQuery->ioperator.isEmpty() ||
       ("请选择" == stOperationRecordQuery->ioperator))
    {
        stOperationRecordQuery->ioperator = "1";
    }
    else
    {
        stOperationRecordQuery->ioperator = QString("操作人='%1'")
                                                   .arg(stOperationRecordQuery->ioperator);
    }

    if(stOperationRecordQuery->begin_time.isEmpty() ||
       ("请选择" == stOperationRecordQuery->begin_time))
    {
        stOperationRecordQuery->begin_time = "1";
    }
    else
    {
        stOperationRecordQuery->begin_time = QString("时间 > '%1'")
                                                    .arg(stOperationRecordQuery->begin_time);
    }

    if(stOperationRecordQuery->end_time.isEmpty() ||
       ("请选择" == stOperationRecordQuery->end_time))
    {
        stOperationRecordQuery->end_time = "1";
    }
    else
    {
        stOperationRecordQuery->end_time = QString("时间 < '%1'")
                                                  .arg(stOperationRecordQuery->end_time);
    }

    QString sql_cmd = QString("SELECT * from OperationRecords WHERE %1 AND %2 AND %3 AND %4")
                             .arg(stOperationRecordQuery->operat_type)
                             .arg(stOperationRecordQuery->ioperator)
                             .arg(stOperationRecordQuery->begin_time)
                             .arg(stOperationRecordQuery->end_time);
    model->setQuery(sql_cmd);
    qDebug() << sql_cmd;
}

void MaterialSql::MaterialQuery(MATERIAL_QUERY_ST *stMatrialQuery, QSqlQueryModel *model)
{
    /* query stock */
    QString sql_cmd = QString("SELECT * from MaterialList WHERE 1");

    if(!stMatrialQuery->sn.isEmpty())
    {
        sql_cmd += QString(" AND 物料编号='%1'").arg(stMatrialQuery->sn);
    }

    if(!stMatrialQuery->name.isEmpty())
    {
        sql_cmd += QString(" AND 物料名称='%1'").arg(stMatrialQuery->name);
    }

    qDebug() << sql_cmd;
    model->setQuery(sql_cmd);
}

bool MaterialSql::CheckPassword(const QString &name, const QString &password)
{
    QString sql_cmd = QString("SELECT 密码 FROM MaterialManagers WHERE 姓名='%1'")
                             .arg(name);
    QSqlQuery query;
    query.exec(sql_cmd);
    if(query.next())
    {
        if(password == query.value(0).toString())
        {
            return true;
        }
        else
        {
            qDebug() << "password is not right!";
        }
    }
    else
    {
        qDebug() << "name not exist!";
    }

    return false;
}

bool MaterialSql::ChangePassword(const QString &name, const QString &password, const QString &password_)
{
    if(!CheckPassword(name, password))
        return false;

    QString sql_cmd = QString("UPDATE MaterialManagers set 密码='%1' WHERE 姓名='%2'")
                             .arg(password_).arg(name);
    QSqlQuery query;
    query.exec(sql_cmd);

    return true;
}

bool MaterialSql::CheckLogin(QString &name)
{
    QString name_, password_;
    if(!UtilHandle->parseJsonUser(name_, password_))
    {
        qDebug() << "parsed name";
        return false;
    }

    if(!CheckPassword(name_, password_))
    {
        qDebug() << "name or password in config.json is not right!";
        return false;
    }

    name = name_;
    qDebug() << "login with config.json!";
    return true;
}

QString typeList[QUERY_TYPE_COUNT] = {"省", "市", "区", "街道", "社区", "小区"};
QList<QString> MaterialSql::GetAreaList(const QString list[QUERY_TYPE_COUNT],
                           EN_QUERY_TYPE type)
{
    QList<QString> areaList;
    QString list_[QUERY_TYPE_COUNT];

    for(int i = QUERY_TYPE_PROVINCE; i < QUERY_TYPE_COUNT; ++i)
    {
        if(i < type)
        {
            list_[i] = typeList[i] + QString("='%1'").arg(list[i]);
        }
        else if(i == type)
        {
            list_[i] = typeList[i] + "!=''";
        }
        else
        {
            list_[i] = typeList[i] + "=''";
        }
    }
    QString sql_cmd = QString("SELECT %1 from CityStatistiscCode WHERE %2 AND %3 AND %4 AND %5 AND %6")
                             .arg(typeList[type]).arg(list_[0]).arg(list_[1])
                             .arg(list_[2]).arg(list_[3]).arg(list_[4]);
    QSqlQuery query;
    query.exec(sql_cmd);
    while(query.next())
    {
        areaList << query.value(0).toString();
    }
    qDebug() << sql_cmd;
    return areaList;
}

void MaterialSql::AddCommunity(const ADDRESS_ST *stAddress)
{
    QString sql_cmd = QString("insert into CityStatistiscCode values('000000000000', '%1', '%2', '%3', '%4', '%5', '%6')")
                             .arg(stAddress->province).arg(stAddress->city).arg(stAddress->county)
                             .arg(stAddress->town).arg(stAddress->village).arg(stAddress->community);
    QSqlQuery query;
    query.exec(sql_cmd);
}
