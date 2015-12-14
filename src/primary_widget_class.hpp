#ifndef primary_widget_class_hpp
#define primary_widget_class_hpp

#include "misc_includes.hpp"
#include "layout_widget_classes.hpp"


class primary_widget : public QMainWindow
{
	Q_OBJECT
	
	
protected:		// variables
	// Menu Stuff
	QMenu* file_menu, * second_menu;
	
	QAction* menu_laugh_action, * menu_quit_action;
	
	// Toolbar Stuff
	static const QString laugh_icon_file_name, quit_icon_file_name;
	QToolBar* toolbar;
	QAction* toolbar_laugh_action, * toolbar_quit_action;
	
	// The TEMPORARY central widget.
	grid_widget* the_grid_widget;
	
	
public:		// functions
	primary_widget( QWidget* parent = 0 );
	
	
protected:		// functions
	void generate_menus();
	void generate_toolbar();
	void generate_central_widget();
	
	inline void keyPressEvent( QKeyEvent* event )
	{
		if ( event->key() == Qt::Key_Escape )
		{
			quit_non_slot();
		}
	}
	inline void closeEvent( QCloseEvent* event )
	{
		show_quit_message();
	}
	
	inline void show_quit_message()
	{
		cout << "Exiting....\n";
	}
	
	inline void quit_non_slot()
	{
		show_quit_message();
		qApp->quit();
	}
	
protected slots:		// slots
	void laugh();
	void quit();
	
	
};


#endif		// primary_widget_class_hpp