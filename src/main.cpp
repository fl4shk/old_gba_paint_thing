#include "misc_includes.hpp"
#include "primary_widget_class.hpp"



int main( int argc, char** argv )
{
	vector<string> argv_copy;
	
	for ( int i=0; i<argc; ++i )
	{
		argv_copy.push_back(string(argv[i]));
	}
	
	QApplication app( argc, argv );
	
	primary_widget window(argv_copy);
	window.show();
	
	return app.exec();
}
