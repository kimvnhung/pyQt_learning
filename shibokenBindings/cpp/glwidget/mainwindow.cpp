#include "mainwindow.h"
#include "src/widgets/glwidget.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(new GLWidget());
    m_mainWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout;
    m_mainWidget->setLayout(layout);
    m_gridLayout = new QGridLayout;
    QPushButton *addWidgetBtn = new QPushButton(this);
    connect(addWidgetBtn,&QPushButton::clicked,this,&MainWindow::on_addWidgetBtnClicked);
    addWidgetBtn->setText("Add Widget");
    layout->addLayout(m_gridLayout);
    layout->addWidget(addWidgetBtn);

    ui->stackedWidget->addWidget(m_mainWidget);
    qDebug()<<ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(3);
    qDebug()<<ui->stackedWidget->currentIndex();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addWidgetBtnClicked()
{
    GLWidget *widget = new GLWidget;
    m_gridLayout->addWidget(widget);
}

