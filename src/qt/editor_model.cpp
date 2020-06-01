////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor_model.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "editor_model.h"

void EditorModel::InitialiseShaders(int selected, const std::vector<std::string>& shaders)
{
    m_shaders.clear();
    m_shaders.reserve(shaders.size());
    for (const auto& shader : shaders)
    {
        m_shaders.push_back(QString::fromStdString(shader));
    }
    emit ShadersChanged();

    SetShaderIndex(selected);
}

bool EditorModel::HasShaders() const
{
    return m_shaders.length() > 0;
}

void EditorModel::SetShaderText(const QString& text)
{
    if (m_shaderText != text)
    {
        m_shaderText = text;
        emit ShaderTextChanged();
    }
}

const QString& EditorModel::ShaderText() const
{
    return m_shaderText;
}

void EditorModel::SetShaderAssembly(const QString& assembly)
{
    if (m_shaderAssembly != assembly)
    {
        m_shaderAssembly = assembly;
        emit ShaderAssemblyChanged();
    }
}

const QString& EditorModel::ShaderAssembly() const
{
    return m_shaderAssembly;
}

void EditorModel::SetShaderIndex(int index)
{
    if (m_shaderIndex != index)
    {
        m_shaderIndex = index;
        emit ShaderIndexChanged(index);
    }
}

const QStringList& EditorModel::Shaders() const
{
    return m_shaders;
}

int EditorModel::ShaderIndex() const
{
    return m_shaderIndex;
}

void EditorModel::CompileSelectedShader(const QString& text)
{
    emit RequestCompileSelectedShader(text);
}