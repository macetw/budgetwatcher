#include "mainwindow.h"
#include <QApplication>

/*---------------------------------------------------------------------
 *       main()
 */
/*! 
 * \brief   Runs the application.
 *
 * \param argc   Argument count
 * \param argv   Argument list
 *
 * \return   Status code.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
