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

  QPalette pal( palette() );
  pal.setColor( QPalette::ButtonText, Qt::darkRed );
  ui->mDebitButton->setPalette( pal );
  pal.setColor( QPalette::ButtonText, Qt::darkGreen );
  ui->mCreditButton->setPalette( pal );

  ui->mDate->setDate( QDate::currentDate() );
  ui->mDate->setMinimumDate( QDate::currentDate() );

  const bool supported = QSqlDatabase::isDriverAvailable( "QMYSQL" );
  if (!supported) {
    QMessageBox::critical( this, "Unsupported Database", "MySQL is not supported on this device. Cannot continue." );
    accept();
  }

  connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &MainWindow::accept );
  connect(ui->mCreditButton, SIGNAL(clicked()), this, SLOT(addCredit()) );
  connect(ui->mDebitButton, SIGNAL(clicked()), this, SLOT(addDebit()) );
  connect(ui->mAmount, SIGNAL(valueChanged(double)), this, SLOT(enableDisableWidgets()) );
  connect(ui->mDescription, SIGNAL(textEdited(const QString&)), this, SLOT(enableDisableWidgets()) );

  model = new BudgetHistoryModel( this );
  ui->tableView->setModel( model );

  QSqlDatabase db = QSqlDatabase::addDatabase( "QMYSQL" );
  db.setHostName( "localhost" );
  db.setUserName( "budgetmaker" );
  db.setDatabaseName( "budget" );
  db.open();

  updateData();
  enableDisableWidgets();
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
 *       MainWindow::addCredit()
 */
/*! 
 * \brief   Wrapper to respond to the "Credit" button.
 */
void MainWindow::addCredit()
{
  add(1);
}


/*---------------------------------------------------------------------
 *       MainWindow::addDebit()
 */
/*! 
 * \brief   Wrapper to respond to the "Debit" button.
 */
void MainWindow::addDebit()
{
  add(-1);
}


/*---------------------------------------------------------------------
 *       MainWindow::enableDisableWidgets()
 */
/*! 
 * \brief   Responds to user editing to reflect the enable state of "add"
 */
void MainWindow::enableDisableWidgets()
{
  bool enable = (ui->mAmount->value() != 0.0 &&
                 !ui->mDescription->text().trimmed().isEmpty());
  ui->mCreditButton->setEnabled( enable );
  ui->mDebitButton->setEnabled( enable );
}


/*---------------------------------------------------------------------
 *       MainWindow::add()
 */
/*! 
 * \brief   Responds to the "Add" button to add a single row.
 */
void MainWindow::add(int multiplier)
{
  QString date( ui->mDate->date().toString( Qt::ISODate ) );
  QString insert = "INSERT INTO transactions VALUES (NULL, 1, ?, ?, ?)";
  
  QSqlQuery query;
  query.prepare( insert );
  query.bindValue( 0, ui->mDate->date() );
  query.bindValue( 1, ui->mAmount->value()*100*multiplier );
  query.bindValue( 2, ui->mDescription->text().trimmed() );
  query.exec();

  updateData();

  ui->mAmount->setValue(0.0);
  ui->mDescription->setText("");
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


