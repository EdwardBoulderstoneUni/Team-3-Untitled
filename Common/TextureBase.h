#pragma once

namespace NCL
{
	namespace Rendering
	{
		class TextureBase
		{
		public:
			virtual ~TextureBase() = default;
			unsigned get_width() const { return width_; }
			unsigned get_height() const { return height_; }
			void reserve(unsigned address);
			bool is_reserved() const { return reserved_; }
			unsigned get_reserved_address() const { return reserved_address_; }

		protected:
			TextureBase();
			unsigned width_;
			unsigned height_;
			bool reserved_;
			unsigned reserved_address_;
		};
	}
}
