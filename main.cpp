

#include <QApplication>
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
    QCoreApplication::setOrganizationName("e1z0");
    QApplication a(argc, argv);
#ifdef FULLBROWSER
    qDebug() << "full browser mode enabled";
    // fix font
    QFont font;
    font.setPixelSize(12);
    a.setFont(font);
    a.setWindowIcon(QIcon(QStringLiteral(":AppLogoColor.png")));
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    QWebEngineProfile::defaultProfile()->setUseForGlobalCertificateVerification();
#endif

    QUrl url = commandLineUrlArgument();

    Browser browser;
    BrowserWindow *window = browser.createWindow();
    window->tabWidget()->setUrl(url);
    
#else
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
#endif
    return a.exec();


}
