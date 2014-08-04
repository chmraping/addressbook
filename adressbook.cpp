#include "adressbook.h"
//修饰符  类名::方法(参数)[:参数构造表]
//::这是直接访问类变量或者函数的方式 .
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QIODevice>
#include <QDataStream>
#include <QFile>
AdressBook::AdressBook(QWidget *parent) :
    QWidget(parent)
{

  QLabel *nameLabel = new QLabel(tr("Name:"));
  nameLine = new QLineEdit ;
  QLabel *addressLabel = new QLabel(tr("Adress:"));
  addressText = new QTextEdit;
  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->addWidget(nameLabel, 0, 0,Qt::AlignTop);
  mainLayout->addWidget(nameLine, 0, 1);


  mainLayout->addWidget(addressLabel, 1, 0,Qt::AlignTop);
  mainLayout->addWidget(addressText, 1, 1);

  this->setLayout(mainLayout);
  this->setWindowTitle(tr("Addrss Book"));


  QVBoxLayout *buttonLayout = new QVBoxLayout;
  QHBoxLayout *pageLayout = new QHBoxLayout;

  addButton = new QPushButton(tr("Add"));
  submitButton = new QPushButton(tr("Submit"));
  cancelButton = new QPushButton(tr("Cancel"));

  editButton = new QPushButton(tr("Edit"));
  deleteButton = new QPushButton(tr("Delete"));

  previousButton = new QPushButton(tr("previous"));
  nextButton = new QPushButton(tr("next"));




  pageLayout->addWidget(previousButton);
  pageLayout->addWidget(nextButton);

  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(submitButton);
  buttonLayout->addStretch();
  buttonLayout->addWidget(cancelButton);
  buttonLayout->addWidget(editButton);
  buttonLayout->addWidget(deleteButton);

  mainLayout->addLayout(buttonLayout,1,2);
  mainLayout->addLayout(pageLayout,3,1);


  //-------------add QMenuBar-------------------------------
  loadFile = new QAction(tr("&load"),this);
  saveFile = new QAction(tr("&save"),this);
  QAction *xx = new QAction(tr("xx"),this);
  loadFile->setToolTip(tr("load a contact File!"));
  QMenuBar *fileMenuBar = new QMenuBar();
  QMenu *fileMenu= fileMenuBar->addMenu(tr("&File"));
  QMenu *fileMenu2 = fileMenuBar->addMenu(tr("&B"));
  fileMenu->addAction(loadFile);
  fileMenu->addAction(saveFile);
//  fileMenu->addAction(xx);
  fileMenu2->addAction(xx);
  mainLayout->setMenuBar(fileMenuBar);





  updateInterface(Navigation);

  connect(addButton, SIGNAL(clicked()), this, SLOT(addContact()));
  connect(submitButton, SIGNAL(clicked()), this, SLOT(submitContact()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelContact()));
  connect(nextButton, SIGNAL(clicked()), this, SLOT(nextContact()));
  connect(previousButton, SIGNAL(clicked()), this, SLOT(previousContact()));
  connect(editButton, SIGNAL(clicked()), this, SLOT(editContact()));
  connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteContact()));
  //menubar
  connect(loadFile, SIGNAL(triggered()), this, SLOT(load()));
  connect(saveFile, SIGNAL(triggered()), this, SLOT(save()));
}
void AdressBook::addContact()
{
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();
    updateInterface(Adding);

}
void AdressBook::submitContact()
{

    QString name = nameLine->text();
    QString address = addressText->toPlainText();
    if(currentMode == Adding){
        if(""==name||""==address){
            QMessageBox::information(this,tr("empty"),tr("please fulfill the blanks!"));
            return;
        }
        if(contacts.contains(name)){
            QMessageBox::information(this,tr("exists!"),tr("%1 is already exists").arg(name));
            return;
        }
        contacts.insert(name,address);
        QMessageBox::information(this,tr("secess"),tr("%1 is insert").arg(name));
    }else if(currentMode==Editing){
        if(contacts.contains(oldName)){
            contacts.remove(oldName);
            contacts.insert(name,address);
            QMessageBox::information(this,tr("Editing"),tr("%1 is edit to  %2,address %3 update").arg(oldName,name,address));
        }
    }


    updateInterface(Navigation);



}
void AdressBook::cancelContact()
{
    if(contacts.empty()){
        nameLine->clear();
        addressText->clear();
    }else{
        nameLine->setText(oldName);
        addressText->setText(oldAddress);


    }
    updateInterface(Navigation);
}

void AdressBook::nextContact()
{
   QString name=nameLine->text();

   QMap<QString,QString>::iterator i =contacts.find(name);
   if(i!=contacts.end()){
       i++;

   }
   if(i==contacts.end()){
       i=contacts.begin();
   }
   nameLine->setText(i.key());
   addressText->setText(i.value());
}

void AdressBook::previousContact(){
    QString name=nameLine->text();

    QMap<QString,QString>::iterator i =contacts.find(name);
//    if(i!=contacts.begin()){
//        i--;

//    }else{
//        i=contacts.end()-1;
//    }
    if(i==contacts.begin()){
        i=contacts.end();
    }
    i--;
    nameLine->setText(i.key());
    addressText->setText(i.value());
}

void AdressBook::editContact()
{
    oldName=nameLine->text();
    oldAddress=addressText->toPlainText();
    updateInterface(Editing);

}
void AdressBook::deleteContact()
{
    QString name = nameLine->text();
    if(contacts.contains(name)){
        int cho = QMessageBox::question(this, tr("Confirm delete?"), tr("Really wanna remove %1?").arg(name), QMessageBox::Yes|QMessageBox::No);
        if(cho==QMessageBox::Yes){
            nextContact();
            contacts.remove(name);
            QMessageBox::information(this,tr("Editing"),tr("%1 was removed").arg(name));
        }
        if(contacts.count()==0){
            nameLine->clear();
            addressText->clear();
        }
    }
    updateInterface(Navigation);
}

//menubar
void AdressBook::load()
{

    QString fileName =QFileDialog::getOpenFileName(this,tr("Open contact"),tr(""),tr("Address Book(*.ab)::All Files(*)"));
    if(fileName=="")return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(this,tr("Failed"),tr("File is under using by other process!"));
        return;
    }
    QDataStream in(&file);

    try{

        in >> contacts;
        QMap<QString,QString>::iterator i = contacts.begin();

        nameLine->setText(i.key());
        addressText->setText(i.value());
    }catch(...){
        QMessageBox::information(this,tr("Failed"),tr("Unabel to load this file!"));
        return;
    }
    updateInterface(Navigation);




}
void AdressBook::save()
{
    if(contacts.size()==0){
        QMessageBox::information(this,tr("empty"),tr("it's empty!"));
        return;
    }
    QString fileName =QFileDialog::getSaveFileName(this,tr("save contact"),tr(""),tr("Address Book(*.ab)::All Files(*)"));
    qDebug()<<fileName;
    if(fileName=="")return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::information(this,tr("Failed"),tr("File is under using by other process!"));
        return;
    }

    QDataStream out(&file);
    if(contacts.size()>0){
        out<<contacts;
    }
}




void AdressBook::updateInterface(Mode mode)
{
    currentMode = mode;
    switch (currentMode) {
    case Navigation:
        nameLine->setReadOnly(true);
        addressText->setReadOnly(true);
        addButton->show();
        submitButton->hide();
        cancelButton->hide();
        nextButton->show();
        previousButton->show();
        editButton->show();
        deleteButton->show();

        nextButton->setEnabled(contacts.size()>=1);
        previousButton->setEnabled(contacts.size()>=1);
        editButton->setEnabled(contacts.size()>=1);
        deleteButton->setEnabled(contacts.size()>=1);
        break;
    case Adding:
        nameLine->setReadOnly(false);
        addressText->setReadOnly(false);
        nameLine->clear();
        addressText->clear();
        addButton->hide();
        submitButton->show();
        cancelButton->show();
        nextButton->hide();
        previousButton->hide();
        editButton->hide();
        deleteButton->hide();
        break;
    case Editing:
        nameLine->setReadOnly(false);
        addressText->setReadOnly(false);
        addButton->hide();
        submitButton->show();
        cancelButton->show();
        nextButton->hide();
        previousButton->hide();
        editButton->hide();
        deleteButton->hide();
    }
}

