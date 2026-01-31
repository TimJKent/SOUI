#pragma once

#include "layout.hpp"

namespace SOUI
{
    class GridLayout : public Layout
    {
    public:
        GridLayout() : rows_(0), cols_(0), padding_x_(0), padding_y_(0), spacing_x_(0), spacing_y_(0) {}
        virtual ~GridLayout() = default;

        virtual void ReadjustLayout() override
        {
            if (rows_ <= 0 || cols_ <= 0)
                return;

            int available_width = size_.x - 2 * padding_x_ - (cols_ - 1) * spacing_x_;
            int available_height = size_.y - 2 * padding_y_ - (rows_ - 1) * spacing_y_;

            if (available_width <= 0 || available_height <= 0)
                return;

            int cell_width = available_width / cols_;
            int cell_height = available_height / rows_;

            glm::ivec2 grid_start = {position_.x + padding_x_, position_.y + padding_y_};

            int child_index = 0;
            for (int r = 0; r < rows_; ++r)
            {
                for (int c = 0; c < cols_; ++c)
                {
                    if (child_index >= children_.size())
                        return;

                    auto &child = children_[child_index++];
                    child->SetPosition({grid_start.x + c * (cell_width + spacing_x_),
                                        grid_start.y + r * (cell_height + spacing_y_)});
                    child->SetSize({cell_width, cell_height});
                }
            }
        }

        void SetGridSize(int cols, int rows)
        {
            rows_ = rows;
            cols_ = cols;
            ReadjustLayout();
        }

        void SetPadding(int padding_x, int padding_y)
        {
            padding_x_ = padding_x;
            padding_y_ = padding_y;
            ReadjustLayout();
        }

        void SetSpacing(int spacing_x, int spacing_y)
        {
            spacing_x_ = spacing_x;
            spacing_y_ = spacing_y;
            ReadjustLayout();
        }

    private:
        int rows_;
        int cols_;
        int padding_x_;
        int padding_y_;
        int spacing_x_;
        int spacing_y_;
    };
}