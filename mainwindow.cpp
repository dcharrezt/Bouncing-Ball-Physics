#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    timer = new QTimer(this);
    timer->setInterval(10);

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
    // retrieve data from buttons
    ball_1_gravity = ui->gravityBox->text().toFloat();
    ball_1_mass = ui->massBox->text().toFloat();
    ball_1_height = ui->heightBox->text().toFloat();

    ball_1_current_height = ball_1_height;

    isGoingToFall = true;
    isMoving = false;

    // create a timer to check every ms, the state of the ball
    statusTimer = new QTimer(this);
    QObject::connect(statusTimer,SIGNAL(timeout()),this,SLOT(checkBallStatus()));
    statusTimer->setInterval(1);
    statusTimer->start();


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
    ball_1_partialTimeElapsed += 0.01; // miliseconds

    ball_1_velocityY = (ball_1_current_height * 140 / ball_1_height) * 0.01 /time_to_fall;
    ball_1->setPos(ball_1->x() + ball_1_velocityX, ball_1->y() + ball_1_velocityY);
}

void MainWindow::bouncingUp()
{
    ball_1->setPos(ball_1->x() + ball_1_velocityX, ball_1->y() - ball_1_velocityY);
}

void MainWindow::checkBallStatus()
{
    if( isGoingToFall && !isMoving)
    {
        cout << "FALLING " << endl;
        ball_1_partialTimeElapsed = 0.0;
        isMoving = true;

        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(freeFalling()));

        timer->start();
        time_to_fall = timeGoingDown(ball_1_current_height, ball_1_gravity);
        QTimer::singleShot(time_to_fall*1000,this,SLOT(disconnectFall()));
    }

    if( isGoingToBounce && !isMoving )
    {
        cout << "NOT FALLING " << endl;
        ball_1_partialTimeElapsed = 0.0;
        isMoving = true;
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(bouncingUp()));
        timer->start();

        ball_1_velocityYBeforeImpact = 0 + ball_1_gravity * time_to_fall;
        cout << "vel before: " << ball_1_velocityYBeforeImpact << endl;
        ball_1_COR = 0.85;
        ball_1_velocityYAfterImpact = ball_1_COR * ball_1_velocityYBeforeImpact;

        time_to_go_up = timeGoingUp(ball_1_velocityYAfterImpact, ball_1_gravity)/2;

        if(time_to_go_up < 0.01){
            QObject::disconnect(statusTimer,SIGNAL(timeout()),this,SLOT(checkBallStatus()));
        }

        //time_to_go_up = timeGoingUp(ball_1_velocityYAfterImpact, ball_1_gravity);
        cout << "vel after: " << ball_1_velocityYAfterImpact << endl;

        height_till_top = ball_1_velocityYAfterImpact * ball_1_velocityYAfterImpact / (2*ball_1_gravity);
        ball_1_current_height = height_till_top;
        pixels_till_top = 140*height_till_top / ball_1_height;

        cout << "Pixels till top " << pixels_till_top << endl;

        ball_1_velocityY = pixels_till_top * 0.01 / time_to_go_up;

        QTimer::singleShot(time_to_go_up*1000,this,SLOT(disconnectBounce()));
    }

}

void MainWindow::disconnectFall()
{
    cout << "Fall "<< time_to_fall <<" SECONDS AFTER " << endl;
    QObject::disconnect(timer,SIGNAL(timeout()),this,SLOT(freeFalling()));
    timer->stop();

    isGoingToFall = false;
    isGoingToBounce = true;
    isMoving = false;
}

void MainWindow::disconnectBounce()
{
    cout << "Bounce "<< time_to_go_up <<" SECONDS AFTER " << endl;
    QObject::disconnect(timer,SIGNAL(timeout()),this,SLOT(bouncingUp()));
    timer->stop();

    isGoingToFall = true;
    isGoingToBounce = false;
    isMoving = false;
}
