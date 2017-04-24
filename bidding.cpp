#include "bidding.h"
#include "game.h"


Bidding::Bidding(Game* game): QDialog(game)
{
    this->setWindowTitle("Bidding");
    this->network = Http_Manager::Get_Http_Manager();
    this->game = game;

    this->Create_Window();
}

void Bidding::Create_Window()
{
    QHBoxLayout* layout = new QHBoxLayout;

        // current user
    QLabel* label = new QLabel("Now bidding: ");
    label->setAlignment(Qt::AlignCenter);
    this->current_user = new QLabel;
    this->current_user->setAlignment(Qt::AlignCenter);

    QVBoxLayout* current_layout = new QVBoxLayout;
    current_layout->addWidget(label);
    current_layout->addWidget(this->current_user);
    current_layout->addSpacerItem(new QSpacerItem(1, 1,
        QSizePolicy::Minimum, QSizePolicy::Expanding));

        // form layout
    this->finish = new QPushButton("Finish");
    this->finish->setHidden(true);
    QObject::connect(this->finish, SIGNAL(clicked(bool)),
                     this, SLOT(Finish()));

    this->boom = new QPushButton("Boom");
    this->boom->setHidden(true);
    QObject::connect(this->boom, SIGNAL(clicked(bool)),
                     this, SLOT(Boom()));

    this->pass = new QPushButton("Pass");
    QObject::connect(this->pass, SIGNAL(clicked(bool)),
                     this, SLOT(Pass()));

    this->bid = new QPushButton("Bid");
    QObject::connect(this->bid, SIGNAL(clicked(bool)),
                     this, SLOT(Bid()));

    this->points = new QLineEdit;
    this->points->setPlaceholderText("Points");

    QVBoxLayout* button_layout = new QVBoxLayout;
    button_layout->addLayout(current_layout);
    button_layout->addWidget(this->finish);
    button_layout->addWidget(this->boom);
    button_layout->addWidget(this->pass);
    button_layout->addWidget(this->bid);
    button_layout->addWidget(this->points);
    layout->addLayout(button_layout);

        // users list
    this->biddings = new QListWidget;
    layout->addWidget(this->biddings);

        // window
    this->setLayout(layout);
}

void Bidding::Add_Bidding_Widget(QString points, QString user)
{
        // widget
    Style widget_style("QLabel");
    widget_style.Add_Value("font-size", "20px");

    QWidget* widget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* label_points = new QLabel(points);
    label_points->setStyleSheet(widget_style.Get_Style());
    layout->addWidget(label_points);

    QLabel* label_user = new QLabel(user);
    label_user->setStyleSheet(widget_style.Get_Style());
    layout->addWidget(label_user);

    widget->setLayout(layout);

        // adding to list widgets
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(widget->sizeHint());
    this->biddings->addItem(item);
    this->biddings->setItemWidget(item, widget);
}

void Bidding::Manage_Butttons(QJsonObject reply)
{
        // win bidding user not exist
    if (!reply.contains("win_bidding"))
    {
        this->pass->setHidden(false);
        this->bid->setHidden(false);
        this->points->setDisabled(false);

        this->finish->setHidden(true);
        this->boom->setHidden(true);

        return;
    }

        // get data
    QString user = this->game->user->Get_Username();
    QString win_user = reply.value("win_bidding").toObject()
            .value("user").toString();

        // for winner bidding user
    if (user == win_user)
    {
        this->finish->setHidden(false);
        this->boom->setHidden(false);
        this->pass->setHidden(true);
    }
    else // for other
    {
        this->pass->setHidden(true);
        this->bid->setHidden(true);
        this->points->setDisabled(true);
    }
}

void Bidding::Pass()
{
        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/bidding/");
    data.Add_Variable("__add__", "");
    data.Add_Variable("points", "0");

        // send post
    QJsonObject reply = this->network->Send(data);
    this->Show_Message(reply);
}

void Bidding::Bid()
{
    if (this->points->text().isEmpty())
        return;

        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/bidding/");
    data.Add_Variable("__add__", "");
    data.Add_Variable("points", this->points->text());

        // send post
    QJsonObject reply = this->network->Send(data);
    this->Show_Message(reply);
}

void Bidding::Finish()
{
        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/bidding/");
    data.Add_Variable("__finish__", "");

        // send post
    QJsonObject reply = this->network->Send(data);
    this->Show_Message(reply);
}

void Bidding::Boom()
{
        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("game/bidding/");
    data.Add_Variable("__boom__", "");

        // send post
    QJsonObject reply = this->network->Send(data);
    if (!reply.value("status").toBool())
    {
        QMessageBox box(this);
        box.setText("You used yours booms or not "
                    "exceed the limit 500 points. "
                    "Check score board.");

        box.exec();
    }
}

void Bidding::Load_Biddings(QJsonObject reply)
{
    QJsonArray biddings = reply.value("biddings").toArray();

        // display in message list
    if (old_biddings == biddings)
        return;

    this->biddings->clear();
    this->old_biddings = biddings;
    this->current_user->setText(reply.value("current_user").toString());

    for (int k = 0; k < biddings.size(); ++k)
    {
        QJsonObject bidding_object = biddings.at(k).toObject();
        int points = bidding_object.value("points").toInt();
        QString user = bidding_object.value("user").toString();

        this->Add_Bidding_Widget(QString::number(points), user);
    }

    this->biddings->scrollToBottom();
    this->Manage_Butttons(reply);
}

void Bidding::Show_Message(QJsonObject reply)
{
    QMessageBox box(this);
    switch (reply.value("status").toInt())
    {
        case 1:
            box.setText("It's not your turn");
            break;

        case 2:
            box.setText("Number of points is too small");
            break;

        case 3:
            box.setText("It's not number");
            break;

        case 4:
            box.setText("Number of points is too big");
            break;

        case 5:
            box.setText("Number must be round to tens");
            break;

        default:
            return;
    }

    box.exec();
}

