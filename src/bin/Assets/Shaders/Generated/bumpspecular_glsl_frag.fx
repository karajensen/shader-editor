
#version 150

out vec4 out_Color[2];

in float ex_Depth;
in vec2 ex_UVs;
in vec3 ex_PositionWorld;
in vec3 ex_Normal;
in vec3 ex_Tangent;
in vec3 ex_Bitangent;
in vec3 ex_VertToCamera;

uniform float lightActive[1];
uniform vec3 lightPosition[1];
uniform vec3 lightDiffuse[1];
uniform vec3 lightAttenuation[1];
uniform vec3 lightSpecular[1];
uniform float lightSpecularity[1];

uniform float meshAmbience;
uniform float meshDiffuse;
uniform float meshBump;
uniform float meshSpecular;
uniform float meshSpecularity;

uniform sampler2D DiffuseSampler;
uniform sampler2D NormalSampler;
uniform sampler2D SpecularSampler;

void main(void)
{
    vec4 diffuseTex = texture(DiffuseSampler, ex_UVs);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);

    vec3 normal = normalize(ex_Normal);

    vec4 normalTex = texture(NormalSampler, ex_UVs);
    vec2 bump = meshBump * (normalTex.rg - 0.5);
    normal = normalize(normal + bump.x * normalize(ex_Tangent) 
        + bump.y * normalize(ex_Bitangent));

    vec3 vertToCamera = normalize(ex_VertToCamera);
    vec4 specularTex = texture(SpecularSampler, ex_UVs);
    vec3 specular = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < 1; ++i)
    {
        vec3 lightColour = lightDiffuse[i];
        vec3 vertToLight = lightPosition[i] - ex_PositionWorld;
        float lightLength = length(vertToLight);
    
        float attenuation = 1.0 / (lightAttenuation[i].x 
            + lightAttenuation[i].y * lightLength 
            + lightAttenuation[i].z * lightLength * lightLength);

        vertToLight /= lightLength;

        lightColour *= ((dot(vertToLight, normal) + 1.0) *
            ((1.0 - meshDiffuse) * 0.5)) + meshDiffuse;

        diffuse += lightColour * attenuation * lightActive[i];

        float specularity = lightSpecularity[i] * meshSpecularity;
        vec3 halfVector = normalize(vertToLight + vertToCamera);
        float specularFactor = pow(max(dot(normal, halfVector), 0.0), specularity); 
        specular += specularFactor * lightSpecular[i] * 
            attenuation * lightActive[i] * meshSpecular;
    }

    out_Color[0].rgb = diffuseTex.rgb * diffuse;
    out_Color[0].rgb += specularTex.rgb * specular;
    out_Color[0].rgb *= meshAmbience;
    out_Color[0].a = 1.0;
    out_Color[1] = vec4(ex_Depth, ex_Depth, ex_Depth, 1.0);
}
