#include "style.h"

Style::Style(QString widget): widget(widget)
{
}

void Style::Add_Value(QString name, QString value)
{
    this->style += name + ":" + value + ";";
}

QString Style::Get_Style()
{
    return this->widget + "{" + this->style + "}";
}
