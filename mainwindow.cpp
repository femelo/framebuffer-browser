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
#include <QDebug>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineProfile>
#include <QWebEngineSettings>


void MainWindow::closeEvent(QCloseEvent *) {
  // This will be called whenever this window is closed.
  writeSettings();
  qDebug() << "closing2";
}

void MainWindow::configureWebView() {
  // Enable basic JavaScript support
  webView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
  webView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
  webView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
  // Don't allow JavaScript to open/close windows
  webView->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows,false);
  // Allow JavaScript to access clipboard
  webView->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard,true);
  // Allow universal access from file URLs
  webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls,true);
  webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
  // Enable full screen request
  webView->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
}


void MainWindow::writeSettings() {
  // Write the values to disk in categories.
  QString site = webView->url().toString();
  appSettings->setValue("site", site);
  qDebug() << " write settings:" << site;
  QString hostname = webView->url().host();
  webView->setUrl(appSettings->value("site").toString());
}

void MainWindow::fullScreenRequested(QWebEngineFullScreenRequest request) {
  // fullscreen on video players

  if (request.toggleOn() && !this->isFullScreen()) {
    this->showFullScreen();
  } else {
    this->showNormal();
  }
  request.accept();
}

// Slot handler for Ctrl + Q
void MainWindow::slotShortcutCtrlQ() {
  qDebug() << "closing1";
//  writeSettings();
  webView->deleteLater(); // leave gracefully
  QApplication::quit();
}

// Slot handler for Ctrl + Left
void MainWindow::slotShortcutBack() {
   qDebug() << "Going back..";
   webView->page()->triggerAction(QWebEnginePage::Back);
}

// Slot handler for Ctrl + Right
void MainWindow::slotShortcutForward() {
   qDebug() << "Going forward..";
   webView->page()->triggerAction(QWebEnginePage::Forward);
}

MainWindow::MainWindow()
    : QMainWindow() {
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

  // Create the web view
  webView = new QWebEngineView(this);

  configureWebView();
  // Add the web view to our window
  setCentralWidget(webView);

  // connect handler for fullscreen press on video
  connect(webView->page(), &QWebEnginePage::fullScreenRequested, this, &MainWindow::fullScreenRequested);

  // Ctrl + Q
  keyCtrlQ = new QShortcut(this);         // Initialize the object
  keyCtrlQ->setKey(Qt::CTRL | Qt::Key_Q); // Set the key code
  // connect handler to keypress
  connect(keyCtrlQ, SIGNAL(activated()), this, SLOT(slotShortcutCtrlQ()));

  keyBack = new QShortcut(this);
  keyBack->setKey(Qt::CTRL | Qt::Key_Left);
  connect(keyBack, SIGNAL(activated()), this, SLOT(slotShortcutBack()));

  keyForward = new QShortcut(this);
  keyForward->setKey(Qt::CTRL | Qt::Key_Right);
  connect(keyForward, SIGNAL(activated()), this, SLOT(slotShortcutForward()));

  this->webView->page()->profile()->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64; rv:89.0) Gecko/20100101 Firefox/89.0");

  // Handle page load events
  connect(webView, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));

  // After 1000ms, load the URL
  QTimer::singleShot(1000, this, SLOT(loadStartupUrl()));
}

MainWindow::~MainWindow() {}

void MainWindow::onLoadStarted() { 
    qDebug() << "onLoadStarted"; 
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
}



