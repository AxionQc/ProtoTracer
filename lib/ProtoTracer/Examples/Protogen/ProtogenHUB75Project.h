#pragma once

#include "../Templates/ProtogenProjectTemplate.h"
#include "../../Assets/Models/OBJ/DeltaDisplayBackground.h"
#include "../../Assets/Models/FBX/NukudeFlat.h"

#include "../../Camera/CameraManager/Implementations/HUB75DeltaCameras.h"
#include "../../Controller/HUB75Controller.h"

class ProtogenHUB75Project : public ProtogenProject {
bool animationEnabled = false;
bool Bonked = false;

private:
    HUB75DeltaCameraManager cameras;
    HUB75Controller controller = HUB75Controller(&cameras, 50, 50);
    NukudeFace pM;
    DeltaDisplayBackground deltaDisplayBackground;
    
    bool isObjectInScene = true; 

    const __FlashStringHelper* faceArray[10] = {F("DEFAULT"), F("ANGRY"), F("DOUBT"), F("FROWN"), F("LOOKUP"), F("SAD"), F("BONK"), F("AUDIO1"), F("AUDIO2"), F("AUDIO3")};

    void LinkControlParameters() override {//Called from parent
        AddParameter(NukudeFace::Anger, pM.GetMorphWeightReference(NukudeFace::Anger), 15);
        AddParameter(NukudeFace::Sadness, pM.GetMorphWeightReference(NukudeFace::Sadness), 15, IEasyEaseAnimator::InterpolationMethod::Cosine);
        AddParameter(NukudeFace::Surprised, pM.GetMorphWeightReference(NukudeFace::Surprised), 15);
        AddParameter(NukudeFace::Doubt, pM.GetMorphWeightReference(NukudeFace::Doubt), 15);
        AddParameter(NukudeFace::Frown, pM.GetMorphWeightReference(NukudeFace::Frown), 15);
        AddParameter(NukudeFace::LookUp, pM.GetMorphWeightReference(NukudeFace::LookUp), 15);
        AddParameter(NukudeFace::LookDown, pM.GetMorphWeightReference(NukudeFace::LookDown), 15);

        AddParameter(NukudeFace::HideBlush, pM.GetMorphWeightReference(NukudeFace::HideBlush), 15, IEasyEaseAnimator::InterpolationMethod::Cosine, true);

        AddViseme(Viseme::MouthShape::EE, pM.GetMorphWeightReference(NukudeFace::vrc_v_ee));
        AddViseme(Viseme::MouthShape::AH, pM.GetMorphWeightReference(NukudeFace::vrc_v_aa));
        AddViseme(Viseme::MouthShape::UH, pM.GetMorphWeightReference(NukudeFace::vrc_v_dd));
        AddViseme(Viseme::MouthShape::AR, pM.GetMorphWeightReference(NukudeFace::vrc_v_rr));
        AddViseme(Viseme::MouthShape::ER, pM.GetMorphWeightReference(NukudeFace::vrc_v_ch));
        AddViseme(Viseme::MouthShape::OO, pM.GetMorphWeightReference(NukudeFace::vrc_v_oh));
        AddViseme(Viseme::MouthShape::SS, pM.GetMorphWeightReference(NukudeFace::vrc_v_ss));

        AddBlinkParameter(pM.GetMorphWeightReference(NukudeFace::Blink));
    }
    void Default(){
    }

    void Angry(){
        AddParameterFrame(NukudeFace::Anger, 1.0f);
        AddMaterialFrame(Color::CRED);
        animationEnabled = false;
        Bonked = false;
    } 

    void Sad(){
        AddParameterFrame(NukudeFace::Sadness, 1.0f);
        AddParameterFrame(NukudeFace::Frown, 1.0f);
        AddMaterialFrame(Color::CBLUE);
        animationEnabled = false;
        Bonked = false;
    }

    void Surprised(){
        AddParameterFrame(NukudeFace::Surprised, 1.0f);
        AddParameterFrame(NukudeFace::HideBlush, 0.0f);
        AddMaterialFrame(Color::CRAINBOW);
        animationEnabled = false;
        Bonked = false;
    }

    void Bonk(){
        animationEnabled = true;
        Bonked = true;
    }
    
    void Doubt(){
        AddParameterFrame(NukudeFace::Doubt, 1.0f);
        animationEnabled = false;
        Bonked = false;
    }
    
    void Frown(){
        AddParameterFrame(NukudeFace::Frown, 1.0f);
        animationEnabled = false;
        Bonked = false;
    }

    void LookUp(){
        AddParameterFrame(NukudeFace::LookUp, 1.0f);
        Bonked = false;
        animationEnabled = false;
    }

    void LookDown(){
        AddParameterFrame(NukudeFace::LookDown, 1.0f);
        Bonked = false;
        animationEnabled = false;
    }


    void SpectrumAnalyzerCallback() override {
        AddMaterialFrame(Color::CHORIZONTALRAINBOW, 0.8f);
        animationEnabled = false;
        Bonked = false;
    }

    void AudioReactiveGradientCallback() override {
        AddMaterialFrame(Color::CHORIZONTALRAINBOW, 0.8f);
        animationEnabled = false;
        Bonked = false;
    }

    void OscilloscopeCallback() override {
        AddMaterialFrame(Color::CHORIZONTALRAINBOW, 0.8f);
        animationEnabled = false;
        Bonked = false;
    }

public:
    ProtogenHUB75Project() : ProtogenProject(&cameras, &controller, 2, Vector2D(), Vector2D(192.0f, 94.0f), 22, 20, 10){
        scene.AddObject(pM.GetObject());
        scene.AddObject(deltaDisplayBackground.GetObject());

        pM.GetObject()->SetMaterial(GetFaceMaterial());
        deltaDisplayBackground.GetObject()->SetMaterial(GetFaceMaterial());

        hud.SetFaceArray(faceArray);

        isObjectInScene = true;

        LinkControlParameters();
        
        SetWiggleSpeed(5.0f);
        SetMenuWiggleSpeed(0.0f, 0.0f, 0.0f);
        SetMenuOffset(Vector2D(17.5f, -3.0f));
        SetMenuSize(Vector2D(192, 56));
    }

    void Update(float ratio) override {
        pM.Reset();
        uint8_t mode = Menu::GetFaceState();

        isObjectInScene = true;
        SetBackgroundMaterial(GetBackgroundMaterial());
            if (animationEnabled) {
                if (Bonked) {
                    SetBackgroundMaterial(&bonksequence);
                }
                isObjectInScene = false;
            }
            else if (!animationEnabled) {
                SetBackgroundMaterial(GetBackgroundMaterial());
                isObjectInScene = true;
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

        switch(code) {
            case 0: Default();  break;
            case 1: Angry();    break;
            case 2: Doubt();    break;
            case 3: Frown();    break;
            case 4: LookUp();   break;
            case 5: Sad();      break;
            case 6: Bonk();     break;
            case 7: AudioReactiveGradientFace();    break;
            case 8: OscilloscopeFace();             break;
            default: SpectrumAnalyzerFace();        break;
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