/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ToolboxWidget.h"
#include "BrushTool.h"
#include "BucketTool.h"
#include "EllipseTool.h"
#include "EraseTool.h"
#include "LineTool.h"
#include "MoveTool.h"
#include "PenTool.h"
#include "PickerTool.h"
#include "RectangleTool.h"
#include "SprayTool.h"
#include "ZoomTool.h"
#include <AK/StringBuilder.h>
#include <LibGUI/Action.h>
#include <LibGUI/BoxLayout.h>
#include <LibGUI/Button.h>
#include <LibGUI/Window.h>

namespace PixelPaint {

class ToolButton final : public GUI::Button {
    C_OBJECT(ToolButton)
public:
    ToolButton(ToolboxWidget& toolbox, const String& name, const GUI::Shortcut& shortcut, OwnPtr<Tool> tool)
        : m_toolbox(toolbox)
        , m_tool(move(tool))
    {
        StringBuilder builder;
        builder.append(name);
        builder.append(" (");
        builder.append(shortcut.to_string());
        builder.append(")");
        set_tooltip(builder.to_string());

        m_action = GUI::Action::create_checkable(
            name, shortcut, [this](auto& action) {
                if (action.is_checked())
                    m_toolbox.on_tool_selection(m_tool);
                else
                    m_toolbox.on_tool_selection(nullptr);
            },
            toolbox.window());

        m_tool->set_action(m_action);
        set_action(*m_action);
        m_toolbox.m_action_group.add_action(*m_action);
    }

    const Tool& tool() const { return *m_tool; }
    Tool& tool() { return *m_tool; }

    virtual bool is_uncheckable() const override { return false; }

    virtual void context_menu_event(GUI::ContextMenuEvent& event) override
    {
        m_action->activate();
        m_tool->on_tool_button_contextmenu(event);
    }

private:
    ToolboxWidget& m_toolbox;
    OwnPtr<Tool> m_tool;
    RefPtr<GUI::Action> m_action;
};

ToolboxWidget::ToolboxWidget()
{
    set_fill_with_background_color(true);

    set_frame_thickness(1);
    set_frame_shape(Gfx::FrameShape::Panel);
    set_frame_shadow(Gfx::FrameShadow::Raised);

    set_fixed_width(48);

    set_layout<GUI::VerticalBoxLayout>();
    layout()->set_margins({ 4, 4, 4, 4 });

    m_action_group.set_exclusive(true);
    m_action_group.set_unchecking_allowed(false);

    setup_tools();
}

ToolboxWidget::~ToolboxWidget()
{
}

void ToolboxWidget::setup_tools()
{
    auto add_tool = [&](const StringView& name, const StringView& icon_name, const GUI::Shortcut& shortcut, NonnullOwnPtr<Tool> tool) -> ToolButton& {
        m_tools.append(tool.ptr());
        auto& button = add<ToolButton>(*this, name, shortcut, move(tool));
        button.set_focus_policy(GUI::FocusPolicy::TabFocus);
        button.set_fixed_height(32);
        button.set_checkable(true);
        button.set_icon(Gfx::Bitmap::load_from_file(String::formatted("/res/icons/pixelpaint/{}.png", icon_name)));
        return button;
    };

    add_tool("Move", "move", { 0, Key_M }, make<MoveTool>());
    add_tool("Pen", "pen", { 0, Key_N }, make<PenTool>());
    add_tool("Brush", "brush", { 0, Key_P }, make<BrushTool>());
    add_tool("Bucket Fill", "bucket", { Mod_Shift, Key_B }, make<BucketTool>());
    add_tool("Spray", "spray", { Mod_Shift, Key_S }, make<SprayTool>());
    add_tool("Color Picker", "picker", { 0, Key_O }, make<PickerTool>());
    add_tool("Erase", "eraser", { Mod_Shift, Key_E }, make<EraseTool>());
    add_tool("Line", "line", { Mod_Ctrl | Mod_Shift, Key_L }, make<LineTool>());
    add_tool("Rectangle", "rectangle", { Mod_Ctrl | Mod_Shift, Key_R }, make<RectangleTool>());
    add_tool("Ellipse", "circle", { Mod_Ctrl | Mod_Shift, Key_E }, make<EllipseTool>());
    add_tool("Zoom", "zoom", { 0, Key_Z }, make<ZoomTool>());
}

}
