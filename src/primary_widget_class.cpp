#include "primary_widget_class.hpp"


primary_widget::primary_widget( QWidget* parent ) : QMainWindow(parent)
{
	laugh_button = new QPushButton( "Laugh", this );
	grid_layout = new QGridLayout();
	
	grid_layout->addWidget( laugh_button, 1, 1, 1, 1 );
	grid_layout->setSpacing(10);
	
	connect( laugh_button, &QPushButton::clicked, this, 
		&primary_widget::laugh );
	
}

void primary_widget::laugh()
{
	cout << "AHAHAHAHA\n";
}
