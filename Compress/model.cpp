#include "model.h"
#include <QDebug>
Model::Model()
{

}

double Model::zip()
{
    int * freq = new int[256]();

    QString filename = QFileDialog::getOpenFileName();
    if(filename == NULL)
        return 0;

    QString save_path = QFileDialog::getSaveFileName(NULL, "压缩为…", QDir::currentPath(), "MyZip文件(*.myzip);");
    if(save_path == NULL)
        return 0;

    QFile file(filename);
    file.open(QIODevice::ReadOnly);


    QFile file2(save_path);
    file2.open(QIODevice::Append);


    char * byte = new char[1]();

    while (!file.atEnd())
    {
        file.getChar(byte);
        freq[byte[0] + 128]++;
    }
    file.close();

    for(int i = 0; i < 256; i++)
    {
        qDebug() << freq[i];
    }

    Pnode * ptree[511]; //指针数组

    for(int i = 0; i < 511; i++)
    {
        Pnode * p = new Pnode;

        p->lrtag = -1;
        p->parent = NULL;
        p->weight = freq[i];
        ptree[i] = p;
    }

    for( int i = 0; i < 256; i++)
    {
        ptree[i]->weight = freq[i];
    }

    int emptyindex = 256;
    while(emptyindex != 511)
    {
        int index1 = -1;//最小权重
        int index2 = -1;//次小权重
        for(int i = 0; i < emptyindex; i++)
        {
            if(ptree[i]->parent == NULL)
            {
                if(index2 == -1)
                {
                    index2 = i;
                }
                else if(index1 == -1)
                {
                    if(ptree[i]->weight <= ptree[index2]->weight)
                    {
                        index1 = i;
                    }
                    else
                    {
                        index1 = index2;
                        index2 = i;
                    }
                }
                else if(ptree[i]->weight <= ptree[index2]->weight)
                {
                    if(ptree[i]->weight >= ptree[index1]->weight)
                    {
                        index2 = i;
                    }
                    else
                    {
                        index2 = index1;
                        index1 = i;
                    }
                }
            }
        }


//        qDebug() << index1 << ":" << ptree[index1]->weight << "and" << index2 << ":" << ptree[index2]->weight;
        qDebug() << emptyindex;

        ptree[index1]->lrtag = '0';
        ptree[index2]->lrtag = '1';

        ptree[index1]->parent = ptree[emptyindex];
        ptree[index2]->parent = ptree[emptyindex];

        ptree[emptyindex]->weight = ptree[index1]->weight + ptree[index2]->weight;
        index1 = -1;
        index2 = -1;
        emptyindex++;

    }

    //根节点ptree[510]已得到，parent==NULL，lr==-1
    char *huffman[256];
    for(int i = 0; i < 256; i++)
    {
        QString code = "";
        Pnode * p = ptree[i];
        huffman[i] = new char[256]();//申请最大值避免溢出
        while(p->parent != NULL)
        {
            code.insert(0, p->lrtag);
            p = p->parent;
        }
        for(int j = 0; j < code.length(); j++)
            huffman[i][j] = code[j].toLatin1();

//        qDebug() << i << ":" << huffman[i];
    }

//写入解码
    for(int i = 0; i < 256; i++)
    {
        file2.write(huffman[i]);
        file2.putChar('\n');
    }

    file.open(QIODevice::ReadOnly);

//开始压缩
    QString buf = "";
    while(!file.atEnd())
    {
        file.getChar(byte);
        buf = buf + huffman[byte[0] + 128];
        while(buf.length() >= 8)
        {
            file2.putChar((buf[0].toLatin1()-48)*128 + (buf[1].toLatin1()-48)*64 + (buf[2].toLatin1()-48)*32 + (buf[3].toLatin1()-48)*16 + (buf[4].toLatin1()-48)*8 + (buf[5].toLatin1()-48)*4 + (buf[6].toLatin1()-48)*2 + (buf[7].toLatin1()-48) - 128);
            buf = buf.mid(8);
        }
    }
    if(buf.length() != 0)
    {
        int count = 8 - buf.length();
        for(int i = 0; i < count; i++)
        {
            buf.append('0');
        }
//偶尔控制台报错，以后可以再补充
        file2.putChar((buf[0].toLatin1()-48)*128 + (buf[1].toLatin1()-48)*64 + (buf[2].toLatin1()-48)*32 + (buf[3].toLatin1()-48)*16 + (buf[4].toLatin1()-48)*8 + (buf[5].toLatin1()-48)*4 + (buf[6].toLatin1()-48)*2 + (buf[7].toLatin1()-48) - 128);
    }

    double cpratio = (double)file2.size() / (double)file.size();
    file.close();
    file2.close();

    delete []byte;
    delete []freq;
    for(int i = 0; i < 511; i++)
    {
        delete ptree[i];
    }
    for(int i = 0; i < 256; i++)
    {
        delete huffman[i];
    }

    return cpratio;
}





void Model::unzip()
{
    QString filename = QFileDialog::getOpenFileName(NULL, "打开压缩文件…", QDir::currentPath(), "MyZip文件(*.myzip);");
    if(filename == NULL)
        return;

    QString save_path = QFileDialog::getSaveFileName();
    if(save_path == NULL)
        return;

    QFile file(filename);
    file.open(QIODevice::ReadOnly);

    QFile file2(save_path);
    file2.open(QIODevice::Append);

    char * huffman[256];
    char * currentchar = new char[1]();

    for(int i = 0; i < 256; i++)
    {
        huffman[i] = new char[256]();
        int j = 0;
        while(true)
        {
            file.getChar(currentchar);
            if(*currentchar == '\n')
                break;
            huffman[i][j] = *currentchar;
            j++;
        }
        qDebug() << huffman[i];
    }

    Cnode * ctree = new Cnode();
    ctree->data = 0;
    ctree->lc = NULL;
    ctree->rc = NULL;
    for(int i = 0; i < 256; i++)
    {
        Cnode * p = ctree;
        for(int j = 0; huffman[i][j] != '\0'; j++)
        {
            if(huffman[i][j] == '1')
            {
                if(p->lc == NULL)
                {
                    p->lc = new Cnode();
                    p->lc->lc = NULL;
                    p->lc->rc = NULL;
                    p->lc->data = 0;
                }
                p = p->lc;
            }
            else if(huffman[i][j] == '0')
            {
                if(p->rc == NULL)
                {
                    p->rc = new Cnode();
                    p->rc->lc = NULL;
                    p->rc->rc = NULL;
                    p->rc->data = 0;
                }
                p = p->rc;
            }
        }
        p->data = i;
    }

    QString buf = "";
    int temp;
    int addindex;
    Cnode * p = ctree;
    while(!file.atEnd())
    {
        p = ctree;
        addindex = buf.length();
        file.getChar(currentchar);
        temp = *currentchar + 128;
        for(int i = 0; i < 8; i++)
        {
            buf.insert(addindex,(int)(temp)%2 + 48);
            temp = (int)(temp)/2;
        }

        for(int index = 0; index < buf.length(); index++)
        {
            if(buf[index] == '1')
            {
                if(p->lc != NULL)
                {
                    p = p->lc;
                    continue;
                }
            }
            else if(buf[index] == '0')
            {
                if(p->rc != NULL)
                {
                    p = p->rc;
                    continue;
                }
            }
            file2.putChar(p->data - 128);
            p = ctree;
            buf = buf.mid(index);
            index = -1;
        }
    }

    for(int i = 0; i < 256; i++)
    {
        delete huffman[i];
    }
    delete []currentchar;

}
