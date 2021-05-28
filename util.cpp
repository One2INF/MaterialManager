#include "util.h"

#include <QAxObject>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

const QString user_json_path = "./user.json";
const QString database_json_path = "./database.json";

util::util()
{

}

bool util::parseExcel(QString path, QStandardItemModel *model)
{
    QAxObject *excel = new QAxObject("Excel.Application");
    excel->dynamicCall("SetVisible(bool)", true);

    QAxObject *workbooks = excel->querySubObject("WorkBooks");
    QAxObject *workbook = workbooks->querySubObject("Open(QString&)", path);
    QAxObject *worksheets = workbook->querySubObject("WorkSheets");
    QAxObject *worksheet = worksheets->querySubObject("Item(int)", 1);

    QAxObject *usedrange = worksheet->querySubObject("UsedRange");
    QAxObject *rows = usedrange->querySubObject("Rows");
    int iRows = rows->property("Count").toInt();
    QAxObject * columns = usedrange->querySubObject("Columns");
    int iColumns = columns->property("Count").toInt();

    model->clear();
    QAxObject *cell;
    for(int i = 2; i <= iRows; ++i)
    {
        for(int j = 1; j <= iColumns; ++j)
        {
            cell = worksheet->querySubObject("Cells(int,int)", i, j);
            QStandardItem *item = new QStandardItem(cell->property("Value").toString());
            model->setItem(i - 2, j - 1, item);
        }
    }

    workbook->dynamicCall("Close()");
    workbooks->dynamicCall("Close()");
    delete(excel);

    return true;
}

bool util::parseJsonUser(QString &name, QString &password)
{
    QFile jsonFile(user_json_path);
    if(!jsonFile.open(QIODevice::ReadOnly))
    {
        error = "no login information find!";
        return false;
    }

    QByteArray allData = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));
    if(jsonError.error != QJsonParseError::NoError)
    {
      qDebug() << "json parse error:" << jsonError.error;
      return false;
    }

    QJsonValue value = jsonDoc.object();
    name     = value["name"].toString();
    password = value["password"].toString();

    return true;
}

bool util::parseJsonDatabase(DATABASE_CONFIG_ST *database)
{
    QFile jsonFile(database_json_path);
    if(!jsonFile.open(QIODevice::ReadOnly))
    {
        error = "no database information find!";
        qDebug() << error;
        return false;
    }

    QByteArray allData = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));
    if(jsonError.error != QJsonParseError::NoError)
    {
      qDebug() << "json parse error:" << jsonError.error;
      return false;
    }

    QJsonValue value = jsonDoc.object();
    database->hostname     = value["hostname"].toString();
    database->port         = value["port"].toInt();
    database->databasename = value["databasename"].toString();
    database->username     = value["username"].toString();
    database->password     = value["password"].toString();

    return true;
}

void util::saveJson(const QString &name, const QString &password)
{
    QJsonObject qobj;
    qobj.insert("name", name);
    qobj.insert("password", password);


    QJsonDocument jsonDoc;
    jsonDoc.setObject(qobj);
    QByteArray data = jsonDoc.toJson();

    QFile file(user_json_path);
    file.open(QIODevice::WriteOnly);
    file.write(data);
}
