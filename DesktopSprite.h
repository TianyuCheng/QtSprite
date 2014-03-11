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

#include <iostream>
#include <vector>

// define beforehand
class DesktopSprite;
class TaskReminder;

#include "QNotify.h"
#include "Configuration.h"
#include "TaskRecorder.h"
#include "AddTaskDialog.h"
#include "TasksBrowser.h"
#include "ScreenLocker.h"
#include "TaskReminder.h"

class DesktopSprite : public QWidget
{
    Q_OBJECT

    public:
        DesktopSprite(QApplication *app, Configuration *config);
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
        void initMenuItems();
        void initDisplay();
        void initSystemTray();
        void initScreenLocker();

    public slots:
        void showMouseRightButtonMenu(const QPoint&);
        void iconActivated(QSystemTrayIcon::ActivationReason);

        void addTask();
        void browseTasks();
        void lockscreen();
    private:
        QApplication *app_handle;
        Configuration *configs;

        QMenu *qMenu;
        QPixmap image; 
        QSystemTrayIcon *tray;

        // pop menu actions
        QAction *exit_all;
        QAction *add_task;
        QAction *browse_tasks;

        // for drag and drop
        QPoint prevPt;
        bool pressed;

        TaskRecorder *recorder;
        TaskReminder *reminder;

        // manage screen locker
        QTimer *idle;
        QNotify *notify;

};
 #endif /* end of include guard: DESKTOP_SPRITE */
