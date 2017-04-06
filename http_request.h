#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <QtNetwork>
#include <QRegularExpression>


class Post_Data
{

    private:

        QString url;
        QString data;

    public:

        Post_Data(QString csrf="");

        void Add_Variable(QString name, QString value);
        void Set_Url(QString url);

        QByteArray Get_Url();
        QByteArray Get_Data();

};



class Http_Request: public QObject
{
    Q_OBJECT

    private:

        QNetworkAccessManager* network;
        QEventLoop* event_loop;
        QString csrf;

    public:

        Http_Request();

        QJsonObject GET(QString url);
        QJsonObject POST(Post_Data data);

        void Set_CSRF(QString url);
        QString Get_CSRF();

};



class Form_Manager
{

    private:

        Http_Request http;
        QString url;

    public:

        Form_Manager(QString url="");

        void Set_Url(QString url);
        QJsonObject Send(Post_Data& data);

        Post_Data Login(QString email, QString password);

};


#endif
