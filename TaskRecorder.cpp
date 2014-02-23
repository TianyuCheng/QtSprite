#include "TaskRecorder.h"


TaskRecorder::TaskRecorder() 
{
   this->initDatabase();
   if (!dbconn.open())
   {
        QMessageBox::critical(0, QObject::tr("Database"), QObject::tr("Sorry, I couldn't connect to SQLite database"));
        return;
   }

   query = QSqlQuery(dbconn);
   query.exec("create table if not exists tasks(id integer primary key autoincrement,event varchar, time datetime)");
   qDebug() << "open sqlite connection";
}

TaskRecorder::~TaskRecorder()
{
    dbconn.close();
}

int TaskRecorder::initDatabase()
{
    dbconn = QSqlDatabase::addDatabase("QSQLITE");
    dbconn.setDatabaseName("mytasks.sqlite");

    return 0;
}

int TaskRecorder::addNewTask(QString &event, QDateTime &datetime)
{
    qDebug() << event << datetime.toString("yyyy-MM-dd hh:mm:ss");
    QString sql_statement = QString(
            "insert into tasks (event, time) values('%1', '%2')").arg(
            event, datetime.toString("yyyy-MM-dd hh:mm:ss"));

    qDebug() << sql_statement;
    query.exec(sql_statement);
    return 1;
}

int TaskRecorder::removeTask(int id)
{
    QString sql_statement = QString("delete from tasks where id = %1").arg(id);
    query.exec(sql_statement);
    qDebug() << sql_statement;

    query.exec("select id,event from tasks where id >= 0");
//    query.exec("update tasks set id = rowid");

    while(query.next())
    {
          int ele0=query.value(0).toInt();
          QString ele1=query.value(1).toString();
          qDebug()<<ele0<<ele1;
    }
    return 1;
}


std::vector<Task*> TaskRecorder::getAllTasks()
{
    std::vector<Task*> tasks;

    query.exec("select id,event,time from tasks where id >= 0");
    while(query.next())
    {
        int id=query.value(0).toInt();
        QString description=query.value(1).toString();
        QDateTime datetime = query.value(2).toDateTime();
//        qDebug() << id << description << datetime;
        tasks.push_back(new Task(id, description, datetime));
    }
    return tasks;
}

int TaskRecorder::removeAll()
{
    query.exec("delete from tasks");
    return 1;
}

std::vector<Task*> TaskRecorder::getTasksByDate(QDateTime &datetime) {
    std::vector<Task*> tasks;

    QString sql_statement = QString(
            "select id, event, time from tasks where date(time) = date('%1') order by time").arg(datetime.toString("yyyy-MM-dd"));

    qDebug() << sql_statement;
    query.exec(sql_statement);

    while(query.next())
    {
        int id=query.value(0).toInt();
        QString description=query.value(1).toString();
        QDateTime datetime = query.value(2).toDateTime();
        qDebug() << id << description << datetime;
        tasks.push_back(new Task(id, description, datetime));
    }

    return tasks;
}

void TaskRecorder::updateDB()
{
    // don't know why its not working
    // query.exec("delete from tasks where datetime(time) < datetime('now')");
}
