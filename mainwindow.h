#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>


using namespace std;

typedef float FF;
typedef int II;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void drawScene();
    FF findVelocityAtTheMoment();
    FF timeGoingUp(FF vel, FF gravity);
    FF timeGoingDown(FF height, FF gravity);

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QGraphicsEllipseItem *ball_1;
    QGraphicsEllipseItem *ball_2;
    QGraphicsRectItem *floor;
    QGraphicsTextItem *text;

    QTimer * timer;

    FF heightBetweenBallFloor;

    FF time_to_fall;

    FF ball_1_gravity;
    FF ball_1_mass;
    FF ball_1_height;
    FF ball_1_velocityX = 0.0;
    FF ball_1_velocityY = 0.0;
    FF ball_1_COR;
    FF ball_1_KE;
    FF ball_1_PE;
    FF ball_1_velocityYBeforeImpact;
    FF ball_1_velocityYAfterImpact;
    FF ball_1_partialTimeElapsed = 0.0; // miliseconds
    FF ball_1_totalTimeElapsed;

    FF ball_2_gravity;
    FF ball_2_mass;
    FF ball_2_height;
    FF ball_2_velocity;

    FF COR_golf_ball = 0.858;
    FF COR_tennis_ball = 0.712;
    FF COR_billiard_ball = 0.804;
    FF COR_wooden_ball = 0.603;
    FF COR_steel_ball = 0.597;
    FF COR_rubber_ball = 0.828;
    FF COR_plastic_ball = 0.688;

public slots:
    void doTheFuckingMath();
    void processData();
    void advance();
    void resetScene();
    void freeFalling(); // accelerating
    void bouncingUp(); // decelerating

    void disconnectFall();

};

#endif // MAINWINDOW_H
