#ifndef NAIVEALGORITHM_H
#define NAIVEALGORITHM_H
#include <QPainterPath>
#include <QWidget>
#include <vector>
#include <QPainter>
#include<QColor>
class NaiveAlgorithm : public QWidget
{
    Q_OBJECT
public:
    explicit NaiveAlgorithm(int, int, int, int, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*)
    {
        static QColor brightPurple(128, 0, 128);
        QPainter painter1(this);
        double h = this->height();
        double w = this->width();
        h/=2;
        w/=2;
        double hScale = 20;
        double wScale = 20;
        painter1.translate(w,h);
        painter1.setPen(QPen(brightPurple, 3));
        painter1.setBrush(brightPurple);
        for(int i = 0; i < points.size(); i++){
            painter1.drawRect(points[i].x()*wScale,-(points[i].y()+1)*hScale,wScale,hScale);
        }
        QPainterPath text;
        QFont f ("Times", 8);
        f.setStyleStrategy(QFont::ForceOutline);
        painter1.setPen(QPen(Qt::gray, 1));
        painter1.drawLine(-this->width()/2, 0, this->width()/2, 0);
        painter1.drawLine(0, -this->height()/2, 0, this->height()/2);
        painter1.setPen(QPen(Qt::gray, 3));
        text.addText(0, 0, f, QString::number(0));
        for(int i = 1; i < this->width()/(wScale*2); i++){
            painter1.setPen(QPen(Qt::gray, 3));
            painter1.drawPoint(i*wScale, 0);
            text.addText(i*wScale, 12, f, QString::number(i));
            painter1.drawPoint(-i*wScale, 0);
            text.addText(-i*wScale, 12, f, QString::number(-i));
            painter1.setPen(QPen(Qt::gray, 1));
            painter1.drawLine(-i*wScale, -this->height()/2, -i*wScale, this->height()/2);
            painter1.drawLine(i*wScale, -this->height()/2, i*wScale, this->height()/2);
        }
        for(int i = 1; i < this->height()/(hScale*2); i++){
            painter1.setPen(QPen(Qt::gray, 3));
            painter1.drawPoint(0, i*hScale);
            text.addText(0, -i*hScale, f, QString::number(i));
            painter1.drawPoint(0, -i*hScale);
            text.addText(0, i*hScale, f, QString::number(-i));
            painter1.setPen(QPen(Qt::gray, 1));
            painter1.drawLine(-this->width()/2, -i*hScale, this->width()/2, -i*hScale);
            painter1.drawLine(-this->width()/2, i*hScale, this->width()/2, i*hScale);
        }
        painter1.setPen(QPen(Qt::gray, 1));
        painter1.setBrush(QBrush(Qt::gray));
        painter1.drawPath(text);
    }
signals:

private:
    std::vector <QPointF> points;
};

#endif // NAIVEALGORITHM_H
