#ifndef MATERIALDATA_H
#define MATERIALDATA_H

#include <QTableWidget>

class MaterialData
{
private:
    QString error;

public:
    MaterialData();
    bool isTableWidgetEmpty(QTableWidget *table);
    bool isSnValid(QString sn);
    bool isNamerValid(QString name);
    bool isNumberrValid(int number);
    bool isOwnerValid(QString owner);
    bool isNoteValid(QString note);
    bool isCommunityValid(QString community);
    bool isOperaterValid(QString operater);
    QString lastError();
};

#endif // MATERIALDATA_H
