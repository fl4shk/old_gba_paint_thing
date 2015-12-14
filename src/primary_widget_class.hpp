#ifndef primary_widget_class_hpp
#define primary_widget_class_hpp

#include "misc_includes.hpp"

class grid_widget : public QWidget
{
public:		// functions
	inline grid_widget( QWidget* parent = 0 ) : QWidget(parent)
	{
		layout = new QGridLayout(this);
	}
	
public:		// variables
	QGridLayout* layout;
	
};

class primary_widget : public QMainWindow
{
	Q_OBJECT
	
public:		// functions
	primary_widget( QWidget* parent = 0 );
	
protected:		// functions
	void generate_menus();
	void generate_buttons();
	void generate_central_widget();
	
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
	void quit();
	
protected:		// variables
	QPushButton* laugh_button;
	
	//QVBoxLayout* main_layout;
	//QGridLayout* secondary_layout;
	grid_widget* the_grid_widget;
	
	QMenu* file_menu, * second_menu;
	
	QAction* laugh_action, * quit_action;
	
};


#endif		// primary_widget_class_hpp
