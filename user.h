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
        QLineEdit* username_form;
        QLineEdit* password_form;
        bool is_logged;
        QString username;

        void Create_Dialog();

    private slots:

        void Sign_In_SLOT();
        void Sign_Up_SLOT();

    public:

        User(QWidget* parent);

        bool Sign_In(QString username, QString password);
        bool Sign_Up(QString username, QString password);
        bool Sign_Out();

        QString Get_Username();

        void keyPressEvent(QKeyEvent* event);
        void closeEvent(QCloseEvent* event);

};


#endif
