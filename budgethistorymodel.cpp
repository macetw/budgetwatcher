#include "budgethistorymodel.h"

#include <QBrush>
#include <QDate>
#include <QPalette>

static const int DateColumn = 0;

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
    QPalette palette;
    QBrush brush( palette.base() );

    int row = index.row();
    if (row == 0) {
      brush = palette.alternateBase();
    } else {
      QModelIndex myDateIndex = sibling( row, DateColumn, index );
      QModelIndex compareDateIndex = sibling( row-1, DateColumn, index );
      QDate myDate = myDateIndex.data(Qt::DisplayRole).toDate();
      QDate compareDate = compareDateIndex.data(Qt::DisplayRole).toDate();

      if (myDate.month() != compareDate.month() ||
          myDate.year() != compareDate.year()) {
        brush = palette.alternateBase();
      }
    }
    return brush;
  }
}
    

