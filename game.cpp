#include "game.h"


Init_Game::Init_Game(QWidget *parent): QDialog(parent)
{
    this->setWindowModality(Qt::WindowModal);
    this->network = Http_Manager::Get_Http_Manager();

    this->Create_Window();
}

void Init_Game::Create_Window()
{
    QHBoxLayout* layout = new QHBoxLayout;

        // form
    this->user_1 = new QLineEdit;
    this->user_2 = new QLineEdit;

    QPushButton* start = new QPushButton("Start");
    QObject::connect(start, SIGNAL(clicked(bool)),
                     this, SLOT(Start_Game_SLOT()));

        // form layout
    QVBoxLayout* form_layout = new QVBoxLayout;
    form_layout->addWidget(this->user_1);
    form_layout->addWidget(this->user_2);
    form_layout->addWidget(start);
    layout->addLayout(form_layout);

        // users list
    this->users = new QListWidget;
    this->Load_Online_Users();
    layout->addWidget(this->users);

        // window
    this->setLayout(layout);
}

void Init_Game::Load_Online_Users()
{
        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("user/online/");

        // send post
    QJsonObject reply = this->network->Send(data);
    QJsonArray users = reply.value("users").toArray();

    this->users->clear();
    for (int k = 0; k < users.size(); ++k)
    {
        QJsonObject user_object = users.at(k).toObject();
        QString username = user_object.value("username").toString();

        this->Add_User_Widget(username);
    }
}

void Init_Game::Add_User_Widget(QString username)
{
        // widget
    Style widget_style("QLabel");
    widget_style.Add_Value("font-size", "20px");

    QLabel* widget = new QLabel(username);
    widget->setStyleSheet(widget_style.Get_Style());

        // adding to list widgets
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(widget->sizeHint());
    this->users->addItem(item);
    this->users->setItemWidget(item, widget);
}

void Init_Game::Start_Game_SLOT()
{
        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/create/");
    data.Add_Variable("user_1", this->user_1->text());
    data.Add_Variable("user_2", this->user_2->text());

        // send post
    QJsonObject reply = this->network->Send(data);
    bool status = reply.value("status").toBool();

    if (status)
        this->close();

    else
    {
        QMessageBox error;
        error.setText("The usernames of users is incorect.");
        error.exec();
    }
}



Game::Game(QWidget* parent): QMainWindow(parent)
{
    this->network = Http_Manager::Get_Http_Manager();
    this->user = new User(this);
    this->user->show();

    this->Create_Window();
}

void Game::Create_Window()
{
    QWidget* game = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;

    this->table = new Table;
    this->chat = new Chat;

        // buttons
    Style button_style("QPushButton");
    button_style.Add_Value("font-size", "20px");

    QPushButton* logout = new QPushButton("Logout");
    logout->setMinimumHeight(70);
    logout->setStyleSheet(button_style.Get_Style());
    QObject::connect(logout, SIGNAL(clicked(bool)),
                     this, SLOT(Logout_SLOT()));

    QPushButton* create_game = new QPushButton("Create\ngame");
    create_game->setMinimumHeight(70);
    create_game->setStyleSheet(button_style.Get_Style());
    QObject::connect(create_game, SIGNAL(clicked(bool)),
                     this, SLOT(Create_Game_SLOT()));

        // button layout
    QHBoxLayout* button_layout = new QHBoxLayout;
    button_layout->setMargin(0);
    button_layout->addWidget(logout);
    button_layout->addWidget(create_game);

        // chat layout
    QVBoxLayout* chat_layout = new QVBoxLayout;
    chat_layout->setMargin(0);
    chat_layout->addLayout(button_layout);
    chat_layout->addWidget(this->chat);

        // game layout
    layout->addWidget(this->table);
    layout->addLayout(chat_layout);

    game->setLayout(layout);
    this->setCentralWidget(game);
}

void Game::Logout_SLOT()
{
    qApp->exit();
}

void Game::Create_Game_SLOT()
{
    Init_Game* dialog = new Init_Game(this);
    dialog->show();
}

void Game::Check_Status_Game()
{
        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/status/");

        // send post
    QJsonObject reply = this->network->Send(data);
}


