#pragma once

#include <glm/glm.hpp>
#include <SOGLR/SOGLR.hpp>

class UIComponent
{
public:
    UIComponent() = default;
    virtual ~UIComponent() = default;
    const glm::ivec2& GetPosition() const { return position_; }
    const glm::ivec2& GetSize() const { return size_; }

    void SetPosition(const glm::ivec2& new_position)
    {
        position_ = new_position;
        OnComponentMove();
    }

    void SetSize(const glm::ivec2& new_size)
    {
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
};