#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    explicit BackEnd(QObject *parent = nullptr);

    QString userName();
    void setUserName(const QString &userName);

    Q_INVOKABLE
    void loadGeoGridFromFile(const QString &file, QObject* map);

signals:
    void userNameChanged();

private:
    QString m_userName;
    QObject *_map = nullptr;

    /*
     * Places the markers on the map, according to the current checked boxes.
     * @Pre: _map is initialized correctly and holds the map we want to place markers on
     */
    void placeMarkers();
};

#endif // BACKEND_H
