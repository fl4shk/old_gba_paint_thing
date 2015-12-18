#include "palette_chooser_widget_class.hpp"
#include "sfml_color_comparison_stuff.hpp"

palette_chooser_widget::palette_chooser_widget
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	current_color_index(0), palette_modified_recently(false)
{
	
	palette_image.create( num_colors_per_row * palette_slot_outer_width, 
		num_colors_per_column * palette_slot_outer_height, 
		sf::Color::Blue );
	palette_texture.loadFromImage(palette_image);
	palette_sprite.setTexture(palette_texture);
	
	full_resize( QSize( palette_image.getSize().x, 
		palette_image.getSize().y ) );
	set_min_max_sizes( QSize( palette_image.getSize().x,
		palette_image.getSize().y ) );
	//cout << palette_image.getSize().x << ", " << palette_image.getSize().y
	//	<< endl;
	
}

//bool palette_chooser_widget::grab_palette_from_canvas_widget_image()
//{
//	set< sf::Color, sfml_color_compare_for_set_or_map >
//		unique_sfml_colors_set;
//	map< sf::Color, u32, sfml_color_compare_for_set_or_map >
//		sfml_color_to_palette_index_map;
//	
//	for ( u32 i=0; i<num_colors_per_palette; ++i )
//	{
//		palette_image_arr[i].create( 1, 1 );
//		
//	}
//	
//	// Find all the unique sf::Colors in the image.
//	for ( u32 j=0; j<canvas_widget_image->getSize().y; ++j )
//	{
//		// Loop across rows before columns.
//		for ( u32 i=0; i<canvas_widget_image->getSize().x; ++i )
//		{
//			sf::Color the_color = canvas_widget_image->getPixel( i, j );
//			
//			if ( unique_sfml_colors_set.find(the_color) 
//				== unique_sfml_colors_set.end() )
//			{
//				unique_sfml_colors_set.insert(the_color);
//			}
//		}
//	}
//	
//	cout << unique_sfml_colors_set.size() << endl;
//	
//	if ( unique_sfml_colors_set.size() > num_colors_per_palette )
//	{
//		cout << "There are too many colors (more than 256) in the source "
//			<< "image!\n";
//		return false;
//	}
//	
//	{
//		u32 i = 0;
//		for ( const sf::Color& the_color : unique_sfml_colors_set )
//		{
//			palette_image_arr[i].setPixel( 0, 0, the_color );
//			sfml_color_to_palette_index_map[the_color] = i;
//			++i;
//		}
//	}
//	
//	//for ( u32 j=0; j<index_pixel_image.get_height(); ++j )
//	//{
//	//	for ( u32 i=0; i<index_pixel_image.get_width(); ++i )
//	//	{
//	//		//index_pixel_image.set_pixel( i, j, png::index_pixel(0) );
//	//		//index_pixel_image.set_pixel( i, j, 
//	//		//	rgb_pixel_to_index_pixel_map.at( rgb_pixel_image.get_pixel
//	//		//	( i, j ) ) );
//	//		
//	//		palette_image_arr
//	//	}
//	//}
//	
//	return true;
//}

void palette_chooser_widget::on_update()
{
	if (palette_modified_recently)
	{
		palette_modified_recently = false;
		
		palette_texture.loadFromImage(palette_image);
	}
	
	clear(sf::Color::White);
	draw(palette_sprite);
}



