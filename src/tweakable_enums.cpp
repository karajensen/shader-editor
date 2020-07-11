////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_enums.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweakable_enums.h"

namespace Tweakable
{
    const char* Mesh::toString(Mesh::Attribute value)
    {
        switch (value)
        {
        case Ambience:
            return "Ambience";
        case Bump:
            return "Bump";
        case CausticsAmount:
            return "Caustics Amount";
        case CausticsScale:
            return "Caustics Scale";
        case Specularity:
            return "Specularity";
        }
        return "";
    }

    Mesh::Group Mesh::group(Mesh::Attribute)
    {
        return None;
    }

    const char* Terrain::toString(Terrain::Attribute value)
    {
        switch (value)
        {
        case Ambience:
            return "Ambience";
        case Bump:
            return "Bump";
        case CausticsAmount:
            return "Caustics Amount";
        case CausticsScale:
            return "Caustics Scale";
        case Specularity:
            return "Specularity";
        case MinHeight:
            return "Min Height";
        case MaxHeight:
            return "Max Height";
        case Scale:
            return "Scale";
        }
        return "";
    }

    Terrain::Group Terrain::group(Terrain::Attribute)
    {
        return None;
    }

    const char* Water::toString(Water::Attribute value)
    {
        switch (value)
        {
        case Bump:
            return "Bump";
        case BumpScaleX:
            return "Bump X";
        case BumpScaleY:
            return "Bump Y";
        case Speed:
            return "Speed";
        case ScaleU:
            return "Scale U";
        case ScaleV:
            return "Scale V";
        case FresnalScale:
            return "Fresnal Scale";
        case FresnalBias:
            return "Fresnal Bias";
        case FresnalPower:
            return "Fresnal Power";
        case ShallowR:
            return "Shallow R";
        case ShallowG:
            return "Shallow G";
        case ShallowB:
            return "Shallow B";
        case ShallowA:
            return "Shallow A";
        case DeepR:
            return "Deep R";
        case DeepG:
            return "Deep G";
        case DeepB:
            return "Deep B";
        case DeepA:
            return "Deep A";
        case ReflectionR:
            return "Reflection R";
        case ReflectionG:
            return "Reflection G";
        case ReflectionB:
            return "Reflection B";
        case Reflection:
            return "Reflection";
        }
        return "";
    }

    Water::Group Water::group(Water::Attribute)
    {
        return None;
    }

    const char* Wave::toString(Wave::Attribute value)
    {
        switch (value)
        {
        case Frequency:
            return "Frequency";
        case Amplitude:
            return "Amplitude";
        case Phase:
            return "Phase";
        case DirectionX:
            return "Direction X";
        case DirectionZ:
            return "Direction Z";
        }
        return "";
    }

    Wave::Group Wave::group(Wave::Attribute)
    {
        return None;
    }

    const char* Light::toString(Light::Attribute value)
    {
        switch (value)
        {
        case PositionX:
            return "Position X";
        case PositionY:
            return "Position Y";
        case PositionZ:
            return "Position Z";
        case AttenuationX:
            return "Attenuation X";
        case AttenuationY:
            return "Attenuation Y";
        case AttenuationZ:
            return "Attenuation Z";
        case Specularity:
            return "Specularity";
        case DiffuseR:
            return "Diffuse R";
        case DiffuseG:
            return "Diffuse G";
        case DiffuseB:
            return "Diffuse B";
        case SpecularR:
            return "Specular R";
        case SpecularG:
            return "Specular G";
        case SpecularB:
            return "Specular B";
        case Active:
            return "Activity";
        }
        return "";
    }

    Light::Group Light::group(Light::Attribute value)
    {
        switch (value)
        {
        case PositionX:
        case PositionY:
        case PositionZ:
            return Position;
        case AttenuationX:
        case AttenuationY:
        case AttenuationZ:
            return Attenuation;
        case DiffuseR:
        case DiffuseG:
        case DiffuseB:
            return Diffuse;
        case Specularity:
        case SpecularR:
        case SpecularG:
        case SpecularB:
            return Specular;
        }
        return None;
    }

    const char* Camera::toString(Camera::Attribute value)
    {
        switch (value)
        {
        case PositionX:
            return "Position X";
        case PositionY:
            return "Position Y";
        case PositionZ:
            return "Position Z";
        case Pitch:
            return "Pitch";
        case Yaw:
            return "Yaw";
        case Roll:
            return "Roll";
        case ForwardSpd:
            return "Forward Speed";
        case RotationSpd:
            return "Rotation Speed";
        }
        return "";
    }

    Camera::Group Camera::group(Camera::Attribute)
    {
        return None;
    }

    const char* Texture::toString(Texture::Attribute value)
    {
        switch (value)
        {
        case Scale:
            return "Scale";
        case Amplitude:
            return "Amplitude";
        case Contrast:
            return "Contrast";
        case Iterations:
            return "Iterations";
        }
        return "";
    }

    Texture::Group Texture::group(Texture::Attribute)
    {
        return None;
    }

    const char* Emitter::toString(Emitter::Attribute value)
    {
        switch (value)
        {
        case LifeTime:
            return "LifeTime";
        case LifeFade:
            return "LifeFade";
        case Width:
            return "Width";
        case Length:
            return "Length";
        case DirX:
            return "Direction X";
        case DirY:
            return "Direction Y";
        case DirZ:
            return "Direction Z";
        case TintR:
            return "Tint R";
        case TintG:
            return "Tint G";
        case TintB:
            return "Tint B";
        case MinSpeed:
            return "Min Speed";
        case MaxSpeed:
            return "Max Speed";
        case MinFreq:
            return "Min Frequency";
        case MaxFreq:
            return "Max Frequency";
        case MinAmp:
            return "Min Amplitude";
        case MaxAmp:
            return "Max Amplitude";
        case MinSize:
            return "Min Size";
        case MaxSize:
            return "Max Size";
        case MinWait:
            return "Min Wait";
        case MaxWait:
            return "Max Wait";
        }
        return "";
    }

    Emitter::Group Emitter::group(Emitter::Attribute value)
    {
        switch (value)
        {
        case MinSpeed:
        case MaxSpeed:
        case MinFreq:
        case MaxFreq:
        case MinAmp:
        case MaxAmp:
        case MinSize:
        case MaxSize:
        case MinWait:
        case MaxWait:
            return MinMax;
        }
        return None;
    }

    const char* Post::toString(Post::Attribute value)
    {
        switch (value)
        {
        case Contrast:
            return "Contrast";
        case Saturation:
            return "Saturation";
        case CausticSpeed:
            return "Caustic Speed";
        case BlurStep:
            return "Blur Step";
        case DofStart:
            return "DOF Start";
        case DofFade:
            return "DOF Fade";
        case DepthNear:
            return "Depth Near";
        case DepthFar:
            return "Depth Far";
        case BloomAmount:
            return "Bloom Amount";
        case BloomStart:
            return "Bloom Start";
        case BloomFade:
            return "Bloom Fade";
        case FogStart:
            return "Fog Start";
        case FogFade:
            return "Fog Fade";
        case FogRed:
            return "Fog R";
        case FogGreen:
            return "Fog G";
        case FogBlue:
            return "Fog B";
        case MinRed:
            return "Min R";
        case MinGreen:
            return "Min G";
        case MinBlue:
            return "Min B";
        case MaxRed:
            return "Max R";
        case MaxGreen:
            return "Max G";
        case MaxBlue:
            return "Max B";
        }
        return "";
    }

    Post::Group Post::group(Post::Attribute value)
    {
        switch (value)
        {
        case Contrast:
        case Saturation:
        case MinRed:
        case MinGreen:
        case MinBlue:
        case MaxRed:
        case MaxGreen:
        case MaxBlue:
            return Correction;
        case FogStart:
        case FogFade:
        case FogRed:
        case FogGreen:
        case FogBlue:
            return Fog;
        }
        return None;
    }

    const char* GuiPage::toString(GuiPage::Page value)
    {
        switch (value)
        {
        case Scene:
            return "Scene";
        case Area:
            return "Area";
        case Mesh:
            return "Mesh";
        case Post:
            return "Post";
        }
        return "";
    }
}
