#ifndef ADRESSBOOK_H
#define ADRESSBOOK_H
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QMap>
#include <QMenuBar>
#include <QDebug>
class AdressBook : public QWidget
{
    Q_OBJECT
public:
    explicit  AdressBook(QWidget *parent = 0);

private:
    QLineEdit *nameLine;
    QTextEdit *addressText;
    QPushButton *addButton;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *editButton;
    QPushButton *deleteButton;


    QMap<QString,QString> contacts;
    QString oldName;
    QString oldAddress;

    enum Mode {Navigation, Adding,Editing};
    Mode currentMode;
    void updateInterface(Mode mode);

    QAction *loadFile;
    QAction *saveFile;




signals:


public slots:
    void addContact();
    void submitContact();
    void cancelContact();
    void nextContact();
    void previousContact();
    void editContact();
    void deleteContact();
    //MenuBar
    void load();
    void save();

};
#endif // ADRESSBOOK_H
