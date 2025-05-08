#pragma once

#include "Arduino.h"
#include "../../../../Scene/Materials/Static/Image.h"
#include "../../../../Scene/Materials/Material.h"


//Use this if your color space changes in your gif so it loads color spaces per frame.
//If you have a rainbow animation for example you'll need to sequence the RGB colors along with the frames.
class ImageSequenceRGB : public Material{
private:
    Image* image;
    const uint8_t** data;
    const uint8_t** rgb;
    unsigned long startTime = 0;
    unsigned int imageCount = 0;
    float fps = 24.0f;
    float frameTime = 0.0f;
    unsigned int currentFrame = 0;

        
protected:
    ImageSequenceRGB(Image* image, const uint8_t** data, const uint8_t** rgb, unsigned int imageCount, float fps){
        this->startTime = millis();
        this->image = image;
        this->data = data;
        this->rgb = rgb;
        this->imageCount = imageCount;
        this->fps = fps;
        this->frameTime = ((float)imageCount) / fps;
    }

public:
    void SetFPS(float fps){
        this->fps = fps;
    }
    
    void SetSize(Vector2D size){
        image->SetSize(size);
    }

    void SetPosition(Vector2D offset){
        image->SetPosition(offset);
    }

    void SetRotation(float angle){
        image->SetRotation(angle);
    }

    void SetHueAngle(float hueAngle){
        image->SetHueAngle(hueAngle);
    }

    void Reset(){
        startTime = millis();
    }

    void Update(){
        float currentTime = fmod((millis() - startTime) / 1000.0f, frameTime) / frameTime;//normalize time to ratio

        currentFrame = (unsigned int)Mathematics::Map(float(currentTime), 0.0f, 1.0f, 0.0f, float(imageCount - 1));

        image->SetColorPalette(rgb[currentFrame]);
        image->SetData(data[currentFrame]);

    }

    RGBColor GetRGB(const Vector3D& position, const Vector3D& normal, const Vector3D& uvw){
        return image->GetRGB(position, normal, uvw);
    }
};
