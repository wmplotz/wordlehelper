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
    on_pushButton_2_clicked();
}

WordleHelperMainWindow::~WordleHelperMainWindow()
{
    delete ui;
}

void WordleHelperMainWindow::set_wordlist(QStringList data) {
    ui->wordlist->clear();
    ui->wordlist->addItems(data);
}

void WordleHelperMainWindow::process_commandline() {

    QString currentItem;

    currentItem = ui->correctLetters->text().trimmed().toUpper();
    for(int pos=0; pos<5 && pos < currentItem.length(); pos++) {
        if (currentItem.at(pos).isLetter()) {
            m_Logic->require_letter_at(pos, currentItem.at(pos));
        }
    }

    currentItem = ui->not_in_word->text().trimmed().toUpper();
    for(int pos=0; pos<currentItem.length(); pos++) {
        m_Logic->exclude_letter(currentItem.at(pos));
    }

    QLineEdit* items[5] = {ui->not_in_1, ui->not_in_2, ui->not_in_3, ui->not_in_4, ui->not_in_5};

    for(int letterPos=0; letterPos < 5; letterPos++) {
        currentItem = items[letterPos]->text().trimmed().toUpper();
        for(int pos=0; pos<currentItem.length(); pos++) {
            m_Logic->require_letter_elswhere(letterPos, currentItem.at(pos));
        }
    }

    m_Logic->apply();
}

void WordleHelperMainWindow::on_pushButton_clicked()
{
    process_commandline();
}

void WordleHelperMainWindow::on_pushButton_2_clicked()
{
    m_Logic->reset();
    ui->correctLetters->setText(".....");
    ui->not_in_1->clear();
    ui->not_in_2->clear();
    ui->not_in_3->clear();
    ui->not_in_4->clear();
    ui->not_in_5->clear();
    ui->not_in_word->clear();
}

