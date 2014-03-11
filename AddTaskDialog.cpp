#include "AddTaskDialog.h"

AddTaskDialog::AddTaskDialog(TaskRecorder *record, TaskReminder *remind, QWidget *parent): QDialog(parent), recorder(record), reminder(remind)
{
    this->resize(QSize(400, 300));
    this->setWindowTitle(QWidget::tr("Add a Task"));
    
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
    // acutually Qt takes care of that
    // all the children items will be taken care of the parent
    main_layout->deleteLater();
}

void AddTaskDialog::initWidgets()
{
    label_title = new QLabel(QWidget::tr("Event Description"));
    label_date  = new QLabel(QWidget::tr("Date"));
    label_time  = new QLabel(QWidget::tr("Time"));

    content = new QTextEdit;
    date = new QCalendarWidget;
    time = new QDateTimeEdit;

    button_okay = new QPushButton(QWidget::tr("OK"));
    button_cancel = new QPushButton(QWidget::tr("Cancel"));

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

    QDateTime now = QDateTime::currentDateTime();
    long time = time_stamp.toMSecsSinceEpoch() - now.toMSecsSinceEpoch();

    if (time <= 0)
    {
        QMessageBox::warning(this, QWidget::tr("Task Manager"), QWidget::tr("The datetime you entered is in the past!"));
        return;
    }

    if(!event.isEmpty())
    {
        qDebug() << event << time_stamp;
        recorder->addNewTask(event, time_stamp);
        close();

        reminder->updateTasks();
    }
    else
    {
        QMessageBox::warning(this, QWidget::tr("Task Manager"), QWidget::tr("Please enter the event description!"));
    }
}
