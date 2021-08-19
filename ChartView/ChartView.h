#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QChart>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QGraphicsLineItem>
#include <QTimer>
#include <QValueAxis>
#include <QLineSeries>
#include <QRandomGenerator>
QT_CHARTS_USE_NAMESPACE
class ChartView : public QChartView
{
    Q_OBJECT
public:
    ChartView(QWidget *parent);
    ChartView(QPoint x, int kx, QPoint y, int ky, QString title, QWidget *parent);
    void appendData(qreal pointf);

private:
    int m_minX,m_maxX;
    int m_minY,m_maxY;
    int m_countX,m_countY;
    int chart_i=0;
    qreal m_dwidth,m_dheight;
    QValueAxis *axisy;
    QLineSeries *series;
    QValueAxis *axisx;
    QChart *chart;
    QChartView *chartView;

    //鼠标控制参数
    bool isClicking;

    int xOld=0;
    int yOld=0;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
};


class Callout : public QGraphicsItem
{
public:
    Callout(QChart *parent);

    void setText(const QString &text);
    void setAnchor(QPointF point);
    void updateGeometry();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QString text;
    QRectF textRect;
    QRectF rect;
    QPointF anchor;
    QFont font;
    QChart *chart;
};

#endif // CHARTVIEW_H
