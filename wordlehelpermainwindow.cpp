#include <qdebug.h>

#include "wordlehelpermainwindow.h"
#include "ui_wordlehelpermainwindow.h"
#include "wordllogic.h"


WordleHelperMainWindow::WordleHelperMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_Logic(new WordlLogic(this)),
      ui(new Ui::WordleHelperMainWindow)
{
    ui->setupUi(this);
    connect(m_Logic, &WordlLogic::show_wordlist, this, &WordleHelperMainWindow::set_wordlist);
    m_Logic->reset();

}

WordleHelperMainWindow::~WordleHelperMainWindow()
{
    delete ui;
}

void WordleHelperMainWindow::set_wordlist(QStringList data) {
    ui->wordlist->clear();
    ui->wordlist->addItems(data);
}

void WordleHelperMainWindow::process_commandline(QString commands)
{
    QStringList items(commands.split(' ', QString::SkipEmptyParts));
    qDebug() << items;
    while (!items.empty()) {
        QString currentItem = items.takeFirst();
        if (currentItem.at(0).isDigit()) {
            int letterPos = currentItem.at(0).toLatin1() - '1';
            for(int pos=2; pos<currentItem.length(); pos++) {
                m_Logic->require_letter_elswhere(letterPos, currentItem.at(pos));
            }
        } else if (currentItem.at(0) == '!') {
            for(int pos=1; pos<currentItem.length(); pos++) {
                m_Logic->exclude_letter(currentItem.at(pos));
            }
        } else {
            for(int pos=0; pos<5 && pos < currentItem.length(); pos++) {
                if (currentItem.at(pos).isLetter()) {
                    m_Logic->require_letter_at(pos, currentItem.at(pos));
                }
            }
        }
    }

    m_Logic->apply();
}

void WordleHelperMainWindow::on_pushButton_clicked()
{
     process_commandline(ui->letterDesc->text().toUpper());
}

void WordleHelperMainWindow::on_pushButton_2_clicked()
{
    m_Logic->reset();
    ui->letterDesc->clear();
}

