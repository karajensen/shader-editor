////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "editor.h"

Editor::Editor(SignalCallbacks& callbacks)
    : m_callbacks(callbacks)
{
    //onnect(m_ui.revertBtn, SIGNAL(pressed(void)), 
    //   this, SLOT(RevertPressed(void)));  
    //
    //onnect(m_ui.compileBtn, SIGNAL(pressed(void)), 
    //   this, SLOT(CompilePressed(void)));  
}

void Editor::InitialiseShaders(int selected, const std::vector<std::string>& shaders)
{
    m_shaders.reserve(shaders.size());
    for (const auto& shader : shaders)
    {
        m_shaders.push_back(QString::fromStdString(shader));
    }

    setShaderIndex(selected);
}

bool Editor::HasShaders() const
{
    return false;
    //return m_shaders.IsInitialised();
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

void Editor::CompilePressed()
{
    //m_text = m_ui.shaderTextBox->toPlainText();
    //m_callbacks.CompileShader(m_text.toUtf8().constData());
}

void Editor::RevertPressed()
{
    //m_ui.shaderTextBox->setText(m_text);
    //m_ui.assemblyTextBox->setText(m_assembly);
}

void Editor::setShaderIndex(int index)
{
    if (m_shaderIndex != index)
    {
        m_shaderIndex = index;
        emit shaderIndexChanged();
    }
}

int Editor::shaderIndex() const
{
    return m_shaderIndex;
}