#ifndef BIDDING_H
#define BIDDING_H

#include <QtWidgets>
#include "style.h"
#include "http.h"


class Bidding: public QDialog
{
    Q_OBJECT

    private:

        Http_Manager* network;
        QListWidget* biddings;
        QLineEdit* points;
        QJsonArray old_biddings;
        QLabel* current_user;

        void Create_Window();
        void Add_Bidding_Widget(QString points, QString user);

    private slots:

        void Pass();
        void Bid();
        void Finish();

    public:

        Bidding(QWidget* parent);

        void Load_Biddings(QJsonObject reply);
        void Show_Message(QJsonObject reply);

};


#endif
