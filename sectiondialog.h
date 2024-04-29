#ifndef SECTIONDIALOG_H
#define SECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class sectionDialog;
}

class sectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sectionDialog(QWidget *parent = nullptr);
    ~sectionDialog();

private:
    Ui::sectionDialog *ui;

private slots:
    void ok_button();

signals:
    void text_entered(QString);
};

#endif // SECTIONDIALOG_H
