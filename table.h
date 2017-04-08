#ifndef TABLE_H
#define TABLE_H

#include "card.h"


class Table: public QWidget
{
    Q_OBJECT

    private:

        void Create_Table();
        QLabel* Create_User_Details(QString name);

    public:

        Table();

};


#endif
