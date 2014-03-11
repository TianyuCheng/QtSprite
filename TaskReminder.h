#ifndef TASKREMINDER_H
#define TASKREMINDER_H

/**
 * This class is the encapsulation for task reminding (popup and alarm)
 * by the integration with timer, and updatation of the tasks.
 */

#include <QApplication>
#include <QSound>
#include <QTimer>

#include <map>
#include <set>

#include "DesktopSprite.h"
#include "QNotify.h"
#include "Configuration.h"
#include "TaskRecorder.h"
#include "Task.h"

class TaskReminder : public QObject
{
    Q_OBJECT
    public:
        TaskReminder(Configuration *config, TaskRecorder *taskRecorder, DesktopSprite *p);
        virtual ~TaskReminder();

    public slots:
        void alarm();
        void updateTasks();

    private:
       void addTimer(Task *task, long time);

    private:
        Configuration *configs;

        DesktopSprite *parent;
        QSound *alertSound;

        // managing tasks
        std::map<QTimer*, Task*> taskTimers;
        std::set<int> tasksId;
        TaskRecorder *recorder;
};

#endif // TASKREMINDER_H
