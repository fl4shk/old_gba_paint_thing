#include "image_editor_widget_class.hpp"

const QString image_editor_widget::default_parent_title 
	= QString("GBA Paint Thing");

image_editor_widget::image_editor_widget( vector<string>* s_argv_copy,
	QWidget* s_parent ) : QWidget(s_parent), argv_copy(s_argv_copy),
	parent(s_parent)
{
	//setAttribute(Qt::WA_StaticContents);
	//setMouseTracking(true);
	
	parent->resize( 600, 600 );
	parent->setWindowTitle(default_parent_title);
	
	
	//the_sfml_canvas_widget = new sfml_canvas_widget( this, QPoint( 0, 0 ), 
	//	QSize( 200, 200 ), string("the_powerup_gfx.png") );
	the_sfml_canvas_widget = new sfml_canvas_widget( this, QPoint( 0, 0 ),
		QSize( 200, 200 ), argv_copy->at(1) );
	the_palette_chooser_widget = new palette_chooser_widget( this, 
		QPoint( 0, 0 ), QSize( 200, 200 ) );
	
	the_sfml_canvas_widget->set_the_palette_chooser_widget
		(the_palette_chooser_widget);
	the_palette_chooser_widget->set_the_sfml_canvas_widget
		(the_sfml_canvas_widget);
	
	if ( !the_sfml_canvas_widget->open_image() )
	{
		quit_non_slot();
	}
	
	// scroll_area stuff
	scroll_area = new QScrollArea(this);
	scroll_area->setWidget(the_sfml_canvas_widget);
	scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	
	// hbox_layout stuff
	hbox_layout = new QHBoxLayout(this);
	hbox_layout->addWidget(scroll_area);
	hbox_layout->addWidget(the_palette_chooser_widget);
	
	//setLayout(hbox_layout);
	
	
	////vbox_layout = new QVBoxLayout(this);
	////vbox_layout->addWidget(the_palette_chooser_widget);
	////vbox_layout->setGeometry( QRect( QPoint( 0, 0 ), QSize( 200, 200 ) ) );
	////hbox_layout->addLayout(vbox_layout);
	//
	////// Show Horizontal ScrollBar Stuff
	////QPushButton* show_horiz_sb_stuff_button 
	////	= new QPushButton( "Show Horiz", this );
	////connect( show_horiz_sb_stuff_button, &QPushButton::clicked, this,
	////	&image_editor_widget::show_horizontal_scroll_bar_stuff );
	////vbox_layout->addWidget(show_horiz_sb_stuff_button);
	////
	////// Show Vertical ScrollBar Stuff
	////QPushButton* show_vert_sb_stuff_button 
	////	= new QPushButton( "Show Vert", this );
	////connect( show_vert_sb_stuff_button, &QPushButton::clicked, this,
	////	&image_editor_widget::show_vertical_scroll_bar_stuff );
	////vbox_layout->addWidget(show_vert_sb_stuff_button);
	////
	////// Show Geometry Stuff
	////QPushButton* show_geometry_stuff_button
	////	= new QPushButton( "Show Geometry", this );
	////connect( show_geometry_stuff_button, &QPushButton::clicked, this,
	////	&image_editor_widget::show_geometry_stuff );
	////vbox_layout->addWidget(show_geometry_stuff_button);
	////
	////
	//////hbox_layout->addLayout( vbox_layout, Qt::AlignTop | Qt::AlignRight );
	////hbox_layout->addLayout( vbox_layout );
	
}


//void image_editor_widget::adjust_scroll_bar( QScrollBar* scroll_bar )
//{
//	scroll_bar->setValue
//		( ( the_sfml_canvas_widget->scale_factor * scroll_bar->value() ) 
//		+ ( ( the_sfml_canvas_widget->scale_factor - 1 ) 
//		* ( scroll_bar->pageStep() / 2 ) ));
//}

bool image_editor_widget::zoom_in()
{
	if ( the_sfml_canvas_widget->scale_factor == 16 )
	{
		return false;
	}
	
	the_sfml_canvas_widget->scale_factor <<= 1;
	the_sfml_canvas_widget->zoomed_recently = true;
	
	scroll_area->horizontalScrollBar()->setValue
		( scroll_area->horizontalScrollBar()->value() << 1 );
	scroll_area->verticalScrollBar()->setValue
		( scroll_area->verticalScrollBar()->value() << 1 );
	
	return true;
}


bool image_editor_widget::zoom_out()
{
	if ( the_sfml_canvas_widget->scale_factor == 1 )
	{
		return false;
	}
	
	the_sfml_canvas_widget->scale_factor >>= 1;
	the_sfml_canvas_widget->zoomed_recently = true;
	
	scroll_area->horizontalScrollBar()->setValue
		( scroll_area->horizontalScrollBar()->value() >> 1 );
	scroll_area->verticalScrollBar()->setValue
		( scroll_area->verticalScrollBar()->value() >> 1 );
	
	return true;
}

void image_editor_widget::keyPressEvent( QKeyEvent* event )
{
	if ( event->key() == Qt::Key_A )
	{
		zoom_in();
	}
	else if ( event->key() == Qt::Key_Z )
	{
		zoom_out();
	}
}


void image_editor_widget::hello()
{
	cout << "Hello!\n";
}


void image_editor_widget::show_horizontal_scroll_bar_stuff()
{
	cout << scroll_area->horizontalScrollBar()->value() << " "
		<< scroll_area->horizontalScrollBar()->minimum() << " "
		<< scroll_area->horizontalScrollBar()->maximum() << endl;
}

void image_editor_widget::show_vertical_scroll_bar_stuff()
{
	cout << scroll_area->verticalScrollBar()->value() << " "
		<< scroll_area->verticalScrollBar()->minimum() << " "
		<< scroll_area->verticalScrollBar()->maximum() << endl;
}

void image_editor_widget::show_geometry_stuff()
{
	cout << scroll_area->geometry().x() << " " 
		<< scroll_area->geometry().y() << endl;
	cout << scroll_area->frameGeometry().x() << " " 
		<< scroll_area->frameGeometry().y() << endl;
}


