#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  const bool supported = QSqlDatabase::isDriverAvailable( "QMYSQL" );
  if (!supported) {
    QMessageBox::critical( this, "Unsupported Database", "MySQL is not supported on this device. Cannot continue." );
    accept();
  }

  connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &MainWindow::accept );

  model = new QSqlQueryModel;
  ui->tableView->setModel( model );

  updateData();
}

void MainWindow::accept()
{
  qApp->closeAllWindows();
}

void MainWindow::updateData()
{
  QSqlDatabase db = QSqlDatabase::addDatabase( "QMYSQL" );
  db.setHostName( "localhost" );
  db.setUserName( "budgetmaker" );
  db.setDatabaseName( "budget" );
  db.open();

  //QString select( "SELECT Date,cents/100 AS Value,title AS Description FROM transactions "
  //                "WHERE account=1 ORDER BY date;" );
  QString select( "SELECT Date, cents/100 AS Value, ( select sum(-cents) + (select balance from balances where account = 1) from transactions where date <= O.date AND account = O.account) AS Balance, title as Description from transactions O where account = 1 order by date;" );

  QSqlQuery query( select, db );

  model->setQuery( query );
}


MainWindow::~MainWindow()
{
    delete ui;
}
