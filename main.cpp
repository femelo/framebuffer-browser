
#include <qapplication.h>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication *app = new QApplication(argc, argv);
    app->setApplicationName(QStringLiteral("framebuffer-browser"));
    app->setOrganizationDomain(QStringLiteral("fdemelo"));
    MainWindow* window = new MainWindow();
    window->show();
    int ret = app->exec();
    delete window;
    delete app;
    return ret;
}
