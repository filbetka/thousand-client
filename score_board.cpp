#include "score_board.h"

Score_Board::Score_Board()
{
    this->setWindowTitle("Score board");
    this->Create_Window();
}

void Score_Board::Create_Window()
{
    QVBoxLayout* layout = new QVBoxLayout;

        // title
    this->user_1 = new QLabel;
    this->user_2 = new QLabel;
    this->user_3 = new QLabel;

    QHBoxLayout* title_layout = new QHBoxLayout;
    title_layout->addWidget(this->user_1);
    title_layout->addWidget(this->user_2);
    title_layout->addWidget(this->user_3);
    layout->addLayout(title_layout);

        // score list
    this->scores_list = new QListWidget;
    layout->addWidget(this->scores_list);

    this->setLayout(layout);
}

void Score_Board::Update_Title(QJsonArray scores)
{
    QString user_1 = scores.at(0).toObject().value("scores__user__username").toString();
    QString user_2 = scores.at(1).toObject().value("scores__user__username").toString();
    QString user_3 = scores.at(2).toObject().value("scores__user__username").toString();

    this->user_1->setText(user_1);
    this->user_2->setText(user_2);
    this->user_3->setText(user_3);
}

void Score_Board::Add_Line_Widget(int score[],
                    int bidder_user, bool is_boom)
{
        // create widget
    QWidget* widget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* labels[3];
    labels[0] = new QLabel(QString::number(score[0]));
    labels[1] = new QLabel(QString::number(score[1]));
    labels[2] = new QLabel(QString::number(score[2]));
    labels[bidder_user]->setStyleSheet("color: green;");

    if (is_boom)
        labels[bidder_user]->setText("BOOM");

    layout->addWidget(labels[0]);
    layout->addWidget(labels[1]);
    layout->addWidget(labels[2]);
    widget->setLayout(layout);

        // create list item
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(widget->sizeHint());

        // adding to list
    this->scores_list->addItem(item);
    this->scores_list->setItemWidget(item, widget);
}

void Score_Board::Load_Scores(QJsonObject reply)
{
    QJsonArray scores = reply.value("scores").toArray();

        // if not changes
    if (this->old_scores == scores)
        return;

    this->old_scores = scores;
    this->scores_list->clear();
    this->Update_Title(scores);

    int score[3] = {0};
    int bidder_user = -1;

        // update scores board
    for (int k = 0; k < scores.size() - 3; k += 3)
    {
        for (int i = 0; i < 3; ++i)
        {
            QJsonObject obiect = scores.at(k+i).toObject();
            score[i] += obiect.value("scores__score").toInt();

            QString scores_user = obiect.value("scores__user__username").toString();
            QString bidded_user = obiect.value("bidded_user").toString();

            if (scores_user == bidded_user)
                bidder_user = i;
        }

        bool is_boom = scores.at(k).toObject().value("is_boom").toBool();
        this->Add_Line_Widget(score, bidder_user, is_boom);
    }
}
