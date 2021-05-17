#ifndef MODEL_H
#define MODEL_H
#include <QFileDialog>

class Model
{
    struct Pnode
    {
        int weight;
        Pnode * parent;
        int lrtag;
    };

    struct Cnode
    {
        char data;
        Cnode * lc;
        Cnode * rc;
    };

public:
    Model();
    double zip();
    void unzip();
private:

};

#endif // MODEL_H
