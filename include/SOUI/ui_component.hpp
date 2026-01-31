#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <SOGLR/SOGLR.hpp>

namespace SOUI
{
    class UIController;

    class UIComponent
    {
    public:
        UIComponent() = default;
        virtual ~UIComponent() = default;
        const glm::ivec2 &GetPosition() const { return position_; }
        const glm::ivec2 &GetSize() const { return size_; }

        void SetPosition(const glm::ivec2 &new_position)
        {
            if (new_position == position_)
                return;
            position_ = new_position;
            OnComponentMove();
        }

        void SetSize(const glm::ivec2 &new_size)
        {
            if (new_size == size_)
                return;
            size_ = new_size;
            OnComponentResize();
        }

        virtual std::vector<std::shared_ptr<SOGLR::RenderObject>> GetRenderObjects() const { return {}; }
        virtual void OnMouseEnter() {};
        virtual void OnMouseExit() {};
        virtual void OnComponentResize() = 0;
        virtual void OnComponentMove() = 0;
        virtual void OnMouseDown(uint32_t button) {}
        virtual void OnMouseUp(uint32_t button) {}

    public:
        bool mouse_over = false;

    protected:
        glm::ivec2 position_;
        glm::ivec2 size_;
        UIController *ui_controller_;
        friend class UIController;
    };
}