#include "font.hpp"

#include <algorithm>

#include "core/memory.hpp"
#include "platform/platform-font-loader.hpp"

#define MIN_CHAR '!' // Char number 33
#define MAX_CHAR '~' // Char number 126
#define CHAR_TEXTURE_HEIGHT 256

struct CharacterData {
	char symbol;
	
	uint32 width;
	uint32 height;
	uint8* data;

	int32 bearingX;
	int32 bearingY;

	uint32 advance;
};

static inline uint32 leastPOT(uint32 n) {
	uint32 res = 1;

	while (n) {
		n >>= 1;
		res <<= 1;
	}

	return res;
}

static bool loadCharacter(FT_Face& face, char c, Array<CharacterData>& charData);
static void blit(uint8* dest, uint8* src, int32 x, int32 y,
		int32 srcWidth, int32 srcHeight,
		int32 destWidth, int32 destHeight);

FontLoader::FontLoader()
		: fontLoader(new PlatformFontLoader()) {
	if (FT_Init_FreeType(&fontLoader->lib) != 0) {
		DEBUG_LOG(LOG_ERROR, LOG_TYPE_FONT_LOADER, "Error initializing FreeType Library");
	}
}

void FontLoader::loadFont(const String& fontFile, uint32 fontSize, Array<Font>& fonts) {
	FT_Face face;

	if (FT_New_Face(fontLoader->lib, fontFile.c_str(), 0, &face) != 0) {
		DEBUG_LOG(LOG_ERROR, LOG_TYPE_FONT_LOADER, "Error loading font %s", fontFile.c_str());
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	Array<CharacterData> charData;
	uint32 totalWidth = 0;

	for (char c = MIN_CHAR; c <= MAX_CHAR; ++c) {
		if (!loadCharacter(face, c, charData)) {
			DEBUG_LOG(LOG_ERROR, LOG_TYPE_FONT_LOADER, "Error loading font char %c for font %s",
					c, fontFile.c_str());
		}

		totalWidth += charData.back().width;
	}

	if (!loadCharacter(face, ' ', charData)) {
		DEBUG_LOG(LOG_ERROR, LOG_TYPE_FONT_LOADER, "Error loading font char %c for font %s",
				' ', fontFile.c_str());
	}

	totalWidth += charData.back().width;

	std::sort(std::begin(charData), std::end(charData), [](CharacterData& a, CharacterData& b) {
		return a.width > b.width;
	});

	uint32 numRows = CHAR_TEXTURE_HEIGHT / fontSize;
	uint32 rowWidthPx = leastPOT(totalWidth / numRows);

	uint8* fontBitmap = (uint8*)Memory::malloc(rowWidthPx * CHAR_TEXTURE_HEIGHT);
	Memory::memset(fontBitmap, 0, rowWidthPx * CHAR_TEXTURE_HEIGHT);

	uint32 rowX = 0;
	uint32 rowY = 0;

	float invWidth = 1.f / (float)(rowWidthPx);
	float invHeight = 1.f / (float)(CHAR_TEXTURE_HEIGHT);

	fonts.emplace_back(fontSize, rowWidthPx, CHAR_TEXTURE_HEIGHT);
	Font& font = fonts.back();

	for (auto data = std::begin(charData), end = std::end(charData); data != end; ++data) {
		if (rowX + data->width > rowWidthPx) {
			rowX = 0;
			rowY += fontSize;
		}

		blit(fontBitmap, data->data,
				rowX, rowY, data->width, data->height, rowWidthPx, CHAR_TEXTURE_HEIGHT);

		Font::Character chr;
		chr.bearingX = data->bearingX;
		chr.bearingY = data->bearingY;
		chr.advance = (float)(data->advance >> 6);

		chr.sizeX = (float)(data->width);
		chr.sizeY = (float)(data->height);

		chr.texCoordData[0] = (float)(rowX) * invWidth;
		chr.texCoordData[1] = (float)(rowY) * invHeight;
		chr.texCoordData[2] = (float)(data->width) * invWidth;
		chr.texCoordData[3] = (float)(data->height) * invHeight;

		font.characters.emplace(data->symbol, chr);

		rowX += data->width;

		Memory::free(data->data);
	}

	Memory::memcpy(&font.data[0], fontBitmap, rowWidthPx * CHAR_TEXTURE_HEIGHT);

	Memory::free(fontBitmap);
	FT_Done_Face(face);
}

FontLoader::~FontLoader() {
	FT_Done_FreeType(fontLoader->lib);
}

static bool loadCharacter(FT_Face& face, char c, Array<CharacterData>& charData) {
	if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0) {
		return false;
	}

	uint32 width = (uint32)face->glyph->bitmap.width;
	uint32 height = (uint32)face->glyph->bitmap.rows;


	CharacterData cd;

	cd.symbol = c;

	cd.width = width;
	cd.height = height;
	cd.data = (uint8*)Memory::malloc(width * height);

	cd.bearingX = (int32)face->glyph->bitmap_left;
	cd.bearingY = (int32)face->glyph->bitmap_top;

	cd.advance = (uint32)face->glyph->advance.x;
	
	Memory::memcpy(cd.data, face->glyph->bitmap.buffer, width * height);

	charData.push_back(cd);

	return true;
}

static void blit(uint8* dest, uint8* src, int32 destX, int32 destY,
		int32 srcWidth, int32 srcHeight,
		int32 destWidth, int32 destHeight) {
	for (uint32 y = 0; y < srcHeight; ++y) {
		for (uint32 x = 0; x < srcWidth; ++x) {
			if (src[y * srcWidth + x] > 0) {
				dest[destWidth * (destY + y) + destX + x] = src[y * srcWidth + x];
			}
		}
	}
}
