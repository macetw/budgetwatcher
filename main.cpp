#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

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
    a.setStyle( QStyleFactory::create("Fusion") );
    MainWindow w;
    w.show();

    return a.exec();
}
