#include "compress.h"
#include "ui_compress.h"
#include <QMessageBox>

Compress::Compress(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Compress)
{
    ui->setupUi(this);
}

Compress::~Compress()
{
    delete ui;
}


void Compress::on_btn_zip_clicked()
{
    QMessageBox::information(this, "消息", "压缩率：" + QString::number(this->model->zip() * 100) + "%", QMessageBox::Ok, QMessageBox::NoButton);
}

void Compress::on_btn_unzip_clicked()
{
    this->model->unzip();
    QMessageBox::information(this, "消息", "已解压", QMessageBox::Ok,QMessageBox::NoButton);

}
