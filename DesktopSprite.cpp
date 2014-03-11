#include "DesktopSprite.h"

DesktopSprite::DesktopSprite(QApplication *app, Configuration *conf) : QWidget(0),app_handle(app), configs(conf), pressed(false)
{
    qDebug() << "greetings:" << configs->greeting();
    qDebug() << "muted:" << configs->isMuted();
    qDebug() << "database:" << configs->getDatabaseName();
    qDebug() << "interval:" << configs->getInterval();

    // load image and set up the window
    this->initDisplay();

    // init menu items and set popup menu policy
    this->initMenuItems();
    this->initMenu();

    // system tray
    this->initSystemTray();

    // screen locker timer init
    this->initScreenLocker();

    recorder = new TaskRecorder();
    reminder = new TaskReminder(configs, recorder, this);

    // display the user define greetings
    notify = new QNotify(this);
    notify->notify(configs->greeting(), QNotify::SUCCESS);
}

DesktopSprite::~DesktopSprite()
{
    // delete the pointers
    idle->deleteLater();
    tray->deleteLater();
    notify->deleteLater();

    // delete actions 
    exit_all->deleteLater();
    add_task->deleteLater();
    browse_tasks->deleteLater();

    qMenu->deleteLater();

    delete recorder;
    delete reminder;

    recorder = 0;
    reminder = 0;
    notify = 0;
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
    AddTaskDialog dialog(recorder, reminder, this);
    dialog.exec();
}

void DesktopSprite::browseTasks()
{
    TasksBrowser dialog(recorder, reminder, this);
    dialog.exec();
}

void DesktopSprite::lockscreen() {
    ScreenLocker locker(0);
    locker.setCountdown(300);
    locker.exec();

    // update the database after lockscreen
    reminder->updateTasks();
}

/***************************************************************************
 *                           auxillary functions                           *
 ***************************************************************************/
void DesktopSprite::initDisplay()
{
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
}

void DesktopSprite::initMenuItems()
{
    // init actions
    exit_all = new QAction(this);
    add_task = new QAction(this);
    browse_tasks = new QAction(this);

    // set texts
    exit_all->setText(QWidget::tr("Exit Program"));
    add_task->setText(QWidget::tr("Add a Task"));
    browse_tasks->setText(QWidget::tr("Browse All Tasks"));

    // connect the action with functions
    QObject::connect(exit_all, SIGNAL(triggered()), app_handle, SLOT(quit()));
    QObject::connect(add_task, SIGNAL(triggered()), this, SLOT(addTask()));
    QObject::connect(browse_tasks, SIGNAL(triggered()), this, SLOT(browseTasks()));

    // initialize the settings and load the image
    this->setContextMenuPolicy(Qt::CustomContextMenu);      // enable the right click menu
    this->setFocusPolicy(Qt::ClickFocus);

    // set the right click menu
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButtonMenu(const QPoint&)));
}

void DesktopSprite::initMenu()
{
    qMenu = new QMenu();

    qMenu->addAction(add_task);
    qMenu->addAction(browse_tasks);
    qMenu->addAction(exit_all);
}

void DesktopSprite::initSystemTray()
{
    // load icon and set the icon for the application
    QIcon icon(":/images/gift.png");
    this->setWindowIcon(icon);

    // init system tray
    tray = new QSystemTrayIcon(this);
    tray->setContextMenu(qMenu);
    tray->setIcon(icon);
    tray->setToolTip(QString("QtSprite"));

    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    tray->show();
}

void DesktopSprite::initScreenLocker()
{
    // init the idle timer
    idle = new QTimer(this);
    idle->setInterval(configs->getInterval());
    idle->start();
    QObject::connect(idle, SIGNAL(timeout()), this, SLOT(lockscreen()));
}

// ======================  drag and drop gui implementation ========================
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
