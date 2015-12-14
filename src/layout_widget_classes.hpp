#ifndef layout_widget_classes_hpp
#define layout_widget_classes_hpp

#include "misc_includes.hpp"

// These classes are intended to be used as child widgets.

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

class vbox_widget : public QWidget
{
public:		// functions
	inline vbox_widget( QWidget* parent = 0 ) : QWidget(parent)
	{
		layout = new QVBoxLayout(this);
	}
	
public:		// variables
	QVBoxLayout* layout;
	
};

class hbox_widget : public QWidget
{
public:		// functions
	inline hbox_widget( QWidget* parent = 0 ) : QWidget(parent)
	{
		layout = new QHBoxLayout(this);
	}
	
public:		// variables
	QHBoxLayout* layout;
	
};

#endif		// layout_widget_classes_hpp
