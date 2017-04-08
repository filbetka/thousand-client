#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <QtNetwork>
#include <QRegularExpression>


class Post_Data
{

    private:

        QString url;
        QString path;
        QString data;

    public:

        Post_Data(QString csrf="");

        void Add_Variable(QString name, QString value);
        void Set_Url(QString url);
        void Set_Path(QString path);

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



class Http_Manager
{

    private:

        Http_Request http;
        QString url;

    public:

        Http_Manager(QString url="");
        static Http_Manager* Get_Http_Manager(); // singleton

        void Set_Url(QString url);
        QJsonObject Send(Post_Data& data);
        Post_Data Create_Data();

};


#endif
