#include "ScreenLocker.h"

ScreenLocker::ScreenLocker(QWidget *parent) : QDialog(parent)
{
    // load icon and set the icon for the application
    QIcon icon(":/images/gift.png");
    this->setWindowIcon(icon);
    this->showFullScreen();

    // load the image to display
    image.load(":/images/screenlocker.jpg");

    // remove window borders and set always on top
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // move the window to the right-bottom position
    displaySize = QApplication::desktop()->screenGeometry(this).size();
    rect.adjust(0, 0, displaySize.width(), displaySize.height());

    // init label
    this->layout = new QHBoxLayout();
    this->centralText = new QLabel();
    this->centralText->setAlignment(Qt::AlignCenter);
    this->layout->addWidget(centralText);

    centralText->setStyleSheet("QLabel {color: green; background-color: rgba(255,255,255,40%)}");
    centralText->setFont(QFont("Sans serif", 28));
    this->setLayout(layout);

    QObject::connect(&timer,SIGNAL(timeout()), this, SLOT(updateTime()));

    // set interval as 1s
    timer.setInterval(1000);
    timer.start();
}

ScreenLocker::~ScreenLocker()
{
    // the parent will take care of the children
    this->layout->deleteLater();
}

void ScreenLocker::paintEvent(QPaintEvent *)
{
    // repaint the ui
    QPainter painter(this);
    painter.drawImage(rect, image);
}

// shortcuts
void ScreenLocker::keyReleaseEvent(QKeyEvent * event)
{
    switch (event->key())
    {
        case Qt::Key_Escape:             // exit()
            this->hide();
            break;
        default:
            break;
    }
}


void ScreenLocker::updateTime()
{
    countdown--;

    QString prompt = QString("Please relax for %1 seconds before the screen is unlocked!").arg(QString::number(countdown, 10));
    centralText->setText(prompt);
    if (countdown <= 0)
    {
        timer.stop();
        this->close();
    }
}

void ScreenLocker::setCountdown(long t)
{
    this->countdown = t;
    QString prompt = QString("Please relax for %1 seconds before the screen is unlocked!").arg(QString::number(countdown, 10));
    centralText->setText(prompt);
}
