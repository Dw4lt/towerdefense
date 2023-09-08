#ifndef OWNERSHIP_HPP
#define OWNERSHIP_HPP

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

// Forward declarations
template <typename T> class RReader;


/// @brief Sole owner of a resource. Not thread safe!
/// @tparam T Resource type to be stored on the heap.
template <typename T>
class ROwner {
public:
    // Constructors from heap-allocated types
    explicit ROwner(T* ptr = nullptr) : resource(ptr) {}
    explicit ROwner(std::shared_ptr<T>& ptr) : resource(ptr) {}
    explicit ROwner(std::shared_ptr<T>&& ptr) : resource(std::move(ptr)) {}

    // Moving required in lieu of copying
    explicit ROwner(ROwner&& other) noexcept = default;
    ROwner& operator=(ROwner&& other) noexcept = default;

    // Copying sole owner is meaningless
    ROwner(const ROwner&) = delete;
    ROwner& operator=(const ROwner&) = delete;

    bool isValid() const noexcept {
        return (bool)resource;
    }

    operator bool() const noexcept {
        return (bool) resource;
    }

    /// @brief Create a reader instance from this owner. Reader can alter but not delete the resource.
    RReader<T> makeReader() const {
        if (!isValid()) {
            throw std::runtime_error("Cannot Create a reader of an invalid owner.");
        }
        return RReader<T>(*this);
    }

    /// @brief Replace the underlying resource. Resource will be destroyed.
    void operator=(T* newPtr) {
        resource.reset(newPtr);
    }

    /// @brief Akin to std::shared_ptr<T>.get().
    /// @return May return null if resource is invalid.
    T* get() const noexcept {
        return resource.get();
    }

    T* operator->() const noexcept {
        return resource.get();
    }

    T& operator*() const {
        return *resource;
    }

    // Grant resource access to RReader
    friend class RReader<T>;

private:
    /// @brief Resource being managed
    std::shared_ptr<T> resource;
}; // ROwner


/// @brief Provides safe access to resource managed by an owner. Not thread safe!
/// @tparam T Type of resource managed externally.
template <typename T>
class RReader {
private:
    // Declare a `RReader<U>` to be friends with `RReader<T>`. Cute.
    template <typename U> friend class RReader;

public:

    RReader(const ROwner<T>& owner) : resource(std::weak_ptr(owner.resource)) {}

    // Moving allowed
    explicit RReader(RReader&& other) noexcept = default;
    RReader& operator=(RReader&& other) noexcept = default;

    // Down-casting based on stored type allowed
    template<typename U,
        typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    RReader<T>(const RReader<U>& other) : resource(other.resource){};

    // Copy Constructor and Assignment Operator are default
    RReader(const RReader&) = default;
    RReader& operator=(const RReader&) = default;

    bool isValid() const {
        return (bool)resource.lock();
    }

    operator bool() const {
        return (bool) resource.lock();
    }

    bool operator==(const RReader& other) {
        return resource.lock() == other.resource.lock();
    }

    T& operator*() const {
                auto shared_resource = resource.lock().get();
        if (!shared_resource) {
            throw std::runtime_error("Attempted to access an invalid resource.");
        }
        return *shared_resource;
    }

    /// @brief Akin to std::shared_ptr<T>.get().
    /// @return May return null if resource is invalid.
    T* get() const noexcept {
        return resource.lock().get();
    }

    /// @brief Akin to std::shared_ptr<T>.operator->().
    /// @throws if resource is invalid
    T* operator->() const {
        auto ret = resource.lock().get();
        if (ret == nullptr) throw std::runtime_error("Attempted to dereference an invalid resource.");
        return ret;
    }

    friend class ROwner<T>;

private:
    std::weak_ptr<T> resource;
}; // RReader


/// @brief Wrapper used to iterate over a datastructure with a lambda-in-the-loop
/// @tparam Container Datastructure to iterate over
/// @tparam Func Function processing the underlying items while iterating
template <typename Container, typename Func>
class ContainerWrapper {
public:
    ContainerWrapper(Container& c, Func f) : original_container(c), func(f) {}

    class Iterator {
    public:
        Iterator(typename Container::iterator it, Func f) : original_it(it), func(f) {}

        // Prefix increment
        Iterator& operator++() {
            ++original_it;
            return *this;
        }

        // Dereferencing
        auto operator*() {
            return func(*original_it);
        }

        // Comparison
        bool operator!=(const Iterator& other) const {
            return original_it != other.original_it;
        }

    private:
        typename Container::iterator original_it;
        Func func;
    };

    Iterator begin() {
        return Iterator(original_container.begin(), func);
    }

    Iterator end() {
        return Iterator(original_container.end(), func);
    }

private:
    Container& original_container;
    Func func;
}; // ContainerWrapper

// TODO find neater way of doing this
// Functor needed to instantiate template in subsequent code
template <typename T>
struct MakeReader {
    RReader<T> operator()(ROwner<T>& e) const {
        return e.makeReader();
    }
};

// Type alias using the functor
template <typename T>
using RReaderIterable = ContainerWrapper<std::vector<ROwner<T>>, MakeReader<T>>;

/// @brief Wrap iterator of vector of owners to behave like an iterator of vector of readers
template <typename T>
auto makeOwnerToReaderWrapper(std::vector<ROwner<T>>& container) -> RReaderIterable<T> {
    return ContainerWrapper(
        container,
        MakeReader<T>()
    );
}

#endif
