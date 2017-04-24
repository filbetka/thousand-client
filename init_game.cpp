#include "init_game.h"


Init_Game::Init_Game(QWidget *parent): QDialog(parent)
{
    this->setWindowTitle("Create game");
    this->setWindowModality(Qt::WindowModal);
    this->network = Http_Manager::Get_Http_Manager();

    this->Create_Window();
}

void Init_Game::Create_Window()
{
    QVBoxLayout* layout = new QVBoxLayout;

        // labels
    QHBoxLayout* label_layout = new QHBoxLayout;
    QLabel* label_first_player = new QLabel;
    label_first_player->setAlignment(Qt::AlignCenter);
    label_first_player->setText("First player");
    label_layout->addWidget(label_first_player);

    QLabel* label_second_player = new QLabel;
    label_second_player->setAlignment(Qt::AlignCenter);
    label_second_player->setText("Second player");
    label_layout->addWidget(label_second_player);

        // lists cards for users
    QHBoxLayout* players_layout = new QHBoxLayout;
    this->first_player = new QListWidget;
    this->second_player = new QListWidget;
    players_layout->addWidget(this->first_player);
    players_layout->addWidget(this->second_player);

        // button
    QHBoxLayout* buttons_layout = new QHBoxLayout;
    QPushButton* start = new QPushButton("Start");
    QObject::connect(start, SIGNAL(clicked(bool)),
                     this, SLOT(Start_Game_SLOT()));

    buttons_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    buttons_layout->addWidget(start);

        // layouts
    layout->addLayout(label_layout);
    layout->addLayout(players_layout);
    layout->addLayout(buttons_layout);
    this->setLayout(layout);
}

void Init_Game::Add_User_Widget(QString username)
{
        // widget
    Style widget_style("QLabel");
    widget_style.Add_Value("font-size", "20px");

    QLabel* widget_first = new QLabel(username);
    widget_first->setStyleSheet(widget_style.Get_Style());

        // adding to list widgets
    QListWidgetItem* item_first = new QListWidgetItem;
    item_first->setSizeHint(widget_first->sizeHint());
    this->first_player->addItem(item_first);
    this->first_player->setItemWidget(item_first, widget_first);


        // widget
    QLabel* widget_second = new QLabel(username);
    widget_second->setStyleSheet(widget_style.Get_Style());

        // adding to list widgets
    QListWidgetItem* item_second = new QListWidgetItem;
    item_second->setSizeHint(widget_second->sizeHint());
    this->second_player->addItem(item_second);
    this->second_player->setItemWidget(item_second, widget_second);
}

QString Init_Game::Get_Selected_User(QListWidget* list)
{
    QListWidgetItem* item = list->selectedItems().at(0);
    QLabel* label = qobject_cast<QLabel*>(list->itemWidget(item));
    return label->text();
}

bool Init_Game::Validate_Selected()
{
        // if rows is not selected
    if (this->first_player->selectedItems().size() == 0)
        return false;

    if (this->second_player->selectedItems().size() == 0)
        return false;

    return true;
}

void Init_Game::Start_Game_SLOT()
{
        // validate
    if (!this->Validate_Selected())
        return;

        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/create/");
    data.Add_Variable("user_1", this->Get_Selected_User(this->first_player));
    data.Add_Variable("user_2", this->Get_Selected_User(this->second_player));

        // send post
    QJsonObject reply = this->network->Send(data);
    bool status = reply.value("status").toBool();

    if (status)
        this->close();

    else
    {
        QMessageBox error;
        error.setText("The usernames of users are incorect.");
        error.exec();
    }
}

QStringList Init_Game::Get_Players()
{
    if (this->players.isEmpty())
    {
            // create post
        Post_Data data = this->network->Create_Data();
        data.Set_Path("game/game_users/");

            // send post
        QJsonObject reply = this->network->Send(data);
        QJsonArray users = reply.value("users").toArray();

        for (int k = 0; k < 3; ++k)
        {
            QString user = users.at(k).toObject()
                    .value("username").toString();

            this->players.append(user);
        }
    }

    return this->players;
}

void Init_Game::Load_Online_Users()
{
        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("user/online/");

        // send post
    QJsonObject reply = this->network->Send(data);
    QJsonArray users = reply.value("users").toArray();

    this->first_player->clear();
    this->second_player->clear();

    for (int k = 0; k < users.size(); ++k)
    {
        QJsonObject user_object = users.at(k).toObject();
        QString username = user_object.value("username").toString();

        this->Add_User_Widget(username);
    }
}

