#include "game.h"


Game::Game(QWidget* parent): QMainWindow(parent)
{
    this->network = Http_Manager::Get_Http_Manager();
    this->init_game = new Init_Game(this);
    this->user = new User(this);
    this->user->show();

    this->Create_Window();
}

void Game::Create_Window()
{
    QWidget* game = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;

    this->table = new Table(this);
    this->chat = new Chat(this);
    this->bidding = new Bidding(this);

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

        // timer
    QTimer* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()),
        this, SLOT(Get_Status_Game()));

    timer->start(1000);
}

void Game::Launch_Bidding(QJsonObject reply)
{
        // users don't bidding
    if (!reply.value("is_bidding").toBool())
    {
        if (this->bidding->isVisible())
            this->bidding->close();

        return;
    }

        // bidding not shown
    if (!this->bidding->isVisible())
        bidding->show();

    this->bidding->Load_Biddings(reply);
}

void Game::Winner_Bidding(QJsonObject reply)
{
    if (reply.contains("win_bidding"))
    {
        QJsonObject win_bidding = reply.value("win_bidding").toObject();
        QString username = win_bidding.value("user").toString();
        int points = win_bidding.value("points").toInt();

        if (this->user->Get_Username() == username)
        {
            Dealing_Stock* dealing = new Dealing_Stock(this);
            dealing->exec();
        }
    }
}

void Game::Logout_SLOT()
{
    this->user->Sign_Out();
    qApp->exit();
}

void Game::Create_Game_SLOT()
{
    this->init_game->exec();
}

void Game::Get_Status_Game()
{
    Post_Data data = this->network->Create_Data();
    data.Set_Path("status/");

        // send post
    QJsonObject reply = this->network->Send(data);
    this->chat->Load_All_Messages(reply);
    this->table->Load_Cards(reply);
    this->table->Load_Stock(reply);
    this->table->Update_Users(reply);
    this->Launch_Bidding(reply);
    this->Winner_Bidding(reply);
}

void Game::closeEvent(QCloseEvent* event)
{
    this->user->Sign_Out();
    QMainWindow::closeEvent(event);
}
