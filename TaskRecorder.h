#ifndef TASK_RECORDER_H
#define TASK_RECORDER_H

#include <QtGui>
#include <QtWidgets>
#include <QMessageBox>
#include <QtSql>
#include <QDate>
#include <QDateTime>
#include <QString>

#include <vector>

#include "Task.h"

class TaskRecorder
{
public:
    TaskRecorder();
    virtual ~TaskRecorder();

    int initDatabase();
    int addNewTask(QString &event, QDateTime &datetime);
    int removeTask(int id);
    int removeAll();

    std::vector<Task*> getAllTasks();
    std::vector<Task*> getTasksByDate(QDateTime &datetime);

    void updateDB();
private:
    QSqlDatabase dbconn;
    QSqlQuery query;
};


#endif /* end of include guard: TASK_RECORDER_H */
