#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addWidgetBtnClicked();

private:
    Ui::MainWindow *ui;
    QWidget *m_mainWidget = NULL;
    QGridLayout *m_gridLayout = NULL;
};

#endif // MAINWINDOW_H
