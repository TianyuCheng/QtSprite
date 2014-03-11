#include "TaskReminder.h"

TaskReminder::TaskReminder(Configuration *c, TaskRecorder *taskRecorder, DesktopSprite *p) : QObject(0), configs(c), parent(p), recorder(taskRecorder)
{
    this->alertSound = new QSound(":/sound/alarmecho.wav");

    this->updateTasks();
}

TaskReminder::~TaskReminder()
{
    delete alertSound;

    alertSound = 0;

    for (std::map<QTimer*, Task*>::iterator it = taskTimers.begin(); it != taskTimers.end(); it++)
    {
        delete it->first;
        delete it->second;
    }
}


void TaskReminder::updateTasks()
{
    recorder->updateDB();

    QDateTime datetime = QDateTime::currentDateTime();
    std::vector<Task*> tasksForToday = recorder->getTasksByDate(datetime);     // already sorted

    for (size_t i = 0; i < tasksForToday.size(); i++)
    {
        Task *currentTask = tasksForToday.front();
        long time = currentTask->datetime.toMSecsSinceEpoch() - datetime.toMSecsSinceEpoch();

        if (time < 0) {
            recorder->removeTask(currentTask->id);
            delete currentTask;
        }
        else {
            this->addTimer(currentTask, time);
        }
    }
}

void TaskReminder::addTimer(Task *task, long time)
{
    // found
    if (tasksId.find(task->id) != tasksId.end()) return;

    QTimer *timer = new QTimer(0);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(alarm()));
    this->taskTimers[timer] = task;
    timer->setInterval(time);
    timer->start();
    qDebug() << time;

    tasksId.insert(task->id);
}

// ============ public slots implementation ===========================
void TaskReminder::alarm() {
    QTimer* timer= dynamic_cast<QTimer*>(QObject::sender());
    Task * task = taskTimers.find(timer)->second;
    qDebug() << task->description;

    recorder->removeTask(task->id);

    if (!configs->isMuted())
        this->alertSound->play();

    QNotify *popup = new QNotify(parent);
    popup->notify(task->description, QNotify::SUCCESS);

    timer->stop();
}
