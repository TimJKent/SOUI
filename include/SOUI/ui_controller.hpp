#pragma once

#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "SOGLR/SOGLR.hpp"

#include "FontLoader.hpp"
#include "Font.hpp"
#include "ui_component.hpp"
#include "BundledAssets/default_text_shader.hpp"
#include "BundledAssets/default_panel_shader.hpp"

namespace SOUI
{

    enum class UIEventType
    {
        Click,
        MouseEnter,
        MouseExit
    };

    class UIController
    {
    public:
        UIController(std::shared_ptr<SOGLR::Window> window, std::shared_ptr<SOGLR::Scene> scene) : window_(window), scene_(scene)
        {
            default_text_shader_ = std::make_shared<SOGLR::Shader>(text_vert_shader, text_frag_shader);
            default_panel_shader_ = std::make_shared<SOGLR::Shader>(panel_vert_shader, panel_frag_shader);

            FontLoader font_loader;

#ifdef _WIN32
            std::string font_path = "C:/Windows/Fonts/segoeui.ttf";
#else
            std::string font_path = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf";
#endif

            auto expected_font_12 = font_loader.LoadFont(font_path, 12);
            auto expected_font_24 = font_loader.LoadFont(font_path, 24);
            auto expected_font_48 = font_loader.LoadFont(font_path, 48);
            if (!expected_font_12 || !expected_font_24 || !expected_font_48)
            {
                throw std::runtime_error("Failed to load default font size 12: " + expected_font_12.error());
            }

            default_font_12 = expected_font_12.value();
            default_font_24 = expected_font_24.value();
            default_font_48 = expected_font_48.value();
        }
        ~UIController() = default;

        void AddComponent(std::shared_ptr<UIComponent> component)
        {
            component->ui_controller_ = this;
            components_.push_back(component);
            scene_->AddRenderObject(component->GetRenderObjects());
        }

        void Update()
        {
            DispatchMouseOver(window_->GetMousePosition());
            DispatchMouseButtonPressed();
        }

        bool IsInRect(glm::ivec2 point, glm::ivec2 rect_position, glm::ivec2 rect_size)
        {
            return (point.x >= rect_position.x && point.x <= rect_position.x + rect_size.x &&
                    point.y >= rect_position.y && point.y <= rect_position.y + rect_size.y);
        }

        void DispatchMouseOver(glm::vec2 mouse_position)
        {
            for (auto &comp : components_)
            {
                bool is_over = IsInRect(mouse_position, comp->GetPosition(), comp->GetSize());
                if (!comp->mouse_over && is_over)
                {
                    comp->mouse_over = true;
                    comp->OnMouseEnter();
                }
                else if (comp->mouse_over && !is_over)
                {
                    comp->mouse_over = false;
                    comp->OnMouseExit();
                }
            }
        }

        void DispatchMouseButtonPressed()
        {
            for (int i = 0; i < 3; i++)
            {
                if (window_->IsMouseButtonDown(i) && !previous_mouse_button_states_[i])
                {
                    previous_mouse_button_states_[i] = true;
                    for (auto &comp : components_)
                    {
                        comp->OnMouseDown(i);
                    }
                }
                else if (window_->IsMouseButtonUp(i) && previous_mouse_button_states_[i])
                {
                    previous_mouse_button_states_[i] = false;
                    for (auto &comp : components_)
                    {
                        comp->OnMouseUp(i);
                    }
                }
            }
        }

        std::shared_ptr<SOGLR::Shader> GetDefaultTextShader() const { return default_text_shader_; }
        std::shared_ptr<SOGLR::Shader> GetDefaultPanelShader() const { return default_panel_shader_; }
        std::shared_ptr<Font> GetDefaultFont12() const { return default_font_12; }
        std::shared_ptr<Font> GetDefaultFont24() const { return default_font_24; }
        std::shared_ptr<Font> GetDefaultFont48() const { return default_font_48; }
        std::shared_ptr<SOGLR::Window> GetWindow() const { return window_; }

    private:
        std::array<bool, 3> previous_mouse_button_states_ = {false, false, false};
        std::vector<std::shared_ptr<UIComponent>> components_;
        std::shared_ptr<SOGLR::Window> window_;
        std::shared_ptr<SOGLR::Scene> scene_;
        std::shared_ptr<SOGLR::Shader> default_text_shader_;
        std::shared_ptr<SOGLR::Shader> default_panel_shader_;
        std::shared_ptr<Font> default_font_12;
        std::shared_ptr<Font> default_font_24;
        std::shared_ptr<Font> default_font_48;
    };
}