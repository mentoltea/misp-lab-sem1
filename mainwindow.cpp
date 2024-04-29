#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->createButton, &QPushButton::clicked,
                     this, &MainWindow::create_dialog_new_file);
    QObject::connect(ui->openButton, &QPushButton::clicked,
                     this, &MainWindow::create_dialog_open_file);
    QObject::connect(ui->saveButton, &QPushButton::clicked,
                     this, &MainWindow::save_current_file);

    QObject::connect(ui->actionCreate_new, &QAction::triggered,
                     this, &MainWindow::create_dialog_new_file);
    QObject::connect(ui->actionOpen, &QAction::triggered,
                     this, &MainWindow::create_dialog_open_file);
    QObject::connect(ui->actionSave, &QAction::triggered,
                     this, &MainWindow::save_current_file);
    QObject::connect(ui->actionSave_as, &QAction::triggered,
                     this, &MainWindow::create_dialog_save_as_file);
    QObject::connect(ui->actionSave_all, &QAction::triggered,
                     this, &MainWindow::save_files);

    QObject::connect(ui->fileListWidget, &QListWidget::itemClicked,
                     this, &MainWindow::choose_current_file);
    QObject::connect(ui->sectionsComb, &QComboBox::currentTextChanged,
                     this, &MainWindow::changed_section);
    QObject::connect(ui->treeWidget, &QTreeWidget::itemClicked,
                     this, &MainWindow::clicked_item);
    QObject::connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked,
                     this, &MainWindow::selected_item);

    QObject::connect(ui->editButton, &QPushButton::clicked,
                     this, &MainWindow::add_key_current_file);
    QObject::connect(ui->deleteButton, &QPushButton::clicked,
                     this, &MainWindow::delete_key_current_file);

    QObject::connect(ui->sectionAdd, &QPushButton::clicked,
                     this, &MainWindow::add_section_dialog);
    QObject::connect(ui->sectionDelete, &QPushButton::clicked,
                     this, &MainWindow::delete_section_current_file);
}

MainWindow::~MainWindow()
{
    delete_items();
    close_files();
    delete ui;
}

void MainWindow::create_new_file(QString filename) {
    std::string path = filename.toStdString();
    int size = opened_files.size();
    for (int i=0; i<size; i++) {
        if (opened_files[i]->get_path() == path) {
            give_warning(QString("This file already opened"));
            current_file = opened_files[i];
            return;
        }
    }
    current_file = new IniFile(path);
    opened_files.push_back(current_file);
    current_file = nullptr;

    ui->fileListWidget->addItem(filename);
}

void MainWindow::open_file(QString filename) {
    std::string path = filename.toStdString();
    int size = opened_files.size();
    for (int i=0; i<size; i++) {
        if (opened_files[i]->get_path() == path) {
            give_warning(QString("This file already opened"));
            current_file = opened_files[i];
            return;
        }
    }
    current_file = new IniFile(path);
    opened_files.push_back(current_file);
    current_file = nullptr;

    ui->fileListWidget->addItem(filename);
}

void MainWindow::save_current_file() {
    if (current_file != nullptr) {
        save_file(QString::fromStdString(current_file->get_path()));
        return;
    }
    give_warning("No file for saving selected");
}


void MainWindow::save_current_file_as(QString filename) {
    if (current_file != nullptr) {
        if (current_file->get_path() == filename.toStdString()) {
            give_warning("New filename cant be the same as old one");
            return;
        }
        current_file->save_as(filename.toStdString());
        return;
    }
    give_warning("No file for saving selected");
}

void MainWindow::save_file(QString filename) {
    int size = opened_files.size();
    std::string path = filename.toStdString();
    for (int i=0; i<size; i++) {
        if (opened_files[i]->get_path() == path) {
            opened_files[i]->save();
            return;
        }
    }
    give_warning(QString("No opened file with this filename found"));
}

void MainWindow::save_file_as(QString filename, QString new_filename) {
    int size = opened_files.size();
    std::string path = filename.toStdString();
    std::string new_path = new_filename.toStdString();
    if (new_path.empty()) {
        give_warning(QString("Cannot save file with empty filename"));
        return;
    }

    for (int i=0; i<size; i++) {
        if (opened_files[i]->get_path() == path) {
            opened_files[i]->save_as(new_path);
            return;
        }
    }
    give_warning(QString("No opened file with this filename found"));
}

void MainWindow::save_files() {
    int size = opened_files.size();
    for (int i=0; i<size; i++) {
        opened_files[i]->save();
    }
}

void MainWindow::close_file(QString filename) {
    int size = opened_files.size();
    std::string path = filename.toStdString();
    if (current_file->get_path() == path) current_file = nullptr;

    for (int i=0; i<size; i++) {
        if (opened_files[i]->get_path() == path) {
            opened_files[i]->save();
            delete opened_files[i];
            opened_files.erase(std::next(opened_files.begin(), i));
            return;
        }
    }
    give_warning(QString("No opened file with this filename found"));
}

void MainWindow::close_files(){
    int size = opened_files.size();
    for (int i=0; i<size; i++) {
        opened_files[i]->save();
        delete opened_files[i];
    }
    opened_files.clear();
    current_file = nullptr;
}


void MainWindow::give_warning(QString text) {
    QMessageBox::warning(this, QString("Warning"), text);
}


void MainWindow::create_dialog_new_file() {
    QString filename= QFileDialog::getSaveFileName(this,
                                                    "Choose directory and name for new file",
                                                    this->current_directory,
                                                    "Ini file (*.ini);;All files (*.*)");
    if (filename.toStdString().empty()) {
        give_warning("Filename cant be empty");
        return;
    }

    create_new_file(filename);
}

void MainWindow::create_dialog_open_file() {
    QString filename= QFileDialog::getOpenFileName(this,
                                                    "Choose existing file",
                                                    this->current_directory,
                                                    "Ini file (*.ini);;All files (*.*)");
    if (filename.toStdString().empty()) {
        give_warning("Filename cant be empty");
        return;
    }

    open_file(filename);
}

void MainWindow::create_dialog_save_as_file() {
    QString filename= QFileDialog::getSaveFileName(this,
                                                    "Choose existing file",
                                                    this->current_directory,
                                                    "Ini file (*.ini);;All files (*.*)");
    if (filename.toStdString().empty()) {
        give_warning("Filename cant be empty");
        return;
    }

    save_current_file_as(filename);
}


void MainWindow::choose_current_file(QListWidgetItem* item) {
    std::string path = item->text().toStdString();
    int size = opened_files.size();
    for (int i=0; i<size; i++) {
        if (opened_files[i]->get_path() == path) {
            current_file = opened_files[i];
        }
    }

    ui->currentFileLabel->setText(item->text());
    view_file(current_file);
}

void MainWindow::delete_items() {
    int size = items.size();
    for (int i=size-1; i>=0; i--) {
        delete items[i];
        items.pop_back();
    }
}

void MainWindow::view_file(IniFile* file) {
    delete_items();
    ui->deleteComb->clear();
    ui->sectionsComb->clear();
    SectionsMap sections = file->getSections();
    QTreeWidgetItem *temptop = nullptr;
    QTreeWidgetItem *templow = nullptr;
    for (auto it = sections.begin(); it != sections.end(); it++) {
        temptop = new QTreeWidgetItem(ui->treeWidget);
        temptop->setText(0, QString::fromStdString(it->first));
        ui->treeWidget->addTopLevelItem(temptop);
        items.push_back(temptop);
        for (auto kit = it->second.begin(); kit != it->second.end(); kit++) {
            templow = new QTreeWidgetItem(temptop);
            templow->setText(0, QString::fromStdString(kit->first + " = " + kit->second));
            items.push_back(templow);
        }
        ui->sectionsComb->addItem(QString::fromStdString(it->first));
    }
    ui->treeWidget->expandAll();
    changed_section(ui->sectionsComb->currentText());
}

void MainWindow::view_file(IniFile* file, QString section) {
    delete_items();
    ui->deleteComb->clear();
    ui->sectionsComb->clear();
    SectionsMap sections = file->getSections();
    QTreeWidgetItem *temptop = nullptr;
    QTreeWidgetItem *templow = nullptr;
    for (auto it = sections.begin(); it != sections.end(); it++) {
        temptop = new QTreeWidgetItem(ui->treeWidget);
        temptop->setText(0, QString::fromStdString(it->first));
        ui->treeWidget->addTopLevelItem(temptop);
        items.push_back(temptop);
        for (auto kit = it->second.begin(); kit != it->second.end(); kit++) {
            templow = new QTreeWidgetItem(temptop);
            templow->setText(0, QString::fromStdString(kit->first + " = " + kit->second));
            items.push_back(templow);
        }
        ui->sectionsComb->addItem(QString::fromStdString(it->first));
    }
    ui->treeWidget->expandAll();
    ui->sectionsComb->setCurrentIndex(ui->sectionsComb->findText(section));
    changed_section(ui->sectionsComb->currentText());
}


void MainWindow::changed_section(QString text) {
    if (current_file == nullptr) return;
    ui->deleteComb->clear();
    SectionsMap sections = current_file->getSections();
    std::string section = text.toStdString();
    if (section.empty()) return;
    for (auto it = sections.begin(); it != sections.end(); it++) {
        if (it->first != section) continue;
        for (auto kit = it->second.begin(); kit != it->second.end(); kit++) {
            ui->deleteComb->addItem(QString::fromStdString(kit->first));
        }
        break;
    }
}

void MainWindow::clicked_item(QTreeWidgetItem *item, int column) {
    if (item->parent()) {
        std::string text = item->text(0).toStdString();
        text = text.substr(0, text.find('=')-1);
        ui->deleteComb->setCurrentIndex(ui->deleteComb->findText(QString::fromStdString(text)));
        item = item->parent();
    }
    ui->sectionsComb->setCurrentIndex(ui->sectionsComb->findText(item->text(0)));
}

void MainWindow::selected_item(QTreeWidgetItem *item, int column) {
    clicked_item(item, column);
    if (!item->parent()) return;
    std::string text = item->text(0).toStdString();
    size_t pos = text.find('=');
    ui->keyEditLine->setText(QString::fromStdString(text.substr(0, pos-1)));
    ui->valueEditLine->setText(QString::fromStdString(text.substr(pos+2)));
}

void MainWindow::add_key_current_file() {
    if (current_file == nullptr) {
        give_warning("Select file to add key");
        return;
    }
    std::string section = ui->sectionsComb->currentText().toStdString();
    std::string key = ui->keyEditLine->text().toStdString();
    std::string value = ui->valueEditLine->text().toStdString();

    if (section.empty()) {
        give_warning("No section to write");
        return;
    }
    if (key.empty()) {
        give_warning("Key cannot be empty");
        return;
    }
    if (value.empty()) {
        give_warning("Value cannot be empty");
        return;
    }

    current_file->writeString(section, key, value);
    view_file(current_file, QString::fromStdString(section));
}

void MainWindow::delete_key_current_file() {
    if (current_file == nullptr) {
        give_warning("Select file to delete key");
        return;
    }
    std::string section = ui->sectionsComb->currentText().toStdString();
    std::string key = ui->deleteComb->currentText().toStdString();

    if (section.empty()) {
        give_warning("No section to delete of");
        return;
    }
    if (key.empty()) {
        give_warning("Key cannot be empty");
        return;
    }

    current_file->deleteKey(section, key);
    view_file(current_file, QString::fromStdString(section));
}


void MainWindow::add_section_current_file(QString text) {
    std::string section = text.toStdString();
    if (section.empty()) {
        give_warning("Section name cannot be empty");
        return;
    }
    current_file->addNewSection(section);
    view_file(current_file, QString::fromStdString(section));
}

void MainWindow::add_section_dialog() {
    if (current_file == nullptr) {
        give_warning("Select file to add sections");
        return;
    }
    sectionDialog dialog(this);
    QObject::connect(&dialog, &sectionDialog::text_entered,
                     this, &MainWindow::add_section_current_file);
    dialog.exec();
}


void MainWindow::delete_section_current_file() {
    if (current_file == nullptr) {
        give_warning("Select file to delete sections");
        return;
    }
    std::string section = ui->sectionsComb->currentText().toStdString();
    if (section.empty()) {
        give_warning("No section to delete");
    }
    current_file->deleteSection(section);
    view_file(current_file);
}
