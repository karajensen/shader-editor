////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include "tweakable.h"
#include "tweakable_box.h"

#ifdef _VS
#include "generated/ui_editor.h"
#else
#include "ui_editor.h"
#endif

struct SignalCallbacks;

/**
* Allows run time editing of shaders
* @note Read by both VS and Qt Editor
*/
class Editor : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    * @note called by the qt designer
    * @param parent The owner of this widget
    */
    Editor(QWidget* parent = nullptr);

    /**
    * Constructor
    * @note called by shader editor application
    * @param callbacks Functions called when a tweakable value emits a signal
    * @param parent The owner of this widget
    */
    Editor(const SignalCallbacks& callbacks, QWidget* parent = nullptr);

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

private slots:

    /**
    * Removes any edited changes to the shader text
    */
    void RevertPressed();

    /**
    * Sends the current changes to the render engine to be compiled
    */
    void CompilePressed();

private:

    QString m_text;                ///< Text for the selected shader
    QString m_assembly;            ///< Assembly for the selected shader
    TweakableBox m_shaders;        ///< Combo box for holding shaders
    SignalCallbacks m_callbacks;   ///< Callbacks to update the cache
    Ui::Editor m_ui;               ///< User interface object
};
