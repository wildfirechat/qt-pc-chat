#ifndef DEFAULTPORTRAITFACTORY_H
#define DEFAULTPORTRAITFACTORY_H

class QPixmap;
class QString;
class DefaultPortraitFactory
{
public:
    DefaultPortraitFactory();
    static QPixmap defaultPortraitOfUser(const QString &displayName, int width = 40);
};

#endif // DEFAULTPORTRAITFACTORY_H
