#include "primary_widget_class.hpp"

const QString primary_widget::laugh_icon_file_name
	= QString("icons/laugh_icon_32x32.png"),
	primary_widget::quit_icon_file_name
	= QString("icons/quit_32x32.png");


primary_widget::primary_widget( vector<string>& s_argv_copy, 
	QWidget* parent ) : QMainWindow(parent), argv_copy(s_argv_copy)
{
	generate_menus();
	
	if ( !generate_toolbar() )
	{
		quit_non_slot();
	}
	
	generate_central_widget();
	
	//if ( !the_central_widget->open_image
	//	(QString("background_tiles_modded.png")) )
	//{
	//	cout << "Error opening background_tiles_modded.png!\n";
	//	quit_non_slot();
	//}
	
	//quit_non_slot();
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

bool primary_widget::generate_toolbar()
{
	QPixmap laugh_pixmap(laugh_icon_file_name), 
		quit_pixmap(quit_icon_file_name);
	
	if ( laugh_pixmap.isNull() )
	{
		cout << "Unable to open " << laugh_icon_file_name.toStdString()
			<< " for reading!  ";
		return false;
	}
	
	if ( quit_pixmap.isNull() )
	{
		cout << "Unable to open " << quit_icon_file_name.toStdString()
			<< " for reading!  ";
		return false;
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
	
	return true;
}

void primary_widget::generate_central_widget()
{
	the_central_widget = new image_editor_widget( &argv_copy, this );
	
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

