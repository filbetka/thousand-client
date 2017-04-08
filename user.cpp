#include "user.h"


User::User(QWidget *parent): QDialog(parent)
{
    this->setWindowModality(Qt::WindowModal);

    this->network = Http_Manager::Get_Http_Manager();
    this->is_logged = false;

    this->Create_Dialog();
}

void User::Create_Dialog()
{
        // title
    Style title_style("QLabel");
    title_style.Add_Value("font-size", "25px");

    QLabel* main_title = new QLabel("Welcome!");
    main_title->setAlignment(Qt::AlignCenter);
    main_title->setStyleSheet(title_style.Get_Style());

    QLabel* after_title = new QLabel("Before start the game you have to sign in :)");
    after_title->setAlignment(Qt::AlignCenter);

        // username
    Style username_style("QLineEdit");
    username_style.Add_Value("font-size", "15px");
    username_style.Add_Value("color", "#0F0");

    this->username = new QLineEdit;
    this->username->setAlignment(Qt::AlignHCenter);
    this->username->setMinimumHeight(30);
    this->username->setMaximumHeight(30);
    this->username->setPlaceholderText("Username");
    this->username->setStyleSheet(username_style.Get_Style());

        // password
    Style password_style("QLineEdit");
    password_style.Add_Value("font-size", "15px");
    password_style.Add_Value("color", "#0F0");

    this->password = new QLineEdit;
    this->password->setAlignment(Qt::AlignHCenter);
    this->password->setMinimumHeight(30);
    this->password->setMaximumHeight(30);
    this->password->setPlaceholderText("Password");
    this->password->setStyleSheet(password_style.Get_Style());

        // buttons
    QPushButton* sign_in = new QPushButton("Sign in");
    QObject::connect(sign_in, SIGNAL(clicked(bool)), this, SLOT(Sign_In_SLOT()));

    QPushButton* sign_up = new QPushButton("Sign up");
    QObject::connect(sign_up, SIGNAL(clicked(bool)), this, SLOT(Sign_Up_SLOT()));

        // buttons layout
    QHBoxLayout* button_layout = new QHBoxLayout;
    button_layout->addWidget(sign_in);
    button_layout->addWidget(sign_up);

        // layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(main_title);
    layout->addWidget(after_title);
    layout->addWidget(this->username);
    layout->addWidget(this->password);
    layout->addLayout(button_layout);

    this->setLayout(layout);
}

void User::Sign_In_SLOT()
{
    QString username = this->username->text();
    QString password = this->password->text();

    if (this->Sign_In(username, password))
    {
        this->is_logged = true;
        this->close();
    }
    else
    {
        QMessageBox status(this);
        status.setText("Ups. Try again.");
        status.exec();
    }
}

void User::Sign_Up_SLOT()
{
    QString username = this->username->text();
    QString password = this->password->text();

    if (this->Sign_Up(username, password))
    {
        QMessageBox status(this);
        status.setText("Now, you have to sign in.");
        status.exec();
    }
    else
    {
        QMessageBox status(this);
        status.setText("Ups. Try again.");
        status.exec();
    }
}

bool User::Sign_In(QString username, QString password)
{
        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("user/sign_in/");
    data.Add_Variable("username", username);
    data.Add_Variable("password", password);

        // send post
    QJsonObject reply = this->network->Send(data);
    return reply.value("status").toBool();
}

bool User::Sign_Up(QString username, QString password)
{
        // create post
    Post_Data data = this->network->Create_Data();
    data.Set_Path("user/sign_up/");
    data.Add_Variable("username", username);
    data.Add_Variable("password", password);

        // send post
    QJsonObject reply = this->network->Send(data);
    return reply.value("status").toBool();
}

void User::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        this->Sign_In_SLOT();
        return;
    }

    QDialog::keyPressEvent(event);
}

void User::closeEvent(QCloseEvent* event)
{
        // not login
    if (!this->is_logged)
        qApp->exit();

    QDialog::closeEvent(event);
}


