#ifndef STYLE_H
#define STYLE_H

#include <QString>


class Style
{

    private:

        QString widget;
        QString style;

    public:

        Style(QString widget);

        void Add_Value(QString name, QString value);
        QString Get_Style();

};


#endif
