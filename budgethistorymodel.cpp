#include "budgethistorymodel.h"

#include <QBrush>
#include <QDate>
#include <QPalette>

static const int DateColumn = 0;
static const int AmountColumn = 1;
static const int SubtotalColumn = 2;

/*---------------------------------------------------------------------
 *       BudgetHistoryModel::BudgetHistoryModel()
 */
/*! 
 * \brief   Constructor.
 *
 * \param parent   Parent
 */
BudgetHistoryModel::BudgetHistoryModel( QObject* parent )
  : QSqlQueryModel( parent )
{
}


/*---------------------------------------------------------------------
 *       BudgetHistoryModel::data()
 */
/*! 
 * \brief    Handles only the "Background Role" to get specialized
 *           behavior to give a visual clue of the separation of each
 *           month.
 *
 * \param index    Index for the data
 * \param role     Role needed
 *
 * \return   Returns the base or alternateBase of the data.
 *
 * \sa QAbstractItemModel::data()
 */
QVariant BudgetHistoryModel::data(const QModelIndex& index,
                                  int role) const
{
  const int row = index.row();
  const int column = index.column();

  QVariant datum;

  if (role == Qt::BackgroundRole) {
    QPalette palette;
    QBrush brush( palette.base() );

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
    datum = brush;
  } else if (role == Qt::ForegroundRole &&
             (column == AmountColumn || column == SubtotalColumn) ) {
    QPalette palette;
    QBrush brush( palette.text() );
    QVariant amount = QSqlQueryModel::data( index, Qt::DisplayRole );
    if (amount.toDouble() < 0.0) {
      brush.setColor( Qt::darkRed );
    } else {
      brush.setColor( Qt::darkGreen );
    }
    datum = brush;

  } else {
    datum = QSqlQueryModel::data(index, role);
  }
  return datum;
}

