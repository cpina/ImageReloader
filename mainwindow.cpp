#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>
#include <QtNetwork/QNetworkReply>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connect(&m_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    m_timer.setInterval(2*60*1000);
    // m_timer.setInterval(2000);
    m_timer.start();

    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(refreshImage()));

    ui->setupUi(this);
    qDebug() << "=TEST";
    refreshImage();
    ui->image->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshImage()
{
    m_networkAccessManager.get(QNetworkRequest(QUrl("https://legacy.bas.ac.uk/webcams/rrs_james_clark_ross/webcam.jpg")));
}

void MainWindow::replyFinished(QNetworkReply* reply)
{
    QByteArray imageByteArray = reply->readAll();
    QImage image = QImage::fromData(imageByteArray);
    ui->image->setPixmap(QPixmap::fromImage(image));
    qDebug() << "Image Refreshed";
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_F5)
        {
            refreshImage();
            return true;
        }
    }
    return QMainWindow::event(event);
}
