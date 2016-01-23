#include "labelwithtext.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

int OSD_TIMEOUT = 5000;

LabelWithText::LabelWithText(QWidget *parent) :
    QLabel(parent)
{
}

QRect LabelWithText::textRectangle()
{
    return QRect(440, 0, 640-440, 11);
}

void LabelWithText::setImage(const QImage &image)
{
    m_text = image.copy(textRectangle());

    // Changed scaled if we don't want to magnify the text
    m_text = m_text.scaled(m_text.size() * 2);

    setPixmap(QPixmap::fromImage(image));
    m_refreshed = QDateTime::currentDateTimeUtc();

    QTimer::singleShot(OSD_TIMEOUT, this, SLOT(update()));
}

void LabelWithText::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);

    if (pixmap())
    {
        float ratioX = float(width()) / pixmap()->width();
        float ratioY = float(height()) / pixmap()->height();

        if (ratioX < 0.8 || ratioY < 0.8)
        {
            painter.drawImage(width()-m_text.width(), 0, m_text);
        }
    }

    if (!m_startsLine.isNull() && !m_endsLine.isNull())
    {
        painter.setPen(QPen(Qt::yellow));

        QLineF line(m_startsLine, m_endsLine);

        painter.drawLine(line);

        qreal angle = line.angle();

        // angle = qAbs(angle);

        if (angle > 270)
        {
            angle-= 360;
        }
        else if(angle > 90)
        {
            angle-=180;
        }

        angle = qAbs(angle);

        QString angleText = QString::fromUtf8("%1°").arg(angle);

        painter.setPen(QPen(Qt::black));
        painter.drawText(10,100, angleText);
    }

    if ((m_refreshed.msecsTo(QDateTime::currentDateTimeUtc())) < OSD_TIMEOUT)
    {
        painter.setPen(QPen(Qt::black));
        painter.drawText(10,100, tr("Just refreshed"));
    }
}

bool LabelWithText::event(QEvent* event)
{
    bool change = false;

    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent *>(event);
        m_startsLine = mouseEvent->pos();
        change = true;
    }
    else if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent *>(event);
        m_endsLine = mouseEvent->pos();
        change = true;
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        m_startsLine = QPoint();
        m_endsLine = QPoint();
        change = true;
    }

    if (change)
    {
        update();
    }

    return QLabel::event(event);
}

