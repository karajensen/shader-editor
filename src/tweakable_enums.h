
////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_enums.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace Tweakable
{
    namespace Mesh
    {
        enum Attribute
        {
            Ambience,
            Bump,
            CausticsAmount,
            CausticsScale,
            Specularity,
            Max
        };
        const char* toString(Attribute value);
    }

    namespace Terrain
    {
        enum Attribute
        {
            Ambience,
            Bump,
            CausticsAmount,
            CausticsScale,
            Specularity,
            MinHeight,
            MaxHeight,
            Scale,
            Max
        };
        const char* toString(Attribute value);
    }

    namespace Water
    {
        enum Attribute
        {
            Bump,
            BumpScaleX,
            BumpScaleY,
            Speed,
            ScaleU,
            ScaleV,
            FresnalScale,
            FresnalBias,
            FresnalPower,
            ShallowR,
            ShallowG,
            ShallowB,
            ShallowA,
            DeepR,
            DeepG,
            DeepB,
            DeepA,
            ReflectionR,
            ReflectionG,
            ReflectionB,
            Reflection,
            Max
        };
        const char* toString(Attribute value);
    }

    namespace Wave
    {
        enum Attribute
        {
            Frequency,
            Amplitude,
            Phase,
            DirectionX,
            DirectionZ,
            Max
        };
        const char* toString(Attribute value);
    }

    namespace Light
    {
        enum Attribute
        {
            PositionX,
            PositionY,
            PositionZ,
            AttenuationX,
            AttenuationY,
            AttenuationZ,
            Specularity,
            DiffuseR,
            DiffuseG,
            DiffuseB,
            SpecularR,
            SpecularG,
            SpecularB,
            Active,
            Max
        };
        const char* toString(Attribute value);
    }

    namespace Camera
    {
        enum Attribute
        {
            PositionX,
            PositionY,
            PositionZ,
            Pitch,
            Yaw,
            Roll,
            ForwardSpd,
            RotationSpd,
            Max
        };
        const char* toString(Attribute value);
    }

    namespace Texture
    {
        enum Attribute
        {
            Scale,
            Amplitude,
            Contrast,
            Iterations,
            Max
        };
        const char* toString(Attribute value);
    }

    namespace Emitter
    {
        enum Attribute
        {
            LifeTime,
            LifeFade,
            Width,
            Length,
            DirX,
            DirY,
            DirZ,
            TintR,
            TintG,
            TintB,
            MinSpeed,
            MaxSpeed,
            MinFreq,
            MaxFreq,
            MinAmp,
            MaxAmp,
            MinSize,
            MaxSize,
            MinWait,
            MaxWait,
            Max
        };
        const char* toString(Attribute value);
    }

    namespace Post
    {
        enum Attribute
        {
            Contrast,
            Saturation,
            CausticSpeed,
            BlurStep,
            DofStart,
            DofFade,
            DepthNear,
            DepthFar,
            BloomAmount,
            BloomStart,
            BloomFade,
            FogStart,
            FogFade,
            FogRed,
            FogGreen,
            FogBlue,
            MinRed,
            MinGreen,
            MinBlue,
            MaxRed,
            MaxGreen,
            MaxBlue,
            Max
        };
        const char* toString(Attribute value);
    }

    namespace GuiPage
    {
        enum Page
        {
            Scene,
            Area,
            Mesh,
            Post,
            None
        };
        const char* toString(Page value);
    }
}

