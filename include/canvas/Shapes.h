#pragma once

#include "Canvas.h"

#include <functional>

namespace canvas::shapes {

class Shape {
  public:
    virtual ~Shape() noexcept = default;

    virtual void draw(painting::Tool *tool) = 0;
};

class MutableShape : public Shape {
  public:
    using OnChangeListener = std::function<void(MutableShape *)>;

    void bindOnChange(OnChangeListener &&listener) noexcept { onChange = std::move(listener); }

    void change() { changeImpl(); }

  protected:
    OnChangeListener onChange;

    template <typename T>
    static void changeAttributes(MutableState<typename T::Params> &dest, const typename T::Params &src,
                                 const OnChangeListener &listener, MutableShape *target) {
        dest.setPendingState(src);
        if (listener) {
            listener(target);
        }
    }

  private:
    virtual void changeImpl() = 0;
};

class Rectangle : public MutableShape {
  public:
    struct Params {
        Point start{};
        Length width{0};
        Length height{0};
        Color color{colors::White};
    };

    explicit Rectangle(Params params);

    void setAttributes(Params params) noexcept;
    void draw(painting::Tool *tool) override;

  private:
    void changeImpl() override;

    MutableState<Params> attributes;
};

class Circle : public MutableShape {
  public:
    struct Params {
        Point center{};
        Length radius{0};
        Color color{colors::White};
    };

    explicit Circle(Params params);

    void setAttributes(Params params) noexcept;
    void draw(painting::Tool *tool) override;

  private:
    void changeImpl() override;

    MutableState<Params> attributes;
};

class Triangle : public MutableShape {
  public:
    struct Params {
        Point v1{};
        Point v2{};
        Point v3{};
        Color color{colors::White};
    };

    explicit Triangle(Params params);

    void setAttributes(Params params) noexcept;
    void draw(painting::Tool *tool) override;

  private:
    void changeImpl() override;
    void drawLine(Point a, Point b, painting::Tool *tool);

    MutableState<Params> attributes;
};

} // namespace canvas::shapes