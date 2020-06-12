////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_enums.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace Tweakable
{
    struct Mesh
    {
        enum Group
        {
            None
        };
        enum Attribute
        {
            Ambience,
            Bump,
            CausticsAmount,
            CausticsScale,
            Specularity,
            Max
        };
        static const char* toString(Attribute value);
        static Group group(Attribute value);
    };

    struct Terrain
    {
        enum Group
        {
            None
        };
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
        static const char* toString(Attribute value);
        static Group group(Attribute value);
    };

    struct Water
    {
        enum Group
        {
            None
        };
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
        static const char* toString(Attribute value);
        static Group group(Attribute value);
    };

    struct Wave
    {
        enum Group
        {
            None
        };
        enum Attribute
        {
            Frequency,
            Amplitude,
            Phase,
            DirectionX,
            DirectionZ,
            Max
        };
        static const char* toString(Attribute value);
        static Group group(Attribute value);
    };

    struct Light
    {
        enum Group
        {
            None,
            Attenuation,
            Position,
            Specular,
            Diffuse
        };
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
        static const char* toString(Attribute value);
        static Group group(Attribute value);
    };

    struct Camera
    {
        enum Group
        {
            None
        };
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
        static const char* toString(Attribute value);
        static Group group(Attribute value);
    };

    struct Texture
    {
        enum Group
        {
            None
        };
        enum Attribute
        {
            Scale,
            Amplitude,
            Contrast,
            Iterations,
            Max
        };
        static const char* toString(Attribute value);
        static Group group(Attribute value);
    };

    struct Emitter
    {
        enum Group
        {
            None,
            MinMax
        };
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
        static const char* toString(Attribute value);
        static Group group(Attribute value);
    };

    struct Post
    {
        enum Group
        {
            None,
            Correction,
            Fog
        };
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
        static const char* toString(Attribute value);
        static Group group(Attribute value);
    };

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
    };
}
