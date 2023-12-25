#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <brforcircle.h>
#include <brforsegment.h>
#include <QGridLayout>
#include <naivealgorithm.h>
#include <QPushButton>
#include<QRadioButton>
#include <QLabel>
#include <dda.h>
#include<QLineEdit>

enum dialogType{
    segment,
    circle
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QPointF getP0(){
        return p0;
    }
    QPointF getP1(){
        return p1;
    }
    int getRadius(){
        return radius;
    }
    ~MainWindow();
public slots:
    void showBrCircle();
    void showBrSegment();
    void showNaive();
    void showDDA();
    void outOfRangeError();
    void onBrCycleRadioButtonToggled(bool checked);
    void onBrSegmentRadioButtonToggled(bool checked);
    void onNativeRadioButtonToggled(bool checked);
    void onDDARadioButtonToggled(bool checked);
    void onOKButClicked();
    void setText();
    void SetStyle();
    void setX0(){
        p0.setX(leX0->text().toInt());
        qDebug()<<1;
    }
    void setX1(){
        p1.setX(leX1->text().toInt());
    }
    void setY0(){
        p0.setY(leY0->text().toInt());
    }
    void setY1(){
        p1.setY(leY1->text().toInt());
    }
    void setRadius(){
        radius = leRadius->text().toInt();
    }
private:
    Ui::MainWindow *ui;
    QGridLayout *l = new QGridLayout;
    BrForCircle *brfc = new BrForCircle(0,0,5);
    BrForSegment *brfs = new BrForSegment(0,0,5,5);
    NaiveAlgorithm *nativeEl = new NaiveAlgorithm(0,0,5,5);
    DDA *dda = new DDA(0,0,5,5);
    QPushButton *pbBrCircle = new QPushButton;
    QPushButton *pbBrSegment= new QPushButton;
    QRadioButton *BrCycleRBut = new QRadioButton;
    QRadioButton *BrSegmentRBut = new QRadioButton;
    QRadioButton *NativeRBut = new QRadioButton;
    QRadioButton *DDARBut = new QRadioButton;
    QPushButton *pbNaiveAlgorithm = new QPushButton;
    QLabel* lText = new QLabel;
    QPushButton *pbDDA = new QPushButton;
    QLineEdit *leX0 = new QLineEdit;
    QLineEdit *leY0 = new QLineEdit;
    QLineEdit *leX1 = new QLineEdit;
    QLineEdit *leY1 = new QLineEdit;
    QLineEdit *leRadius = new QLineEdit;
    QPointF p0;
    QPointF p1;
    int radius;
    QPushButton *OKBut = new QPushButton;
    dialogType myDialogType = circle;
    bool pressed = false;
    int choosed =0;

};
#endif // MAINWINDOW_H
