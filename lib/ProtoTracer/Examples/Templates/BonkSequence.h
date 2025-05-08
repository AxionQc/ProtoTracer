#include "../../Assets/Textures/Animated/Utils/ImageSequenceRGB.h"

class BonkSequence : public ImageSequenceRGB{
private:
	static const uint8_t frame0000[];
	static const uint8_t frame0001[];
	static const uint8_t frame0002[];
	static const uint8_t frame0003[];
	static const uint8_t frame0004[];
	static const uint8_t frame0005[];
	static const uint8_t frame0006[];
	static const uint8_t frame0007[];
	static const uint8_t frame0008[];
	static const uint8_t frame0009[];
	static const uint8_t frame0010[];
	static const uint8_t frame0011[];
	static const uint8_t frame0012[];
	static const uint8_t frame0013[];
	static const uint8_t frame0014[];
	static const uint8_t frame0015[];

	static const uint8_t rgbColors0000[];
	static const uint8_t rgbColors0001[];
	static const uint8_t rgbColors0002[];
	static const uint8_t rgbColors0003[];
	static const uint8_t rgbColors0004[];
	static const uint8_t rgbColors0005[];
	static const uint8_t rgbColors0006[];
	static const uint8_t rgbColors0007[];
	static const uint8_t rgbColors0008[];
	static const uint8_t rgbColors0009[];
	static const uint8_t rgbColors0010[];
	static const uint8_t rgbColors0011[];
	static const uint8_t rgbColors0012[];
	static const uint8_t rgbColors0013[];
	static const uint8_t rgbColors0014[];
	static const uint8_t rgbColors0015[];

	static const uint8_t* sequence[16];


	static const uint8_t* RGBsequence[16];

	Image image = Image(frame0000, rgbColors0000, 150, 150, 8);

public:
	BonkSequence(Vector2D size, Vector2D offset, float fps) : ImageSequenceRGB(&image, sequence, RGBsequence, (unsigned int)16, fps) {
		image.SetSize(size);
		image.SetPosition(offset);
	}
};

