#ifndef TABWIDGETCATEGORIES_H
#define TABWIDGETCATEGORIES_H

#include <QTabWidget>

namespace Ui {
class TabWidgetCategories;
}

class TabWidgetCategories : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabWidgetCategories(QWidget *parent = 0);
    ~TabWidgetCategories();

private:
    Ui::TabWidgetCategories *ui;
};

#endif // TABWIDGETCATEGORIES_H
