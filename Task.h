#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDateTime>

class Task
{
public:
    Task(int id, QString description, QDateTime datetime);
    
public:
    int id;
    QString description;
    QDateTime datetime;
};

#endif // TASK_H
