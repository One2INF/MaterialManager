#include "materialdata.h"
#include <QDebug>

MaterialData::MaterialData()
{

}

bool MaterialData::isTableWidgetEmpty(QTableWidget *table)
{
    for(int i = 0; i < table->rowCount(); ++i)
    {
        for(int j = 0; j < table->columnCount() - 1; ++j)
        {
            if((nullptr == table->item(i, j)) ||
               (table->item(i, j)->text().isEmpty()))
            {
              qDebug() << i << j;
              error = table->horizontalHeaderItem(j)->text() + " is empty!";
              return true;
            }
        }
    }
    return false;
}

bool MaterialData::isSnValid(QString sn)
{
    if(sn.isEmpty())
    {
      error = "sn is empty!";
      return false;
    }
    return true;
}

bool MaterialData::isNamerValid(QString name)
{
    if(name.isEmpty())
    {
      error = "name is empty!";
      return false;
    }

    return true;
}
bool MaterialData::isNumberrValid(int number)
{
    if(number <= 0)
    {
      error = "invalid number!";
      return false;
    }

    return true;
}

bool MaterialData::isOwnerValid(QString owner)
{
    if(owner.isEmpty())
    {
      error = "sn is empty!";
      return false;
    }
    else if ("请选择" == owner)
    {
      error = "please select a owner!";
      return false;
    }

    return true;
}

bool MaterialData::isNoteValid(QString note)
{
    if(note.isEmpty())
    {
      error = "note is empty!";
      return false;
    }

    return true;
}

bool MaterialData::isOperaterValid(QString Operater)
{
    if(Operater.isEmpty())
    {
      error = "sn is empty!";
      return false;
    }
    else if ("请选择" == Operater)
    {
      error = "please select a Operater!";
      return false;
    }

    return true;
}

bool MaterialData::isCommunityValid(QString community)
{
    if(community.isEmpty())
    {
      error = "community is empty!";
      return false;
    }

    return true;
}

QString MaterialData::lastError()
{
  return error;
}

