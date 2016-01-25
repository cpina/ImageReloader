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

void LabelWithText::showText(QPainter& painter, const QString &text)
{
    painter.setPen(Qt::black);

    QPair<float, float> ratios = getRatios();

    QRect rect(QPoint(440*ratios.first, 10*ratios.second), QPoint(640*ratios.first, 25*ratios.second));
    painter.fillRect(rect, QBrush(Qt::SolidPattern));

    painter.setPen(Qt::yellow);
    QFont font;
    font.setFamily("Monospaced");
    font.setBold(true);
    painter.setFont(font);

    rect.setBottom(rect.bottom()-2);
    painter.drawText(rect.bottomLeft(), text);
}

void LabelWithText::setImage(const QImage &image)
{
    m_text = image.copy(textRectangle());

    // Changed scaled if we don't want to magnify the text
    m_text = m_text.scaled(m_text.size() * 1);

    setPixmap(QPixmap::fromImage(image));
    m_refreshed = QDateTime::currentDateTimeUtc();

    QTimer::singleShot(OSD_TIMEOUT, this, SLOT(repaint()));
}

QPair<float, float> LabelWithText::getRatios() const
{
    if (!pixmap())
    {
        return QPair<float,float>(1,1);
    }
    float ratioX = float(width()) / pixmap()->width();
    float ratioY = float(height()) / pixmap()->height();

    return QPair<float, float>(ratioX, ratioY);
}

void LabelWithText::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);

    if (pixmap())
    {
        QPair<float,float> ratios = getRatios();

        if (ratios.first < 0.8 || ratios.second < 0.8)
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

        if (angle > 270)
        {
            angle-= 360;
        }
        else if(angle > 90)
        {
            angle-=180;
        }

        angle = qAbs(angle);

        QString angleText = QString::fromUtf8("%1°").arg(QString::number(angle,'f',2));

        showText(painter, angleText);
    }

    if ((m_refreshed.msecsTo(QDateTime::currentDateTimeUtc())) < OSD_TIMEOUT)
    {
        showText(painter, tr("Just refreshed"));
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

