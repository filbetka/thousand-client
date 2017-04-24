#include "dealing_stock.h"
#include "game.h"


Row_Card::Row_Card(QString color, QString rank)
{
    this->color = color;
    this->rank = rank;
    this->Create();
}

void Row_Card::Create()
{
    QLabel* label_color = new QLabel(color);
    QLabel* label_rank = new QLabel(rank);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(label_color);
    layout->addWidget(label_rank);
    this->setLayout(layout);
}



Dealing_Stock::Dealing_Stock(Game* game)
{
    this->setWindowTitle("Dealing cards");
    this->network = Http_Manager::Get_Http_Manager();
    this->game = game;
    this->Create_Window();
}

void Dealing_Stock::Create_Window()
{
    QVBoxLayout* layout = new QVBoxLayout;

        // title
    Style title_style("QLabel");
    title_style.Add_Value("font-size", "20px");
    title_style.Add_Value("margin", "10px");
    title_style.Add_Value("margin-bottom", "20px");

    QLabel* title = new QLabel;
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(title_style.Get_Style());
    title->setText("Select your or stock cards. "
                   "Selected card will give first player.");

        // labels
    QHBoxLayout* label_layout = new QHBoxLayout;
    QStringList players = this->Get_Other_Players();

    QLabel* label_first_player = new QLabel;
    label_first_player->setAlignment(Qt::AlignCenter);
    label_first_player->setText("Player: " + players.at(0));
    label_layout->addWidget(label_first_player);

    QLabel* label_second_player = new QLabel;
    label_second_player->setAlignment(Qt::AlignCenter);
    label_second_player->setText("Player: " + players.at(1));
    label_layout->addWidget(label_second_player);

        // lists cards for users
    QHBoxLayout* players_layout = new QHBoxLayout;
    this->first_player_card = new QListWidget;
    this->second_player_card = new QListWidget;
    players_layout->addWidget(this->first_player_card);
    players_layout->addWidget(this->second_player_card);

        // buttons accepts
    QHBoxLayout* buttons_layout = new QHBoxLayout;
    QPushButton* accept = new QPushButton("Accept");
    QObject::connect(accept, SIGNAL(clicked(bool)),
                     this, SLOT(Accept()));

    buttons_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    buttons_layout->addWidget(accept);

        // layouts
    layout->addWidget(title);
    layout->addLayout(label_layout);
    layout->addLayout(players_layout);
    layout->addLayout(buttons_layout);
    this->setLayout(layout);
    this->Load_Cards();
}

void Dealing_Stock::Load_Cards()
{
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/bidding/");
    data.Add_Variable("__win__", "");

        // send post
    QJsonObject reply = this->network->Send(data);
    QJsonArray cards = reply.value("cards").toArray();

        // adding new cards
    for (int k = 0; k < cards.size(); ++k)
    {
        QJsonObject card_object = cards.at(k).toObject();
        QString color = card_object.value("color__name").toString();
        QString rank = card_object.value("rank__name").toString();

        this->Add_Line_Widget(color, rank);
    }
}

void Dealing_Stock::Add_Line_Widget(QString color, QString rank)
{
        // adding to left list widgets
    Row_Card* row_left = new Row_Card(color, rank);
    QListWidgetItem* item_left = new QListWidgetItem;
    item_left->setSizeHint(row_left->sizeHint());

    this->first_player_card->addItem(item_left);
    this->first_player_card->setItemWidget(item_left, row_left);

        // adding to right list widgets
    Row_Card* row_right = new Row_Card(color, rank);
    QListWidgetItem* item_rigth = new QListWidgetItem;
    item_rigth->setSizeHint(row_right->sizeHint());

    this->second_player_card->addItem(item_rigth);
    this->second_player_card->setItemWidget(item_rigth, row_right);
}

bool Dealing_Stock::Validate_Selected()
{
        // if rows is not selected
    if (this->first_player_card->selectedItems().size() == 0)
        return false;

    if (this->second_player_card->selectedItems().size() == 0)
        return false;

        // get items
    QListWidgetItem* first_item = this->first_player_card->selectedItems().at(0);
    QListWidgetItem* second_item = this->second_player_card->selectedItems().at(0);

    Row_Card* first_row = qobject_cast<Row_Card*>(this->first_player_card->itemWidget(first_item));
    Row_Card* second_row = qobject_cast<Row_Card*>(this->second_player_card->itemWidget(second_item));

        // check cards are the same
    if (first_row->color == second_row->color)
        if (first_row->rank == second_row->rank)
            return false;

    return true;
}

QString Dealing_Stock::Get_Card_Information(QListWidget* list)
{
    QListWidgetItem* item = list->selectedItems().at(0);
    Row_Card* row = qobject_cast<Row_Card*>(list->itemWidget(item));
    return row->color + " " + row->rank;
}

QStringList Dealing_Stock::Get_Other_Players()
{
    QString user = this->game->user->Get_Username();
    QStringList players = this->game->
            init_game->Get_Players();

        // remove logged user
    for (int k = 0; k < 3; ++k)
    {
        if (players[k] == user)
        {
            players.removeAt(k);
            break;
        }
    }

    return players;
}

void Dealing_Stock::Accept()
{
        // validate
    if (!this->Validate_Selected())
        return;

    QStringList players = this->Get_Other_Players();

        // post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/bidding/");
    data.Add_Variable("__dealing__", "");
    data.Add_Variable("first_user", players.at(0));
    data.Add_Variable("second_user", players.at(1));

    data.Add_Variable("first_card",
        this->Get_Card_Information(this->first_player_card));

    data.Add_Variable("second_card",
        this->Get_Card_Information(this->second_player_card));

        // send post
    QJsonObject reply = this->network->Send(data);
    if (reply.value("status").toBool())
        this->close();
}




