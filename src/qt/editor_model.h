////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "qt/tweakable.h"

#include <QObject>

struct SignalCallbacks;

/**
* Allows run time editing of shaders
*/
class EditorModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int shaderIndex READ ShaderIndex WRITE SetShaderIndex NOTIFY ShaderIndexChanged)
    Q_PROPERTY(QStringList shaders READ Shaders NOTIFY ShadersChanged)

public:

    /**
    * Constructor
    * @param callbacks Functions called when a tweakable value emits a signal
    */
    explicit EditorModel(SignalCallbacks& callbacks, QObject* parent = nullptr);

    /**
    * Sets the available shaders for the combo box
    * @param selected The initially selected shader
    * @param shaders The shader names to set
    */
    void InitialiseShaders(int selected, const std::vector<std::string>& shaders);

    /**
    * @return whether the combo box is filled or not
    */
    bool HasShaders() const;

    /**
    * Sets the editable shader text for the selected shader
    * @param text The text to edit
    */
    void SetShaderText(const std::string& text);

    /**
    * Sets the readonly shader assembly for the selected shader
    * @param assembly The assembly text
    */
    void SetShaderAssembly(const std::string& assembly);

    /**
    * Removes any edited changes to the shader text
    */
    Q_INVOKABLE void RevertSelectedShader();

    /**
    * Sends the current changes to the render engine to be compiled
    */
    Q_INVOKABLE void CompileSelectedShader();

    /**
    * Property setter/getter for selected shader index
    */
    void SetShaderIndex(int index);
    int ShaderIndex() const;

    /**
    * Property getter for shaders list
    */
    const QStringList& Shaders() const;

signals:

    /**
    * Emitted when the selected shader index changes
    */
    void ShaderIndexChanged();

    /**
    * Emitted when the shaders list changes
    */
    void ShadersChanged();

private:

    int m_shaderIndex = -1;         ///< Currently selected shader
    QStringList m_shaders;          ///< List of editable shaders
    QString m_text;                 ///< Text for the selected shader
    QString m_assembly;             ///< Assembly for the selected shader
    SignalCallbacks& m_callbacks;   ///< Callbacks to update the cache
};
