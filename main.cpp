

#include <QApplication>
#include <qapplication.h>
#include "config.h"

#ifdef FULLBROWSER
#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QtWebEngineWidgets/QWebEngineSettings>
#include <QtWebEngineWidgets/QWebEngineProfile>

QUrl commandLineUrlArgument()
{
    const QStringList args = QCoreApplication::arguments();
    for (const QString &arg : args.mid(1)) {
        if (!arg.startsWith(QLatin1Char('-')))
            return QUrl::fromUserInput(arg);
    }
    return QUrl(QStringLiteral("https://www.google.com"));
}
#else
#include "mainwindow.h"
#endif

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication *app = new QApplication(argc, argv);
    app->setApplicationName(QStringLiteral("framebuffer-browser"));
    app->setOrganizationDomain(QStringLiteral("fdemelo"));
#ifdef FULLBROWSER
    qDebug() << "full browser mode enabled";
    // fix font
    QFont font;
    font.setPixelSize(12);
    app->setFont(font);
    app->setWindowIcon(QIcon(QStringLiteral(":AppLogoColor.png")));
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    QWebEngineProfile::defaultProfile()->setUseForGlobalCertificateVerification();
#endif
    QUrl url = commandLineUrlArgument();
    Browser *browser = Browser();
    BrowserWindow *window = browser->createWindow();
    window->tabWidget()->setUrl(url);
#else
    MainWindow* window = new MainWindow();
    window->show();
#endif
    int ret = app->exec();
    delete window;
    delete app;
    return ret;
}
