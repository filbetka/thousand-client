#include "chat.h"


Message_Edit::Message_Edit(Chat* chat): chat(chat)
{
}

void Message_Edit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        this->chat->Send_New_Message(this->toPlainText());
        this->clear();
        return;
    }

    QTextEdit::keyPressEvent(event);
}



Chat::Chat(): QWidget(NULL)
{
    this->setObjectName("chat");

    this->network = Http_Manager::Get_Http_Manager();
    this->Create_Chat();
}

void Chat::Create_Chat()
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);

        // list of messages
    this->message_list = new QListWidget;
    this->message_list->setEnabled(false);
    this->message_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->message_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    layout->addWidget(message_list);

        // sender new message
    Message_Edit* new_message = new Message_Edit(this);
    new_message->setMaximumHeight(50);
    layout->addWidget(new_message);

    this->setLayout(layout);
    this->setMinimumWidth(300);

        // timer
    QTimer* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Load_All_Messages()));
    timer->start(1000);
}

void Chat::Create_Message(QString user, QString message, QString date)
{
    QWidget* widget = new QWidget;
    widget->setObjectName("widget");

    Style widget_style("#widget");
    widget_style.Add_Value("border-bottom", "1px solid green");
    widget->setStyleSheet(widget_style.Get_Style());

        // title
    Style title_style("QLabel");
    title_style.Add_Value("font-size", "12px");
    title_style.Add_Value("font", "bold");
    title_style.Add_Value("color", "green");

    QLabel* title = new QLabel(user);
    title->setStyleSheet(title_style.Get_Style());

        // time
    Style time_style("QLabel");
    time_style.Add_Value("font-size", "12px");
    time_style.Add_Value("font", "bold");
    time_style.Add_Value("color", "green");

    QLabel* time = new QLabel(date.mid(11, 5));
    time->setStyleSheet(time_style.Get_Style());

        // content
    Style content_style("QLabel");
    content_style.Add_Value("color", "#FFF");

    QLabel* content = new QLabel(message);
    content->setStyleSheet(content_style.Get_Style());
    content->setWordWrap(true);

        // adding title and time to layout
    QHBoxLayout* title_layout = new QHBoxLayout;
    title_layout->addWidget(title);
    title_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    title_layout->addWidget(time);

        // adding to main layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(title_layout);
    layout->addWidget(content);
    widget->setLayout(layout);

        // adding to list widgets
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(widget->sizeHint());
    this->message_list->addItem(item);
    this->message_list->setItemWidget(item, widget);
}

void Chat::Load_All_Messages()
{

                    // create post
                Post_Data x = this->network->Create_Data();
                x.Set_Path("game/status/");

                    // send post
                this->network->Send(x);


        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/chat/");
    data.Add_Variable("__get__", "");

        // send post
    QJsonObject reply = this->network->Send(data);
    QJsonArray messages = reply.value("messages").toArray();

        // display in message list
    if (old_messages == messages)
        return;

    this->message_list->clear();
    this->old_messages = messages;

    for (int k = 0; k < messages.size(); ++k)
    {
        QJsonObject message_object = messages.at(k).toObject();
        QString user = message_object.value("user").toString();
        QString message = message_object.value("message").toString();
        QString date = message_object.value("date").toString();

        this->Create_Message(user, message, date);
    }

        // scroll
    this->message_list->scrollToBottom();
}

void Chat::Send_New_Message(QString message)
{
    if (message.isEmpty())
        return;

        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/chat/");
    data.Add_Variable("__add__", "");
    data.Add_Variable("message", message);

        // send post
    QJsonObject reply = this->network->Send(data);
}
