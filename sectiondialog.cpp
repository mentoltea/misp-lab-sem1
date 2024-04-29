#include "sectiondialog.h"
#include "ui_sectiondialog.h"

sectionDialog::sectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::sectionDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->buttonBox, &QDialogButtonBox::accepted,
                     this, &sectionDialog::ok_button);
}

sectionDialog::~sectionDialog()
{
    delete ui;
}

void sectionDialog::ok_button() {
    emit text_entered(ui->lineEdit->text());
}
