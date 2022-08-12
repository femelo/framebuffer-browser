#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QWidget>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineView>

class JSBridge : public QObject {
  Q_OBJECT
 public:
  QMainWindow *mainWindow;
  JSBridge(QMainWindow *mainWindow) : QObject(mainWindow) {
    this->mainWindow = mainWindow;
  }
};

class OSBridge : public QObject {
  Q_OBJECT
 public:
  QMainWindow *mainWindow;
  OSBridge(QMainWindow *mainWindow) : QObject(mainWindow) {
    this->mainWindow = mainWindow;
  }
 public slots:
  QString runCmd(QStringList args);
};

class MainWindow : public QMainWindow {
  Q_OBJECT

  // Define signals and slots
  QString optStartupUrl = "./www/index.html";
  int optWidth = 0;
  int optHeight = 0;
  bool optEnableJSBridge = true;
  bool optEnableOSBridge = true;
  QString optProxyHost = "";
  int optProxyPort = 0;

 public:
  JSBridge jsBridge;
  OSBridge osBridge;
  QWebEngineView *webView;
  explicit MainWindow();
  ~MainWindow();
  void configureWebView();
  void loadConfig();
  void loadUrl(QString url);

 public slots:
  void loadStartupUrl();
  void onLoadStarted();
};

#endif  // BROWSERWINDOW_H
