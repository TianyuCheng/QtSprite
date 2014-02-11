#include "AddTaskDialog.h"

AddTaskDialog::AddTaskDialog(TaskRecorder *task,QWidget *parent): QDialog(parent), recorder(task)
{
    this->resize(QSize(400, 300));
    this->setWindowTitle(tr("Add an Event"));
    
    // move the window to the center of the screen 
    QSize displaySize = QApplication::desktop()->screenGeometry(this).size();
    QSize _size = displaySize - size();
    this->move(QPoint(_size.width() / 2, _size.height() / 2));

    // init all widgets
    this->initWidgets();

    QObject::connect(button_okay, SIGNAL(clicked()), this, SLOT(button_okay_clicked()));
    QObject::connect(button_cancel, SIGNAL(clicked()), this, SLOT(close()));

    QObject::connect(time, SIGNAL(dateChanged(const QDate &)), date, SLOT(setSelectedDate(const QDate &)));
    QObject::connect(date, SIGNAL(clicked(const QDate &)), time, SLOT(setDate(const QDate &)));

    time->setDateTime(QDateTime::currentDateTime());
}

AddTaskDialog::~AddTaskDialog()
{
    // delete all pointers for ui
    label_time->deleteLater();
    label_date->deleteLater();
    label_title->deleteLater();

    content->deleteLater();
    date->deleteLater();
    time->deleteLater();

    button_okay->deleteLater();
    button_cancel->deleteLater();

    grid_layout->deleteLater();
    hbox_layout->deleteLater();
    main_layout->deleteLater();
}

void AddTaskDialog::initWidgets()
{
    label_title = new QLabel(tr("Event Description"));
    label_date = new QLabel(tr("Date"));
    label_time = new QLabel(tr("Time"));

    content = new QTextEdit;
    date = new QCalendarWidget;
    time = new QDateTimeEdit;

    button_okay = new QPushButton(tr("OK"));
    button_cancel = new QPushButton(tr("Cancel"));

    grid_layout = new QGridLayout;
    grid_layout->addWidget(label_title, 0, 0);
    grid_layout->addWidget(content, 0, 1);

    grid_layout->addWidget(label_time, 1, 0);
    grid_layout->addWidget(time, 1, 1);

    grid_layout->addWidget(label_date, 2, 0);
    grid_layout->addWidget(date, 2, 1);

    hbox_layout = new QHBoxLayout;
    hbox_layout->addWidget(button_okay);
    hbox_layout->addWidget(button_cancel);
    
    main_layout = new QGridLayout(this);
    main_layout->addLayout(grid_layout, 0, 0);
    main_layout->addLayout(hbox_layout, 1, 0);
}

/***************************************************************************
 *                       public slots implementation                       *
 ***************************************************************************/
void AddTaskDialog::button_okay_clicked() 
{
    QString event = content->toPlainText();
    QDateTime time_stamp = time->dateTime();

    if(!event.isEmpty()) 
    {
        qDebug() << event << time_stamp;
        recorder->addNewTask(event, time_stamp);
        close();

//        notification.notify(QString(tr("The task has been added")), QNotify::SUCCESS, 1000);
    }
    else
    {
        QMessageBox::warning(this, QObject::tr("Task Manager"), QObject::tr("Please enter the event description!"));
    }
}
