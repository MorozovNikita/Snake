#pragma once

namespace sf
{
	class Texture;
	class Font;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}

namespace SoundBuffers
{
	enum ID
	{

	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>		  TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>				  FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundBuffers::ID> SoundBufferHolder;