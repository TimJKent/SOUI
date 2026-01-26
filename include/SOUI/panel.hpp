#pragma once

#include <functional>
#include <iostream>

#include "ui_component.hpp"
#include "SOGLR/SOGLR.hpp"

namespace SOUI
{
    class Panel : public UIComponent
    {
    public:
        Panel(glm::ivec2 position, glm::ivec2 size, std::shared_ptr<SOGLR::Shader> shader, std::shared_ptr<SOGLR::Material> material) : UIComponent()
        {
            panel_object_ = std::make_shared<SOGLR::RenderObject>();
            position_ = position;
            size_ = size;
            shader_ = shader;
            material_ = material;
            OnComponentResize();
            OnComponentMove();
        }

        ~Panel() = default;

        void OnComponentResize() final
        {
            auto quad_model = std::make_shared<SOGLR::Model>();
            auto quad_mesh = SOGLR::Mesh(GetVertices(), SOGLR::Primitives::Quad::indicies, {});
            quad_model->AddMesh(quad_mesh);

            panel_object_ = std::make_shared<SOGLR::RenderObject>();
            panel_object_->SetModel(quad_model);
            panel_object_->SetShader(shader_);
            panel_object_->SetMaterial(material_);
        }

        void OnComponentMove() final
        {
            panel_object_->GetTransform().position = glm::vec3(static_cast<float>(position_.x), static_cast<float>(position_.y), 0.0f);
        }

        std::vector<SOGLR::Vertex> GetVertices() const
        {
            float w = static_cast<float>(size_.x);
            float h = static_cast<float>(size_.y);

            return {
                {0.0f, h, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},    // top-left
                {w, h, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},       // top-right
                {w, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},    // bottom-right
                {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f}, // bottom-left
            };
        }

        std::vector<std::shared_ptr<SOGLR::RenderObject>> GetRenderObjects() const override
        {
            return {panel_object_};
        }

    private:
        std::shared_ptr<SOGLR::Shader> shader_;
        std::shared_ptr<SOGLR::RenderObject> panel_object_;
        std::shared_ptr<SOGLR::Material> material_;
    };
}