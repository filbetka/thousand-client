#include "http.h"


Post_Data::Post_Data(QString csrf)
{
    if (!csrf.isEmpty())
        this->Add_Variable("csrfmiddlewaretoken", csrf);
}

void Post_Data::Add_Variable(QString name, QString value)
{
    if (data.isEmpty())
        data = name + "=" + value;

    else data += "&" + name + "=" + value;
}

void Post_Data::Set_Url(QString url)
{
    this->url = url;
}

void Post_Data::Set_Path(QString path)
{
    this->path = path;
}

QByteArray Post_Data::Get_Url()
{
    return (this->url + this->path).toUtf8();
}

QByteArray Post_Data::Get_Data()
{
    return this->data.toUtf8();
}



Http_Request::Http_Request()
{
    this->network = new QNetworkAccessManager(this);
    this->event_loop = new QEventLoop;

    QObject::connect(this->network, SIGNAL(finished(QNetworkReply*)),
                     this->event_loop, SLOT(quit()));
}

QJsonObject Http_Request::GET(QString url)
{
        // create request and send
    QNetworkRequest request(QUrl(url.toUtf8()));
    QNetworkReply* reply = this->network->get(request);
    this->event_loop->exec();

        // if error
    if (reply->error() != QNetworkReply::NoError)
        qDebug() << reply->errorString();

        // return date
    QString content = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());

    delete reply;
    return document.object();
}

QJsonObject Http_Request::POST(Post_Data data)
{
        // create request and send
    QNetworkRequest request(QUrl(data.Get_Url()));
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");

    QNetworkReply* reply = this->network->post(request, data.Get_Data());
    this->event_loop->exec();

        // if error
    if (reply->error() != QNetworkReply::NoError)
        qDebug() << reply->errorString();

        // return date
    QString content = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());

    delete reply;
    return document.object();
}

void Http_Request::Set_CSRF(QString url)
{
    static QJsonObject reply = this->GET(url);
    this->csrf = reply.value("csrfmiddlewaretoken").toString();
}

QString Http_Request::Get_CSRF()
{
    return this->csrf;
}



Http_Manager::Http_Manager(QString url)
{
    if (!url.isEmpty())
        this->Set_Url(url);
}

Http_Manager* Http_Manager::Get_Http_Manager()
{
    static QString URL = "http://thousand.arbuz.team/"; //"http://localhost:8000/";
    static Http_Manager first(URL);
    static QNetworkCookieJar* cookie =
            first.http.network->cookieJar();

    Http_Manager* connection = new Http_Manager(URL);
    connection->http.network->setCookieJar(cookie);
    connection->http.csrf = first.http.csrf;

    return connection;
}

void Http_Manager::Set_Url(QString url)
{
    this->url = url;
    this->http.Set_CSRF(url);
}

QJsonObject Http_Manager::Send(Post_Data& data)
{
    return this->http.POST(data);
}

Post_Data Http_Manager::Create_Data()
{
    Post_Data data(this->http.Get_CSRF());
    data.Set_Url(this->url);

    return data;
}
