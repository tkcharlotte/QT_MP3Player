#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QProcess"
#include <QListWidgetItem>
#include "QDir"
#include <QListWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void play_music(QString);

    QStringList filename(QString);
    ~MainWindow();

private slots:
    void on_prev_clicked();
    void ReadOutput();
    void on_next_clicked();
    void doubleclicked(QListWidgetItem* item);
    void on_pushButton_clicked();
    void press();
    void slide(int );
    void con();
    void changeVolume(int);

private:
    Ui::MainWindow *ui;
    QProcess *process;
    QListWidget *listWidget;
    QListWidgetItem *newItem ;

};

#endif // MAINWINDOW_H
