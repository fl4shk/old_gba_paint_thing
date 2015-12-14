#ifndef primary_widget_class_hpp
#define primary_widget_class_hpp

#include "misc_includes.hpp"

class primary_widget : public QMainWindow
{
	Q_OBJECT
	
public:		// functions
	primary_widget( QWidget* parent = 0 );
	
protected:		// functions
	inline void keyPressEvent( QKeyEvent* event )
	{
		if ( event->key() == Qt::Key_Escape )
		{
			show_quit_message();
			qApp->quit();
		}
	}
	inline void closeEvent( QCloseEvent* event )
	{
		show_quit_message();
	}
	
	inline void show_quit_message()
	{
		cout << "Bye-bye!\n";
	}
	
protected slots:		// slots
	void laugh();
	
protected:		// variables
	QPushButton* laugh_button;
	QGridLayout* grid_layout;
	
	QMenu* file_menu, * second_menu;
	QAction* laugh_action;
	
};


#endif		// primary_widget_class_hpp
