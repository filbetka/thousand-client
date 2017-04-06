#include "http_request.h"


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

QByteArray Post_Data::Get_Url()
{
    return this->url.toUtf8();
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
    QJsonObject reply = this->GET(url);
    qDebug() << reply.value("csrfmiddlewaretoken").toString();
    this->csrf = reply.value("csrfmiddlewaretoken").toString();
}

QString Http_Request::Get_CSRF()
{
    return this->csrf;
}



Form_Manager::Form_Manager(QString url)
{
    if (!url.isEmpty())
        this->Set_Url(url);
}

void Form_Manager::Set_Url(QString url)
{
    this->url = url;
    this->http.Set_CSRF(url);
}

QJsonObject Form_Manager::Send(Post_Data& data)
{
    return this->http.POST(data);
}

Post_Data Form_Manager::Login(QString email, QString password)
{
    Post_Data data(this->http.Get_CSRF());
    data.Set_Url(this->url + "user/sign_in/");

    data.Add_Variable("email", email);
    data.Add_Variable("password", password);
    data.Add_Variable("__form__", "login");

    return data;
}



