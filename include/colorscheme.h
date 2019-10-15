#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include <QColor>
#include <QBrush>


class ColorScheme
{
private:
    void initDark();
    void initLight();
public:
    ColorScheme();
    void init();
    QBrush base;
    QBrush alternateBase;
    QBrush sameKey;
    QBrush alternateSameKey;
    QBrush relatedKey;
    QBrush alternateRelatedKey;
    QBrush boostKey;
    QBrush alternateBoostKey;
    QBrush boostKey2;
    QBrush alternateBoostKey2;
};



#endif // COLORSCHEME_H
