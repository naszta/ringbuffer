#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <stdexcept>

namespace example
{
    template <class Type, size_t ringsize>
    class ring_buffer
    {
        using single_store = std::array<std::int8_t, sizeof(Type)>;
        using buffer_store = std::array<single_store, ringsize>;

        std::uint64_t _begin{ 0 };
        std::uint64_t _end{ 0 };
        buffer_store _buffer;

    public:
        ring_buffer() = default;
        ~ring_buffer()
        {
            while (this->try_pop())
            {
            }
        }

        ring_buffer(const ring_buffer&) = delete;
        ring_buffer& operator= (const ring_buffer&) = delete;

        template <class ... Args>
        void emplace(Args ... args)
        {
            if (this->size() == ringsize)
            {
                throw std::runtime_error{ "ring buffer is full" };
            }
            const auto index = _end % ringsize;
            ++_end;
            new (_buffer.data() + index) Type(std::forward<Args>(args)...);
        }

        std::optional<Type> try_pop()
        {
            if (this->empty())
            {
                return std::nullopt;
            }
            const auto index = _begin % ringsize;
            ++_begin;
            const auto typeptr = reinterpret_cast<Type*>(_buffer[index].data());

            std::optional<Type> retval;
            retval.emplace(std::move(*typeptr));
            typeptr->Type::~Type();
            return retval;
        }

        size_t size() const
        {
            return _end - _begin;
        }

        bool empty() const
        {
            return this->size() == 0;
        }
    };
}