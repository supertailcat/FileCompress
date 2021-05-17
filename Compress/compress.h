#ifndef COMPRESS_H
#define COMPRESS_H
#include "model.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Compress; }
QT_END_NAMESPACE

class Compress : public QMainWindow
{
    Q_OBJECT

public:
    Compress(QWidget *parent = nullptr);
    ~Compress();

private slots:
    void on_btn_zip_clicked();

    void on_btn_unzip_clicked();

private:
    Ui::Compress *ui;
    Model * model;
};
#endif // COMPRESS_H
