#ifndef TABLE_H
#define TABLE_H

#include "card.h"


class Table: public QWidget
{
    Q_OBJECT

    private:

        QHBoxLayout* table_cards;
        QHBoxLayout* table_stock;
        QJsonArray old_cards;
        QJsonArray old_stock;

        void Create_Table();
        QLabel* Create_User_Details(QString name);

    public:

        Table();

        void Load_Cards(QJsonObject reply);
        void Load_Stock(QJsonObject reply);

};


#endif
