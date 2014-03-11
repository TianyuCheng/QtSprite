#include "TasksBrowser.h"

TasksBrowser::TasksBrowser(TaskRecorder *record, TaskReminder *remind, QWidget *parent): QDialog(parent), recorder(record), reminder(remind)
{
    this->resize(QSize(700, 300));
    this->setWindowTitle(tr("Browsing Events"));
    
    // move the window to the center of the screen 
    QSize displaySize = QApplication::desktop()->screenGeometry(this).size();
    QSize _size = displaySize - size();
    this->move(QPoint(_size.width() / 2, _size.height() / 2));

    // init all widgets
    this->initWidgets();
    this->loadData();

    QObject::connect(button_delete, SIGNAL(clicked()), this, SLOT(button_delete_clicked()));
    QObject::connect(button_clear, SIGNAL(clicked()), this, SLOT(button_clear_clicked()));
    QObject::connect(button_add, SIGNAL(clicked()), this, SLOT(button_add_clicked()));
    QObject::connect(button_close, SIGNAL(clicked()), this, SLOT(button_close_clicked()));
}

TasksBrowser::~TasksBrowser() 
{
    this->clearItems();

    // delete all ui
    // the parent takes care
    main_layout->deleteLater();
    
}

void TasksBrowser::initWidgets()
{
    table = new QTableWidget;

    // header bold font
    QFont font = table->horizontalHeader()->font();
    font.setBold(true);
    table->horizontalHeader()->setFont(font);

    // header content
    table->setColumnCount(3);

    QStringList header;
    header << QWidget::tr("ID") << QWidget::tr("Datetime") << QWidget::tr("Description");
    table->setHorizontalHeaderLabels(header);

    table->setColumnWidth(0, 50);
    table->setColumnWidth(1, 250);

    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);

    table->verticalHeader()->setDefaultSectionSize(20);

    table->setFrameShape(QFrame::NoFrame);
    table->setShowGrid(false); 
    table->verticalHeader()->setVisible(false);
    table->setSelectionMode(QAbstractItemView::ExtendedSelection); 
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->resizeSection(0,150);
    table->horizontalHeader()->setFixedHeight(35);
    table->setStyleSheet("selection-background-color:lightblue;");
    //table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); 

    table->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:20px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
    table->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 20px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");

    button_add = new QPushButton(QWidget::tr("Add a Task"));
    button_delete = new QPushButton(QWidget::tr("Delete"));
    button_clear = new QPushButton(QWidget::tr("Clear"));
    button_close = new QPushButton(QWidget::tr("Close"));

    hbox = new QHBoxLayout;
    hbox->addWidget(button_add);
    hbox->addWidget(button_clear);
    hbox->addWidget(button_delete);
    hbox->addWidget(button_close);

    main_layout = new QGridLayout(this);
    main_layout->addWidget(table, 0, 0);
    main_layout->addLayout(hbox, 1, 0);
}

/***************************************************************************
 *                               Data update                               *
 ***************************************************************************/
void TasksBrowser::loadData() 
{
    std::vector<Task*> tasks = recorder->getAllTasks();
    table->setRowCount(tasks.size());
    int rowIndex = 0;
    for (std::vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); ++it, ++rowIndex) {
        table->setItem(rowIndex, 0, new QTableWidgetItem(QString::number((*it)->id)));
        table->setItem(rowIndex, 1, new QTableWidgetItem(((*it)->datetime).toString("yyyy-MM-dd hh:mm")));
        table->setItem(rowIndex, 2, new QTableWidgetItem((*it)->description));
        qDebug() << (*it)->id << (*it)->description << ((*it)->datetime).toString("yyyy-MM-dd hh:mm");
    }
    for (unsigned int i = 0; i < tasks.size(); i++) {
        delete tasks[i];
    }
    tasks.clear();
}

void TasksBrowser::clearItems()
{
    while(table->rowCount() > 0)
    {
        table->removeRow(0);
    }
}

/***************************************************************************
 *                       public slot implementation                        *
 ***************************************************************************/

void TasksBrowser::button_delete_clicked() 
{
    int row = table->currentRow();
    qDebug() << "try to delete row " << row;
    if (row >= 0) {
        QTableWidgetItem *idItem = table->itemAt(row, 0);
        QVariant id = idItem->data(Qt::EditRole);
        recorder->removeTask(id.toInt());

        table->removeRow(row);
    }
}

void TasksBrowser::button_clear_clicked()
{
    recorder->removeAll();
    this->clearItems();
}

void TasksBrowser::button_add_clicked()
{
    AddTaskDialog dialog(recorder, reminder, this);
    dialog.exec();

    this->loadData();
}

void TasksBrowser::button_close_clicked()
{
    this->close();
}
