#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>

// Forward declarations
template <typename T> class RReader;


/// @brief Sole owner of a resource. Not thread safe!
/// @tparam T Resource type to be stored on the heap.
template <typename T>
class ROwner {
public:
    template <typename U>
    friend class ROwner;

    // Constructors from heap-allocated types
    explicit ROwner(T* ptr = nullptr) : resource(ptr) {}
    explicit ROwner(std::shared_ptr<T>& ptr) : resource(ptr) {}
    explicit ROwner(std::shared_ptr<T>&& ptr) : resource(std::move(ptr)) {}

    // Moving required in lieu of copying
    template <typename U>
    ROwner(ROwner<U>&& other) noexcept {
        static_assert(std::is_base_of_v<T, U> || std::is_convertible_v<U*, T*>,
                    "Cannot convert ROwner<U> to ROwner<T>");
        resource = std::static_pointer_cast<T>(other.resource);
        other.resource.reset();
    };

    template <typename U>
    ROwner<T>& operator=(ROwner<U>&& other) noexcept {
        static_assert(std::is_base_of_v<T, U> || std::is_convertible_v<U*, T*>,
                    "Cannot convert ROwner<U> to ROwner<T>");
        resource = std::static_pointer_cast<T>(other.resource);
        other.resource.reset();
        return *this;
    }

    // Copying sole owner is meaningless
    ROwner(const ROwner&) = delete;
    ROwner& operator=(const ROwner&) = delete;

    bool isValid() const noexcept {
        return (bool)resource;
    }

    explicit operator bool() const noexcept {
        return (bool) resource;
    }

    bool operator==(const ROwner& other) const noexcept {
        return resource.get() == other.resource.get();
    }

    /// @brief Create a reader instance from this owner. Reader can alter but not delete the resource.
    RReader<T> makeReader() const {
        if (!isValid()) {
            throw "Cannot Create a reader of an invalid owner.";
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

    RReader() : resource(std::weak_ptr<T>()) {}

    // From owner
    RReader(const ROwner<T>& owner) : resource(std::weak_ptr(owner.resource)) {}

    // Implicit static casting to base
    template<typename U,
        typename = std::enable_if_t<
            std::conjunction_v<
                std::is_base_of<T, U>,
                std::negation<std::is_same<U, T>>
            >
        >,
        char = 0
    >
    RReader(const RReader<U>& other) : resource(std::static_pointer_cast<T>(other.resource.lock())) {};

    // Explicit dynamic casting to derived
    template<typename U,
        typename = std::enable_if_t<
            std::conjunction_v<
                std::is_base_of<U, T>,
                std::negation<std::is_same<U, T>>
            >
        >,
        int = 0
    >
    explicit RReader(const RReader<U>& other) : resource(std::dynamic_pointer_cast<T>(other.resource.lock())) {};

    // Moving allowed
    explicit RReader(RReader&&) noexcept = default;
    RReader& operator=(RReader&& other) noexcept = default;

    // Copy Constructor and Assignment Operator are default
    RReader(const RReader&) = default;
    RReader& operator=(const RReader&) = default;

    bool isValid() const {
        return (bool)resource.lock();
    }

    explicit operator bool() const {
        return (bool) resource.lock();
    }

    template <typename U>
    bool operator==(const RReader<U>& other) const noexcept {
        return resource.lock() == other.resource.lock();
    }

    T& operator*() const {
        auto shared_resource = resource.lock().get();
        if (!shared_resource) {
            throw "Attempted to access an invalid resource.";
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
        if (ret == nullptr) {
            throw "Attempted to dereference an invalid resource.";
        }
        return ret;
    }

    friend class ROwner<T>;

private:
    std::weak_ptr<T> resource;
}; // RReader


template<typename T, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> : std::true_type {};

// Template interface to simplify return type of subsequent iterator wrappers
template<typename T>
struct IIterator {
    IIterator() = default;

    // Disable copy to avoid accidentally losing type information
    IIterator(const IIterator& other) = delete;
    IIterator operator=(const IIterator& other) = delete;

    // Leave iterator logic to derived class
    virtual T operator*() const = 0;
    virtual T operator->() const { return operator*(); };

    virtual IIterator& operator++() = 0;
    IIterator& operator++(int) = delete;

    virtual bool operator==(const IIterator&) const = 0;
    virtual bool operator!=(const IIterator&) const = 0;
};

template<typename IteratorType>
struct IIterable {
    using iter = IIterator<IteratorType>;
    static_assert(std::is_invocable_v<decltype(&iter::operator*), iter>, "Template type passed to IIterable is not dereferenceable");

    explicit IIterable(std::unique_ptr<iter>&& s, std::unique_ptr<iter> && e) : s_{std::move(s)}, e_{std::move(e)} {};
    virtual ~IIterable() = default;
    virtual iter& begin() {
        auto& a = *s_;
        return a;
    };
    virtual iter& end() { return *e_; };

private:
    std::unique_ptr<iter> s_;
    std::unique_ptr<iter> e_;
};



/// @brief Wrapper used to iterate over a datastructure with a lambda-in-the-loop
/// @tparam ContainerIterator Datastructure to iterate over
/// @tparam Func Function processing the underlying items while iterating
template <typename ContainerIterator, typename Callable>
class MethodApplyingIterator : public IIterator<typename std::invoke_result_t<Callable, typename ContainerIterator::reference>> {
public:

    // Allow access to other template instantiations
    template <typename U, typename R>
    friend class MethodApplyingIterator;

    using input_type = typename ContainerIterator::reference;
    using value_type = typename std::invoke_result_t<Callable, input_type>;
    using returned_iterator = IIterator<value_type>;
    using iterator_category = typename std::iterator_traits<ContainerIterator>::iterator_category;

    using reference = value_type&;
    using pointer = value_type*;

    explicit MethodApplyingIterator(ContainerIterator it, Callable callable)
        : it_(std::move(it)), callable_(std::move(callable)) {}

    virtual value_type operator*() const override {
        return std::invoke(callable_, *it_);
    }

    virtual MethodApplyingIterator<ContainerIterator, Callable>& operator++() override {
        ++it_;
        return *this;
    }

    virtual bool operator==(const MethodApplyingIterator& other) const {
        return it_ == other.it_;
    }

    virtual bool operator!=(const MethodApplyingIterator& other) const {
        return !(*this == other);
    }

    virtual bool operator==(const returned_iterator& other) const override {
        auto& o = dynamic_cast<const MethodApplyingIterator&>(other);
        return it_ == o.it_;
    }

    virtual bool operator!=(const returned_iterator& other) const override {
        auto& o = dynamic_cast<const MethodApplyingIterator&>(other);
        return it_ != o.it_;
    }

private:
    ContainerIterator it_;
    Callable callable_;
}; // MethodApplyingIterator

template <typename K, typename T = K>
struct MakeReader {
    auto operator()(ROwner<T>& e) const -> RReader<T> {
        if (e.isValid()) return e.makeReader();
        return RReader<T>();
    }

    template <typename Key = K, typename std::enable_if<!std::is_abstract<Key>::value, int>::type = 0>
    auto operator()(std::pair<const K, ROwner<T>>& e) const -> RReader<T>
    {
        if (e.second.isValid()) return e.second.makeReader();
        return RReader<T>();
    }
};

template <
    typename OriginalIterable,
    typename Callable,
    typename value_type = typename std::invoke_result<Callable, typename OriginalIterable::reference>::type
> auto make(OriginalIterable& iterable, Callable callable) -> IIterable<value_type> {
    static_assert(std::is_invocable_v<Callable, typename OriginalIterable::reference>,
            "Callable cannot be invoked on the type that BaseIterator points to");

    using iter_type = MethodApplyingIterator<decltype(iterable.begin()), Callable>;
    static_assert(std::is_base_of<IIterator<value_type>, iter_type>::value, "Cast attempt is not valid."); // Regression test
    return IIterable<value_type>(
        std::unique_ptr<IIterator<value_type>>(new iter_type(iterable.begin(), callable)),
        std::unique_ptr<IIterator<value_type>>(new iter_type(iterable.end(), callable))
    );
};

/// @brief Wrap iterator of vector of owners to behave like an iterator of vector of readers
template <typename T>
auto makeOwnerToReaderWrapper(std::vector<ROwner<T>>& container) -> IIterable<RReader<T>> {
    return make(
        container,
        MakeReader<T, T>()
    );
}


/// @brief Wrap iterator of map of owners to behave like an iterator of vector of readers
template <typename K, typename T>
auto makeOwnerToReaderWrapper(std::map<K, ROwner<T>>& container) -> IIterable<RReader<T>> {
    return make(
        container,
        MakeReader<K, T>()
    );
}
