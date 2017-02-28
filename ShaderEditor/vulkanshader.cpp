////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanshader.h"
#include "vulkanutils.h"
#include "vulkandata.h"
#include "glslang/SPIRV/GlslangToSpv.h"

namespace
{
    const std::string VS("Vertex Shader: ");   ///< Text for vertex shader diagnostics
    const std::string FS("Fragment Shader: "); ///< Text for fragment shader diagnostics

    EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type)
    {
        switch (shader_type)
        {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return EShLangVertex;
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
            return EShLangTessControl;
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
            return EShLangTessEvaluation;
        case VK_SHADER_STAGE_GEOMETRY_BIT:
            return EShLangGeometry;
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return EShLangFragment;
        case VK_SHADER_STAGE_COMPUTE_BIT:
            return EShLangCompute;
        default:
            return EShLangVertex;
        }
    }

    void InitResources(TBuiltInResource& resources)
    {
        resources.maxLights = 32;
        resources.maxClipPlanes = 6;
        resources.maxTextureUnits = 32;
        resources.maxTextureCoords = 32;
        resources.maxVertexAttribs = 64;
        resources.maxVertexUniformComponents = 4096;
        resources.maxVaryingFloats = 64;
        resources.maxVertexTextureImageUnits = 32;
        resources.maxCombinedTextureImageUnits = 80;
        resources.maxTextureImageUnits = 32;
        resources.maxFragmentUniformComponents = 4096;
        resources.maxDrawBuffers = 32;
        resources.maxVertexUniformVectors = 128;
        resources.maxVaryingVectors = 8;
        resources.maxFragmentUniformVectors = 16;
        resources.maxVertexOutputVectors = 16;
        resources.maxFragmentInputVectors = 15;
        resources.minProgramTexelOffset = -8;
        resources.maxProgramTexelOffset = 7;
        resources.maxClipDistances = 8;
        resources.maxComputeWorkGroupCountX = 65535;
        resources.maxComputeWorkGroupCountY = 65535;
        resources.maxComputeWorkGroupCountZ = 65535;
        resources.maxComputeWorkGroupSizeX = 1024;
        resources.maxComputeWorkGroupSizeY = 1024;
        resources.maxComputeWorkGroupSizeZ = 64;
        resources.maxComputeUniformComponents = 1024;
        resources.maxComputeTextureImageUnits = 16;
        resources.maxComputeImageUniforms = 8;
        resources.maxComputeAtomicCounters = 8;
        resources.maxComputeAtomicCounterBuffers = 1;
        resources.maxVaryingComponents = 60;
        resources.maxVertexOutputComponents = 64;
        resources.maxGeometryInputComponents = 64;
        resources.maxGeometryOutputComponents = 128;
        resources.maxFragmentInputComponents = 128;
        resources.maxImageUnits = 8;
        resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
        resources.maxCombinedShaderOutputResources = 8;
        resources.maxImageSamples = 0;
        resources.maxVertexImageUniforms = 0;
        resources.maxTessControlImageUniforms = 0;
        resources.maxTessEvaluationImageUniforms = 0;
        resources.maxGeometryImageUniforms = 0;
        resources.maxFragmentImageUniforms = 8;
        resources.maxCombinedImageUniforms = 8;
        resources.maxGeometryTextureImageUnits = 16;
        resources.maxGeometryOutputVertices = 256;
        resources.maxGeometryTotalOutputComponents = 1024;
        resources.maxGeometryUniformComponents = 1024;
        resources.maxGeometryVaryingComponents = 64;
        resources.maxTessControlInputComponents = 128;
        resources.maxTessControlOutputComponents = 128;
        resources.maxTessControlTextureImageUnits = 16;
        resources.maxTessControlUniformComponents = 1024;
        resources.maxTessControlTotalOutputComponents = 4096;
        resources.maxTessEvaluationInputComponents = 128;
        resources.maxTessEvaluationOutputComponents = 128;
        resources.maxTessEvaluationTextureImageUnits = 16;
        resources.maxTessEvaluationUniformComponents = 1024;
        resources.maxTessPatchComponents = 120;
        resources.maxPatchVertices = 32;
        resources.maxTessGenLevel = 64;
        resources.maxViewports = 16;
        resources.maxVertexAtomicCounters = 0;
        resources.maxTessControlAtomicCounters = 0;
        resources.maxTessEvaluationAtomicCounters = 0;
        resources.maxGeometryAtomicCounters = 0;
        resources.maxFragmentAtomicCounters = 8;
        resources.maxCombinedAtomicCounters = 8;
        resources.maxAtomicCounterBindings = 1;
        resources.maxVertexAtomicCounterBuffers = 0;
        resources.maxTessControlAtomicCounterBuffers = 0;
        resources.maxTessEvaluationAtomicCounterBuffers = 0;
        resources.maxGeometryAtomicCounterBuffers = 0;
        resources.maxFragmentAtomicCounterBuffers = 1;
        resources.maxCombinedAtomicCounterBuffers = 1;
        resources.maxAtomicCounterBufferSize = 16384;
        resources.maxTransformFeedbackBuffers = 4;
        resources.maxTransformFeedbackInterleavedComponents = 64;
        resources.maxCullDistances = 8;
        resources.maxCombinedClipAndCullDistances = 8;
        resources.maxSamples = 4;
        resources.limits.nonInductiveForLoops = 1;
        resources.limits.whileLoops = 1;
        resources.limits.doWhileLoops = 1;
        resources.limits.generalUniformIndexing = 1;
        resources.limits.generalAttributeMatrixVectorIndexing = 1;
        resources.limits.generalVaryingIndexing = 1;
        resources.limits.generalSamplerIndexing = 1;
        resources.limits.generalVariableIndexing = 1;
        resources.limits.generalConstantMatrixVectorIndexing = 1;
    }

    std::string GlslToSpb(const VkShaderStageFlagBits shader_type, const char* pshader, std::vector<unsigned int>& spirv)
    {
        std::string errorBuffer;

        EShLanguage stage = FindLanguage(shader_type);
        glslang::TShader shader(stage);
        glslang::TProgram program;
        const char *shaderStrings[1];

        TBuiltInResource resources;
        InitResources(resources);

        // Enable SPIR-V and Vulkan rules when parsing GLSL
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

        shaderStrings[0] = pshader;
        shader.setStrings(shaderStrings, 1);

        if (!shader.parse(&resources, 100, false, messages))
        {
            errorBuffer += shader.getInfoLog();
            errorBuffer += shader.getInfoDebugLog();
            return errorBuffer;
        }

        program.addShader(&shader);

        // Program-level processing
        if (!program.link(messages))
        {
            errorBuffer += shader.getInfoLog();
            errorBuffer += shader.getInfoDebugLog();
            return errorBuffer;
        }

        glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
        return errorBuffer;
    }
}

VkShader::VkShader(VulkanData& info)
    : m_info(info)
{
    // TODO: Make this generic for scene
    m_vertexText =
        "#version 400\n"
        "#extension GL_ARB_separate_shader_objects : enable\n"
        "#extension GL_ARB_shading_language_420pack : enable\n"
        "layout (std140, binding = 0) uniform bufferVals {\n"
        "    mat4 mvp;\n"
        "} myBufferVals;\n"
        "layout (location = 0) in vec4 pos;\n"
        "layout (location = 1) in vec4 inColor;\n"
        "layout (location = 0) out vec4 outColor;\n"
        "out gl_PerVertex { \n"
        "    vec4 gl_Position;\n"
        "};\n"
        "void main() {\n"
        "   outColor = inColor;\n"
        "   gl_Position = myBufferVals.mvp * pos;\n"
        "}\n";

    m_fragmentText =
        "#version 400\n"
        "#extension GL_ARB_separate_shader_objects : enable\n"
        "#extension GL_ARB_shading_language_420pack : enable\n"
        "layout (location = 0) in vec4 color;\n"
        "layout (location = 0) out vec4 outColor;\n"
        "void main() {\n"
        "   outColor = color;\n"
        "}\n";
}

VkShader::~VkShader()
{
    Release();
}

void VkShader::Release()
{
}

const std::string& VkShader::GetName() const
{
    return m_tempName;
}

bool VkShader::InitFramework()
{
    return glslang::InitializeProcess();
}

void VkShader::ReleaseFramework()
{
    glslang::FinalizeProcess();
}

std::string VkShader::CompileShader()
{
    VkShaderModuleCreateInfo moduleCreateInfo;
    std::vector<unsigned int> vtx_spv;
    m_info.shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_info.shaderStages[0].pNext = NULL;
    m_info.shaderStages[0].pSpecializationInfo = NULL;
    m_info.shaderStages[0].flags = 0;
    m_info.shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    m_info.shaderStages[0].pName = "main";

    const std::string vertErrors = GlslToSpb(VK_SHADER_STAGE_VERTEX_BIT, m_vertexText.c_str(), vtx_spv);
    if (!vertErrors.empty())
    {
        return VS + vertErrors;
    }

    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = vtx_spv.data();

    VkResult res = vkCreateShaderModule(m_info.device, &moduleCreateInfo, NULL, &m_info.shaderStages[0].module);
    if (VulkanUtils::Failed(res))
    {
        return VS + VulkanUtils::GetFailText(res);
    }

    std::vector<unsigned int> frag_spv;
    m_info.shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_info.shaderStages[1].pNext = NULL;
    m_info.shaderStages[1].pSpecializationInfo = NULL;
    m_info.shaderStages[1].flags = 0;
    m_info.shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    m_info.shaderStages[1].pName = "main";

    const std::string fragErrors = GlslToSpb(VK_SHADER_STAGE_FRAGMENT_BIT, m_fragmentText.c_str(), frag_spv);
    if(!fragErrors.empty())
    {
        return FS + fragErrors;
    }

    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = frag_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = frag_spv.data();
    res = vkCreateShaderModule(m_info.device, &moduleCreateInfo, NULL, &m_info.shaderStages[1].module);
    if (VulkanUtils::Failed(res))
    {
        return FS + VulkanUtils::GetFailText(res);
    }

    return "";
}