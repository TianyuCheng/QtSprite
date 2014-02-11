#include "TasksBrowser.h"

TasksBrowser::TasksBrowser(TaskRecorder *task,QWidget *parent): QDialog(parent), recorder(task)
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
}

TasksBrowser::~TasksBrowser() 
{
    this->clearItems();

    table->deleteLater();

    button_delete->deleteLater();
    button_clear->deleteLater();
    button_add->deleteLater();

    hbox->deleteLater();
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
    header << tr("ID") << tr("Date & Time") << tr("Description");
    table->setHorizontalHeaderLabels(header);

//    table->horizontalHeader()->setDefaultSectionSize(150);
    table->horizontalHeader()->setStretchLastSection(true);

    table->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);

//    table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
//    table->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

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

    button_add = new QPushButton(tr("Add a Task"));
    button_delete = new QPushButton(tr("Delete"));
    button_clear = new QPushButton(tr("Clear"));

    hbox = new QHBoxLayout;
    hbox->addWidget(button_add);
    hbox->addWidget(button_clear);
    hbox->addWidget(button_delete);

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
    while(table->rowCount())
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
    QTableWidgetItem *idItem = table->itemAt(row, 0);
    QVariant id = idItem->data(Qt::EditRole);
    recorder->removeTask(id.toInt());

    table->removeRow(row);

    QNotify notification(0);
//    notification.notify(QString(tr("The task has been cleared")), QNotify::SUCCESS, 1000);
}

void TasksBrowser::button_clear_clicked()
{
    recorder->removeAll();
    this->clearItems();

//    notification.notify(QString(tr("All tasks have been cleared")), QNotify::SUCCESS, 1000);
}

void TasksBrowser::button_add_clicked()
{
    AddTaskDialog dialog(recorder, this);
    dialog.exec();

    this->loadData();
}
