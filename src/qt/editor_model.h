////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>

class StringListModel;

/**
* Allows run time editing of shaders
*/
class EditorModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StringListModel* shadersModel READ ShadersModel CONSTANT)
    Q_PROPERTY(QString shaderText READ ShaderText NOTIFY ShaderTextChanged)
    Q_PROPERTY(QString shaderAssembly READ ShaderAssembly NOTIFY ShaderAssemblyChanged)

public:

    /**
    * Constructor
    */
    explicit EditorModel(QObject* parent = nullptr);

    /**
    * @return The model for list of editable shaders
    */
    StringListModel* ShadersModel() const;

    /**
    * Property setter/getter for the selected shader's text
    */
    void SetShaderText(const QString& text);
    const QString& ShaderText() const;

    /**
    * Property setter/getter for the selected shader's assembly
    */
    void SetShaderAssembly(const QString& assembly);
    const QString& ShaderAssembly() const;

    /**
    * Sends the current changes to the render engine to be compiled
    * @param text New text to compile the shader to
    */
    Q_INVOKABLE void CompileSelectedShader(const QString& text);

signals:

    void RequestCompileSelectedShader(const QString& text);
    void ShaderAssemblyChanged();
    void ShaderTextChanged();

private:

    StringListModel* m_shadersModel = nullptr; ///< Model for list of editable shaders
    QString m_shaderText;                      ///< Text for the selected shader
    QString m_shaderAssembly;                  ///< Assembly for the selected shader
};
