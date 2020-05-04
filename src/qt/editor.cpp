////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "editor.h"

Editor::Editor(SignalCallbacks& callbacks)
    : m_callbacks(callbacks)
{
}

void Editor::InitialiseShaders(int selected, const std::vector<std::string>& shaders)
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

bool Editor::HasShaders() const
{
    return m_shaders.length() > 0;
}

void Editor::SetShaderText(const std::string& text)
{
    //m_text = QString(text.c_str());
    //m_ui.shaderTextBox->setText(m_text);
}

void Editor::SetShaderAssembly(const std::string& assembly)
{
    //m_assembly = QString(assembly.c_str());
    //m_ui.assemblyTextBox->setText(m_assembly);
}

void Editor::CompileSelectedShader()
{
    //m_text = m_ui.shaderTextBox->toPlainText();
    //m_callbacks.CompileShader(m_text.toUtf8().constData());
}

void Editor::RevertSelectedShader()
{
    //m_ui.shaderTextBox->setText(m_text);
    //m_ui.assemblyTextBox->setText(m_assembly);
}

const QStringList& Editor::Shaders() const
{
    return m_shaders;
}

void Editor::SetShaderIndex(int index)
{
    if (m_shaderIndex != index)
    {
        m_shaderIndex = index;
        emit ShaderIndexChanged();
    }
}

int Editor::ShaderIndex() const
{
    return m_shaderIndex;
}