#ifndef LABELWITHTEXT_H
#define LABELWITHTEXT_H

#include <QLabel>
#include <QDateTime>

class LabelWithText : public QLabel
{
    Q_OBJECT
public:
    explicit LabelWithText(QWidget *parent = 0);

    void setImage(const QImage& image);
    QImage image() const;
    QPair<float, float> getRatios() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* event) override;

signals:

public slots:

private:
    static QRect textRectangle();
    void showText(QPainter &painter, const QString& text);

    QPoint m_startsLine;
    QPoint m_endsLine;

    QDateTime m_refreshed;

    QImage m_text;
};

#endif // LABELWITHTEXT_H
