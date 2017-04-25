#include "game.h"


Game::Game(QWidget* parent): QMainWindow(parent)
{
    this->setWindowTitle("Thousand");
    this->network = Http_Manager::Get_Http_Manager();
    this->init_game = new Init_Game(this);
    this->user = new User(this);
    this->scores = new Score_Board;
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

    Style create_style("QPushButton");
    create_style.Add_Value("font-size", "20px");
    create_style.Add_Value("background-color", "darkgreen");

    QPushButton* create_game = new QPushButton("Create\ngame");
    create_game->setMinimumHeight(70);
    create_game->setStyleSheet(create_style.Get_Style());
    QObject::connect(create_game, SIGNAL(clicked(bool)),
                     this, SLOT(Create_Game_SLOT()));

    QPushButton* score_board = new QPushButton("Score\nboard");
    score_board->setMinimumHeight(70);
    score_board->setStyleSheet(button_style.Get_Style());
    QObject::connect(score_board, SIGNAL(clicked(bool)),
                     this, SLOT(Score_Board_SLOT()));

    QPushButton* leave_game = new QPushButton("Leave the\ngame");
    leave_game->setMinimumHeight(70);
    leave_game->setStyleSheet(button_style.Get_Style());
    QObject::connect(leave_game, SIGNAL(clicked(bool)),
                     this, SLOT(Leave_Game_SLOT()));

        // button layout
    QGridLayout* button_layout = new QGridLayout;
    button_layout->setMargin(0);
    button_layout->addWidget(logout, 0, 0);
    button_layout->addWidget(create_game, 0, 1);
    button_layout->addWidget(score_board, 1, 0);
    button_layout->addWidget(leave_game, 1, 1);

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
    if (reply.value("is_dealing").toBool())
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

void Game::Check_Marriage(QJsonObject reply)
{
        // not contain
    if (!reply.contains("marriage_color"))
        return;

    QString color = reply.value("marriage_color").toString();

        // color not change
    if (this->marriage == color)
        return;

    this->marriage = color;

        // show message
    QMessageBox message(this);
    message.setText("Marriage: " + color);
    message.exec();
}

void Game::Game_Over(QJsonObject reply)
{
    bool game_over = reply.value("game_over").toBool();
    if (game_over)
    {
        QString winner = reply.value("winner").toString();

        QMessageBox message(this);
        message.setText("Game over! Winner: " + winner);
        message.exec();

        this->Leave_Game_SLOT();
    }
}

void Game::Logout_SLOT()
{
    this->user->Sign_Out();
    qApp->exit();
}

void Game::Create_Game_SLOT()
{
    this->init_game->Load_Online_Users();
    this->init_game->exec();
}

void Game::Score_Board_SLOT()
{
    this->scores->show();
}

void Game::Leave_Game_SLOT()
{
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/finish/");
    this->network->Send(data);
}

void Game::Get_Status_Game()
{
    Post_Data data = this->network->Create_Data();
    data.Set_Path("status/");

        // send post
    QJsonObject reply = this->network->Send(data);
    this->Check_Marriage(reply);
    this->chat->Load_All_Messages(reply);
    this->table->Load_Cards(reply);
    this->table->Load_Stock(reply);
    this->table->Update_Users(reply);
    this->scores->Load_Scores(reply);
    this->Launch_Bidding(reply);
    this->Winner_Bidding(reply);
    this->Game_Over(reply);
}

void Game::closeEvent(QCloseEvent* event)
{
    this->user->Sign_Out();
    QMainWindow::closeEvent(event);
}
