#ifndef OGRAPHWIDGET_H
#define OGRAPHWIDGET_H

#include "ui_GraphWidget.h"
#include <vector>

class GraphWidget : public QWidget
{
  Q_OBJECT
public:
    GraphWidget(QWidget* parent = nullptr);
    ~GraphWidget();

//    double F(double x);
    inline double Xmax() {return xMax_;}
    inline double Xmin() {return xMin_;}
    const std::vector<double>& Points(const int& channelIndex) {return *colors_[channelIndex];}
    
    void InitData(const double& xmin, const double& xmax);

signals:
    void signalColorChanged(int);

protected:
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private slots:
    void OnChannelButtonToggled(bool toggled);

private:
    void SetupScene();
    void AddDataLines(const int& channelIndex);
    void ResizeScene(bool changeLabels = false);
    void ResizeDataLines(const int& channelIndex);
    void ChangePoints(const int& startIndex, const int& endIndex, const double& endValue);
//    void ChangeLines(const int& startIndex, const int& endIndex);

private:
    Ui_GraphWidget ui_;

    //graphics view inf
    double xSize_;
    double ySize_;
    double ratio_;

    //graph inf
    int nxGridLines_;
    int nyGridLines_;

    int nPoints_;
    double xMax_;
    double xMin_;
    double yMax_;
    double yMin_;
    std::vector<double>* colors_[4];
    //std::vector<double> points;
    std::vector<double> alpha_;
    std::vector<double> red_;
    std::vector<double> green_;
    std::vector<double> blue_;
    std::vector<QGraphicsLineItem*>* colorLines_[4];
    //std::vector<QGraphicsLineItem*> lines;
    std::vector<QGraphicsLineItem*> alphaLines_;
    std::vector<QGraphicsLineItem*> redLines_;
    std::vector<QGraphicsLineItem*> greenLines_;
    std::vector<QGraphicsLineItem*> blueLines_;
    std::vector<QGraphicsLineItem*> xGridLines_;
    std::vector<QGraphicsLineItem*> yGridLines_;
    std::vector<QGraphicsTextItem*> xGridStrings_;
    std::vector<QGraphicsTextItem*> yGridStrings_;
    int activeChannel_;

    bool dragStarted_;
    int startIndex_;
    double lastValue_;
    double filter_;
};

#endif // OGRAPHWIDGET_H
