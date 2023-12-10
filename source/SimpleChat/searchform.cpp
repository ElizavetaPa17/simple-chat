#include "searchform.h"
#include "ui_searchform.h"

SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);

    connect(ui->searchButton, &QPushButton::clicked, this, &SearchForm::sltFindUser);
}

SearchForm::~SearchForm()
{
    delete ui;
}

void SearchForm::sltFindUser() {
    if (ui->searchLineEdit->text().isEmpty()) {
        return;
    }

    emit sgnlFindUser(ui->searchLineEdit->text().toUtf8().toStdString().c_str());
}
