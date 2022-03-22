#pragma once

namespace NCL
{
	namespace Rendering
	{
		class TextureBase
		{
		public:
			virtual ~TextureBase();
			unsigned get_width() const { return width_; }
			unsigned get_height() const { return height_; }
		protected:
			TextureBase();
			unsigned width_;
			unsigned height_;
		};
	}
}
