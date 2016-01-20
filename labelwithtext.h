#ifndef LABELWITHTEXT_H
#define LABELWITHTEXT_H

#include <QLabel>

class LabelWithText : public QLabel
{
    Q_OBJECT
public:
    explicit LabelWithText(QWidget *parent = 0);

    void setImage(const QImage& image);

protected:
    void paintEvent(QPaintEvent* event) override;

signals:

public slots:

private:
    static QRect textRectangle();

    QImage m_text;
};

#endif // LABELWITHTEXT_H
