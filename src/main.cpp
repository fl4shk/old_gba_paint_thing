#include "misc_includes.hpp"
#include "primary_widget_class.hpp"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	
	primary_widget window;
	
	window.show();
	
	return app.exec();
}
