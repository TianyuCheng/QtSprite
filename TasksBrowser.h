#ifndef TASKS_BRWOSER_H
#define TASKS_BRWOSER_H

#include <QApplication>
#include <QtGui>
#include <QDialog>
#include <QDesktopWidget>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>

#include <vector>

#include "TaskRecorder.h"
#include "TaskReminder.h"
#include "AddTaskDialog.h"
#include "QNotify.h"
#include "Task.h"

class TasksBrowser : public QDialog
{
    Q_OBJECT
public:
    TasksBrowser(TaskRecorder *recorder, TaskReminder *reminder, QWidget *parent = 0);
    virtual ~TasksBrowser();

    void initWidgets();
    void loadData();
public slots:
    void button_delete_clicked();
    void button_clear_clicked();
    void button_add_clicked();
    void button_close_clicked();

private:
    void clearItems();

private:
	// data
    TaskRecorder *recorder;
    TaskReminder *reminder;

    // ui
    QTableWidget *table;

    QPushButton *button_delete;
    QPushButton *button_clear;
    QPushButton *button_add;
    QPushButton *button_close;

    QGridLayout *main_layout;
    QHBoxLayout *hbox;

    QNotify notification;
};


#endif /* end of include guard: TASKS_BRWOSER_H */

