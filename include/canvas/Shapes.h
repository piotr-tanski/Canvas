#pragma once

#include "Canvas.h"

#include <functional>
#include <optional>

namespace canvas::shapes {

template <typename T>
struct MutableAttributes {
public:
    explicit MutableAttributes(const T& state) : current{state} {}

    void reset(const T& other) noexcept {
        pending = other;
    }

    void change() noexcept {
        if (pending.has_value()) {
            current = pending.value();
            pending = std::nullopt;
        }
    }

    [[nodiscard]] const T& get() const noexcept {
        return current;
    }

private:
    T current{};
    std::optional<T> pending = std::nullopt;
};

class Shape {
public:
    virtual ~Shape() noexcept = default;

    virtual void draw(painting::Tool *tool) = 0;
};

class MutableShape : public Shape {
public:
    using OnChangeListener = std::function<void(MutableShape*)>;

    void bindOnChange(OnChangeListener &&listener) {
        onChange = std::move(listener);
    }

    void change() {
        changeImpl();
    }

protected:
    OnChangeListener onChange;

    template <typename T>
    static void changeAttributes(MutableAttributes<typename T::Params>& dest, const typename T::Params &src, const OnChangeListener &listener, MutableShape *target) {
        dest.reset(src);
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
        Point start{0U, 0U};
        Length width{0};
        Length height{0};
        Color color{colors::White};
    };

    explicit Rectangle(Params params);

    void setAttributes(Params params) noexcept;
    void draw(painting::Tool *tool) override;

private:
    MutableAttributes<Params> attributes;
    void changeImpl() override;
};

class Circle : public MutableShape {
public:
    struct Params {
        Point center;
        Length radius{0};
        Color color{colors::White};
    };

    explicit Circle(Params params);

    void setAttributes(Params params) noexcept;
    void draw(painting::Tool *tool) override;

private:
    MutableAttributes<Params> attributes;
    void changeImpl() override;
};

class Triangle : public MutableShape {
public:
    struct Params {
        Point v1;
        Point v2;
        Point v3;
        Color color{colors::White};
    };

    explicit Triangle(Params params);

    void setAttributes(Params params) noexcept;
    void draw(painting::Tool *tool) override;

private:
    MutableAttributes<Params> attributes;
    void changeImpl() override;
};

} // namespace canvas::shapes