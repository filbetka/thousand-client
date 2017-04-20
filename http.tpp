template <typename CLASS>
Http_Manager<CLASS>::Http_Manager()
{
    this->network = Http_Request::Get_Http_Request();
    this->timer = new QTimer(this);
    QObject::connect(this->timer, SIGNAL(timeout()),
        this, SLOT(Try_Request()));
}

template <typename CLASS>
void Http_Manager<CLASS>::Try_Request()
{
        // if not running, run
    if (!this->network->Is_Running())
        return;

    else
    {
        this->timer->stop();
        this->Reply();
    }
}

template <typename CLASS>
void Http_Manager<CLASS>::Request(
        Post_Data data, class_method method)
{
        // its work now
    if (this->timer->isAvtive())
        return;

    this->callback = method;
    this->post_data = data;

        // if not running, run
    if (!this->network->Is_Running())
        this->Reply();

    else this->timer->start(50);
}

template <typename CLASS>
void Http_Manager<CLASS>::Reply()
{
    QJsonObject reply = this->network->
            POST(this->post_data);

        // launch reply method - callback
    (this->*(this->callback))(reply);
}

template <typename CLASS>
Post_Data Http_Manager<CLASS>::Create_Data()
{
    Post_Data data(this->network->Get_CSRF());
    data.Set_Url(this->url);

    return data;
}
