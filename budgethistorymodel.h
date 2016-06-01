#include <QSqlQueryModel>


class BudgetHistoryModel : public QSqlQueryModel
{
 public:
  BudgetHistoryModel(QObject* parent);

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

 private: //disabled defualt functions
  BudgetHistoryModel();
  BudgetHistoryModel(BudgetHistoryModel&);
};
  
