// mainwindow.cpp

#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // common part of window
    setWindowTitle("MinimalClicker");
    m_wideWidget = new QWidget;
    setCentralWidget(m_wideWidget);

    m_hLayoutWide = new QHBoxLayout(m_wideWidget);
    m_v1Layout = new QVBoxLayout;

    m_hLayoutWide->addLayout(m_v1Layout, 2);
    m_v2Layout = new QVBoxLayout;

    m_v2Layout->setAlignment(Qt::AlignCenter);
    m_hLayoutWide->addLayout(m_v2Layout, 3);

    m_v3Layout = new QVBoxLayout;
    m_hLayoutWide->addLayout(m_v3Layout, 1);

    // build 1-st column of window (btns)
    m_btnStart = new QPushButton(tr("Старт"));
    m_btnStop = new QPushButton(tr("Стоп"));

    m_countTimerElapsed = new QLabel("Время раунда: ");
    m_countTimerElapsed->setStyleSheet(QString("font-size: %1px").arg(20));
    m_countTimerFull = new QLabel("Время игры: ");
    m_countTimerFull->setStyleSheet(QString("font-size: %1px").arg(20));

    m_timerFull = new QTimer();
    m_timerElapsed = new QTimer();

    m_v1Layout->addWidget(m_btnStart, 1);
    m_v1Layout->addWidget(m_btnStop, 1);
    //    m_v1Layout->addWidget(m_countTimerElapsed, 1);
    m_v1Layout->addWidget(m_countTimerFull, 1);

    connect(m_btnStart, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(m_btnStop, &QPushButton::clicked, this, &MainWindow::stopGame);
    connect(m_timerFull, &QTimer::timeout, this, &MainWindow::updateTimer);

    // build 2-nd column of window (quest number)
    m_showNumber = new QLabel;
    m_pbar = new QProgressBar();
    m_pbar->setTextVisible(false);
    m_pbar->setRange(0, TIME_ELAPSED);

    m_v2Layout->addWidget(m_showNumber, 1);
    m_v2Layout->addWidget(m_pbar, 1);
    m_showNumber->setAlignment(Qt::AlignCenter);
    m_showNumber->setStyleSheet(QString("font-size: %1px").arg(120));

    // build 3-d column of window (counters)
    m_groupWins = new QGroupBox(tr("Победы"));
    m_winsLayout = new QVBoxLayout;
    m_winsLayout->setAlignment(Qt::AlignCenter);
    m_groupWins->setLayout(m_winsLayout);

    m_groupFails = new QGroupBox(tr("Поражения"));
    m_failsLayout = new QVBoxLayout;
    m_failsLayout->setAlignment(Qt::AlignCenter);
    m_groupFails->setLayout(m_failsLayout);

    m_v3Layout->addWidget(m_groupWins, 1);
    m_v3Layout->addWidget(m_groupFails, 1);

    m_winResult = new QLabel;
    m_winResult->setAlignment(Qt::AlignCenter);
    m_winResult->setStyleSheet(QString("font-size: %1px").arg(60));
    m_winsLayout->addWidget(m_winResult);

    m_failResult = new QLabel();
    m_failResult->setAlignment(Qt::AlignCenter);
    m_failsLayout->addWidget(m_failResult);
    m_failResult->setStyleSheet(QString("font-size: %1px").arg(60));

    show();
}

MainWindow::~MainWindow() {
    disconnect(m_btnStart, &QPushButton::clicked, this, &MainWindow::startGame);
    disconnect(m_btnStop, &QPushButton::clicked, this, &MainWindow::stopGame);
    disconnect(m_timerFull, &QTimer::timeout, this, &MainWindow::updateTimer);

    delete m_failResult;
    delete m_winResult;
    delete m_failsLayout;
    delete m_groupFails;
    delete m_winsLayout;
    delete m_groupWins;
    delete m_showNumber;
    delete m_timerElapsed;
    delete m_timerFull;
    delete m_countTimerFull;
    delete m_countTimerElapsed;
    delete m_pbar;
    delete m_btnStart;
    delete m_btnStop;
    delete m_v3Layout;
    delete m_v2Layout;
    delete m_v1Layout;
    delete m_hLayoutWide;
    delete m_wideWidget;

    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        ++m_countPressBtn;

        if (this->statusBar()->currentMessage() != "") {
            this->statusBar()->clearMessage();
        }
    } else {
        this->statusBar()->showMessage("Нужно нажимать ЛКМ!");
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    stopGame();
    this->hide();
    QMessageBox msgBox;
    msgBox.setModal(true);
    msgBox.setWindowTitle(tr("Пока!"));
    msgBox.setText(tr("Приходите еще."));

    QAbstractButton* mbBtn =
        msgBox.addButton(tr("Обязательно приду!"), QMessageBox::YesRole);
    msgBox.exec();

    if (msgBox.clickedButton() == mbBtn) {
        qApp->quit();
    }
}

void MainWindow::startGame() {
    m_timerFull->start(10);
    gameTime = gameTime.currentTime();
    roundTime = roundTime.currentTime();
    changeNumber();
    m_countPressBtn = 0;
    m_showNumber->setText(QString::number(m_number));
    m_countWins = 0;
    m_winResult->setText(QString::number(m_countWins));
    m_countFails = 0;
    m_failResult->setText(QString::number(m_countFails));
}

void MainWindow::stopGame() {
    m_timerFull->stop();
    m_pbar->reset();
}

void MainWindow::updateTimer() {
    QTime currTime = QTime::currentTime();
    int gt = this->gameTime.msecsTo(currTime);
    int rt = this->roundTime.msecsTo(currTime);

    if (rt > TIME_ELAPSED) {
        roundTime = roundTime.currentTime();
        checkResult();
        changeNumber();
        rt = 0;
    }

    updateShowTime(rt, gt);
}

void MainWindow::updateShowTime(int& rt, int& gt) {
    QTime showRoundTime{0, 0, 0, 0};
    showRoundTime = showRoundTime.addMSecs(rt);

    QTime showGameTime{0, 0, 0, 0};
    showGameTime = showGameTime.addMSecs(gt);

    //    m_countTimerElapsed->setText("Время раунда: " +
    //    showRoundTime.toString("s:zzz"));
    m_pbar->setValue(rt);
    m_countTimerFull->setText("Время игры: " + showGameTime.toString("mm:ss"));
}

void MainWindow::changeNumber() {
    m_number = 1 + QRandomGenerator::global()->bounded(NUM_RANGE);
    m_showNumber->setText(QString::number(m_number));
}

void MainWindow::checkResult() {
    if (m_countPressBtn == m_number) {
        m_winResult->setText(QString::number(++m_countWins));
    } else {
        m_failResult->setText(QString::number(++m_countFails));
    }
    m_countPressBtn = 0;
    m_pbar->reset();
}
