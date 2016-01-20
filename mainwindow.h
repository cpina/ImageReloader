#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool event(QEvent* event) override;

private Q_SLOTS:
    void refreshImage();
    void replyFinished(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;
    QTimer m_timer;
    QPixmap m_image;
    QNetworkAccessManager m_networkAccessManager;
};

#endif // MAINWINDOW_H
