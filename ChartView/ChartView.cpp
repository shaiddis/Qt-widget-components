#include "ChartView.h"
#include <QDebug>

ChartView::ChartView(QWidget *parent = 0):
    m_minX(-50), m_maxX(50), m_countX(20),m_minY(-20), m_maxY(20),m_countY(20)
{
    setRubberBand(QChartView::RectangleRubberBand);
    axisx = new QValueAxis();
    axisx->setRange(m_minX,m_maxX);
    axisx->setTickCount(m_countX+1);
    axisx->setLabelFormat("%d");

    axisy=new QValueAxis;
    axisy->setRange(m_minY,m_maxY);
    axisy->setTickCount(m_countY+1);
    //axisy->setLabelFormat("%d");
    series =new QLineSeries();
    chart= new QChart();
    chart->createDefaultAxes();
    chart->addSeries(series);
    chart->setTitle("ShowMe");
    chart->setAxisX(axisx,series);
    chart->setAxisY(axisy,series);
    //chart->setAxisY(axisy);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();

    this->setChart(chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setRubberBand(QChartView::RectangleRubberBand);

    //m_dwidth = chart->plotArea().width()/(m_maxX-m_minX-1); //一次滚动多少宽度
//    m_dheight= chart->plotArea().height()/((m_countY-1)*5);
//    m_dwidth=8;
}

ChartView::ChartView(QPoint x, int kx, QPoint y, int ky,QString title,QWidget *parent=0):
    m_minX(x.rx()), m_maxX(x.ry()), m_countX(kx),m_minY(y.rx()), m_maxY(y.ry()),m_countY(ky)
{

    setRubberBand(QChartView::RectangleRubberBand);
    axisx = new QValueAxis();
    axisx->setRange(x.rx(),x.ry());
    axisx->setTickCount(kx+1);
    //axisx->setLabelFormat("%d");

    axisy=new QValueAxis;
    axisy->setRange(y.rx(),y.ry());
    axisy->setTickCount(ky+1);
    //axisy->setLabelFormat("%d");
    series =new QLineSeries();
    chart= new QChart();
    chart->createDefaultAxes();
    chart->addSeries(series);
    chart->setTitle(title);
    chart->setAxisX(axisx,series);
    chart->setAxisY(axisy,series);
    //chart->setAxisY(axisy);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();

    this->setChart(chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setRubberBand(QChartView::RectangleRubberBand);

    m_dwidth = chart->plotArea().width()/(m_maxX-m_minX-1); //一次滚动多少宽度
    m_dheight= chart->plotArea().height()/(m_maxY-m_minX-1);


}

void ChartView::appendData(qreal pointf){
    QPointF p((qreal) chart_i, pointf);
    *series << p;
    //qreal dx= 10/(axisx->tickCount()*2);
//    if(chart_i>10){

//        //chart->axisX()->setRange(chart_i-20,chart_i);
//        chart->scroll(m_dwidth, 0);
//        qDebug()<<"m_dwidth:"<<m_dwidth;
//    }
    if(chart_i<=40){
        chart->axisX()->setRange(m_minX,m_maxX);
    }else{
        chart->axisX()->setRange(chart_i+m_minX-40,m_maxX+chart_i-40);
    }
    qDebug()<<"chart_i:"<<chart_i;
    this->update();
    chart_i++;
}

//鼠标事件
void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        isClicking = true;
    } else if (event->button() & Qt::RightButton) {
        chart->zoomReset();
    }

    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    int x, y;

    if (isClicking) {
        if (xOld == 0 && yOld == 0) {

        } else {
            x = event->x() - xOld;
            y = event->y() - yOld;
            chart->scroll(-x, y);
        }

        xOld = event->x();
        yOld = event->y();

        return;
    }

    QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (isClicking) {
        xOld = yOld = 0;
        isClicking = false;
    }

    /* Disable original right click event */
    if (!(event->button() & Qt::RightButton)) {
        QChartView::mouseReleaseEvent(event);
    }
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart->zoomIn();
        break;
    case Qt::Key_Minus:
        chart->zoomOut();
        break;
    case Qt::Key_Left:
        chart->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart->scroll(10, 0);
        break;
    case Qt::Key_Up:
        chart->scroll(0, 10);
        break;
    case Qt::Key_Down:
        chart->scroll(0, -10);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        chart->zoom(1.1);
    } else {
        chart->zoom(10.0/11);
    }

    QWidget::wheelEvent(event);
}
