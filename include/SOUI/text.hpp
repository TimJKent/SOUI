#pragma once

#include <string>
#include <glm/glm.hpp>

#include "ui_component.hpp"
#include "SOGLR/SOGLR.hpp"
#include "Font.hpp"

namespace SOUI
{

    enum class HorizontalTextAlign
    {
        LEFT,
        CENTER,
        RIGHT
    };

    enum class VerticalTextAlign
    {
        TOP,
        CENTER,
        BOTTOM
    };

    class Text : public UIComponent
    {
    public:
        Text(glm::ivec2 position, glm::ivec2 size, const std::string &text, std::shared_ptr<SOGLR::Shader> shader, std::shared_ptr<Font> font, HorizontalTextAlign h_align = HorizontalTextAlign::LEFT, VerticalTextAlign v_align = VerticalTextAlign::BOTTOM)
            : text_object_(std::make_shared<SOGLR::RenderObject>()), text_(text), shader_(shader), font_(font), h_align_(h_align), v_align_(v_align)
        {
            position_ = position;
            size_ = size;
            SetText(text);
            OnComponentMove();
        }

        ~Text() = default;
        const std::string &GetText() const { return text_; }

        void SetText(const std::string &new_text)
        {
            text_ = new_text;
            float x = 0.0f;
            float y = 0.0f;
            float scale = 1.0f;
            auto quad_model = std::make_shared<SOGLR::Model>();

            // Calculate total width for alignment
            float total_width = 0.0f;
            for (char c : text_)
            {
                const Character &ch = font_->GetCharacter(c);
                total_width += (ch.Advance >> 6) * scale;
            }

            // TODO: WE SHOULD PROBABLY CALCULATE THIS ONCE IN THE FONT LOADER AND SAVE TO FONT.hpp
            float tallest_height = 0.0f;
            for (char c : text_)
            {
                const Character &ch = font_->GetCharacter(c);
                if (ch.Size.y > tallest_height)
                {
                    tallest_height = static_cast<float>(ch.Size.y);
                }
            }

            if (h_align_ == HorizontalTextAlign::CENTER)
            {
                x += size_.x / 2.0f - total_width / 2.0f;
            }
            else if (h_align_ == HorizontalTextAlign::RIGHT)
            {
                x += size_.x - total_width;
            }

            if (v_align_ == VerticalTextAlign::CENTER)
            {
                y += size_.y / 2.0f - tallest_height / 2.0f;
            }
            else if (v_align_ == VerticalTextAlign::TOP)
            {
                y += size_.y - tallest_height;
            }

            for (char c : text_)
            {
                const Character &ch = font_->GetCharacter(c);

                // Calculate character quad position and size
                float xpos = x + ch.Bearing.x * scale;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
                float w = ch.Size.x * scale;
                float h = ch.Size.y * scale;

                std::vector<SOGLR::Vertex> vertices = {
                    {xpos, ypos + h, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},     // top-left
                    {xpos + w, ypos + h, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f}, // top-right
                    {xpos + w, ypos, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},     // bottom-right
                    {xpos, ypos, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},         // bottom-left
                };

                SOGLR::Mesh quad_mesh{
                    vertices,
                    SOGLR::Primitives::Quad::indicies,
                    {ch.texture}};
                quad_model->AddMesh(quad_mesh);
                x += (ch.Advance >> 6) * scale;
            }

            text_object_->SetModel(quad_model);
            text_object_->SetShader(shader_);
        }

        virtual void OnComponentResize() final {}
        virtual void OnComponentMove() final
        {
            text_object_->GetTransform().position = glm::vec3(static_cast<float>(position_.x), static_cast<float>(position_.y), 0.0f);
        }

        std::vector<std::shared_ptr<SOGLR::RenderObject>> GetRenderObjects() const override
        {
            return {text_object_};
        }

    private:
        std::shared_ptr<SOGLR::Shader> shader_;
        std::shared_ptr<Font> font_;
        std::shared_ptr<SOGLR::RenderObject> text_object_;
        std::string text_;
        HorizontalTextAlign h_align_;
        VerticalTextAlign v_align_;
    };
}