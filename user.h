#ifndef USER_H
#define USER_H

#include <QtWidgets>
#include "http.h"
#include "style.h"


class User: public QDialog
{
    Q_OBJECT

    private:

        Http_Manager* network;
        QLineEdit* username;
        QLineEdit* password;
        bool is_logged;

        void Create_Dialog();

    private slots:

        void Sign_In_SLOT();
        void Sign_Up_SLOT();

    public:

        User(QWidget* parent);

        bool Sign_In(QString username, QString password);
        bool Sign_Up(QString username, QString password);

        void keyPressEvent(QKeyEvent* event);
        void closeEvent(QCloseEvent* event);

};


#endif
