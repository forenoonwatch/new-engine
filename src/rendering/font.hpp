#pragma once

#include "core/common.hpp"
#include "containers/array.hpp"
#include "containers/hash-map.hpp"
#include "containers/string.hpp"

class FontLoader;
class PlatformFontLoader;

class Font {
	public:
		struct Character {
			float texCoordData[4]; // posX, posY, offsetX, offsetY
			float sizeX;
			float sizeY;
			
			int32 bearingX;
			int32 bearingY;
			float advance;
		};

		inline Font(uint32 fontSize, uint32 width, uint32 height,
					uint32 textureID = -1)
				: fontSize(fontSize)
				, width(width)
				, height(height)
				, textureID(textureID) {
			data.reserve(width * height);
		}

		inline Font(const Font& font)
				: fontSize(font.fontSize)
				, width(font.width)
				, height(font.height)
				, textureID(font.textureID)
				, data(std::move(font.data))
				, characters(std::move(font.characters)) {}

		inline void setTextureID(uint32 textureID) { this->textureID = textureID; }

		inline int32 getWidth() const { return width; }
		inline int32 getHeight() const { return height; }
		inline const uint8* getData() const { return &data[0]; }
		inline uint32 getTextureID() const { return textureID; }

		inline const Character& getCharacter(char c) { return characters[c]; }

	private:
		uint32 fontSize;
		uint32 width;
		uint32 height;
		Array<uint8> data;

		HashMap<char, Character> characters;

		uint32 textureID;

		friend class FontLoader;
};

class FontLoader final {
	public:
		FontLoader();

		void loadFont(const String& fontFile, uint32 fontSize, Array<Font>& fonts);

		~FontLoader();
	private:
		PlatformFontLoader* fontLoader;

		NULL_COPY_AND_ASSIGN(FontLoader);
};
