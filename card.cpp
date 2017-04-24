#include "card.h"
#include "table.h"


Rotated_Text::Rotated_Text(QString text): QLabel(text)
{
}

void Rotated_Text::paintEvent(QPaintEvent* event)
{
    QStylePainter painter(this);

        // rotate at center
    painter.translate(rect().center());
    painter.rotate(180);
    painter.translate(-rect().center());

    painter.drawText(rect(), alignment(), text());
}



Card::Card(COLOR color, RANK rank, Table* table):
    QPushButton(table)
{
    this->table = table;
    this->color = color;
    this->rank = rank;
    this->Create_Card();
}

Card::Card(QString color, QString rank, Table* table):
    QPushButton(table)
{
    this->table = table;
    this->color = this->Get_Color(color);
    this->rank = this->Get_Rank(rank);
    this->Create_Card();
}

void Card::Create_Card()
{
    this->network = Http_Manager::Get_Http_Manager();

        // layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(0);

        // up name
    QHBoxLayout* up_layout = new QHBoxLayout;
    QLabel* up_name = new QLabel(this->Get_Rank_Name(this->rank));
    up_name->setObjectName("name");
    up_layout->addWidget(up_name);
    up_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));

        // bottom name
    QHBoxLayout* bottom_layout = new QHBoxLayout;
    Rotated_Text* bottom_name = new Rotated_Text(this->Get_Rank_Name(this->rank));
    bottom_name->setObjectName("name");
    bottom_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    bottom_layout->addWidget(bottom_name);

        // set layouts
    layout->addLayout(up_layout);
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout(bottom_layout);
    this->setLayout(layout);

        // style
    this->setIcon(QIcon(this->Get_Color_Path(this->color)));
    this->setIconSize(QSize(64, 64));
    this->setMinimumSize(100, 150);
    this->setMaximumSize(100, 150);
    this->setObjectName("card");
    this->Set_Style();
}

void Card::Set_Style()
{
    Style card("#card");
    card.Add_Value("background-color", "white");

    Style text("#name");
    text.Add_Value("font-size", "25px");
    text.Add_Value("font", "bold");

    if (this->color == HEART || this->color == DIAMOND)
        text.Add_Value("color", "#D80027");

    if (this->color == CLOVER || this->color == SPADES)
        text.Add_Value("color", "#000");

    if (this->color == NONE_COLOR)
        card.Add_Value("background-color", "darkgreen");

    this->setStyleSheet(card.Get_Style() +
                        text.Get_Style());
}

COLOR Card::Get_Color(QString color)
{
    if (color == "Heart")
        return HEART;

    if (color == "Diamond")
        return DIAMOND;

    if (color == "Clover")
        return CLOVER;

    if (color == "Spades")
        return SPADES;

    return NONE_COLOR;
}

RANK Card::Get_Rank(QString rank)
{
    if (rank == "Ace")
        return ACE;

    if (rank == "10")
        return _10;

    if (rank == "King")
        return KING;

    if (rank == "Queen")
        return QUEEN;

    if (rank == "Jack")
        return JACK;

    if (rank == "9")
        return _9;

    return NONE_RANK;
}

QString Card::Get_Color_Name(COLOR color)
{
    if (color == HEART)
        return "Heart";

    if (color == DIAMOND)
        return "Diamond";

    if (color == CLOVER)
        return "Clover";

    if (color == SPADES)
        return "Spades";

    return "";
}

QString Card::Get_Rank_Name(RANK rank)
{
    switch (rank)
    {
        case ACE:
            return "A";

        case _10:
            return "10";

        case KING:
            return "K";

        case QUEEN:
            return "Q";

        case JACK:
            return "J";

        case _9:
            return "9";

        case NONE_RANK:
            return "";
    }

    return "";
}

QString Card::Get_Rank_Full_Name(RANK rank)
{
    switch (rank)
    {
        case ACE:
            return "Ace";

        case _10:
            return "10";

        case KING:
            return "King";

        case QUEEN:
            return "Queen";

        case JACK:
            return "Jack";

        case _9:
            return "9";

        case NONE_RANK:
            return "";
    }

    return "";
}

QString Card::Get_Color_Path(COLOR color)
{
    switch (color)
    {
        case HEART:
            return "../thousand-client/icons/heart.png";

        case DIAMOND:
            return "../thousand-client/icons/diamond.png";

        case CLOVER:
            return "../thousand-client/icons/clover.png";

        case SPADES:
            return "../thousand-client/icons/spades.png";

        case NONE_COLOR:
            return "../thousand-client/icons/watermelon.png";
    }

    return "";
}

void Card::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (!this->table)
        return;

    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/throw_card/");
    data.Add_Variable("__throw__", "");
    data.Add_Variable("color", this->Get_Color_Name(this->color));
    data.Add_Variable("rank", this->Get_Rank_Full_Name(this->rank));

        // send post
    QJsonObject reply = this->network->Send(data);
    if (reply.value("status").toBool() == false)
    {
            // message
        QString text = "Selected card is not valid because "
                       "is different color than first card in "
                       "stock or the card have too small rank. "
                       "Check also if it's your turn.";

        QMessageBox message(this);
        message.setText(text);
        message.exec();
    }

    this->table->Check_Stock(reply);
}




