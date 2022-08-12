#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkProxy>
#include <QTimer>
#include <QProcess>


void MainWindow::configureWebView() {
  // Disable anti-aliasing for better performance
//  webView->setRenderHint(QPainter::Antialiasing, false);
//  webView->setRenderHint(QPainter::TextAntialiasing, false);
//  webView->setRenderHint(QPainter::SmoothPixmapTransform, false);
  // Enable basic JavaScript support
//  webView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
//  webView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
//  webView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
//  webView->settings()->setAttribute(QWebEngineSettings::OfflineStorageDatabaseEnabled,
  //                                  true);
 // webView->settings()->setAttribute(
  //    QWebEngineSettings::OfflineWebApplicationCacheEnabled, true);

  // Enable WebAudio
 // webView->settings()->setAttribute(QWebEngineSettings::WebAudioEnabled, true);
  // Don't allow JavaScript to open/close windows
 // webView->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows,
    //                                false);
  //webView->settings()->setAttribute(QWebEngineSettings::JavascriptCanCloseWindows,
  //                                  false);
  // Allow JavaScript to access clipboard
 // webView->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard,
   //                                 true);
  // Allow universal access from file URLs
 // webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls,true);
//  webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);

  // Show Web Inspector
//  webView->settings()->setAttribute(QWebEngineSettings::DeveloperExtrasEnabled, true);

  // Disallow cache
  
}

MainWindow::MainWindow()
    : QMainWindow(),
      jsBridge(this) ,
      osBridge(this) {
  loadConfig();

  // Load proxy settings
  if (optProxyHost != "") {
    qDebug() << "Setting proxy to" << optProxyHost << ":" << optProxyPort;
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(optProxyHost);
    proxy.setPort(optProxyPort);
    QNetworkProxy::setApplicationProxy(proxy);
  }

  // Set window size
  if (optWidth) {
      resize(optWidth, optHeight);
  }

  // Make our window full-screen
  // setWindowState(Qt::WindowFullScreen);

  // Create the web view
  webView = new QWebEngineView(this);
  configureWebView();
  // Add the web view to our window
  setCentralWidget(webView);
  // Handle page load events
  connect(webView, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));

  // After 1000ms, load the URL
  QTimer::singleShot(1000, this, SLOT(loadStartupUrl()));
}

MainWindow::~MainWindow() {}

void MainWindow::onLoadStarted() { 
    qDebug() << "onLoadStarted"; 
    bool enableJSBridge = false;
    bool enableOSBridge = false;

    enableJSBridge = optEnableJSBridge;
    enableOSBridge = optEnableOSBridge;

/*    if (enableJSBridge) {
      webView->page()->mainFrame()->addToJavaScriptWindowObject("FB_JSBridge", &jsBridge);
    }
    if (enableOSBridge) {
      webView->page()->mainFrame()->addToJavaScriptWindowObject("FB_OSBridge", &osBridge);
    }                                                          
*/
}

void MainWindow::loadUrl(QString url) {
  if (url.startsWith("./")) {
    url = "file:///" + QDir::currentPath() + QDir::separator() + url.mid(2);
  }
  qDebug() << "Loading URL:" << url;
  webView->load(QUrl(url));
}

void MainWindow::loadStartupUrl() { loadUrl(optStartupUrl); }

void MainWindow::loadConfig() {
  bool haveUrlInCmdLine = false;
  // Check for url in argv
  if (qApp->arguments().size() > 1) {
    optStartupUrl = qApp->arguments()[1];
    haveUrlInCmdLine = true;
  }
  // Load config.json
  QFile configFile("config.json");
  if (!configFile.open(QIODevice::ReadOnly)) {
    qDebug() << "Could not open config.json";
    return;
  }
  QByteArray configData = configFile.readAll();
  configFile.close();
  QJsonDocument configJson = QJsonDocument::fromJson(configData);
  QJsonObject configObject = configJson.object();
  // Load url
  if (configObject.contains("url") && (!haveUrlInCmdLine)) {
    optStartupUrl = configObject["url"].toString();
  }

  // Load window size
  if (configObject.contains("width")) {
    optWidth = configObject["width"].toInt();
  }
  if (configObject.contains("height")) {
    optHeight = configObject["height"].toInt();
  }
  // Load proxy settings
  if (configObject.contains("proxyHost")) {
    optProxyHost = configObject["proxyHost"].toString();
  }
  if (configObject.contains("proxyPort")) {
    optProxyPort = configObject["proxyPort"].toInt();
  }
  if (configObject.contains("enableJSBridge")) {
    optEnableJSBridge = configObject["enableJSBridge"].toBool();
  }
  if (configObject.contains("enableOSBridge")) {
    optEnableOSBridge = configObject["enableOSBridge"].toBool();
  }
}


QString OSBridge::runCmd(QStringList args) {
  QProcess proc;
  QString program = args[0];
  QStringList arguments = args.mid(1);
  proc.start(program, arguments);
  proc.waitForFinished();
  QString output = proc.readAllStandardOutput();
  return output;
}
