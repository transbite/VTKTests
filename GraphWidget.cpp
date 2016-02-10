#include "GraphWidget.h"

#include <QResizeEvent>
#include <QGraphicsTextItem>

GraphWidget::GraphWidget(QWidget* parent)
    : QWidget(parent)
    , ratio_(0.8)
    , colors_{&alpha_, &red_, &green_, &blue_}
    , colorLines_{&alphaLines_, &redLines_, &greenLines_, &blueLines_}
    , activeChannel_(0)
{
    ui_.setupUi(this);
    
    QSize sz = ui_.graphicsView->size();

    xSize_ = ratio_ * sz.width();
    ySize_ = ratio_ * sz.height();

    nxGridLines_ = 9;
    nyGridLines_ = 9;
    xGridLines_.reserve(nxGridLines_);
    yGridLines_.reserve(nyGridLines_);
    xGridStrings_.reserve(nxGridLines_);
    yGridStrings_.reserve(nyGridLines_);

    xMax_ = 400.0;
    xMin_ = -1200.0;
    yMax_ = 1.0;
    yMin_ = 0.0;

    nPoints_ = 200;

    alpha_.reserve(nPoints_);
    red_.reserve(nPoints_);
    green_.reserve(nPoints_);
    blue_.reserve(nPoints_);
    alphaLines_.reserve(nPoints_ - 1);
    redLines_.reserve(nPoints_ - 1);
    greenLines_.reserve(nPoints_ - 1);
    blueLines_.reserve(nPoints_ - 1);

    for (int i = 0; i < nPoints_; i++) {
        alpha_.push_back(0.5);
        red_.push_back(0.5);
        green_.push_back(0.5);
        blue_.push_back(0.5);
    }

    ui_.graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents);

    SetupScene();

    dragStarted_ = false;
    startIndex_ = -1;
    lastValue_ = 0.0;
    filter_ = 0.3;
}

void GraphWidget::InitData(const double &xmin, const double &xmax)
{
    xMin_ = xmin;
    xMax_ = xmax;
    const double defaultValue = 0.3;
    for (int i = 0; i < nPoints_; ++i)
    {
        alpha_[i] = defaultValue;
        red_[i] = defaultValue;
        green_[i] = defaultValue;
        blue_[i] = defaultValue;
    }

    ResizeScene(true);
}

GraphWidget::~GraphWidget()
{

}

void GraphWidget::SetupScene()
{
    QGraphicsView* view = ui_.graphicsView;
    QGraphicsScene* scene = view->scene();
    if (scene) {
        scene->clear();
        xGridLines_.clear();
        yGridLines_.clear();
        xGridStrings_.clear();
        yGridStrings_.clear();
    } else {
        scene = new QGraphicsScene();
        view->setScene(scene);
    }
    scene->setSceneRect(0, 0, xSize_, ySize_);
    scene->setBackgroundBrush(QBrush(QColor(230, 230, 230, 230)));

    QTransform t;
    t.reset();
    view->setTransform(t);
    view->scale(1, -1);

    QPen pen(QColor(100, 100, 100, 200));
    pen.setStyle(Qt::DashLine);

    float dx = xSize_ / (nxGridLines_ - 1.0);
    for (int i = 0; i < nxGridLines_; i++) {
        QGraphicsLineItem* line = scene->addLine(i * dx, 0, i * dx, ySize_, pen);
        xGridLines_.push_back(line);
        QString str;
        QGraphicsTextItem* txt = scene->addText(str.sprintf("%.2f", xMin_ + i * (xMax_ - xMin_) / (nxGridLines_ - 1)));
        qreal tw = txt->boundingRect().width();
        txt->setPos(i * dx - tw / 2, 0);
        txt->setTransform(QTransform(1, 0, 0, -1, 0, 0));
        xGridStrings_.push_back(txt);
    }

    float dy = ySize_ / (nyGridLines_ - 1.0);
    for (int i = 0; i < nyGridLines_; i++) {
        QGraphicsLineItem* line = scene->addLine(0, i * dy, xSize_, i * dy, pen);
        yGridLines_.push_back(line);
        QString str;
        QGraphicsTextItem* txt = scene->addText(str.sprintf("%.2f", i * yMax_ / (nyGridLines_ - 1)));
        qreal tw = txt->boundingRect().width();
        qreal th = txt->boundingRect().height();
        txt->setPos(-tw / 2, i * dy + th);
        txt->setTransform(QTransform(1, 0, 0, -1, 0, 0));
        yGridStrings_.push_back(txt);
    }

    for(int i = 0; i < 4; ++i) {
        AddDataLines(i);
    }
}

void GraphWidget::AddDataLines(const int& channelIndex)
{
    std::vector<double>& points = *colors_[channelIndex];
    std::vector<QGraphicsLineItem*>& lines = *colorLines_[channelIndex];

    QGraphicsScene* scene = ui_.graphicsView->scene();
    lines.clear();
    QPen lpen;
    lpen.setWidth(2);
    switch(channelIndex) {
    case 0:
        lpen.setColor(QColor(0, 0, 0, 250));
        break;
    case 1:
        lpen.setColor(QColor(200, 0, 0, 200));
        break;
    case 2:
        lpen.setColor(QColor(0, 200, 0, 200));
        break;
    case 3:
        lpen.setColor(QColor(0, 0, 200, 200));
        break;

    }

    float dx, x0, y0, x1, y1;
    x0 = 0;
    y0 = ySize_ * points[0] / yMax_;
    dx = xSize_ / (nPoints_ - 1);
    for (int i = 1; i < nPoints_; i++) {
        x1 = i * dx;
        y1 = ySize_ * points[i] / yMax_;
        QGraphicsLineItem* line = scene->addLine(x0, y0, x1, y1, lpen);
        lines.push_back(line);
        x0 = x1;
        y0 = y1;
    }
}

void GraphWidget::ResizeScene(bool changeLabels)
{
    float dx = xSize_ / (nxGridLines_ - 1.0);
    for (int i = 0; i < nxGridLines_; i++) {
        QGraphicsLineItem* line = xGridLines_[i];
        line->setLine(i * dx, 0, i * dx, ySize_);
        QGraphicsTextItem* txt = xGridStrings_[i];
        qreal tw = txt->boundingRect().width();
        txt->setPos(i * dx - tw / 2, 0);
        if(changeLabels) {
            QString str;
            str.sprintf("%.2f", xMin_ + i * (xMax_ - xMin_) / (nxGridLines_ - 1));
            txt->setPlainText(str);
        }
    }

    float dy = ySize_ / (nyGridLines_ - 1.0);
    for (int i = 0; i < nyGridLines_; i++) {
        QGraphicsLineItem* line = yGridLines_[i];
        line->setLine(0, i * dy, xSize_, i * dy);
        QGraphicsTextItem* txt = yGridStrings_[i];
        qreal tw = txt->boundingRect().width();
        qreal th = txt->boundingRect().height();
        txt->setPos(-tw / 2, i * dy + th);
        if(changeLabels) {
            QString str;
            str.sprintf("%.2f", i * yMax_ / (nyGridLines_ - 1));
            txt->setPlainText(str);
        }
    }

    for(int i = 0; i < 4; ++i) {
        ResizeDataLines(i);
    }
}

void GraphWidget::ResizeDataLines(const int& channelIndex)
{
    std::vector<double>& points = *colors_[channelIndex];
    std::vector<QGraphicsLineItem*>& lines = *colorLines_[channelIndex];

    float dx, x0, y0, x1, y1;
    x0 = 0;
    y0 = ySize_ * points[0] / yMax_;
    dx = xSize_ / (nPoints_ - 1);
    for (int i = 1; i < nPoints_; i++) {
        QGraphicsLineItem* line = lines[i - 1];
        x1 = i * dx;
        y1 = ySize_ * points[i] / yMax_;
        line->setLine(x0, y0, x1, y1);
        x0 = x1;
        y0 = y1;
    }
}

void GraphWidget::resizeEvent(QResizeEvent* event)
{
    QSize sz = ui_.graphicsView->size();

    xSize_ = ratio_ * sz.width();
    ySize_ = ratio_ * sz.height();

    QGraphicsView* view = ui_.graphicsView;
    QGraphicsScene* scene = view->scene();
    scene->setSceneRect(0, 0, xSize_, ySize_);

    ResizeScene();

    QWidget::resizeEvent(event);
}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        dragStarted_ = true;
    }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        dragStarted_ = false;
        startIndex_ = -1;
        lastValue_ = 0.0;
    }
}

void GraphWidget::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView* view = ui_.graphicsView;
    QGraphicsScene* scene = view->scene();
    QPoint p = event->pos();
    QPoint pGr = view->mapFromParent(p);
    QPointF pSc = view->mapToScene(pGr);

    float dx = xSize_ / (nPoints_ - 1);
    float x = pSc.x();
    float y = pSc.y();
    int i = (x < 0) ? 0 : x / dx;
    i = i > (nPoints_ - 1) ? nPoints_ - 1 : i;
    double pointValue = yMax_ * y / ySize_;
    pointValue = pointValue < 0 ? 0 : (pointValue > 1 ? 1 : pointValue);
    if(dragStarted_) {
        startIndex_ = i;
        dragStarted_ = false;
        lastValue_ = pointValue;
    }
    pointValue = filter_ * pointValue + (1.0 - filter_) * lastValue_;
    lastValue_ = pointValue;
    ChangePoints(startIndex_, i, pointValue);
    //ChangeLines(startIndex_, i);
    ResizeDataLines(activeChannel_);
    startIndex_ = i;
    emit signalColorChanged(activeChannel_);

    if (view->rect().contains(pGr)) {
        return;
    }

    QWidget::mouseMoveEvent(event);
}

void GraphWidget::ChangePoints(const int &startIndex, const int &endIndex, const double &endValue)
{
    std::vector<double>& points = *colors_[activeChannel_];
    std::vector<QGraphicsLineItem*>& lines = *colorLines_[activeChannel_];

    if(startIndex == endIndex)
    {
        points[endIndex] = endValue;
        return;
    }

    double startValue = points[startIndex];
    double dx = (xMax_ - xMin_) / (nPoints_ - 1);
    double xStart = xMin_ + startIndex * dx;
    double xEnd = xMin_ + endIndex * dx;
    double a = (endValue - startValue) / (xEnd - xStart);
    int step = startIndex < endIndex ? 1 : -1;
    for(int i = startIndex + 1; i != (endIndex + step); i += step)
    {
        double x = xMin_ + i * dx;
        points[i] = startValue + a * (x - xStart);
    }
}

//void GraphWidget::ChangeLines(const int &startIndex, const int &endIndex)
//{
//    int minIndex = startIndex < endIndex ? startIndex : endIndex;
//    int maxIndex = endIndex > startIndex ? endIndex : startIndex;
//    int lineStartIndex = minIndex - 1;
//    lineStartIndex = lineStartIndex < 0 ? 0 : lineStartIndex;
//    int lineEndIndex = maxIndex > (nPoints_ - 2) ? nPoints_ - 2 : maxIndex;
//    float dx = xSize_ / (nPoints_ - 1);
//    for(int i = lineStartIndex; i <= lineEndIndex; ++i)
//    {
//        QGraphicsLineItem* line = lines_[i];
//        float x = i * dx;
//        float x1 = (i + 1) * dx;
//        line->setLine(x, ySize_ * points_[i] / yMax_, x1, ySize_ * points_[i + 1] / yMax_);
//    }
//}

//double GraphWidget::F(double x)
//{
//    if (x < 0 || x > xMax_) {
//        return 0.0;
//    }

//    double dx = xMax_ / (nPoints_ - 1);
//    int i = x / dx;

//    const double x1 = i * dx;
//    const double x2 = (i + 1) * dx;
//    const double y1 = points[i];
//    const double y2 = points[i + 1];

//    return ((x - x1) / (x2 - x1)) * (y2 - y1) + y1;
//}

void GraphWidget::OnChannelButtonToggled(bool toggled)
{
    if(!toggled)
    {
        return;
    }

    QObject* sender = this->sender();
    if(sender == ui_.rbAlpha)
    {
        activeChannel_ = 0;
    }
    if(sender == ui_.rbRed)
    {
        activeChannel_ = 1;
    }
    if(sender == ui_.rbGreen)
    {
        activeChannel_ = 2;
    }
    if(sender == ui_.rbBlue)
    {
        activeChannel_ = 3;
    }
}
