#pragma once

#include "ui_component.hpp"
#include "ui_controller.hpp"

namespace SOUI
{
    class Layout : public UIComponent
    {
    public:
        Layout() = default;
        virtual ~Layout() = default;

        virtual void OnComponentResize() override { ReadjustLayout(); }
        virtual void OnComponentMove() override {}

        void AddComponent(std::shared_ptr<UIComponent> child)
        {
            children_.push_back(child);
            if (ui_controller_)
            {
                ui_controller_->AddComponent(child);
            }
            ReadjustLayout();
        }

        virtual void ReadjustLayout() = 0;

    protected:
        std::vector<std::shared_ptr<UIComponent>> children_;
    };
}