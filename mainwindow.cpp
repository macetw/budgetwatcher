#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "budgethistorymodel.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>


/*---------------------------------------------------------------------
 *       MainWindow::MainWindow()
 */
/*! 
 * \brief  Constructor. Connects to the database, sets up some
 *         defaults based on the current day, connects up the slots,
 *         and ensures that the DB can connect.
 *
 * \param parent   The parent of the widget.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ui->mDate->setDate( QDate::currentDate() );
  ui->mDate->setMinimumDate( QDate::currentDate() );

  // DB driver check is untested.
  const bool supported = QSqlDatabase::isDriverAvailable( "QMYSQL" );
  if (!supported) {
    QMessageBox::critical( this, "Unsupported Database", "MySQL is not supported on this device. Cannot continue." );
    accept();
  }

  connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &MainWindow::accept );
  connect(ui->mAddButton, SIGNAL(clicked()), this, SLOT(add()) );

  model = new BudgetHistoryModel( this );
  ui->tableView->setModel( model );

  QSqlDatabase db = QSqlDatabase::addDatabase( "QMYSQL" );
  db.setHostName( "localhost" );
  db.setUserName( "budgetmaker" );
  db.setDatabaseName( "budget" );
  db.open();

  updateData();
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*---------------------------------------------------------------------
 *       MainWindow::accept()
 */
/*! 
 * \brief   To close down the application.
 */
void MainWindow::accept()
{
  qApp->closeAllWindows();
}


/*---------------------------------------------------------------------
 *       MainWindow::add()
 */
/*! 
 * \brief   Responds to the "Add" button to add a single row.
 */
void MainWindow::add()
{
  QString date( ui->mDate->date().toString( Qt::ISODate ) );
  QString insert = "INSERT INTO transactions VALUES (NULL, 1, ?, ?, ?)";
  
  QSqlQuery query;
  query.prepare( insert );
  query.bindValue( 0, ui->mDate->date() );
  query.bindValue( 1, ui->mAmount->value()*100 );
  query.bindValue( 2, ui->mDescription->text() );
  query.exec();

  updateData();
}

/*---------------------------------------------------------------------
 *       MainWindow::updateData()
 */
/*! 
 * \brief   Updates the view to reflect the latest DB.
 */
void MainWindow::updateData()
{
  //QString select( "SELECT Date,cents/100 AS Value,title AS Description FROM transactions "
  //                "WHERE account=1 ORDER BY date;" );
  QString select( "SELECT Date, cents/100 AS Value, ( select sum(-cents) + (select balance from balances where account = 1) from transactions where date <= O.date AND account = O.account)/100 AS Balance, title as Description from transactions O where account = 1 order by date;" );

  QSqlQuery query( select );

  model->setQuery( query );
}


