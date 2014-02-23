#ifndef DESKTOP_SPRITE
#define DESKTOP_SPRITE

#include <QApplication>
#include <QtGui>
#include <QMessageBox>
#include <QWidget>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QMenu>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QTimer>
#include <QDateTime>
#include <QSound>

#include <iostream>
#include <vector>

#include "QNotify.h"

#include "TaskRecorder.h"
#include "AddTaskDialog.h"
#include "TasksBrowser.h"

class DesktopSprite : public QWidget
{
    Q_OBJECT

    public:
        DesktopSprite(QApplication *app);
        virtual ~DesktopSprite();

        // paint function
        void paintEvent(QPaintEvent *event);

    protected:
        void mouseMoveEvent(QMouseEvent * event);
        void mousePressEvent(QMouseEvent * event);
        void mouseReleaseEvent(QMouseEvent * event);
        void keyReleaseEvent(QKeyEvent * event);

    private:
        void initMenu();
        void initTaskSchedule();
        void handleTask();

    public slots:
        void showMouseRightButtonMenu(const QPoint&);
        void iconActivated(QSystemTrayIcon::ActivationReason);

        void addTask();
        void browseTasks();
        void alarm();
    private:
        QMenu *qMenu;
        QPixmap image; 
        QApplication *app_handle;

        // menu actions
        QAction *exit_all;
        QAction *add_task;
        QAction *browse_tasks;

        QPoint prevPt;
        bool pressed;
        std::vector<Task*> tasksForToday;
        Task * currentTask ;

        QTimer *timer;
        QSystemTrayIcon *tray;

        QNotify *notify;
        TaskRecorder *recorder;
        QSound * alertSound;
};

 #endif /* end of include guard: DESKTOP_SPRITE */
