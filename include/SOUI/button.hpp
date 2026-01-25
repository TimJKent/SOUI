#pragma once

#include <functional>
#include <iostream>

#include "ui_component.hpp"
#include "SOGLR/SOGLR.hpp"

#include "text.hpp"
#include "panel.hpp"

namespace SOUI
{
class Button : public UIComponent
{
public:
    Button(const glm::ivec2& position, const glm::ivec2& size, const std::string& text, std::shared_ptr<SOGLR::Shader> text_shader, std::shared_ptr<SOGLR::Shader> panel_shader, std::shared_ptr<SOGLR::Text::Font> font) : UIComponent()
    {
        position_ = position;
        size_ = size;
        text_ = std::make_unique<Text>(position, size, text, text_shader, font, HorizontalTextAlign::CENTER, VerticalTextAlign::CENTER);
        panel_ = std::make_unique<Panel>(position, size, panel_shader);
    }

    ~Button() = default;

    void SetOnClick(std::function<void()> callback)
    {
        on_click_ = callback;
    }

    void SetOnMouseEnter(std::function<void()> callback)
    {
        on_mouse_enter_ = callback;
    }

    void SetOnMouseExit(std::function<void()> callback)
    {
        on_mouse_exit_ = callback;
    }

    void OnMouseEnter() final
    {
        if(on_mouse_enter_)
        {
            on_mouse_enter_();
        }
    }

    void OnMouseExit() final
    {
        pressing_ = false;
        if(on_mouse_exit_)
        {
            on_mouse_exit_();
        }
    }

    void OnComponentResize() final
    {
        panel_->SetSize(size_);
        text_->SetSize(size_);
    }

    void OnComponentMove() final
    {
        panel_->SetPosition(position_);
        text_->SetPosition(position_);
    }

    std::vector<std::shared_ptr<SOGLR::RenderObject>> GetRenderObjects() const override
    {
        auto text_objects = text_->GetRenderObjects();
        const auto& render_objects = panel_->GetRenderObjects();
        text_objects.reserve(render_objects.size() + text_objects.size());
        text_objects.insert(text_objects.end(), render_objects.begin(), render_objects.end());
        return text_objects;
    }

    void OnMouseDown(uint32_t button)
    {
        if(mouse_over)
        {
            pressing_ = true;
        }
    }
    
    void OnMouseUp(uint32_t button) 
    {
        if(pressing_ && mouse_over)
        {
            pressing_ = false;
            if(on_click_)
            {
                on_click_();
            }
        }
    }

private:
    bool pressing_ = false;
    std::function<void()> on_click_;
    std::function<void()> on_mouse_enter_;
    std::function<void()> on_mouse_exit_;
    std::unique_ptr<Text> text_;
    std::unique_ptr<Panel> panel_;
};
}