// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include <QCloseEvent>

#include <QTimer>
#include <QTime>

#include <QProgressBar>
#include <QRandomGenerator>
#include <QDebug>




//#include <QDialog>
//#include <QTextStream>
//#include <QtGlobal>
//#include <QtGui>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    const int TIME_ELAPSED = 2000;  // msec
    const int NUM_RANGE = 5;        // random range 0..NUM_RANGE

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private:
    Ui::MainWindow *ui;

    QWidget *m_wideWidget;

    QHBoxLayout *m_hLayoutWide;
    QVBoxLayout *m_v1Layout;
    QVBoxLayout *m_v2Layout;
    QVBoxLayout *m_v3Layout;
    QVBoxLayout *m_winsLayout;
    QVBoxLayout *m_failsLayout;

    QGroupBox *m_groupWins;
    QGroupBox *m_groupFails;

    QPushButton *m_btnStart;
    QPushButton *m_btnStop;

    QLabel *m_showNumber;
    QLabel *m_winResult;
    QLabel *m_failResult;
    QLabel *m_countTimerElapsed;
    QLabel *m_countTimerFull;

    QTime roundTime{0, 0, 0, 0};
    QTime gameTime{0, 0, 0, 0};

    QTimer *m_timerElapsed;
    QTimer *m_timerFull;

    QProgressBar *m_pbar;

    int m_countPressBtn{0};
    int m_countWins{0};
    int m_countFails{0};

    int m_number;

    void updateShowTime(int& roundTime, int& gameTime);
    void checkResult();
    void changeNumber();

   private slots:
    void startGame();
    void stopGame();
    void updateTimer();
    void mousePressEvent(QMouseEvent*);
    void closeEvent(QCloseEvent*);

};
#endif  // MAINWINDOW_H
