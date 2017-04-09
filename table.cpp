#include "table.h"

Table::Table()
{
    this->Create_Table();
}

void Table::Create_Table()
{
        // create layouts
    QHBoxLayout* other_player = new QHBoxLayout;
    other_player->addWidget(new Card(NONE_COLOR, NONE_RANK));
    other_player->addWidget(this->Create_User_Details("Player 1"));
    other_player->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    other_player->addWidget(this->Create_User_Details("Player 2"));
    other_player->addWidget(new Card(NONE_COLOR, NONE_RANK));

    this->table_stock = new QHBoxLayout;
    this->table_stock->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_stock->addWidget(new Card(NONE_COLOR, NONE_RANK));
    this->table_stock->addWidget(new Card(NONE_COLOR, NONE_RANK));

    QHBoxLayout* stos_layout = new QHBoxLayout;
    stos_layout->setMargin(0);
    stos_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    stos_layout->addLayout(this->table_stock);
    stos_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));

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

QLabel* Table::Create_User_Details(QString name)
{
    QLabel* details = new QLabel(name + "\nScore: 0");
    details->setObjectName("user_details");

    Style style("#user_details");
    style.Add_Value("font-size", "25px");
    style.Add_Value("color", "#DDD");

    details->setStyleSheet(style.Get_Style());
    return details;
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

        this->table_cards->addWidget(new Card(color, rank));
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
        QString color = stock_object.value("stock__color__name").toString();
        QString rank = stock_object.value("stock__rank__name").toString();

        this->table_stock->addWidget(new Card(color, rank));
    }
}

