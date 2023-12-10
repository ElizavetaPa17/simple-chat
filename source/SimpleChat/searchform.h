#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>

namespace Ui {
class SearchForm;
}

class SearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = nullptr);
    ~SearchForm();

private:
    Ui::SearchForm *ui;

signals:
    void sgnlFindUser(const char* username);

private slots:
    void sltFindUser();
};

#endif // SEARCHFORM_H
