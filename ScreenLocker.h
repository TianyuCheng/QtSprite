#ifndef SCREENLOCKER_H
#define SCREENLOCKER_H

#include <QApplication>
#include <QtGui>
#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QImage>
#include <QTimer>
#include <QRect>
#include <QFont>
#include <QColor>

class ScreenLocker : public QDialog
{
    Q_OBJECT
    public:
        ScreenLocker(QWidget *parent);
        virtual ~ScreenLocker();

        // paint function
        void paintEvent(QPaintEvent *event);
        void setCountdown(long t);

        void start();

    public slots:
        void updateTime();

    protected:
        void keyReleaseEvent(QKeyEvent *event);

    private:
        QImage image;
        QRect rect;
        long countdown;
        QTimer timer;
        QTimer idleTimer;
        QSize displaySize;

        QLabel *centralText;
        QHBoxLayout *layout;
};

#endif // SCREENLOCKER_H
