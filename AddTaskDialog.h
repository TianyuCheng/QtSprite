#ifndef ADD_TASK_DIALOG_H
#define ADD_TASK_DIALOG_H

#include <QApplication>
#include <QtGui>
#include <QDialog>
#include <QDesktopWidget>

#include <QLabel>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QPushButton>

#include <QGridLayout>  
#include <QHBoxLayout>

#include <QString>
#include <QDateTime>

#include "TaskRecorder.h"
#include "TaskReminder.h"
#include "QNotify.h"

class AddTaskDialog : public QDialog
{
    Q_OBJECT
public:
    AddTaskDialog(TaskRecorder *recorder, TaskReminder *reminder, QWidget *parent = 0);
    virtual ~AddTaskDialog();

    void initWidgets();

public slots:
    void button_okay_clicked();

private:
    // data
    TaskRecorder *recorder;
    TaskReminder *reminder;

    // ui
    QLabel *label_title;
    QLabel *label_date;
    QLabel *label_time;

    QTextEdit *content;
    QCalendarWidget *date;
    QDateTimeEdit *time;

    QPushButton *button_okay;
    QPushButton *button_cancel;

    QGridLayout *main_layout;
    QGridLayout *grid_layout;
    QHBoxLayout *hbox_layout;
};



#endif /* end of include guard: ADD_TASK_DIALOG_H */
