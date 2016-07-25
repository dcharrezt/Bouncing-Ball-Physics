#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
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
    scene->setSceneRect(0.0,0.0,320,270);
    ui->graphicsView->setScene(scene);

    QRectF rectBall_1(0.0,0.0,50.0,50.0);
    QRectF rectBaLL_2(0.0,0.0,50.0,50.0);

    QBrush greenBrush(Qt::darkGreen);
    QBrush blueBrush(Qt::darkBlue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(0);

    floor = scene->addRect(0, 0, 320, 30, outlinePen, greenBrush);
    floor->setFlag(QGraphicsItem::ItemIsMovable);

    // addEllipse(x,y,w,h,pen,brush)
    ball_1 = scene->addEllipse(rectBall_1, outlinePen, blueBrush);
    ball_1->setFlag(QGraphicsItem::ItemIsMovable);

    ball_2 = scene->addEllipse(rectBaLL_2, outlinePen, blueBrush);
    ball_2->setFlag(QGraphicsItem::ItemIsMovable);

    ball_1->setPos(75.0,50.0);
    ball_2->setPos(200.0,50.0);
    floor->setPos(0,240);
}

FF MainWindow::findVelocityAtTheMoment()
{
    ball_1_velocityY = 0 + ball_1_gravity * ball_1_partialTimeElapsed;
    return ball_1_velocityY;
}

FF MainWindow::timeGoingUp(FF vel, FF gravity)
{
    return ( 2*vel ) / gravity;
}

FF MainWindow::timeGoingDown(FF height, FF gravity)
{
    return sqrt( (2*height)/gravity );
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

    //time_to_fall = sqrt( (2*ball_1_height) / ball_1_gravity );


    {
      timer = new QTimer(this);
      QObject::connect(timer,SIGNAL(timeout()),this,SLOT(freeFalling()));
      timer->setInterval(10);
      timer->start();
    }

    time_to_fall = timeGoingDown(ball_1_height, ball_1_gravity);
    QTimer::singleShot(time_to_fall*1000,this,SLOT(disconnectFall()));
}

void MainWindow::advance()
{
    heightBetweenBallFloor = floor->y() - ball_1->y() - ball_1->boundingRect().height();
    if(ball_1_velocityY)
        //The ball doesn't bounce anymore!

    // The ball hit the floor
    if (ball_1->y() + ball_1_velocityY + (ball_1->boundingRect().height()) > floor->y())
    {
        ball_1_velocityYBeforeImpact = sqrt( 2*ball_1_gravity * ball_1_height);
        std::cout << ball_1_velocityYBeforeImpact << std::endl;
        ball_1_COR = 0.5;
        ball_1_velocityYAfterImpact = ball_1_COR * ball_1_velocityYBeforeImpact;
        std::cout << "WHATS GOING ON ?" << std::endl;
        ball_1_velocityY = -std::abs(ball_1_velocityY);
    }
    ball_1->setPos(ball_1->x() + ball_1_velocityX, ball_1->y() + ball_1_velocityY);

    std::cout << heightBetweenBallFloor << std::endl;
}

void MainWindow::resetScene()
{
    scene->clear();
    QObject::disconnect(timer,SIGNAL(timeout()),this,SLOT(freeFalling()));
    ball_1_velocityX = 0.0;
    ball_1_velocityY = 0.0;
    ball_1_partialTimeElapsed = 0.0;
    drawScene();
    heightBetweenBallFloor = floor->y() - ball_1->y() - ball_1->boundingRect().height();
}

void MainWindow::freeFalling()
{
    heightBetweenBallFloor = floor->y() - ( ball_1->y() + ball_1->boundingRect().height() );
    cout << "VEL " << ball_1_velocityY << endl;
    ball_1_partialTimeElapsed += 0.001; // miliseconds
    cout << "PARTIAL: " << ball_1_partialTimeElapsed << endl;
    cout << "PX PER MILISECOND " << 0.140/time_to_fall << endl;

    if(ball_1_velocityY >= 0)
        ball_1_velocityY = 1.40/time_to_fall;

    if (ball_1->y() + ball_1_velocityY + (ball_1->boundingRect().height()) > floor->y())
    {
        ball_1_velocityYBeforeImpact = sqrt( 2*ball_1_gravity * ball_1_height);
        ball_1_COR = 0.85;
        ball_1_velocityYAfterImpact = ball_1_COR * ball_1_velocityY;
        ball_1_velocityY = -std::abs(ball_1_velocityYAfterImpact);
        ball_1_gravity = -abs(ball_1_gravity);
        ball_1_partialTimeElapsed = 0;
    }

    if ( ball_1_velocityY < 0 ) {
        ball_1_velocityY = ball_1_velocityY - ball_1_gravity * ball_1_partialTimeElapsed;
        ball_1->setPos(ball_1->x() + ball_1_velocityX, ball_1->y() + ball_1_velocityY);
    } else
        ball_1->setPos(ball_1->x() + ball_1_velocityX, ball_1->y() + ball_1_velocityY);
}

void MainWindow::bouncingUp()
{
    cout << "TESTING "<< time_to_fall <<" SECONDS AFTER " << endl;
}

void MainWindow::disconnectFall()
{
    cout << "TESTING "<< time_to_fall <<" SECONDS AFTER " << endl;
    QObject::disconnect(timer,SIGNAL(timeout()),this,SLOT(freeFalling()));
}
