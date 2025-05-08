#pragma once

#include "../Templates/ProtogenProjectTemplate.h"
#include "../../Assets/Models/OBJ/DeltaDisplayBackground.h"
#include "../../Assets/Models/FBX/Axios.h"

#include "../../Camera/CameraManager/Implementations/HUB75DeltaCameras.h"
#include "../../Controller/HUB75Controller.h"


class ProtogenHUB75Project : public ProtogenProject {
bool animationEnabled = false;
bool Bombed = false;
bool Blushed = false;

private:
    HUB75DeltaCameraManager cameras;
    HUB75Controller controller = HUB75Controller(&cameras, 50, 50);
    Axios pM;
    DeltaDisplayBackground deltaDisplayBackground;

    bool isObjectInScene = true; // Track if pM.GetObject() is in the scene
    unsigned long timer = 0; // Timer to track elapsed time


    const __FlashStringHelper* faceArray[7] = {F("DEFAULT"), F("ANGRY"), F("SAD"), F("Blush"), F("AUDIO1"), F("AUDIO2"), F("AUDIO3")};

    void LinkControlParameters() override {
        AddParameter(Axios::Anger, pM.GetMorphWeightReference(Axios::Anger), 15);
        AddParameter(Axios::Sadness, pM.GetMorphWeightReference(Axios::Sadness), 15, IEasyEaseAnimator::InterpolationMethod::Cosine);
        AddParameter(Axios::Surprised, pM.GetMorphWeightReference(Axios::Surprised), 15);
        AddParameter(Axios::Doubt, pM.GetMorphWeightReference(Axios::Doubt), 15);
        AddParameter(Axios::Frown, pM.GetMorphWeightReference(Axios::Frown), 15);
        AddParameter(Axios::LookUp, pM.GetMorphWeightReference(Axios::LookUp), 15);
        AddParameter(Axios::LookDown, pM.GetMorphWeightReference(Axios::LookDown), 15);
        AddParameter(Axios::BiggerNose, pM.GetMorphWeightReference(Axios::BiggerNose), 15);

        AddParameter(Axios::HideBlush, pM.GetMorphWeightReference(Axios::HideBlush), 15, IEasyEaseAnimator::InterpolationMethod::Cosine, true);

        AddViseme(Viseme::MouthShape::EE, pM.GetMorphWeightReference(Axios::vrc_v_ee));
        AddViseme(Viseme::MouthShape::AH, pM.GetMorphWeightReference(Axios::vrc_v_aa));
        AddViseme(Viseme::MouthShape::UH, pM.GetMorphWeightReference(Axios::vrc_v_dd));
        AddViseme(Viseme::MouthShape::AR, pM.GetMorphWeightReference(Axios::vrc_v_rr));
        AddViseme(Viseme::MouthShape::ER, pM.GetMorphWeightReference(Axios::vrc_v_ch));
        AddViseme(Viseme::MouthShape::OO, pM.GetMorphWeightReference(Axios::vrc_v_oh));
        AddViseme(Viseme::MouthShape::SS, pM.GetMorphWeightReference(Axios::vrc_v_ss));

        AddBlinkParameter(pM.GetMorphWeightReference(Axios::Blink));
    }

    void Default() {
        animationEnabled = false;
        Bombed = false;
        Blushed = false;
    }

    void Angry(){
        AddParameterFrame(Axios::Anger, 1.0f);
        AddMaterialFrame(Color::CRED);
        animationEnabled = false;
        Bombed = false;
        Blushed = false;
    } 

    void Sad(){
        AddParameterFrame(Axios::Sadness, 1.0f);
        AddMaterialFrame(Color::CBLUE);
        animationEnabled = false;
        Bombed = false;
        Blushed = false;
    }

    void Surprised(){
        AddParameterFrame(Axios::Surprised, 1.0f);
        AddParameterFrame(Axios::HideBlush, 0.0f);
        AddMaterialFrame(Color::CBLACK);
        animationEnabled = false;
        Bombed = false;
        Blushed = false;    
    }

    void Blush() {
        Blushed = true;
        Bombed = false;
        animationEnabled = true;
    }

    void Bomb(){
        Bombed = true;
        Blushed = false;
        animationEnabled = true;
    }

    void Frown(){
        AddParameterFrame(Axios::Frown, 1.0f);
        animationEnabled = false;
        Bombed = false;
        Blushed = false;
    }

    void LookUp(){
        AddParameterFrame(Axios::LookUp, 1.0f);
        animationEnabled = false;
        Bombed = false;
        Blushed = false;
    }

    void LookDown(){
        AddParameterFrame(Axios::LookDown, 1.0f);
        animationEnabled = false;
        Bombed = false;
        Blushed = false;
    }


    void SpectrumAnalyzerCallback() override {
        AddMaterialFrame(Color::CHORIZONTALRAINBOW, 0.8f);
        animationEnabled = false;
        Bombed = false;
        Blushed = false;
    }

    void AudioReactiveGradientCallback() override {
        AddMaterialFrame(Color::CHORIZONTALRAINBOW, 0.8f);
        animationEnabled = false;
        Bombed = false;
        Blushed = false;
    }

    void OscilloscopeCallback() override {
        AddMaterialFrame(Color::CHORIZONTALRAINBOW, 0.8f);
        animationEnabled = false;
        Bombed = false;
        Blushed = false;
    }

public:
    ProtogenHUB75Project() : ProtogenProject(&cameras, &controller, 2, Vector2D(), Vector2D(192.0f, 94.0f), 23, 20, 7) {
        scene.AddObject(pM.GetObject());
        scene.AddObject(deltaDisplayBackground.GetObject());

        pM.GetObject()->SetMaterial(GetFaceMaterial());
        deltaDisplayBackground.GetObject()->SetMaterial(GetFaceMaterial());

        hud.SetFaceArray(faceArray);

        isObjectInScene = true;

        LinkControlParameters();
        
        SetWiggleSpeed(5.0f);
        SetMenuWiggleSpeed(0.0f, 0.0f, 0.0f);
        SetMenuOffset(Vector2D(0, -3.0f));
        SetMenuSize(Vector2D(192, 56));
    }

    void Update(float ratio) override {
        pM.Reset();
        
        timer++;

        // Ensure the first 5 seconds logic is handled in Update

        isObjectInScene = false;
        SetBackgroundMaterial(&aperturesequence);
        uint8_t mode = Menu::GetFaceState();

        isObjectInScene = true;
        SetBackgroundMaterial(GetBackgroundMaterial());
            if (animationEnabled) {
                if (Bombed) {
                    SetBackgroundMaterial(&bombsequence);
                }
                else if (Blushed) {
                    SetBackgroundMaterial(&blushsequence);
                }
                isObjectInScene = false;
            }
            else if (!animationEnabled) {
                SetBackgroundMaterial(GetBackgroundMaterial());
                isObjectInScene = true;
            }
        }
        UpdateFace(ratio); 

        controller.SetBrightness(Menu::GetBrightness());
        controller.SetAccentBrightness(Menu::GetAccentBrightness());

#ifdef MORSEBUTTON
        SelectFaceFromMorse(mode);
#else
        SelectFace(mode);
#endif

        pM.Update();

        AlignObjectFace(pM.GetObject(), -7.5f);

        if (isObjectInScene) {
            pM.GetObject()->GetTransform()->SetPosition(GetWiggleOffset());
            pM.GetObject()->UpdateTransform();
        } else {
            pM.GetObject()->GetTransform()->SetPosition(Vector3D(500, 500, 0)); // Reset position when hidden
            pM.GetObject()->UpdateTransform();
        }

    }

  void SelectFace(uint8_t code) {
        if (IsBooped() && code != 6) {
            Surprised();
            return;
        }

        switch (code) {
            case 0: Default();  break;
            case 1: Blush();    break;
            case 2: Bomb();    break;
            case 3: Angry();    break;
            case 4: Sad();      break;
            case 10: Surprised();   break;
            case 5: AudioReactiveGradientFace();   break;
            case 6: OscilloscopeFace();            break;
            default: Default();        break;
        }
    }

    void SelectFaceFromMorse(uint8_t code) {
        if (IsBooped() && code != 24) {
            Surprised();
            return;
        }

        switch(code) {
            case 1: Angry();        break; // [A]ngry
            case 2: Surprised();    break; // [B]lush
            case 4: Doubt();        break; // [D]oubt
            case 6: Frown();        break; // [F]rown
            case 19: Sad();         break; // [S]ad
            case 21: LookUp();      break; // Look [U]p
            case 22: LookDown();    break; // Look [V] Down
            case 24: AudioReactiveGradientFace();   break; // [X] X.X
            case 25: OscilloscopeFace();            break; // [Y] Oscilloscope
            case 26: SpectrumAnalyzerFace();        break; // [Z] Spectrum
            default: Default();     break; // [H] Happy
        }
    }
};