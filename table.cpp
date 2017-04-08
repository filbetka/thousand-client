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

    QHBoxLayout* stos = new QHBoxLayout;
    stos->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    stos->addWidget(new Card(HEART, ACE));
    stos->addWidget(new Card(SPADES, ACE));
    stos->addWidget(new Card(CLOVER, ACE));
    stos->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));

    QHBoxLayout* cards = new QHBoxLayout;
    cards->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    cards->addWidget(new Card(NONE_COLOR, NONE_RANK));
    cards->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));

        // adding layouts
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(other_player);
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout(stos);
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout(cards);

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
