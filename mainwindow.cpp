#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{  
    int id = QFontDatabase::addApplicationFont(":/Roboto-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);

    QWidget *centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout();
    centralWidget->setLayout(layout);

    QVBoxLayout* layoutPaste=new QVBoxLayout;

    SerialNumberLineEdit* lineEdit=new SerialNumberLineEdit;
    lineEdit->setFont(font);

    LabelPaste* lab=new LabelPaste;
    lab->setFont(font);
    QObject::connect(lineEdit,SIGNAL(visiblePaste(bool)),lab,SLOT(setVisibility(bool)));

    layoutPaste->addWidget(lab);
    layoutPaste->addWidget(lineEdit);

    layout->addWidget(new QPushButton("Take Focus"),1,0);
    layout->addLayout(layoutPaste,0,0);


    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
}

