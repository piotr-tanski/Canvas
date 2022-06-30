#pragma once

#include <cstdint>
#include <optional>

namespace canvas {

    using Color = std::uint8_t;
    namespace colors {
        static constexpr inline Color White = 0;
        static constexpr inline Color Black = 1;
    } // namespace colors

    using Length = unsigned int;

    struct Point {
        unsigned int x{0U};
        unsigned int y{0U};
    };

    struct CanvasResolution {
        std::uint16_t height{0U};
        std::uint16_t width{0U};
    };

    template <typename T>
    class MutableState {
    public:
        explicit MutableState(const T& state) : current{state} {}

        [[nodiscard]] const T& get() const noexcept { return current; }

        void setPendingState(const T& other) noexcept {
            pending = other;
        }

        void performTransition() noexcept {
            if (pending.has_value()) {
                current = pending.value();
                pending = std::nullopt;
            }
        }

    private:
        T current{};
        std::optional<T> pending = std::nullopt;
    };

} // namespace canvas
