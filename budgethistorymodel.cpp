#include "budgethistorymodel.h"

#include <QBrush>


BudgetHistoryModel::BudgetHistoryModel( QObject* parent )
  : QSqlQueryModel( parent )
{
}


QVariant BudgetHistoryModel::data(const QModelIndex& index,
                                  int role) const
{
  if (role != Qt::BackgroundRole) {
    return QSqlQueryModel::data(index, role);
  } else {
    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    if (index.row() % 2 == 0) {
      brush.setColor( Qt::lightGray );
    } else {
      brush.setColor( Qt::white );
    }
    return brush;
  }
}
    

