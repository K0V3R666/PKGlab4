#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <iostream>
#include <QtMath>
#include <QMessageBox>
#include <QIntValidator>
#include <QPainter>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    connect(BrCycleRBut, &QRadioButton::toggled, this, &MainWindow::onBrCycleRadioButtonToggled);
    connect(BrSegmentRBut, &QRadioButton::toggled, this, &MainWindow::onBrSegmentRadioButtonToggled);
    connect(NativeRBut, &QRadioButton::toggled, this, &MainWindow::onNativeRadioButtonToggled);
    connect(DDARBut, &QRadioButton::toggled, this, &MainWindow::onDDARadioButtonToggled);
    connect(OKBut, &QPushButton::clicked, this, &MainWindow::onOKButClicked);
    connect(leX0, &QLineEdit::textChanged, [this] { setX0(); });
    connect(leX1, &QLineEdit::textChanged, [this] { setX1(); });
    connect(leY0, &QLineEdit::textChanged, [this] { setY0(); });
    connect(leY1, &QLineEdit::textChanged, [this] { setY1(); });
    connect(leRadius, &QLineEdit::textChanged, [this] { setRadius(); });

    this->setMinimumSize(500, 600);
    ui->setupUi(this);
    lText->setText("Какой алгоритм?");
    pbBrCircle->setText("Алгоритм круга Брезенхэма");
    pbBrSegment->setText("Сегментный алгоритм Брезенхэма");
    pbNaiveAlgorithm->setText("Нативный алгоритм");
    pbDDA->setText("DDA");
    lText->setAlignment(Qt::AlignCenter);
    l->addWidget(lText, 0, 0, 1, 4);
    l->addWidget(pbBrCircle, 12, 0, 1, 1);
    l->addWidget(BrCycleRBut, 11, 0, 1, 1);
    l->addWidget(pbBrSegment, 12, 1, 1, 1);
    l->addWidget(BrSegmentRBut, 11, 1, 1, 1);
    l->addWidget(pbNaiveAlgorithm, 12, 2, 1, 1);
    l->addWidget(NativeRBut, 11, 2, 1, 1);
    l->addWidget(pbDDA, 12, 3, 1, 1);
    l->addWidget(DDARBut, 11, 3, 1, 1);
    l->addWidget(dda, 1, 0, 10, 4);
    p0.setX(0);
    p0.setY(0);
    p1.setX(0);
    p1.setY(0);
    radius = 0;
    leX0->setValidator(new QIntValidator(-100, 100, this));
    leX1->setValidator(new QIntValidator(-100, 100, this));
    leY0->setValidator(new QIntValidator(-100, 100, this));
    leY1->setValidator(new QIntValidator(-100, 100, this));
    leRadius->setValidator(new QIntValidator(0, 100, this));
    dda->hide();
    BrCycleRBut->setChecked(true);
    nativeEl->hide();
    brfc->hide();
    brfs->hide();
    ui->centralwidget->setLayout(l);
    showBrCircle();
    brfc = new BrForCircle(p0.x(), p0.y(), radius);
    l->addWidget(brfc, 1, 0, 10, 4);
    SetStyle();
}

MainWindow::~MainWindow()
{
    delete brfc;
    delete brfs;
    delete nativeEl;
    delete dda;
    delete ui;
}

void MainWindow::showBrCircle()
{
    nativeEl->hide();
    brfc->hide();
    brfs->hide();
    dda->hide();
    QPointF p0;
    int radius;

    QElapsedTimer timer;
    timer.start();

    auto start = std::chrono::high_resolution_clock::now();
    while (true)
    {
        if (this->pressed)
        {
            radius = this->getRadius();
            p0 = this->getP0();
        }
        else
        {
            lText->setText("Какой алгоритм?");
            return;
        }
        if (qAbs(p0.x()) >= this->height() / 20 || qAbs(p0.y()) >= this->width() / 20)
        {
            outOfRangeError();
        }
        else
        {
            break;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    statusBar()->showMessage(QString("Алгоритм круга Брезенхэма выполнен за 1 ms").arg(duration.count()));

    brfc = new BrForCircle(p0.x(), p0.y(), radius);
    l->addWidget(brfc, 1, 0, 10, 4);
    lText->setText("Алгоритм круга Брезенхэма");
}

void MainWindow::showBrSegment()
{
    nativeEl->hide();
    brfc->hide();
    brfs->hide();
    dda->hide();
    QPointF p0;
    QPointF p1;

    QElapsedTimer timer;
    timer.start();

    while (true)
    {
        if (this->pressed)
        {
            p1 = this->getP1();
            p0 = this->getP0();
        }
        else
        {
            lText->setText("Какой алгоритм?");
            return;
        }
        if (qAbs(p0.x()) >= this->height() / 20 || qAbs(p0.y()) >= this->width() / 20 || qAbs(p1.x()) >= this->height() / 20 || qAbs(p1.y()) >= this->width() / 20)
        {
            outOfRangeError();
        }
        else
        {
            break;
        }
    }

    qint64 elapsed = timer.elapsed();
    statusBar()->showMessage(QString("Сегментный алгоритм Брезенхэма, выполненный в% 1ms").arg(elapsed));

    brfs = new BrForSegment(p0.x(), p0.y(), p1.x(), p1.y());
    l->addWidget(brfs, 1, 0, 10, 4);
    lText->setText("Сегментный алгоритм Брезенхэма");
    this->pressed = false;
}

void MainWindow::showNaive()
{
    nativeEl->hide();
    brfc->hide();
    brfs->hide();
    dda->hide();
    QPointF p0;
    QPointF p1;

    QElapsedTimer timer;
    timer.start();

    while (true)
    {
        if (this->pressed)
        {
            p1 = this->getP1();
            p0 = this->getP0();
        }
        else
        {
            lText->setText("Какой алгоритм?");
            return;
        }
        if (qAbs(p0.x()) >= this->height() / 20 || qAbs(p0.y()) >= this->width() / 20 || qAbs(p1.x()) >= this->height() / 20 || qAbs(p1.y()) >= this->width() / 20)
        {
            outOfRangeError();
        }
        else
        {
            break;
        }
    }

    qint64 elapsed = timer.elapsed();
    statusBar()->showMessage(QString("Наивный алгоритм, выполненный в% 1ms").arg(elapsed));

    nativeEl = new NaiveAlgorithm(p0.x(), p0.y(), p1.x(), p1.y());
    l->addWidget(nativeEl, 1, 0, 10, 4);
    lText->setText("Нативный алгоритм");
    this->pressed = false;
}

void MainWindow::showDDA()
{
    nativeEl->hide();
    brfc->hide();
    brfs->hide();
    dda->hide();
    QPointF p0;
    QPointF p1;

    QElapsedTimer timer;
    timer.start();

    while (true)
    {
        if (this->pressed)
        {
            p1 = this->getP1();
            p0 = this->getP0();
        }
        else
        {
            lText->setText("Какой алгоритм?");
            return;
        }
        if (qAbs(p0.x()) >= this->height() / 20 || qAbs(p0.y()) >= this->width() / 20 || qAbs(p1.x()) >= this->height() / 20 || qAbs(p1.y()) >= this->width() / 20)
        {
            outOfRangeError();
        }
        else
        {
            break;
        }
    }

    qint64 elapsed = timer.elapsed();
    statusBar()->showMessage(QString("DDA выполнено в% 1ms").arg(elapsed));

    dda = new DDA(p0.x(), p0.y(), p1.x(), p1.y());
    l->addWidget(dda, 1, 0, 10, 4);
    lText->setText("DDA");
    this->pressed = false;
}

void MainWindow::outOfRangeError()
{
    QMessageBox::information(NULL, QObject::tr("Warning"), tr("Segment or circle will be out of coordinate plane"));

    switch (this->choosed)
    {
    case 0:
        leX0->setText("0");
        leY0->setText("0");
        leRadius->setText("0");
        break;
    case 1:
        leX0->setText("0");
        leY0->setText("0");
        leX1->setText("0");
        leY1->setText("0");
        break;
    case 2:
        leX0->setText("0");
        leY0->setText("0");
        leX1->setText("0");
        leY1->setText("0");
        break;
    case 3:
        leX0->setText("0");
        leY0->setText("0");
        leX1->setText("0");
        leY1->setText("0");
        break;
    }
}

void MainWindow::onBrCycleRadioButtonToggled(bool checked)
{
    if (checked)
    {
        this->choosed = 0;
        pbBrCircle->setEnabled(true);
        pbBrSegment->setEnabled(false);
        pbNaiveAlgorithm->setEnabled(false);
        pbDDA->setEnabled(false);
        l->addWidget(leX0, 13, 0, 1, 1);
        l->addWidget(leY0, 13, 1, 1, 1);
        l->addWidget(leRadius, 13, 2, 1, 1);
        l->addWidget(OKBut, 13, 3, 1, 1);
        setText();
        ui->centralwidget->setLayout(l);
        leX1->hide();
        leY1->hide();
    }
}

void MainWindow::onBrSegmentRadioButtonToggled(bool checked)
{
    if (checked)
    {
        this->choosed = 1;
        pbBrSegment->setEnabled(true);
        pbBrCircle->setEnabled(false);
        pbNaiveAlgorithm->setEnabled(false);
        pbDDA->setEnabled(false);
        l->addWidget(leX0, 13, 0, 1, 1);
        l->addWidget(leY0, 13, 1, 1, 1);
        l->addWidget(leX1, 13, 2, 1, 1);
        l->addWidget(leY1, 13, 3, 1, 1);
        l->addWidget(OKBut, 13, 4, 1, 1);
        setText();
        ui->centralwidget->setLayout(l);
        leX1->show();
        leY1->show();
    }
}

void MainWindow::onNativeRadioButtonToggled(bool checked)
{
    if (checked)
    {
        this->choosed = 2;
        pbNaiveAlgorithm->setEnabled(true);
        pbBrCircle->setEnabled(false);
        pbBrSegment->setEnabled(false);
        pbDDA->setEnabled(false);
        l->addWidget(leX0, 13, 0, 1, 1);
        l->addWidget(leY0, 13, 1, 1, 1);
        l->addWidget(leX1, 13, 2, 1, 1);
        l->addWidget(leY1, 13, 3, 1, 1);
        l->addWidget(OKBut, 13, 4, 1, 1);
        setText();
        ui->centralwidget->setLayout(l);
        leX1->show();
        leY1->show();
    }
}

void MainWindow::onDDARadioButtonToggled(bool checked)
{
    if (checked)
    {
        this->choosed = 3;
        pbDDA->setEnabled(true);
        pbBrCircle->setEnabled(false);
        pbNaiveAlgorithm->setEnabled(false);
        pbBrSegment->setEnabled(false);
        l->addWidget(leX0, 13, 0, 1, 1);
        l->addWidget(leY0, 13, 1, 1, 1);
        l->addWidget(leX1, 13, 2, 1, 1);
        l->addWidget(leY1, 13, 3, 1, 1);
        l->addWidget(OKBut, 13, 4, 1, 1);
        setText();
        ui->centralwidget->setLayout(l);
        leX1->show();
        leY1->show();
    }
}

void MainWindow::setText()
{
    leX0->setText("X0");
    leX1->setText("X1");
    leY0->setText("Y0");
    leY1->setText("Y1");
    leRadius->setText("R");
    OKBut->setText("ОК");
}

void MainWindow::onOKButClicked()
{
    this->pressed = true;
    switch (this->choosed)
    {
    case 0:
        showBrCircle();
        break;
    case 1:
        showBrSegment();
        break;
    case 2:
        showNaive();
        break;
    case 3:
        showDDA();
        break;
    }
}


void MainWindow::SetStyle()
{
    pbDDA->setStyleSheet("border: 2px solid #4CAF50;"
                        "background-color: white;"
                        "color: black;"
                        "padding: 8px 16px;"
                        "text-align: center;"
                        "text-decoration: none;"
                        "display: inline-block;"
                        "font-size: 10px;"
                        "margin: 4px 2px;"
                        "cursor: pointer;"
                        "border-radius: 8px;");
    pbNaiveAlgorithm->setStyleSheet("border: 2px solid #4CAF50;"
                                    "background-color: white;"
                                    "color: black;"
                                    "padding: 8px 16px;"
                                    "text-align: center;"
                                    "text-decoration: none;"
                                    "display: inline-block;"
                                    "font-size: 10px;"
                                    "margin: 4px 2px;"
                                    "cursor: pointer;"
                                    "border-radius: 8px;");
    pbBrSegment->setStyleSheet("border: 2px solid #4CAF50;"
                               "background-color: white;"
                               "color: black;"
                               "padding: 8px 16px;"
                               "text-align: center;"
                               "text-decoration: none;"
                               "display: inline-block;"
                               "font-size: 10px;"
                               "margin: 4px 2px;"
                               "cursor: pointer;"
                               "border-radius: 8px;");
    pbBrCircle->setStyleSheet("border: 2px solid #4CAF50;"
                              "background-color: white;"
                              "color: black;"
                              "padding: 8px 16px;"
                              "text-align: center;"
                              "text-decoration: none;"
                              "display: inline-block;"
                              "font-size: 10px;"
                              "margin: 4px 2px;"
                              "cursor: pointer;"
                              "border-radius: 8px;");
    OKBut->setStyleSheet("border: 2px solid #4CAF50;"
                         "background-color: white;"
                         "color: black;"
                         "padding: 8px 16px;"
                         "text-align: center;"
                         "text-decoration: none;"
                         "display: inline-block;"
                         "font-size: 10px;"
                         "margin: 4px 2px;"
                         "cursor: pointer;"
                         "border-radius: 8px;");
    lText->setStyleSheet("border: 2px solid #4CAF50;"
                         "background-color: white;"
                         "color: black;"
                         "padding: 8px 16px;"
                         "text-align: center;"
                         "text-decoration: none;"
                         "display: inline-block;"
                         "font-size: 10px;"
                         "margin: 4px 2px;"
                         "cursor: pointer;"
                         "border-radius: 8px;");
    leX0->setStyleSheet("QLineEdit {"
                       "border: 2px solid #4CAF50;"
                       "background-color: #ccffcc;"
                       "color: black;"
                       "padding: 4px;"
                       "}");
    leY0->setStyleSheet("QLineEdit {"
                       "border: 2px solid #4CAF50;"
                       "background-color: #ccffcc;"
                       "color: black;"
                       "padding: 4px;"
                       "}");
    leX1->setStyleSheet("QLineEdit {"
                       "border: 2px solid #4CAF50;"
                       "background-color: #ccffcc;"
                       "color: black;"
                       "padding: 4px;"
                       "}");
    leY1->setStyleSheet("QLineEdit {"
                       "border: 2px solid #4CAF50;"
                       "background-color: #ccffcc;"
                       "color: black;"
                       "padding: 4px;"
                       "}");
    leRadius->setStyleSheet("QLineEdit {"
                            "border: 2px solid #4CAF50;"
                            "background-color: #ccffcc;"
                            "color: black;"
                            "padding: 4px;"
                            "}");

}
