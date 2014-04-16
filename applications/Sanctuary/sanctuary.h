#ifndef SANCTUARY_H
#define SANCTUARY_H

#include <QtGui/QMainWindow>
#include "ui_sanctuary.h"

class Sanctuary : public QMainWindow
{
	Q_OBJECT

public:
	Sanctuary(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Sanctuary();

private:
	Ui::SanctuaryClass ui;
};

#endif // SANCTUARY_H
