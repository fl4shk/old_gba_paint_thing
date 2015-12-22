#ifndef color_stuff_hpp
#define color_stuff_hpp

#include "misc_includes.hpp"

inline sf::Color png_color_to_sfml_color( const png::color& the_png_color )
{
	return sf::Color( the_png_color.red, the_png_color.green, 
		the_png_color.blue );
}

inline png::color sfml_color_to_png_color
	( const sf::Color& the_sfml_color )
{
	return png::color( the_sfml_color.r, the_sfml_color.g, 
		the_sfml_color.b );
}

class rgb15
{
public:		// constants
	static constexpr u32 red_shift = 0x0, green_shift = 0x5, 
		blue_shift = 0xa; 
	static constexpr u32 red_mask = 0x001f, green_mask = 0x03e0,
		blue_mask = 0x7c00;
	static constexpr u32 individual_component_mask = 0x1f;
	static constexpr u32 rgb24_shift = 0x3;
	
protected:		// variables
	u32 red, green, blue;
	
public:		// functions
	inline rgb15( u32 s_red, u32 s_green, u32 s_blue )
	{
		set_red(s_red);
		set_green(s_green);
		set_blue(s_blue);
	}
	
	inline u32 get_red() const
	{
		return ( red & individual_component_mask );
	}
	inline u32 get_green() const
	{
		return ( green & individual_component_mask );
	}
	inline u32 get_blue() const
	{
		return ( blue & individual_component_mask );
	}
	
	inline void set_red( u32 n_red )
	{
		red = n_red & individual_component_mask;
	}
	inline void set_green( u32 n_green )
	{
		green = n_green & individual_component_mask;
	}
	inline void set_blue( u32 n_blue )
	{
		blue = n_blue & individual_component_mask;
	}
	
	inline u16 to_u16() const
	{
		return (u16)( ( ( get_red() & red_mask ) << red_shift ) 
			| ( ( get_green() & green_mask ) << green_shift )
			| ( ( get_blue() & blue_mask ) << blue_shift ) );
	}
	inline void set_using_u16( u16 the_u16 )
	{
		set_red( ( the_u16 & red_mask ) >> red_shift );
		set_green( ( the_u16 & green_mask ) >> green_shift );
		set_blue( ( the_u16 & blue_mask ) >> blue_shift );
	}
	
	inline sf::Color to_sfml_color() const
	{
		return sf::Color( get_red() << rgb24_shift, 
			get_green() << rgb24_shift, get_blue() << rgb24_shift );
	}
	inline void set_using_sfml_color( const sf::Color& the_sfml_color )
	{
		set_red( the_sfml_color.r >> rgb24_shift );
		set_green( the_sfml_color.g >> rgb24_shift );
		set_blue( the_sfml_color.b >> rgb24_shift );
	}
	
	inline png::color to_png_color() const
	{
		return sfml_color_to_png_color(to_sfml_color());
	}
	inline void set_using_png_color( const png::color& the_png_color )
	{
		set_using_sfml_color(png_color_to_sfml_color(the_png_color));
	}
};




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

struct sfml_color_compare_with_lightness
{
public:		// functions
	inline bool operator () ( const sf::Color& lhs, 
		const sf::Color& rhs ) const
	{
		auto color_max_3 = [&]( const sf::Color& the_color ) -> u8
		{
			return ( ( the_color.r >= the_color.g 
				&& the_color.r >= the_color.b ) 
				? the_color.r 
				: ( ( the_color.g > the_color.r 
				&& the_color.g > the_color.b ) 
				? the_color.g : the_color.b ) );
		};
		auto color_min_3 = [&]( const sf::Color& the_color ) -> u8
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

struct sfml_color_compare_with_hue
{
public:		// functions
	inline bool operator () ( const sf::Color& lhs, 
		const sf::Color& rhs ) const
	{
		auto color_max_3 = [&]( const sf::Color& the_color ) -> u8
		{
			return ( ( the_color.r >= the_color.g 
				&& the_color.r >= the_color.b ) 
				? the_color.r 
				: ( ( the_color.g > the_color.r 
				&& the_color.g > the_color.b ) 
				? the_color.g : the_color.b ) );
		};
		auto color_min_3 = [&]( const sf::Color& the_color ) -> u8
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
		
		u8 lhs_c = lhs_max - lhs_min;
		u8 rhs_c = rhs_max - rhs_min;
		
		// Consider the hue to be 0 if C == 0.  Yes, this is a convention
		// thing.
		if ( lhs_c == 0 )
		{
			return false;
		}
		else if ( lhs_c > 0 && rhs_c == 0 )
		{
			return true;
		}
		
		double lhs_hue, rhs_hue;
		
		if ( lhs_max == lhs.r )
		{
			lhs_hue = fmod( ( (double)(lhs.g) - (double)(lhs.b) )
				/ (double)(lhs_c), 6.0f );
		}
		else if ( lhs_max == lhs.g )
		{
			lhs_hue = ( ( (double)(lhs.b) - (double)(lhs.r) ) 
				/ (double)(lhs_c) ) + 2.0f;
		}
		else //if ( lhs_max == lhs.b )
		{
			lhs_hue = ( ( (double)(lhs.r) - (double)(lhs.g) ) 
				/ (double)(lhs_c) ) + 4.0f;
		}
		
		if ( rhs_max == rhs.r )
		{
			rhs_hue = fmod( ( (double)(rhs.g) - (double)(rhs.b) )
				/ (double)(rhs_c), 6.0f );
		}
		else if ( rhs_max == rhs.g )
		{
			rhs_hue = ( ( (double)(rhs.b) - (double)(rhs.r) ) 
				/ (double)(rhs_c) ) + 2.0f;
		}
		else //if ( rhs_max == rhs.b )
		{
			rhs_hue = ( ( (double)(rhs.r) - (double)(rhs.g) ) 
				/ (double)(rhs_c) ) + 4.0f;
		}
		
		return ( lhs_hue < rhs_hue );
	}
};


#endif		// color_stuff_hpp
