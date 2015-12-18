#ifndef sfml_color_comparison_stuff_hpp
#define sfml_color_comparison_stuff_hpp


struct sfml_color_compare_for_set_or_map
{
public:		// functions
	inline bool operator () ( const sf::Color& lhs, const sf::Color& rhs ) 
		const
	{
		u32 lhs_as_u32 = ( lhs.r << 16 ) | ( lhs.g << 8 ) | lhs.b,
			rhs_as_u32 = ( rhs.r << 16 ) | ( rhs.g << 8 ) | rhs.b;
		
		return ( lhs_as_u32 < rhs_as_u32 );
	}
};

struct sfml_color_compare_for_sort
{
public:		// functions
	inline bool operator () ( const sf::Color& lhs, 
		const sf::Color& rhs ) const
	{
		auto color_max_3 = [&]( const sf::Color& the_color ) 
			-> u8
		{
			return ( ( the_color.r >= the_color.g 
				&& the_color.r >= the_color.b ) 
				? the_color.r 
				: ( ( the_color.g > the_color.r 
				&& the_color.g > the_color.b ) 
				? the_color.g : the_color.b ) );
		};
		auto color_min_3 = [&]( const sf::Color& the_color ) 
			-> u8
		{
			return ( ( the_color.r <= the_color.g 
				&& the_color.r <= the_color.b ) 
				? the_color.r 
				: ( ( the_color.g < the_color.r 
				&& the_color.g < the_color.b ) 
				? the_color.g : the_color.b ) );
		};
		
		u8 lhs_max = color_max_3(lhs);
		u8 lhs_min = color_min_3(lhs);
		
		u8 rhs_max = color_max_3(rhs);
		u8 rhs_min = color_min_3(rhs);
		
		u32 lhs_lightness = ( lhs_max + lhs_min ) / 2;
		u32 rhs_lightness = ( rhs_max + rhs_min ) / 2;
		
		return ( lhs_lightness < rhs_lightness );
	}
};


#endif		// sfml_color_comparison_stuff_hpp
