#include "card.h"


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



Card::Card(COLOR color, RANK rank):
    QPushButton(NULL)
{
    this->color = color;
    this->rank = rank;
    this->Create_Card();
}

void Card::Create_Card()
{
        // layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(0);

        // up name
    QHBoxLayout* up_layout = new QHBoxLayout;
    QLabel* up_name = new QLabel(this->Get_Rank_Name(this->rank));
    up_layout->addWidget(up_name);
    up_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));

        // bottom name
    QHBoxLayout* bottom_layout = new QHBoxLayout;
    Rotated_Text* bottom_name = new Rotated_Text(this->Get_Rank_Name(this->rank));
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
    this->Set_Style();
}

void Card::Set_Style()
{
    Style card("QPushButton");
    card.Add_Value("background-color", "white");

    Style text("QLabel");
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




