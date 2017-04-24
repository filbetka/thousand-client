#ifndef SCORE_BOARD_H
#define SCORE_BOARD_H

#include <QtWidgets>
#include "style.h"
#include "http.h"


class Score_Board: public QWidget
{
    Q_OBJECT

    private:

        QListWidget* scores_list;
        QJsonArray old_scores;
        QLabel* user_1;
        QLabel* user_2;
        QLabel* user_3;

        void Create_Window();
        void Update_Title(QJsonArray scores);
        void Add_Line_Widget(int score[],
                int bidder_user, bool is_boom);

    public:

        Score_Board();

        void Load_Scores(QJsonObject reply);

};


#endif
