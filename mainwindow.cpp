#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &MainWindow::accept );
}

void MainWindow::accept()
{
  qApp->closeAllWindows();
}

MainWindow::~MainWindow()
{
    delete ui;
}
