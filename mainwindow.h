#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSqlQueryModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void accept();

private:
  void updateData();

  Ui::MainWindow *ui;

  QSqlQueryModel *model;
};

#endif // MAINWINDOW_H
