#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "IniFile.h"
#include <vector>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QTreeWidget>
#include "sectiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString current_directory;

private:
    Ui::MainWindow *ui;
    IniFile* current_file = nullptr;
    std::vector<IniFile*> opened_files;
    std::vector<QTreeWidgetItem*> items;

    void create_new_file(QString filename);
    void open_file(QString filename);
    void save_current_file_as(QString filename);
    void save_file(QString filename);
    void save_file_as(QString filename, QString new_filename);
    void close_file(QString filename);
    void close_files();
    void delete_items();
    void view_file(IniFile* file);
    void view_file(IniFile* file, QString section);

public slots:
    void give_warning(QString text);

private slots:


    void create_dialog_new_file();
    void create_dialog_open_file();
    void create_dialog_save_as_file();
    void save_current_file();
    void save_files();

    void choose_current_file(QListWidgetItem* item);

    void clicked_item(QTreeWidgetItem *item, int column);
    void selected_item(QTreeWidgetItem *item, int column);
    void changed_section(QString text);

    void add_key_current_file();
    void delete_key_current_file();
    void add_section_current_file(QString text);
    void add_section_dialog();
    void delete_section_current_file();

};
#endif // MAINWINDOW_H
