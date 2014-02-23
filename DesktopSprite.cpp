#include "DesktopSprite.h"

DesktopSprite::DesktopSprite(QApplication *app) : QWidget(0), app_handle(app), pressed(false)
{
    // load icon and set the icon for the application
    QIcon icon(":/images/gift.png");
    this->setWindowIcon(icon);

    // init actions
    exit_all = new QAction(this);
    add_task = new QAction(this);
    browse_tasks = new QAction(this);

    // set texts
    exit_all->setText(QWidget::tr("Exit"));
    add_task->setText(QWidget::tr("Add a Task"));
    browse_tasks->setText(QWidget::tr("Browser Tasks"));

    // connect the action with functions
    QObject::connect(exit_all, SIGNAL(triggered()), app_handle, SLOT(quit()));
    QObject::connect(add_task, SIGNAL(triggered()), this, SLOT(addTask()));
    QObject::connect(browse_tasks, SIGNAL(triggered()), this, SLOT(browseTasks()));

    // initialize the settings and load the image
    this->setContextMenuPolicy(Qt::CustomContextMenu);      // enable the right click menu
    this->setFocusPolicy(Qt::ClickFocus);
    
    // set the right click menu
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButtonMenu(const QPoint&)));

    // load the image to display
    image.load(":/images/yui.png");

    // remove window borders and set always on top
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);

    // move the window to the right-bottom position
    QSize displaySize = QApplication::desktop()->screenGeometry(this).size();
    
    this->setFixedSize(image.size());
    this->setMask(image.mask());

    QSize size = displaySize - this->size();
    this->move(QPoint(size.width(), size.height()));

    // init system tray
    initMenu();
    tray = new QSystemTrayIcon(this);
    tray->setContextMenu(qMenu);
    tray->setIcon(icon);
    tray->setToolTip(QString("QtSprite"));

    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    tray->show();

    // init the timer
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(alarm()));

    recorder = new TaskRecorder;

    this->initTaskSchedule();

    this->notify = new QNotify(this);
    this->alertSound = new QSound(":/sound/alarmecho.wav");
}

DesktopSprite::~DesktopSprite()
{
    // delete the pointers
    timer->deleteLater();
    tray->deleteLater();

    // delete actions 
    exit_all->deleteLater();
    add_task->deleteLater();
    browse_tasks->deleteLater();

    qMenu->deleteLater();

    delete notify;
    delete recorder;
    delete alertSound;

    notify = 0;
    recorder = 0;
    alertSound = 0;
}

void DesktopSprite::paintEvent(QPaintEvent *)
{
    // repaint the ui
    QPainter painter(this);
    painter.drawPixmap(0, 0, image);
}

/***************************************************************************
 *                       public slots implementation                       *
 ***************************************************************************/
void DesktopSprite::showMouseRightButtonMenu(const QPoint &)
{
    // show right button menu when requested
    qMenu->exec(QCursor::pos());
}

void DesktopSprite::iconActivated(QSystemTrayIcon::ActivationReason reason) 
{
    switch (reason)
    {
        case QSystemTrayIcon::Context:
            tray->contextMenu()->popup(QCursor::pos());
            break;
        case QSystemTrayIcon::Trigger:
            break;
        case QSystemTrayIcon::DoubleClick:
            break;
        default:
            break;
    }
}

void DesktopSprite::addTask()
{
    AddTaskDialog dialog(recorder, this);
    dialog.exec();
}

void DesktopSprite::browseTasks()
{
    TasksBrowser dialog(recorder, this);
    dialog.exec();
}

void DesktopSprite::alarm() {
    recorder->removeTask(currentTask->id);
    this->tasksForToday.erase(this->tasksForToday.begin());

    qDebug() << "alarm:" << currentTask->description;
    this->alertSound->play();
    this->notify->notify(currentTask->description, QNotify::SUCCESS, 3000);
    handleTask();
}

/***************************************************************************
 *                           auxillary functions                           *
 ***************************************************************************/

void DesktopSprite::initMenu()
{
    qMenu = new QMenu();

    qMenu->addAction(add_task);
    qMenu->addAction(browse_tasks);
    qMenu->addAction(exit_all);
}

void DesktopSprite::initTaskSchedule()
{
    recorder->updateDB();

    QDateTime datetime = QDateTime::currentDateTime();
    this->tasksForToday = recorder->getTasksByDate(datetime);     // already sorted

    while (this->tasksForToday.size() > 0)
    {
        currentTask = tasksForToday.front();
        long time = currentTask->datetime.toMSecsSinceEpoch() - datetime.toMSecsSinceEpoch();

        if (time < 0) {
            recorder->removeTask(currentTask->id);
            this->tasksForToday.erase(this->tasksForToday.begin());
        }
        else
            break;
        qDebug() << time;
    }

    this->handleTask();
}

void DesktopSprite::handleTask()
{
    timer->stop();

    if (this->tasksForToday.size() > 0)
    {
        QDateTime datetime = QDateTime::currentDateTime();

        currentTask = tasksForToday.front();
        long time = currentTask->datetime.toMSecsSinceEpoch() - datetime.toMSecsSinceEpoch();
        qDebug() << "Time to next alarm: " << time;

        timer->start(time);
    }
}

// sprite dragging functions
void DesktopSprite::mouseMoveEvent(QMouseEvent * event)
{
    if (pressed)
    {
        const QPoint pos = event->globalPos();
        this->move(this->pos() + pos - prevPt);
        prevPt = pos;
    }
}

void DesktopSprite::mousePressEvent(QMouseEvent * event)
{
    pressed = true;
    prevPt = event->globalPos();
}

void DesktopSprite::mouseReleaseEvent(QMouseEvent *)
{
    pressed = false;
}

// shortcuts
void DesktopSprite::keyReleaseEvent(QKeyEvent * event) 
{
    switch (event->key())
    {
        case Qt::Key_Escape:             // exit()
            app_handle->quit();
            break;
        default:
            break;
    }
}
