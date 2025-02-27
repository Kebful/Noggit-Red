// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#ifndef NOGGIT_BRUSHSTACKITEM_HPP
#define NOGGIT_BRUSHSTACKITEM_HPP

#include <QWidget>
#include <QIcon>
#include <QCheckBox>
#include <QToolButton>
#include <QJsonObject>
#include <ui_BrushStackItem.h>
#include <optional>
#include <variant>

#include <noggit/ui/tools/UiCommon/ReorderableVerticalBox.hpp>

#include <noggit/ui/TerrainTool.hpp>
#include <noggit/ui/texturing_tool.hpp>
#include <noggit/ui/FlattenTool.hpp>
#include <noggit/ui/hole_tool.hpp>
#include <noggit/ui/ShaderTool.hpp>
#include <noggit/ui/ZoneIDBrowser.h>
#include <noggit/ui/Water.h>
#include <noggit/ui/TexturingGUI.h>

class World;

namespace Noggit::Ui::Tools
{
  using operation_type = std::variant
    < Noggit::Ui::TerrainTool*
    , Noggit::Ui::flatten_blur_tool*
    , Noggit::Ui::texturing_tool*
    , Noggit::Ui::ShaderTool*
  >;

  //! \note Keep in same order as variant!
  enum eTools
  {
    eRaiseLower,
    eFlattenBlur,
    eTexturing,
    eShader
  };

  class BrushStackItem : public ReorderableVerticalBox
  {
    Q_OBJECT
  public:
    BrushStackItem(QWidget* parent = nullptr);

    void setTool(operation_type tool);
    QWidget* getTool();
    void setRadius(float radius);
    void setInnerRadius(float inner_radius);
    void setSpeed(float speed);
    void setMaskRotation(int rot);
    void setBrushMode(bool sculpt);
    void execute(glm::vec3 const& cursor_pos, World* world, float dt, bool mod_shift_down, bool mod_alt_down, bool mod_ctrl_down, bool is_under_map);
    void syncSliders(double radius, double inner_radius, double speed, int rot, int brushMode);

    bool isRadiusAffecting() { return _is_radius_affecting->isChecked(); };
    bool isInnerRadiusAffecting() { return _is_inner_radius_affecting->isChecked(); };
    bool isMaskRotationAffecting() { return _is_mask_rotation_affecting->isChecked(); };
    bool isSpeedAffecting() { return _is_speed_affecting->isChecked(); };
    QToolButton* getActiveButton() { return _ui.brushNameLabel; };

    bool isAffecting() const { return _ui.contentWidget->isEnabled(); };

    bool isMaskEnabled();
    void updateMask();



    QJsonObject toJSON();
    void fromJSON(QJsonObject const& json);

  signals:
    void requestDelete(BrushStackItem* item);
    void activated(BrushStackItem* item);
    void settingsChanged(BrushStackItem* item);

  private:
    ::Ui::brushStackItem _ui;
    QIcon _expanded_icon;
    QIcon _collapsed_icon;
    QIcon _enabled_icon;
    QIcon _disabled_icon;

    operation_type _tool_widget;
    QWidget* _settings_popup;
    QCheckBox* _is_radius_affecting;
    QCheckBox* _is_inner_radius_affecting;
    QCheckBox* _is_mask_rotation_affecting;
    QCheckBox* _is_speed_affecting;

    std::optional<scoped_blp_texture_reference> _selected_texture;
    Noggit::Ui::tileset_chooser* _texture_palette = nullptr;
    bool _is_texture_dirty = false;

  };
}

#endif //NOGGIT_BRUSHSTACKITEM_HPP
