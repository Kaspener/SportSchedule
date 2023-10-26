#include "tabwidgetcategories.h"
#include "ui_tabwidgetcategories.h"

TabWidgetCategories::TabWidgetCategories(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::TabWidgetCategories)
{
    ui->setupUi(this);
}

TabWidgetCategories::~TabWidgetCategories()
{
    delete ui;
}
