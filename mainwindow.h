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

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QGraphicsEllipseItem *ball_1;
    QGraphicsEllipseItem *ball_2;
    QGraphicsRectItem *floor;
    QGraphicsTextItem *text;

    QTimer * timer;

    FF ball_1_gravity;
    FF ball_1_mass;
    FF ball_1_height;
    FF ball_1_velocityX = 0.0;
    FF ball_1_velocityY = 1.0;
    FF ball_1_COR;
    FF ball_1_KE;
    FF ball_1_PE;
    FF ball_1_velocityYBeforeImpact;
    FF ball_1_velocityYAfterImpact;
    FF ball_1_timeElapsed = 0.0; // miliseconds


    FF ball_2_gravity;
    FF ball_2_mass;
    FF ball_2_height;
    FF ball_2_velocity;

public slots:
    void doTheFuckingMath();
    void processData();
    void advance();
    void resetScene();
    void freeFalling(); // accelerating
    void bouncingUp(); // decelerating

};

#endif // MAINWINDOW_H
