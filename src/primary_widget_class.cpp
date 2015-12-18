#include "primary_widget_class.hpp"

const QString primary_widget::laugh_icon_file_name
	= QString("icons/laugh_icon_32x32.png"),
	primary_widget::quit_icon_file_name
	= QString("icons/quit_32x32.png");


primary_widget::primary_widget( vector<string>& s_argv_copy, 
	QWidget* parent ) : QMainWindow(parent), argv_copy(s_argv_copy)
{
	generate_menus();
	generate_toolbar();
	generate_central_widget();
	
	//if ( !the_central_widget->open_image
	//	(QString("background_tiles_modded.png")) )
	//{
	//	cout << "Error opening background_tiles_modded.png!\n";
	//	quit_non_slot();
	//}
}

void primary_widget::generate_menus()
{
	menu_laugh_action = new QAction( "&Laugh", this );
	menu_quit_action = new QAction( "&Quit", this );
	
	connect( menu_laugh_action, &QAction::triggered, this, 
		&primary_widget::laugh );
	connect( menu_quit_action, &QAction::triggered, this, 
		&primary_widget::quit );
	
	file_menu = menuBar()->addMenu("&File");
	second_menu = menuBar()->addMenu("&Second");
	
	file_menu->addAction(menu_laugh_action);
	file_menu->addAction(menu_quit_action);
	
	second_menu->addAction(menu_quit_action);
}

void primary_widget::generate_toolbar()
{
	//laugh_button = new QPushButton( "Laugh", this );
	//quit_button = new QPushButton( "Quit", this );
	//
	//connect( laugh_button, &QPushButton::clicked, this, 
	//	&primary_widget::laugh );
	//connect( quit_button, &QPushButton::clicked, this, 
	//	&primary_widget::quit );
	
	QPixmap laugh_pixmap(laugh_icon_file_name), 
		quit_pixmap(quit_icon_file_name);
	
	if ( laugh_pixmap.isNull() )
	{
		cout << "Unable to open " << laugh_icon_file_name.toStdString()
			<< " for reading!  ";
		quit_non_slot();
	}
	
	if ( quit_pixmap.isNull() )
	{
		cout << "Unable to open " << quit_icon_file_name.toStdString()
			<< " for reading!  ";
		quit_non_slot();
	}
	
	toolbar = addToolBar("main toolbar");
	toolbar_laugh_action = toolbar->addAction( QIcon(laugh_pixmap), 
		"Laugh" );
	toolbar_quit_action = toolbar->addAction( QIcon(quit_pixmap), 
		"Quit" );
	
	connect( toolbar_laugh_action, &QAction::triggered, this,
		&primary_widget::laugh );
	connect( toolbar_quit_action, &QAction::triggered, this,
		&primary_widget::quit );
	
}

void primary_widget::generate_central_widget()
{
	//the_grid_widget = new grid_widget(this);
	//the_grid_widget->layout->setSpacing(1);
	//the_grid_widget->layout
	//
	//QPushButton* button_1 = new QPushButton( "Button 1", this );
	//the_grid_widget->layout->addWidget( button_1, 2, 1, 1, 1 );
	//
	//QPushButton* button_2 = new QPushButton( "Button 2", this );
	//the_grid_widget->layout->addWidget( button_2, 2, 2, 1, 1 );
	//
	//setCentralWidget(the_grid_widget);
	
	the_central_widget = new image_editor_widget(this);
	
	setCentralWidget(the_central_widget);
}

void primary_widget::laugh()
{
	cout << "AHAHAHAHA\n";
}

void primary_widget::quit()
{
	quit_non_slot();
}


