#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

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

  QString select( "SELECT date,cents,title FROM transactions "
                  "WHERE account=1 ORDER BY date;" );

  QSqlQuery query( select, db );

  model->setQuery( query );
}


MainWindow::~MainWindow()
{
    delete ui;
}
