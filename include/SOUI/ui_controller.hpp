#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include "SOGLR/SOGLR.hpp"

#include "ui_component.hpp"

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
    UIController(std::shared_ptr<SOGLR::Window> window, std::shared_ptr<SOGLR::Scene> scene) : window_(window), scene_(scene) {}
    ~UIController() = default;

    void AddComponent(std::shared_ptr<UIComponent> component)
    {
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
        for(auto& comp : components_)
        {
            bool is_over = IsInRect(mouse_position, comp->GetPosition(), comp->GetSize());
            if(!comp->mouse_over && is_over)
            {
                comp->mouse_over = true;
                comp->OnMouseEnter();
            }
            else if(comp->mouse_over && !is_over)
            {
                comp->mouse_over = false;
                comp->OnMouseExit();
            }
        }
    }

    void DispatchMouseButtonPressed()
    {
        for(int i =0; i < 3; i++)
        {
            if(window_->IsMouseButtonDown(i) && !previous_mouse_button_states_[i])
            {
                previous_mouse_button_states_[i] = true;
                for(auto& comp : components_)
                {
                    comp->OnMouseDown(i);
                }
            }
            else if(window_->IsMouseButtonUp(i) && previous_mouse_button_states_[i])
            {
                previous_mouse_button_states_[i] = false;
                for(auto& comp : components_)
                {
                    comp->OnMouseUp(i);
                }
            }
        }
        
    }

private:
    std::array<bool, 3> previous_mouse_button_states_ = {false, false, false};
    std::vector<std::shared_ptr<UIComponent>> components_;
    std::shared_ptr<SOGLR::Window> window_;
    std::shared_ptr<SOGLR::Scene> scene_;
};
}