#include "labelwithtext.h"

#include <QDebug>
#include <QPainter>

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
}

void LabelWithText::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);

    if (pixmap())
    {
        float ratioX = float(width()) / pixmap()->width();
        float ratioY = float(height()) / pixmap()->height();

        qDebug() << ratioX << ratioY;

        if (ratioX < 0.8 || ratioY < 0.8)
        {
            painter.drawImage(width()-m_text.width(), 0, m_text);
        }
    }
}
