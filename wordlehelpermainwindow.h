#ifndef WORDLEHELPERMAINWINDOW_H
#define WORDLEHELPERMAINWINDOW_H

#include <QMainWindow>
#include <wordllogic.h>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class WordleHelperMainWindow; }
QT_END_NAMESPACE

class WordleHelperMainWindow : public QMainWindow
{
    Q_OBJECT

    WordlLogic* m_Logic;

    void process_commandline();

public:
    WordleHelperMainWindow(QWidget *parent = nullptr);
    ~WordleHelperMainWindow();

public slots:
    void set_wordlist(QStringList data);


private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::WordleHelperMainWindow *ui;
};
#endif // WORDLEHELPERMAINWINDOW_H
