#ifndef UTIL_H
#define UTIL_H

#include <QStandardItemModel>

typedef struct
{
    QString hostname;
    int port;
    QString databasename;
    QString username;
    QString password;
}DATABASE_CONFIG_ST;

class util
{
private:
    QString error;

public:
    util();
    bool parseExcel(QString path, QStandardItemModel *model);
    bool parseJsonUser(QString &name, QString &password);
    bool parseJsonDatabase(DATABASE_CONFIG_ST *database);
    void saveJson(const QString &name, const QString &password);
};

#endif // UTIL_H
