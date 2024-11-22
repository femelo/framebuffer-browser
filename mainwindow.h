#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QWidget>
#include <QShortcut>
#include <QSettings>
#include <QQuickWidget>
#include <QtWebEngineWidgets/QWebEngineFullScreenRequest>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineView>

class MainWindow : public QMainWindow {
  Q_OBJECT

  // Define signals and slots
  QString optStartupUrl;
  int optWidth = 0;
  int optHeight = 0;
  QString optProxyHost = "";
  int optProxyPort = 0;

 public:
  QQuickWidget *qmlView;
  QWebEngineView *webView;
  explicit MainWindow();
  ~MainWindow();
  void configureWebView();
  void loadConfig();
  void loadUri(QString uri);

 public slots:
  void loadQml(QString uri);
  void loadUrl(QString url);
  void loadStartupUrl();
  void onLoadStarted();
  void slotShortcutCtrlQ();
  void slotShortcutBack();
  void slotShortcutForward();
 private:
  QShortcut *keyCtrlQ; // Entity of Ctrl + Q hotkeys
  QShortcut *keyBack; // back in history
  QShortcut *keyForward; // forward in history
  void fullScreenRequested(QWebEngineFullScreenRequest request);
  void writeSettings();
  QSettings *appSettings;
 protected:
  void closeEvent(QCloseEvent *);
};

#endif  // BROWSERWINDOW_H
