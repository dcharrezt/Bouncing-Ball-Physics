#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->runButton, SIGNAL(clicked()),this, SLOT(processData()) );
    QObject::connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetScene()));

    drawScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawScene()
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0.0,0.0,325,275);
    ui->graphicsView->setScene(scene);

    std::cout << "WIDTH S " << scene->width() << "HEIGHT " << scene->height() << std::endl;
    std::cout << "WIDTH " << ui->graphicsView->width() << "HEIGHT " << ui->graphicsView->height() << std::endl;

    QRectF rectBall_1(0.0,0.0,50.0,50.0);
    QRectF rectBaLL_2(0.0,0.0,50.0,50.0);

    QBrush greenBrush(Qt::darkGreen);
    QBrush blueBrush(Qt::darkBlue);

    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    floor = scene->addRect(0, 0, 320, 30, outlinePen, greenBrush);
    floor->setFlag(QGraphicsItem::ItemIsMovable);

    // addEllipse(x,y,w,h,pen,brush)
    ball_1 = scene->addEllipse(rectBall_1, outlinePen, blueBrush);
    ball_1->setFlag(QGraphicsItem::ItemIsMovable);

    ball_2 = scene->addEllipse(rectBaLL_2, outlinePen, blueBrush);
    ball_2->setFlag(QGraphicsItem::ItemIsMovable);

    //ui->graphicsView->setFixedSize(scene->width(),scene->height());
    std::cout << "WIDTH S " << scene->width() << "HEIGHT " << scene->height() << std::endl;
    std::cout << "WIDTH " << ui->graphicsView->width() << "HEIGHT " << ui->graphicsView->height() << std::endl;

    ball_1->setPos(75.0,50.0);
    ball_2->setPos(200.0,50.0);
    floor->setPos(0,240);
    std::cout << ball_1->x() << std::endl;
    std::cout << floor->x() << " " << floor->y() << std::endl;
}

void MainWindow::doTheFuckingMath()
{
    std::cout <<  ui->massBox->text().toInt() << std::endl;
}

void MainWindow::processData()
{
    ball_1_gravity = ui->gravityBox->text().toFloat();
    ball_1_mass = ui->massBox->text().toFloat();
    ball_1_height = ui->heightBox->text().toFloat();

    {
      timer = new QTimer(this);
      cout << "aagads" << endl;
      QObject::connect(timer,SIGNAL(timeout()),this,SLOT(advance()));
      timer->setInterval(15);
      timer->start();
    }

}

void MainWindow::advance()
{

    if(ball_1_velocityY)
        //The ball doesn't bounce anymore!

    // The ball hit the floor
    if (ball_1->y() + ball_1_velocityY + (ball_1->boundingRect().height() * 0.5) >= floor->y() - (floor->boundingRect().height() * 0.5) - 5)
    {
        ball_1_velocityYBeforeImpact = sqrt( 2*ball_1_gravity * ball_1_height);
        std::cout << ball_1_velocityYBeforeImpact << std::endl;
        ball_1_COR = 0.5;
        ball_1_velocityYAfterImpact = ball_1_COR * ball_1_velocityYBeforeImpact;
        std::cout << "WHATS GOING ON ?" << std::endl;
        ball_1_velocityY = -std::abs(ball_1_velocityY);
    }
    ball_1->setPos(ball_1->x() + ball_1_velocityX, ball_1->y() + ball_1_velocityY);

}

void MainWindow::resetScene()
{
    scene->clear();
    QObject::disconnect(ui->runButton, SIGNAL(clicked()),this, SLOT(processData()));
    QObject::disconnect(timer,SIGNAL(timeout()),this,SLOT(advance()));
    drawScene();
}

void MainWindow::freeFalling()
{

}

void MainWindow::bouncingUp()
{

}
