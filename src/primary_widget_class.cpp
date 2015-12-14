#include "primary_widget_class.hpp"


primary_widget::primary_widget( QWidget* parent ) : QMainWindow(parent)
{
	generate_menus();
	generate_buttons();
	generate_central_widget();
}

void primary_widget::generate_menus()
{
	laugh_action = new QAction( "&Laugh", this );
	quit_action = new QAction( "&Quit", this );
	
	connect( laugh_action, &QAction::triggered, this, 
		&primary_widget::laugh );
	connect( quit_action, &QAction::triggered, this, 
		&primary_widget::quit );
	
	file_menu = menuBar()->addMenu("&File");
	second_menu = menuBar()->addMenu("&Second");
	
	file_menu->addAction(laugh_action);
	file_menu->addAction(quit_action);
	
	second_menu->addAction(quit_action);
}

void primary_widget::generate_buttons()
{
	laugh_button = new QPushButton( "Laugh", this );
	connect( laugh_button, &QPushButton::clicked, this, 
		&primary_widget::laugh );
}

void primary_widget::generate_central_widget()
{
	the_grid_widget = new grid_widget(this);
	the_grid_widget->layout->addWidget( laugh_button, 1, 1, 1, 1 );
	QPushButton* test_button = new QPushButton( "Test", this );
	the_grid_widget->layout->addWidget( test_button, 1, 2, 1, 1 );
	setCentralWidget(the_grid_widget);
}

void primary_widget::laugh()
{
	cout << "AHAHAHAHA\n";
}

void primary_widget::quit()
{
	show_quit_message();
	qApp->quit();
}


