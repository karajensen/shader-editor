////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor_model.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "editor_model.h"

EditorModel::EditorModel(SignalCallbacks& callbacks, QObject* parent)
    : QObject(parent)
    , m_callbacks(callbacks)
{
}

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

void EditorModel::SetShaderText(const std::string& text)
{
    //m_text = QString(text.c_str());
    //m_ui.shaderTextBox->setText(m_text);
}

void EditorModel::SetShaderAssembly(const std::string& assembly)
{
    //m_assembly = QString(assembly.c_str());
    //m_ui.assemblyTextBox->setText(m_assembly);
}

void EditorModel::CompileSelectedShader()
{
    //m_text = m_ui.shaderTextBox->toPlainText();
    //m_callbacks.CompileShader(m_text.toUtf8().constData());
}

void EditorModel::RevertSelectedShader()
{
    //m_ui.shaderTextBox->setText(m_text);
    //m_ui.assemblyTextBox->setText(m_assembly);
}

const QStringList& EditorModel::Shaders() const
{
    return m_shaders;
}

void EditorModel::SetShaderIndex(int index)
{
    if (m_shaderIndex != index)
    {
        m_shaderIndex = index;
        emit ShaderIndexChanged();
    }
}

int EditorModel::ShaderIndex() const
{
    return m_shaderIndex;
}