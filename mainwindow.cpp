#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QProcess"
#include "QMessageBox"
#include "QDir"
#include "QDebug"
#include "QPixmap"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool isplay = true;
    process = new QProcess(this);
    QObject::connect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
    QObject::connect(ui->display,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(doubleclicked(QListWidgetItem*)));
    QObject::connect(ui->horizontalSlider, SIGNAL(sliderPressed()),this, SLOT(press()));
    QObject::connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)),this, SLOT(slide(int )));
    QObject::connect(ui->horizontalSlider, SIGNAL(sliderReleased()),this, SLOT(con()));
    connect(ui->verticalSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeVolume(int)));
    QStringList strList;

    strList = filename("../music/mp3");
    ui->display->addItems(strList);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::play_music(QString file_name)
{
    QStringList args;
     QString p;
    args << "-slave";        //使用slave模式
    args << "-quiet";        //不要输出冗余信息
    p = "../music/mp3/";
    args <<p.append(file_name) ;        //播放file_name文件
    process->start("mplayer", args);

    //启动该进程，并传入参数args
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "play"){
        ui->pushButton->setText("pause");
        process->write("pause\n");
        QObject::disconnect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));

    }
    else if(ui->pushButton->text()=="pause") {
        ui->pushButton->setText("play");
        process->write("get_time_pos\n");
        connect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
        process->write("pause\n");




    }
}

QStringList MainWindow::filename(QString path){
       QDir dir(path);
       QStringList filters;
       filters<<QString("*.mp3");
       dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
       dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式

       int dir_count = dir.count();
//           if(dir_count <= 0)
//               return;
       QStringList string_list;
          for(int i=0; i<dir_count; i++)
          {
              QString file_name = dir[i];  //文件名称
              //ts<<file_name<<"\r\n"<<"\r\n";
              string_list.append(file_name);
          }
        return string_list;
}

void MainWindow::on_prev_clicked()
{
    //
    if (ui->display->count() != 0) {
            if(ui->display->currentRow() == 0)
                ui->display->setCurrentRow(ui->display->count() - 1);
            else
                ui->display->setCurrentRow(ui->display->currentRow() - 1);
            process->close();
            QObject::disconnect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
            process = new QProcess(this);
            QObject::connect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
            play_music(ui->display->currentItem()->text());
            ui->pushButton->setText("pause");
        }

}

void MainWindow::on_next_clicked()
{
    process->close();
   if(ui->display->count() != 0){
       if(ui->display->currentRow() == ui->display->count() - 1)
             ui->display->setCurrentRow(0);
        else
             ui->display->setCurrentRow(ui->display->currentRow() + 1);
      // process->close();
        QObject::disconnect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
        QObject::connect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));



        play_music(ui->display->currentItem()->text());
        ui->pushButton->setText("pause");
   }
}


void MainWindow::ReadOutput()
{
    QString data,length,str_min,str_sec,list;
    float num_length,num_position;

    process->write("get_time_length\n");
    process->write("get_time_pos\n");

    while(process->canReadLine())
    {


     data = process->readLine();
     if(data.startsWith("ANS_LENGTH=")){
             length = data.mid(11);
             float ll = length.toFloat();
             int minute = (int)ll / 60;
             int second = (int)ll %60;
             QString st_min = QString::number(minute);
             QString st_sec = QString::number(second);
             ui->label_3->setText("/ "+ st_min + ":" + st_sec);
             num_length = length.toFloat();
             ui->horizontalSlider->setMaximum(ll);


     }
     else if(data.startsWith("ANS_TIME_POSITION"))
     {
         list = data.mid(18);
         float ll = list.toFloat();
         num_position = ll;
         int minute = (int)ll / 60;
         int second = (int)ll %60;
          str_min = QString::number(minute);
          str_sec = QString::number(second);
          ui->horizontalSlider->setValue(ll);
          ui->label_2->setText("  "+ str_min+": "+str_sec);

     }
     if(num_length == num_position)
    {
         if(ui->display->currentRow() == ui->display->count() - 1)

         {
             process->close();
             process = new QProcess(this);
             ui->display->setCurrentRow(0);
             play_music(ui->display->currentItem()->text());

         }
         else{

             ui->display->setCurrentRow(ui->display->currentRow() + 1);
             process->close();
             QObject::disconnect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
              process = new QProcess(this);
              process->write("get_time_pos\n");
              QObject::connect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
             play_music(ui->display->currentItem()->text());

         }
     }




    }
}




void MainWindow::doubleclicked(QListWidgetItem*item)
{
//    QListWidgetItem *item = new QListWidgetItem(ui->display);
//    item->set
    process->close();
    QString name ;
    name = item->text();
    QObject::disconnect(ui->display,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(doubleclicked(QListWidgetItem*)));
    process = new QProcess(this);
    QObject::connect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
    QObject::connect(ui->display,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(doubleclicked(QListWidgetItem*)));
    play_music(name);
    //ui->pushButton->setText("pause");
}
void MainWindow::press()
{
      QObject::disconnect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
}

void MainWindow::slide(int value){

  process->write(QString("seek " + QString::number(value) + " 2\n").toAscii());

}

void MainWindow::con()
{
      process->write("get_time_length\n");
    process->write("get_time_pos\n");
     QObject::connect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(ReadOutput()));
}

void MainWindow::changeVolume(int v){
//QString::number(v)
       ui->verticalSlider->setValue(v);
       process->write(QString("volume " + QString::number(v) + " 2\n").toUtf8());

}
