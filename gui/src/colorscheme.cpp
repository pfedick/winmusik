#include "colorscheme.h"
#include <QApplication>
#include <QPalette>

ColorScheme::ColorScheme()
{
    init();
}

void ColorScheme::init()
{
    base=QApplication::palette().base();
    alternateBase=QApplication::palette().alternateBase();

    sameKey=base;
    alternateSameKey=alternateBase;
    relatedKey=base;
    alternateRelatedKey=alternateBase;
    boostKey=base;
    alternateBoostKey=alternateBase;
    boostKey2=base;
    alternateBoostKey2=alternateBase;

    sameKey.setStyle(Qt::SolidPattern);
    sameKey.setStyle(Qt::SolidPattern);
    relatedKey.setStyle(Qt::SolidPattern);
    alternateRelatedKey.setStyle(Qt::SolidPattern);
    boostKey.setStyle(Qt::SolidPattern);
    alternateBoostKey.setStyle(Qt::SolidPattern);
    boostKey2.setStyle(Qt::SolidPattern);
    alternateBoostKey2.setStyle(Qt::SolidPattern);


    if (base.color().lightness()<127) {
        initDark();
    } else {
        initLight();
    }
}


void ColorScheme::initDark()
{
    QColor bgc=base.color();
    QColor bgca=alternateBase.color();

    sameKey.setColor(QColor(bgc.lightness(),127,bgc.lightness()));
    alternateSameKey.setColor(QColor(bgca.lightness(),127,bgca.lightness()));
    relatedKey.setColor(QColor(bgc.lightness(),64,bgc.lightness()));
    alternateRelatedKey.setColor(QColor(bgca.lightness(),64,bgca.lightness()));
    boostKey.setColor(QColor(128,bgc.lightness(),bgc.lightness()));
    alternateBoostKey.setColor(QColor(128,bgca.lightness(),bgca.lightness()));
    boostKey2.setColor(QColor(200,bgc.lightness(),bgc.lightness()));
    alternateBoostKey2.setColor(QColor(200,bgca.lightness(),bgca.lightness()));
}

void ColorScheme::initLight()
{
    sameKey.setColor(QColor(192,230,192));
    alternateSameKey.setColor(QColor(192,230,192));
    relatedKey.setColor(QColor(192,255,192));
    alternateRelatedKey.setColor(QColor(192,255,192));
    boostKey.setColor(QColor(220,192,192));
    alternateBoostKey.setColor(QColor(220,192,192));
    boostKey2.setColor(QColor(255,192,192));
    alternateBoostKey2.setColor(QColor(255,192,192));

}
