#include "table.h"
#include "game.h"


Table::Table(Game* game)
{
    this->network = Http_Manager::Get_Http_Manager();
    this->game = game;

    this->timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()),
        this, SLOT(Clear_Stock()));

    this->Create_Table();
}

void Table::Create_Table()
{
        // current
    this->current_user = this->Create_User_Details("Current user:");
    this->current_user->setAlignment(Qt::AlignCenter);

        // player information
    this->first_user = this->Create_User_Details("First user");
    this->first_user->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    this->second_user = this->Create_User_Details("Second user");
    this->second_user->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout* other_player = new QHBoxLayout;
    other_player->addWidget(new Card(NONE_COLOR, NONE_RANK));
    other_player->addWidget(this->first_user);
    other_player->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    other_player->addWidget(this->current_user);
    other_player->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    other_player->addWidget(this->second_user);
    other_player->addWidget(new Card(NONE_COLOR, NONE_RANK));

        // stock
    this->table_stock = new QHBoxLayout;
    this->table_stock->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_stock->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_stock->addWidget(new Card(NONE_COLOR, NONE_RANK));

    QHBoxLayout* stos_layout = new QHBoxLayout;
    stos_layout->setMargin(0);
    stos_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    stos_layout->addLayout(this->table_stock);
    stos_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));

        // player cards
    this->table_cards = new QHBoxLayout;
    this->table_cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_cards->addWidget(new Card(NONE_COLOR, NONE_RANK));

    QHBoxLayout* cards_layout = new QHBoxLayout;
    cards_layout->setMargin(0);
    cards_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    cards_layout->addLayout(this->table_cards);
    cards_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));

        // adding layouts
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(other_player);
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout(stos_layout);
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout(cards_layout);

        // create content widget
    QWidget* content = new QWidget;
    content->setObjectName("table");
    content->setLayout(layout);

    Style style("#table");
    style.Add_Value("background-image",
        "url('../thousand-client/icons/table.jpg')");

    this->setMinimumSize(1000, 600);
    this->setMaximumSize(1000, 600);
    this->setStyleSheet(style.Get_Style());

        // set content to `Table`
    QHBoxLayout* content_layout = new QHBoxLayout;
    content_layout->setMargin(0);
    content_layout->setSpacing(0);
    content_layout->addWidget(content);
    this->setLayout(content_layout);
}

QLabel* Table::Create_User_Details(QString text)
{
    QLabel* details = new QLabel(text);
    details->setObjectName("user_details");

    Style style("#user_details");
    style.Add_Value("font-size", "25px");
    style.Add_Value("color", "#DDD");

    details->setStyleSheet(style.Get_Style());
    return details;
}

void Table::Clear_Stock()
{
    this->timer->stop();

        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/throw_card/");
    data.Add_Variable("__clear__", "");

        // send post
    QJsonObject reply = this->network->Send(data);
    if (reply.contains("status"))
        if (!reply.value("status").toBool())
            this->Clear_Stock();

    if (!reply.contains("status"))
        this->Clear_Stock();
}

void Table::Load_Cards(QJsonObject reply)
{
    QJsonArray cards = reply.value("cards").toArray();

        // if not changes
    if (this->old_cards == cards)
        return;

        // remove old cards
    this->old_cards = cards;
    QLayoutItem* item;

    while ((item = this->table_cards->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }

        // adding new cards
    for (int k = 0; k < cards.size(); ++k)
    {
        QJsonObject card_object = cards.at(k).toObject();
        QString color = card_object.value("cards__color__name").toString();
        QString rank = card_object.value("cards__rank__name").toString();

        this->table_cards->addWidget(new Card(color, rank, this));
    }
}

void Table::Load_Stock(QJsonObject reply)
{
    QJsonArray stock = reply.value("stock").toArray();

        // if not changes
    if (this->old_stock == stock)
        return;

        // remove old cards
    this->old_stock = stock;
    QLayoutItem* item;

    while ((item = this->table_stock->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }

        // adding new cards
    for (int k = 0; k < stock.size(); ++k)
    {
        QJsonObject stock_object = stock.at(k).toObject();
        QString color = stock_object.value("color__name").toString();
        QString rank = stock_object.value("rank__name").toString();

        this->table_stock->addWidget(new Card(color, rank));
    }
}

void Table::Update_Users(QJsonObject reply)
{
        // current user
    QString current_user = reply.value("current_user").toString();
    if (!this->current_user->text().contains(current_user))
        this->current_user->setText("Current user:\n" + current_user);

        // get users
    QString user = this->game->user->Get_Username();
    QJsonArray user_status = reply.value("users_status").toArray();

    if (user_status.isEmpty())
        return;

        // remove logged user
    for (int k = 0; k < 3; ++k)
    {
        QJsonObject temp_user = user_status[k].toObject();
        if (temp_user.value("user__username").toString() == user)
            user_status.removeAt(k);
    }

    QJsonObject first_user = user_status.at(0).toObject();
    QJsonObject second_user = user_status.at(1).toObject();

        // update first
    QString username = first_user.value("user__username").toString();
    QString score = QString::number(first_user.value("score").toInt());
    if (this->first_user->text() != username + "\n" + score)
        this->first_user->setText(username + "\n" + score);

        // update second
    username = second_user.value("user__username").toString();
    score = QString::number(second_user.value("score").toInt());
    if (this->second_user->text() != username + "\n" + score)
        this->second_user->setText(username + "\n" + score);
}

void Table::Check_Stock(QJsonObject reply)
{
        // check if clear stock
    if (this->old_stock.size() >= 2)
    {
        if (reply.value("status").toBool())
        {
                // clear stock after two second
            timer->start(2000);
        }
    }
}



